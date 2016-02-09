/* 
 * File:   draw/atlas.cpp
 * Author: Miles Lacey
 * 
 * Created on February 16, 2014, 2:36 PM
 */

#include <utility>

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Atlas.h"
#include "lightsky/draw/Color.h"
#include "lightsky/draw/FontResource.h"

using ls::math::vec2;
using ls::math::vec2i;

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
Atlas::~Atlas() {
    this->terminate();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Atlas::Atlas() :
    pixelRatio{1.f},
    numEntries{0},
    pEntries{nullptr},
    atlasTex{tex_desc_t::TEX_DESC_2D}
{}

/*-------------------------------------
 * Font Atlas Copy Constructor
-------------------------------------*/
Atlas::Atlas(const Atlas& a) :
    pixelRatio{a.pixelRatio},
    numEntries{a.numEntries},
    pEntries{a.pEntries ? new(std::nothrow) AtlasEntry[a.numEntries] : nullptr},
    atlasTex{a.atlasTex}
{
    if (!this->pEntries) {
        this->terminate();
        return;
    }
    
    for (unsigned i = 0; i < a.numEntries; ++i) {
        this->pEntries[i] = a.pEntries[i];
    }
}

/*-------------------------------------
 * Font Atlas Move constructor
-------------------------------------*/
Atlas::Atlas(Atlas&& a) :
    pixelRatio{a.pixelRatio},
    numEntries{a.numEntries},
    pEntries{std::move(a.pEntries)},
    atlasTex{std::move(a.atlasTex)}
{
    a.numEntries = 0;
    a.pixelRatio = 1.f;
}

/*-------------------------------------
 * Font Atlas Copy Operator
-------------------------------------*/
Atlas& Atlas::operator =(const Atlas& a) {
    if (this->pEntries) {
        this->terminate();
    }
    
    if (!a.pEntries || !a.numEntries) {
        return *this;
    }
    
    pixelRatio = a.pixelRatio;
    numEntries = a.numEntries;
    pEntries.reset(new(std::nothrow) AtlasEntry[numEntries]);
    atlasTex = a.atlasTex;
    
    if (!this->pEntries) {
        this->terminate();
        return *this;
    }
    
    for (unsigned i = 0; i < a.numEntries; ++i) {
        this->pEntries[i] = a.pEntries[i];
    }
    
    return *this;
}

/*-------------------------------------
 * Font Atlas move operator
-------------------------------------*/
Atlas& Atlas::operator =(Atlas&& a) {
    pixelRatio = a.pixelRatio;
    a.pixelRatio = 1.f;
    
    numEntries = a.numEntries;
    a.numEntries = 0;
    
    pEntries = std::move(a.pEntries);
    atlasTex = std::move(a.atlasTex);
    
    return *this;
}

/*-------------------------------------
 * Store a fontFile's texture data on OpenGL server memory
-------------------------------------*/
bool Atlas::init(const FontResource& fr) {
    this->terminate();
    
    // Calculate the size of the atlas.
    // Dimensions is the number of entries in the x/y directions
    const int dimensions = (int)std::sqrt((float)fr.get_num_glyphs());
    const vec2i&& maxGlyphSize = fr.get_max_glyph_size();
    
    LS_LOG_MSG("Attempting to load a font atlas.");
    
    // prepare the array of atlas entries
    this->pEntries.reset(new(std::nothrow) AtlasEntry[fr.get_num_glyphs()]);
    if (this->pEntries == nullptr) {
        LS_LOG_ERR("\tUnable to generate an array of font atlas entries.\n");
        return false;
    }
    
    // Let OpenGL know that fonts only use 1-byte per pixel
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    if (!this->atlasTex.init(0, COLOR_FMT_R_8, maxGlyphSize*dimensions, COLOR_LAYOUT_R, COLOR_TYPE_UNSIGNED_BYTE, nullptr)) {
        LS_LOG_GL_ERR();
        LS_LOG_ERR("\tAn error occurred while allocating space for a font atlas.\n");
        this->pEntries.release();
        return false;
    }
    
    LS_LOG_GL_ERR();
    
    // Upload the data
    this->numEntries = fr.get_num_glyphs();
    const float fDimension = (float)dimensions;
    const math::vec2&& texResolution = (math::vec2)maxGlyphSize*dimensions;
    int iter = 0;
    
    for (int x = 0; x < dimensions; ++x) {
        for (int y = 0; y < dimensions; ++y) {
            
            const glyph& pGlyph = fr.get_glyphs()[iter];
            AtlasEntry& pEntry  = this->pEntries[iter];
            
            this->atlasTex.modify(
                vec2i{x*maxGlyphSize[0], y*maxGlyphSize[1]},
                pGlyph.size, COLOR_LAYOUT_R, COLOR_TYPE_UNSIGNED_BYTE, pGlyph.pData
            );
            
            // top-left glyph corner
            pEntry.uv[0] = vec2{(float)maxGlyphSize[0]*x, (float)maxGlyphSize[1]*y};
            // bottom right corner
            pEntry.uv[1] = pEntry.uv[0] + (vec2)pGlyph.size;
            
            // NORMALIZE ALL THE THINGS!!!
            pEntry.uv[0] /= texResolution;
            pEntry.uv[1] /= texResolution;

            // Add descriptor data for each glyph.
            pEntry.advance = (vec2)pGlyph.advance;
            pEntry.advance /= fDimension;
            
            pEntry.bearing = (vec2)pGlyph.bearing;
            pEntry.bearing /= fDimension;
            
            pEntry.size = (vec2)pGlyph.size;
            pEntry.size /= fDimension;
            
            ++iter;
        }
    }
    
    this->atlasTex.set_texture_slot(tex_slot_t::TEXTURE_SLOT_DEFAULT);
    
    // use linear filtering to generate pseudo signed-distance-field fonts.
    this->atlasTex.set_parameter(TEX_PARAM_MAG_FILTER, TEX_FILTER_LINEAR);
    this->atlasTex.set_parameter(TEX_PARAM_MIN_FILTER, TEX_FILTER_LINEAR);
    this->atlasTex.set_parameter(TEX_PARAM_WRAP_S, TEX_PARAM_CLAMP_EDGE);
    this->atlasTex.set_parameter(TEX_PARAM_WRAP_T, TEX_PARAM_CLAMP_EDGE);
    this->atlasTex.unbind();
    LS_LOG_GL_ERR();
    
    // Restore OpenGL's default pixel packing format 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    LS_LOG_GL_ERR();
    
    this->pixelRatio = 1.f / (float)fr.get_font_size();
    
    LS_LOG_MSG(
        "\tSuccessfully loaded a font atlas.",
        "\n\t\tTotal Resolution:   ", texResolution[0], 'x', texResolution[1],
        "\n\t\tGlyphs Per Row/Col: ", dimensions, " x ", dimensions,
        "\n\t\tTotal Glyph Count:  ", fr.get_num_glyphs(),
        "\n\t\tWidth Per Glyph:    ", maxGlyphSize[0],
        "\n\t\tHeight Per Glyph:   ", maxGlyphSize[1],
        '\n'
    );
    
    
    return true;
}
        
/*-------------------------------------
 * Frees all memory used by *this.
-------------------------------------*/
void Atlas::terminate() {
    this->pixelRatio = 1.f;
    this->numEntries = 0;
    this->pEntries.release();
    this->atlasTex.terminate();
}

} // end draw namespace
} // end ls namespace
