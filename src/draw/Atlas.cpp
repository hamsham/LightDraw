/*
 * File:   draw/atlas.cpp
 * Author: Miles Lacey
 *
 * Created on February 16, 2014, 2:36 PM
 */

#include <algorithm> // std::copy
#include <utility> // std::move

#include "ls/draw/Setup.h"
#include "ls/draw/Atlas.h"
#include "ls/draw/Color.h"
#include "ls/draw/BufferObject.h"
#include "ls/draw/FontResource.h"
#include "ls/draw/GLQuery.h"
#include "ls/draw/TextureAssembly.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
Atlas::~Atlas() noexcept {
    reset_cpu_data();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Atlas::Atlas() noexcept :
    pixelRatio{1.f},
    numEntries{0},
    pEntries{nullptr},
    atlasTex{}
{}

/*-------------------------------------
 * Font Atlas Copy Constructor
-------------------------------------*/
Atlas::Atlas(const Atlas& a) noexcept :
    pixelRatio{a.pixelRatio},
    numEntries{a.numEntries},
    pEntries{a.pEntries ? new(std::nothrow) AtlasGlyph[a.numEntries] : nullptr},
    atlasTex{a.atlasTex}
{
    if (!pEntries) {
        reset_cpu_data();
        return;
    }

    for (unsigned i = 0; i < a.numEntries; ++i) {
        pEntries[i] = a.pEntries[i];
    }
}

/*-------------------------------------
 * Font Atlas Move constructor
-------------------------------------*/
Atlas::Atlas(Atlas&& a) noexcept :
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
Atlas& Atlas::operator =(const Atlas& a) noexcept {
    if (pEntries) {
        reset_cpu_data();
    }

    if (!a.pEntries || !a.numEntries) {
        return *this;
    }

    pixelRatio = a.pixelRatio;
    numEntries = a.numEntries;
    pEntries.reset(new(std::nothrow) AtlasGlyph[numEntries]);
    atlasTex = a.atlasTex;

    if (!pEntries) {
        reset_cpu_data();
        return *this;
    }

    for (unsigned i = 0; i < a.numEntries; ++i) {
        pEntries[i] = a.pEntries[i];
    }

    return *this;
}

/*-------------------------------------
 * Font Atlas move operator
-------------------------------------*/
Atlas& Atlas::operator =(Atlas&& a) noexcept {
    pixelRatio = a.pixelRatio;
    a.pixelRatio = 1.f;

    numEntries = a.numEntries;
    a.numEntries = 0;

    pEntries = std::move(a.pEntries);
    atlasTex = std::move(a.atlasTex);

    return *this;
}

/*-------------------------------------
 * Calculate the dimensions each glyph's X & Y size
-------------------------------------*/
int Atlas::calc_glyph_dimensions(const FontResource& fr) noexcept {
    return (int)std::sqrt((float)fr.get_num_glyphs());
}

/*-------------------------------------
 * Create the texture object which will contain the atlas
-------------------------------------*/
bool Atlas::create_texture(const FontResource& fr) noexcept {
    TextureAssembly assembly;

    // Calculate the size of the atlas.
    const int dimensions = calc_glyph_dimensions(fr);
    const math::vec2i&& maxGlyphSize = fr.get_max_glyph_size() * dimensions;

    // 8 bits per pixel.
    assembly.set_format_attrib(pixel_format_t::COLOR_FMT_R_8);

    // use linear filtering to generate pseudo signed-distance-field fonts.
    assembly.set_int_attrib(TEX_PARAM_MAG_FILTER, tex_filter_t::TEX_FILTER_LINEAR);
    assembly.set_int_attrib(TEX_PARAM_MIN_FILTER, tex_filter_t::TEX_FILTER_LINEAR);
    assembly.set_int_attrib(TEX_PARAM_WRAP_S, tex_wrap_t::TEX_WRAP_CLAMP);
    assembly.set_int_attrib(TEX_PARAM_WRAP_T, tex_wrap_t::TEX_WRAP_CLAMP);
    assembly.set_size_attrib(maxGlyphSize, tex_type_t::TEX_TYPE_2D, tex_2d_type_t::TEX_SUBTYPE_2D);

    return assembly.assemble(atlasTex);
}

/*-------------------------------------
 * Reset all cpu-side data to avoid reallocations on the GPU.
-------------------------------------*/
void Atlas::reset_cpu_data() noexcept {
    pixelRatio = 1.f;
    numEntries = 0;
    pEntries.reset();
    atlasTex = Texture {};
}

/*-------------------------------------
 * Store a fontFile's texture data on OpenGL server memory
-------------------------------------*/
bool Atlas::init(const FontResource& fr) noexcept {
    reset_cpu_data();

    LS_LOG_MSG("Attempting to load a font atlas.");

    // prepare the array of atlas entries
    pEntries.reset(new(std::nothrow) AtlasGlyph[fr.get_num_glyphs()]);
    if (pEntries == nullptr) {
        LS_LOG_ERR("\tUnable to generate an array of font atlas entries.\n");
        return false;
    }

    if (!create_texture(fr)) {
        LS_LOG_GL_ERR();
        LS_LOG_ERR("\tAn error occurred while allocating space for a font atlas.\n");
        pEntries.reset();
        return false;
    }

    atlasTex.bind();
    LS_LOG_GL_ERR();

    // Calculate the size of the atlas.
    pixelRatio = 1.f / (float)fr.get_font_size(); // DPI scaling
    numEntries = fr.get_num_glyphs();

    const math::vec2i&& maxGlyphSize    = fr.get_max_glyph_size();
    const FontGlyph * const pGlyphs     = fr.get_glyphs();
    const int dimensions                = calc_glyph_dimensions(fr);
    const float fDimensionInv           = 1.f / (float)dimensions;
    const math::vec2i&& atlasSize       = math::vec2i {atlasTex.get_size()[0], atlasTex.get_size()[1]};
    const math::vec2&& texResolution    = (math::vec2)atlasSize;
    const GLint standardAlignment       = draw::get_gl_int(GL_UNPACK_ALIGNMENT);

    // uploading 1-byte per pixel
    glPixelStorei(GL_UNPACK_ALIGNMENT, draw::get_num_color_bytes(atlasTex.get_attribs().get_color_type()));

    for (int x = 0, glyphIndex = 0; x < dimensions; ++x) {
        for (int y = 0; y < dimensions; ++y) {
            const FontGlyph& pGlyph = pGlyphs[glyphIndex];
            AtlasGlyph& pEntry = pEntries[glyphIndex];

            // Upload glyph data
            atlasTex.modify(tex_2d_type_t::TEX_SUBTYPE_2D, math::vec2i{x, y} * maxGlyphSize, pGlyph.size, pGlyph.pData);

            // top-left & bottom right glyph corner
            pEntry.uv[0] = math::vec2 {(float)maxGlyphSize[0] * x, (float)maxGlyphSize[1] * y};
            pEntry.uv[1] = pEntry.uv[0] + (math::vec2) pGlyph.size;

            // normalize UV coordinates to within (0, 1)
            pEntry.uv[0] /= texResolution;
            pEntry.uv[1] /= texResolution;

            // Add normalized position/sizing data for each glyph.
            pEntry.advance = fDimensionInv * (math::vec2) pGlyph.advance;
            pEntry.bearing = fDimensionInv * (math::vec2) pGlyph.bearing;
            pEntry.size = fDimensionInv * (math::vec2) pGlyph.size;

            // next glyph
            ++glyphIndex;
        }
    }

    atlasTex.unbind();
    LS_LOG_GL_ERR();

    // Restore OpenGL's default pixel packing format
    glPixelStorei(GL_UNPACK_ALIGNMENT, standardAlignment);
    LS_LOG_GL_ERR();

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
void Atlas::terminate() noexcept {
    atlasTex.terminate();
    reset_cpu_data();
}

} // end draw namespace
} // end ls namespace
