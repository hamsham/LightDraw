/* 
 * File:   draw/texture.cpp
 * Author: Miles Lacey
 * 
 * Created on January 27, 2014, 8:35 PM
 */

#include "lightsky/draw/Texture.h"

namespace ls {
namespace draw {
        
/*-------------------------------------*
    Destructor
-------------------------------------*/
Texture::~Texture() {
    terminate();
}

/*-------------------------------------
    Constructor
-------------------------------------*/
Texture::Texture(tex_desc_t td) :
    dimensions{td},
    gpuId{0},
    slot{tex_slot_t::TEXTURE_SLOT_DEFAULT}
{}

/*-------------------------------------
    Copy Constructor
-------------------------------------*/
Texture::Texture(const Texture& t) :
    dimensions{t.dimensions},
    gpuId{t.gpuId},
    slot{t.slot}
{}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
Texture::Texture(Texture&& t) :
    dimensions{t.dimensions},
    gpuId{t.gpuId},
    slot{t.slot}
{
    t.gpuId = 0;
    t.slot = tex_slot_t::TEXTURE_SLOT_DEFAULT;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
Texture& Texture::operator=(const Texture& t) {
    dimensions = t.dimensions;
    gpuId = t.gpuId;
    slot = t.slot;
    
    return *this;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
Texture& Texture::operator=(Texture&& t) {
    LS_DEBUG_ASSERT(t.dimensions == dimensions);
    
    gpuId = t.gpuId;
    t.gpuId = 0;
    
    slot = t.slot;
    t.slot = tex_slot_t::TEXTURE_SLOT_DEFAULT;
    
    return *this;
}

/*-------------------------------------
    Create an OpenGL texture with no data.
-------------------------------------*/
bool Texture::init() {
    if (!gpuId) {
        glGenTextures(1, &gpuId);
        LOG_GL_ERR();
        
        if (gpuId == 0) {
            LS_LOG_ERR("Unable to generate a texture object");
            return false;
        }
    }
    
    return true;
}

/*-------------------------------------
    Load 1D Textures
-------------------------------------*/
bool Texture::init(pixel_format_t internalFormat, int size) {
    const pixel_layout_t layout = get_color_layout(internalFormat);
    return init(0, internalFormat, size, layout, color_type_t::COLOR_TYPE_DEFAULT, nullptr);
}

/*-------------------------------------
    Load 2D Textures
-------------------------------------*/
bool Texture::init(pixel_format_t internalFormat, const math::vec2i& size) {
    const pixel_layout_t layout = get_color_layout(internalFormat);
    return init(0, internalFormat, size, layout, color_type_t::COLOR_TYPE_DEFAULT, nullptr);
}

/*-------------------------------------
    Load 3D Textures
-------------------------------------*/
bool Texture::init(pixel_format_t internalFormat, const math::vec3i& size) {
    const pixel_layout_t layout = get_color_layout(internalFormat);
    return init(0, internalFormat, size, layout, color_type_t::COLOR_TYPE_DEFAULT, nullptr);
}

/*-------------------------------------
    Load 1D Textures
-------------------------------------*/
bool Texture::init(
    int             mipmapLevel,
    pixel_format_t  internalFormat,
    int             size,
    pixel_layout_t  format,
    color_type_t    dataType,
    void* const     data
) {
    if (!init()) {
        return false;
    }
    
    glBindTexture(dimensions, gpuId);
    LOG_GL_ERR();
    
    glTexImage2D(dimensions, mipmapLevel, internalFormat, size, 1, 0, format, dataType, data);
    LOG_GL_ERR();
    
    return true;
}

/*-------------------------------------
    Load 2D Textures
-------------------------------------*/
bool Texture::init(
    int              mipmapLevel,
    pixel_format_t   internalFormat,
    const math::vec2i& size,
    pixel_layout_t   format,
    color_type_t     dataType,
    void* const      data
) {
    if (!init()) {
        return false;
    }
    
    glBindTexture(dimensions, gpuId);
    LOG_GL_ERR();
    
    glTexImage2D(
        dimensions, mipmapLevel, internalFormat,
        size[0], size[1], 0, format, dataType, data
    );
    LOG_GL_ERR();
    
    return true;
}

/*-------------------------------------
    Load 3D Textures
-------------------------------------*/
bool Texture::init(
    int              mipmapLevel,
    pixel_format_t   internalFormat,
    const math::vec3i& size,
    pixel_layout_t   format,
    color_type_t     dataType,
    void* const      data
) {
    if (!init()) {
        return false;
    }
    
    glBindTexture(dimensions, gpuId);
    LOG_GL_ERR();
    
    glTexImage3D(
        dimensions, mipmapLevel, internalFormat,
        size[0], size[1], size[2], 0, format, dataType,
        data
    );
    LOG_GL_ERR();
    
    return true;
}

} // end draw namespace
} // end ls namespace
