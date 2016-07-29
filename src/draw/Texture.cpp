/*
 * File:   draw/texture.cpp
 * Author: Miles Lacey
 *
 * Created on January 27, 2014, 8:35 PM
 */

#include "ls/utils/Assertions.h"

#include "ls/draw/Texture.h"
#include "ls/draw/PixelBuffer.h"

namespace ls {
namespace draw {

/*-------------------------------------*
    Destructor
-------------------------------------*/
Texture::~Texture() noexcept {
}

/*-------------------------------------
    Constructor
-------------------------------------*/
Texture::Texture() noexcept :
    gpuId {0},
    texType {tex_type_t::TEX_TYPE_2D},
    size {0},
    attribs {}
{}

/*-------------------------------------
    Copy Constructor
-------------------------------------*/
Texture::Texture(const Texture& t) noexcept :
    gpuId {t.gpuId},
    texType {t.texType},
    size {t.size},
    attribs {t.attribs}
{}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
Texture::Texture(Texture&& t) noexcept :
    gpuId {t.gpuId},
    texType {t.texType},
    size {t.size},
    attribs {std::move(t.attribs)}
{
    t.gpuId = 0;
    t.texType = tex_type_t::TEX_TYPE_2D;
    t.size = 0;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
Texture& Texture::operator =(const Texture& t) noexcept {
    gpuId = t.gpuId;
    texType = t.texType;
    size = t.size;
    attribs = t.attribs;

    return *this;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
Texture& Texture::operator =(Texture&& t) noexcept {
    gpuId = t.gpuId;
    t.gpuId = 0;

    texType = t.texType;
    t.texType = tex_type_t::TEX_TYPE_2D;

    size = t.size;
    t.size = 0;

    attribs = std::move(t.attribs);

    return *this;
}

/*-------------------------------------
    Modify the internal data of a texture.
-------------------------------------*/
void Texture::modify(
    const GLenum type,
    const int offset,
    const int modifySize,
    const void* const data,
    const int level
    ) noexcept {
    const GLenum format = attribs.get_basic_format();
    const GLenum dataType = attribs.get_color_type();
    glTexSubImage2D(type, level, offset, 0, modifySize, 0, format, dataType, data);
    LS_LOG_GL_ERR();
}

/*-------------------------------------
    Modify the internal data of a texture using a PBO
-------------------------------------*/
void Texture::modify(
    const GLenum type,
    const int offset,
    const int modifySize,
    const PixelBuffer& pbo,
    const int level
    ) noexcept {
    modify(type, offset, modifySize, (void*)draw::get_buffer_map_offset(pbo.get_type()), level);
}

/*-------------------------------------
    Modify the internal data of a texture.
-------------------------------------*/
void Texture::modify(
    const tex_2d_type_t type,
    const math::vec2i& offset,
    const math::vec2i& modifySize,
    const void* const data,
    const int level
    ) noexcept {
    const GLenum format = attribs.get_basic_format();
    const GLenum dataType = attribs.get_color_type();
    glTexSubImage2D(type, level, offset[0], offset[1], modifySize[0], modifySize[1], format, dataType, data);
    LS_LOG_GL_ERR();
}

/*-------------------------------------
    Modify the internal data of a texture using a PBO
-------------------------------------*/
void Texture::modify(
    const tex_2d_type_t type,
    const math::vec2i& offset,
    const math::vec2i& modifySize,
    const PixelBuffer& pbo,
    const int level
    ) noexcept {
    modify(type, offset, modifySize, (void*)draw::get_buffer_map_offset(pbo.get_type()), level);
}

/*-------------------------------------
    Modify the internal data of a texture.
-------------------------------------*/
void Texture::modify(
    const tex_3d_type_t type,
    const math::vec3i& offset,
    const math::vec3i& modifySize,
    const void* const data,
    const int level
    ) noexcept {
    const GLenum format = attribs.get_basic_format();
    const GLenum dataType = attribs.get_color_type();
    glTexSubImage3D(type, level, offset[0], offset[1], offset[2], modifySize[0], modifySize[1], modifySize[2], format, dataType, data);
    LS_LOG_GL_ERR();
}

/*-------------------------------------
    Modify the internal data of a texture using a PBO
-------------------------------------*/
void Texture::modify(
    const tex_3d_type_t type,
    const math::vec3i& offset,
    const math::vec3i& modifySize,
    const PixelBuffer& pbo,
    const int level
    ) noexcept {
    modify(type, offset, modifySize, (void*)draw::get_buffer_map_offset(pbo.get_type()), level);
}

/*-------------------------------------
    Release all memory referenced by *this.
-------------------------------------*/
void Texture::terminate() noexcept {
    glDeleteTextures(1, &gpuId);
    gpuId = 0;
    texType = tex_type_t::TEX_TYPE_2D;
    size = 0;
    attribs.reset_attribs();
}

} // end draw namespace
} // end ls namespace
