
/*
 * File:   TextureAssembly.h
 * Author: miles
 *
 * Created on February 29, 2016, 2:10 PM
 */

#ifndef __LS_DRAW_TEXTURE_ASSEMBLY_H__
#define __LS_DRAW_TEXTURE_ASSEMBLY_H__



#include "ls/draw/Setup.h"
#include "ls/draw/Color.h"
#include "ls/draw/TextureAttrib.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class Texture;



/**----------------------------------------------------------------------------
 * The texture assembly should be used to handle the creation and modification
 * of OpenGL Texture Objects.
-----------------------------------------------------------------------------*/
class TextureAssembly {
  private:

    tex_type_t texMainType;

    GLenum texSubType;

    unsigned mipmapLevel;

    math::vec3i size;

    TextureAttrib attribs;

    /**
     * Apply all of the current texture parameters.
     *
     * @note This functions should only be called *this texture's GPU ID has
     * been bound to the current rendering context.
     */
    void apply_attribs() const noexcept;

  public:
    ~TextureAssembly() noexcept;

    TextureAssembly() noexcept;

    TextureAssembly(const TextureAssembly& a) noexcept;

    TextureAssembly(TextureAssembly&& a) noexcept;

    TextureAssembly& operator=(const TextureAssembly& a) noexcept;

    TextureAssembly& operator=(TextureAssembly&& a) noexcept;

    bool set_attribs(const TextureAttrib& texAttribs) noexcept;

    bool set_int_attrib(const tex_param_t param, const int attribVal) noexcept;

    bool set_float_attrib(const tex_param_t param, const float attribVal) noexcept;

    bool set_mipmap_attrib(const unsigned level = 0) noexcept;

    bool set_format_attrib(const pixel_format_t internalFormat = pixel_format_t::COLOR_FMT_DEFAULT_RGB) noexcept;
    
    bool set_size_attrib(
        const math::vec2i& size,
        const tex_type_t texType = tex_type_t::TEX_TYPE_2D,
        const tex_2d_type_t subType = tex_2d_type_t::TEX_SUBTYPE_2D
        ) noexcept;

    bool set_size_attrib(
        const math::vec3i& size,
        const tex_type_t texType = tex_type_t::TEX_TYPE_3D,
        const tex_3d_type_t subType = tex_3d_type_t::TEX_SUBTYPE_3D
        ) noexcept;

    void clear() noexcept;

    bool is_assembly_valid() const noexcept;

    bool assemble(Texture&, const void* const pData = nullptr) const noexcept;
};



} // end draw namespace
} // end ls namespace

#endif /* TEXTUREASSEMBLY_H */
