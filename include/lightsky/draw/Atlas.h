/* 
 * File:   draw/atlas.h
 * Author: Miles Lacey
 *
 * Created on February 16, 2014, 2:36 PM
 */

#ifndef __LS_DRAW_ATLAS_H__
#define	__LS_DRAW_ATLAS_H__

#include "lightsky/utils/pointer.h"

#include "lightsky/draw/Texture.h"

namespace ls {
namespace draw {

class FontResource;

/**----------------------------------------------------------------------------
 * @brief atlasEntry
 * 
 * An atlasEntry contains the coordinates of individual glyphs within a texture
 * atlas.
-----------------------------------------------------------------------------*/
struct AtlasEntry {
    /**
     * Size corresponds to the vertex width and height of a glyph.
     */
    math::vec2 size;
    
    /**
     * Advance is mostly used for font kerning
     */
    math::vec2 advance;
    
    /**
     * Bearing is mostly used for font kerning.
     */
    math::vec2 bearing;
    
    /**
     * UV is an array representing the top-left and bottom-right portions of a
     * glyph (an element in a texture atlas).
     */
    math::vec2 uv[2];
};

/**----------------------------------------------------------------------------
 * @brief Texture Atlas Class
 * 
 * A texture atlas contains a collection of textures which may be loaded into
 * GPU memory as a single texture.
-----------------------------------------------------------------------------*/
struct Atlas {
    /**
     * @brief pixelRatio contains the recommended uniform scaling ratio
     * which should be applied to each generated glyph when rendering.
     */
    float pixelRatio;

    /**
     * The number of entries in a texture atlas.
     */
    uint32_t numEntries;

    /**
     * An array of atlas entries. The UVs for these entries is calculated
     * when loading the image data from a texture file.
     */
    utils::Pointer<AtlasEntry[]> pEntries;

    /**
     * Texture Rectangle used to store the texture atlas.
     */
    Texture atlasTex;

    /**
     * @brief Destructor
     * 
     * Releases all data used by *this.
     */
    ~Atlas();

    /**
     * @brief Constructor
     */
    Atlas();

    /**
     * @brief Copy constructor
     * 
     * Copies data from the input parameter into *this.
     * 
     * @param a
     * A constant reference to an Atlas object who's data will be copied
     * into *this (both GPU & CPU data).
     */
    Atlas(const Atlas& a);

    /**
     * @brief Move Constructor
     * 
     * @param a
     * An r-value reference to a temporary atlas object.
     * 
     * Moves all data from the input parameter into *this. No copies are
     * performed.
     */
    Atlas(Atlas&& a);

    /**
     * @brief Copy Operator
     * 
     * Copies data from the input parameter into *this.
     * 
     * @param a
     * A constant reference to an Atlas object who's data will be copied
     * into *this (both GPU & CPU data).
     * 
     * @return A reference to *this.
     */
    Atlas& operator=(const Atlas&);

    /**
     * @brief Move operator
     * 
     * Moves all data from the input parameter into *this.
     * 
     * @param a
     * An r-value reference to a temporary atlas object.
     * 
     * @return A reference to *this
     */
    Atlas& operator=(Atlas&& a);

    /**
     * @brief Store bitmap data from font file into a texture atlas.
     * 
     * @param a
     * A reference to an Atlas object which is to contain a texture of font data.
     * 
     * @param fr
     * A fully loaded font resource object.
     * 
     * @return True if the data was sent to OpenGL. False if an error
     * occurred.
     */
    bool init(const FontResource& fr);

    /**
     * @brief Free all memory used by a texture atlas.
     * 
     * @param a
     * An Atlas object containing both CPU and GPU resources which need to be
     * released from system memory.
    */
    void terminate();
};

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_ATLAS_H__ */

