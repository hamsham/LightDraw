/* 
 * File:   draw/fontResource.cpp
 * Author: Miles Lacey
 * 
 * Created on February 16, 2014, 2:36 PM
 */

#include <algorithm> // std::copy
#include <utility> // std::move
#include <string>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

/*-------------------------------------
    FreeType Error Handling
-------------------------------------*/
#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };
const struct {
    int          code;
    const char*  message;
} FT_Errors[] =
#include FT_ERRORS_H

void foo() {
// empty function to keep IntelliSense from going crazy.
}

#include "lightsky/draw/FontResource.h"

using ls::utils::Resource;

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 *  Helper function to load a glyph
-----------------------------------------------------------------------------*/
bool copy_glyph(glyph& pGlyph, const FT_GlyphSlot ftGlyph) {
    const FT_Glyph_Metrics& metrics = ftGlyph->metrics;
    const FT_Bitmap& ftBitmap = ftGlyph->bitmap;
    
    if (ftBitmap.width > INT_MAX || ftBitmap.rows > INT_MAX) {
        return false;
    }
    
    // These need to be divided by 64 as their measurements are
    // in "points," or, 1/64th of a pixel.
    pGlyph.size = {(int)ftBitmap.width, (int)ftBitmap.rows};
    
    pGlyph.bearing = {(int)metrics.horiBearingX, (int)metrics.horiBearingY};
    pGlyph.bearing /= 64;
    
    pGlyph.advance = {(int)metrics.horiAdvance, (int)metrics.vertAdvance};
    pGlyph.advance /= 64;

    // Copy the data from FreeType into the glyph
    std::copy(ftBitmap.buffer, ftBitmap.buffer+(ftBitmap.width*ftBitmap.rows), pGlyph.pData);
    
    return true;
}

/*-------------------------------------
    Constructor
-------------------------------------*/
FontResource::FontResource() :
    Resource{}
{}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
FontResource::FontResource(FontResource&& f) :
    Resource{}
{
    this->operator =(std::move(f));
}

/*-------------------------------------
    Destructor
-------------------------------------*/
FontResource::~FontResource() {
    unload();
}

/*-------------------------------------
    Move operator
-------------------------------------*/
FontResource& FontResource::operator=(FontResource&& f) {
    unload();
    
    pData = f.pData;
    f.pData = nullptr;
    
    dataSize = f.dataSize;
    f.dataSize = 0;
    
    numGlyphs = f.numGlyphs;
    f.numGlyphs = 0;
    
    glyphSize = f.glyphSize;
    f.glyphSize = FONT_SIZE_DEFAULT;
    
    maxGlyphSize = f.maxGlyphSize;
    f.maxGlyphSize = {0, 0};

    return *this;
}

/*-------------------------------------
    Unload all resources
-------------------------------------*/
void FontResource::unload() {
    // Re-cast to a glyph pointer to avoid a crash caused by CLang.
    delete [] reinterpret_cast<glyph*>(pData);
    pData = nullptr;
    
    dataSize = 0;
    
    numGlyphs = 0;
    
    glyphSize = FONT_SIZE_DEFAULT;
    
    maxGlyphSize = {0, 0};
}

/*-------------------------------------
    Load a font file
-------------------------------------*/
bool FontResource::load_file(const std::string& filename, unsigned pixelSize) {
    FT_Library ftLib;
    FT_Face ftFace;
    FT_Error ftErr;
    
    unload();
    
    LS_LOG_MSG("Attempting to load the font file ", filename, '.');
    
    // Initialize Freetype
    ftErr = FT_Init_FreeType(&ftLib);
    if (ftErr) {
        LS_LOG_ERR(
            "\tAn error occurred while attempting to initialize FreeType.",
            "\n\tFunction:   FT_Init_FreeType",
            "\n\tError Code: ", FT_Errors[ftErr].code,
            "\n\tFT Error:   ", FT_Errors[ftErr].message, '\n'
        );
        return false;
    }
    
    // Load the font face
    ftErr = FT_New_Face(ftLib, filename.c_str(), 0, &ftFace);
    if (ftErr) {
        LS_LOG_ERR(
            "\tUnable to load the font ", filename, '.',
            "\n\tFunction:   FT_New_Face",
            "\n\tError Code: ", FT_Errors[ftErr].code,
            "\n\tFT Error:   ", FT_Errors[ftErr].message, '\n'
        );
        FT_Done_FreeType(ftLib);
        return false;
    }
    
    // Load Unicode characters
    ftErr = FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE);
    if (ftErr) {
        LS_LOG_ERR(
            "\tAn error occurred while selecting Unicode characters within ", filename, '.',
            "\n\tFunction:   FT_Select_Charmap",
            "\n\tError Code: ", FT_Errors[ftErr].code,
            "\n\tFT Error:   ", FT_Errors[ftErr].message, '\n'
        );
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLib);
        return false;
    }
    
    // Set the pixel size for each character in the font
    ftErr = FT_Set_Pixel_Sizes(ftFace, 0, pixelSize);
    if (ftErr) {
        LS_LOG_ERR("\tUnable to set the pixel size of the font ", filename, '.',
            "\n\tFunction:   FT_Set_Pixel_Sizes",
            "\n\tError Code: ", FT_Errors[ftErr].code,
            "\n\tFT Error:   ", FT_Errors[ftErr].message, '\n'
        );
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLib);
        return false;
    }
    
    // Proceed to load the glyphs
    const bool ret = load_glyphs(ftFace);
    
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
    
    if (ret == false) {
        LS_LOG_ERR(
            "\tAn error occurred while attempting to load the font file ", filename, ".\n"
        );
    }
    else {
        glyphSize = pixelSize;
        LS_LOG_MSG("\tData Address:  ", this->pData,
            "\n\tByte Size:       ", this->dataSize,
            "\n\tGlyph Size:      ", this->glyphSize,
            "\n\tNum Glyphs:      ", this->numGlyphs,
            "\n\tSuccessfully loaded the font file ", filename, ".\n"
        );
    }
    
    return ret;
}

