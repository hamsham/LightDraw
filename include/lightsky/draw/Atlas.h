/*
 * File:   draw/atlas.h
 * Author: Miles Lacey
 *
 * Created on February 16, 2014, 2:36 PM
 */

#ifndef __LS_DRAW_ATLAS_H__
#define __LS_DRAW_ATLAS_H__

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/Texture.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class FontResource;



/**----------------------------------------------------------------------------
 * @brief AtlasGlyph
 *
 * An AtlasGlyph contains the coordinates of individual glyphs within a texture
 * atlas.
-----------------------------------------------------------------------------*/
struct AtlasGlyph {
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
class Atlas {
  private:

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
    utils::Pointer<AtlasGlyph[]> pEntries;

    /**
     * Texture Rectangle used to store the texture atlas.
     */
    Texture atlasTex;

    /**
     * Initialize the internal texture object using data from a FontResource.
     *
     * @param fr
     * A constant reference to a FontResource object which contains the data
     * required to initialize a texture on the GPU.
     *
     * @return TRUE if the internal texture was successfully initialized, FALSE
     * if not.
     */
    bool create_texture(const FontResource& fr) noexcept;

    /**
     * Determine the maximum number of glyphs which can be placed along a row
     * of an atlas texture.
     *
     * @param fr
     * A constant reference to a valid FontResource object containing font
     * glyphs.
     *
     * @return The maximum number of glyphs which can be placed along a row of
     * an atlas texture.
     */
    static int calc_glyph_dimensions(const FontResource& fr) noexcept;

    /**
     * Reset all cpu-side data to avoid reallocations on the GPU.
     */
    void reset_cpu_data() noexcept;

  public:
    /**
     * @brief Destructor
     *
     * Releases all CPU-side data used by *this.
     */
    ~Atlas() noexcept;

    /**
     * @brief Constructor
     */
    Atlas() noexcept;

    /**
     * @brief Copy constructor
     *
     * Copies data from the input parameter into *this.
     *
     * @param a
     * A constant reference to an Atlas object who's data will be copied
     * into *this (both GPU & CPU data).
     */
    Atlas(const Atlas& a) noexcept;

    /**
     * @brief Move Constructor
     *
     * @param a
     * An r-value reference to a temporary atlas object.
     *
     * Moves all data from the input parameter into *this. No copies are
     * performed.
     */
    Atlas(Atlas&& a) noexcept;

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
    Atlas& operator=(const Atlas&) noexcept;

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
    Atlas& operator=(Atlas&& a) noexcept;

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
    bool init(const FontResource& fr) noexcept;

    /**
     * @brief Free all memory used by a texture atlas.
     *
     * @param a
     * An Atlas object containing both CPU and GPU resources which need to be
     * released from system memory.
     */
    void terminate() noexcept;

    /**
     * Retrieve the GPU id of the texture used by *this.
     *
     * @return An unsigned integer provided by OpenGL for the texture used by
     * *this atlas.
     */
    unsigned gpu_id() const noexcept;

    /**
     * Bind the atlas texture to the current OpenGL context.
     *
     * The texture used by *this has a texture type of tex_type_t::TEX_TYPE_2D.
     */
    void bind() const noexcept;

    /**
     * Unbind the atlas texture to the current OpenGL context.
     *
     * The texture used by *this has a texture type of tex_type_t::TEX_TYPE_2D.
     */
    void unbind() const noexcept;

    /**
     * Retrieve the texture used by *this.
     *
     * @return A constant reference to the texture object used by *this atlas.
     */
    const Texture& get_texture() const noexcept;

    /**
     * Retrieve the number of glyphs used by *this atlas.
     *
     * @return The number of glyphs currently contained in *this.
     */
    unsigned get_num_glyphs() const noexcept;

    /**
     * Retrieve the pointer to the list of glyphs used by *this atlas.
     *
     * @return A constant reference to a pointer object which manages the
     * lifetime of *this atlas object's glyphs.
     */
    const utils::Pointer<AtlasGlyph[]>& get_glyphs() const noexcept;
};

/*-------------------------------------
 * Retrieve the GPU id of the texture used by *this.
-------------------------------------*/
inline unsigned Atlas::gpu_id() const noexcept {
    return atlasTex.gpu_id();
}

/*-------------------------------------
 * Bind the atlas texture to the current OpenGL context.
-------------------------------------*/
inline void Atlas::bind() const noexcept {
    atlasTex.bind();
}

/*-------------------------------------
 * Unbind the atlas texture to the current OpenGL context.
-------------------------------------*/
inline void Atlas::unbind() const noexcept {
    atlasTex.unbind();
}

/*-------------------------------------
 * Retrieve the texture used by *this.
-------------------------------------*/
inline const Texture& Atlas::get_texture() const noexcept {
    return atlasTex;
}

/*-------------------------------------
 * Retrieve the number of glyphs used by *this atlas.
-------------------------------------*/
inline unsigned Atlas::get_num_glyphs() const noexcept {
    return numEntries;
}

/*-------------------------------------
 * Retrieve the pointer to the list of glyphs used by *this atlas.
-------------------------------------*/
inline const utils::Pointer<AtlasGlyph[]>& Atlas::get_glyphs() const noexcept {
    return pEntries;
}



} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_ATLAS_H__ */