/*-------------------------------------*
    Load glyphs from a FT_Face and return an array of metrics
    
    Most of this information was found at MBSoftworks' OpenGL tutorials.
    http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=12
-------------------------------------*/
bool FontResource::load_glyphs(FT_FaceRec_* ftFace) {
    const unsigned ftNumGlyphs = (unsigned) ftFace->num_glyphs;
    glyph* const pGlyphs = new(std::nothrow) glyph[ftNumGlyphs];
    
    // Dynamic array error
    if (pGlyphs == nullptr) {
        return false;
    }
    
    for (unsigned i = 0; i < ftNumGlyphs; ++i) {
        FT_Error ftErr;
        unsigned charIndex = FT_Get_Char_Index(ftFace, i);
        
        ftErr = FT_Load_Glyph(ftFace, charIndex, FT_LOAD_TARGET_LIGHT); // delayed bitmap generation
        
        if (ftErr) {
            LS_LOG_ERR(
                "\tUnable to load a glyph at index ", charIndex,
                "\n\tFunction:   FT_Load_Glyph",
                "\n\tError Code: ", FT_Errors[ftErr].code,
                "\n\tFT Error:   ", FT_Errors[ftErr].message
            );
            delete [] pGlyphs;
            return false;
        }
        
        ftErr = FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_LIGHT);
        if (ftErr) {
            LS_LOG_ERR(
                "\tUnable to render the glyph at index ", charIndex,
                "\n\tFunction:   FT_Render_Glyph",
                "\n\tError Code: ", FT_Errors[ftErr].code,
                "\n\tFT Error:   ", FT_Errors[ftErr].message
            );
            delete [] pGlyphs;
            return false;
        }
        
        const FT_GlyphSlot ftGlyph  = ftFace->glyph;
        const FT_Bitmap* ftBitmap   = &ftGlyph->bitmap;
        const math::vec2i bmpSize   = {(int)ftBitmap->width, (int)ftBitmap->rows};
        const int byteSize          = bmpSize[0]*bmpSize[1];
        char* const data            = new (std::nothrow) char[byteSize];
        
        if (data == nullptr) {
            LS_LOG_ERR("\tUnable to allocate ", byteSize, " bytes for glyph data.");
            dataSize = 0;
            maxGlyphSize = math::vec2i{0,0};
            delete [] pGlyphs;
            return false;
        }
        
        pGlyphs[i].pData = data;
        
        if (!copy_glyph(pGlyphs[i], ftGlyph)) {
            LS_LOG_ERR("\tGlyph data is too large to be used for a texture.");
            dataSize = 0;
            maxGlyphSize = math::vec2i{0,0};
            delete [] pGlyphs;
            return false;
        }
        
        dataSize += byteSize;
        
        maxGlyphSize[0] = math::max(maxGlyphSize[0], bmpSize[0]);
        maxGlyphSize[1] = math::max(maxGlyphSize[1], bmpSize[1]);
    }
    
    pData = reinterpret_cast<char*>(pGlyphs);
    numGlyphs = ftNumGlyphs;
    
    return true;
}

/*-------------------------------------
    Save a file
-------------------------------------*/
bool FontResource::save_file(const std::string&) const {
    return false;
}

} // end draw namespace
} // end ls namespace
