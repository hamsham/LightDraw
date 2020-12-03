
/*
 * File:   FBOAttrib.h
 * Author: miles
 *
 * Created on March 4, 2016, 4:40 PM
 */

#ifndef __LS_DRAW_FBO_ATTRIB_H__
#define __LS_DRAW_FBO_ATTRIB_H__

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Color.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class Texture;

class RenderBuffer;



/**----------------------------------------------------------------------------
 * Draw buffers for fbo render targets
-----------------------------------------------------------------------------*/
enum fbo_attach_t
{
    FBO_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0,
    FBO_ATTACHMENT_1 = GL_COLOR_ATTACHMENT1,
    FBO_ATTACHMENT_2 = GL_COLOR_ATTACHMENT2,
    FBO_ATTACHMENT_3 = GL_COLOR_ATTACHMENT3,

    FBO_ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
    FBO_ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
    FBO_ATTACHMENT_DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,

    FBO_ATTACHMENT_MAX = 6,
    FBO_ATTACHMENT_MAX_COLORS = 4, // OpenGL-defined for minimum attachment count.
    FBO_ATTACHMENT_INVALID = GL_INVALID_ENUM,
};



/**----------------------------------------------------------------------------
 * Texture targets that can be bound to an FBO
-----------------------------------------------------------------------------*/
enum fbo_texture_t
{
    FBO_2D_TEXTURE = GL_TEXTURE_2D,

    FBO_3D_TEXTURE = GL_TEXTURE_3D,

    FBO_POS_X_TEXTURE = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    FBO_NEG_X_TEXTURE = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    FBO_POS_Y_TEXTURE = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    FBO_NEG_Y_TEXTURE = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    FBO_POS_Z_TEXTURE = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    FBO_NEG_Z_TEXTURE = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};



/**----------------------------------------------------------------------------
 * Draw/Read access control for framebuffer objects.
-----------------------------------------------------------------------------*/
enum fbo_access_t
{
    FBO_ACCESS_RW = GL_FRAMEBUFFER,
    FBO_ACCESS_W = GL_DRAW_FRAMEBUFFER,
    FBO_ACCESS_R = GL_READ_FRAMEBUFFER
};



/**----------------------------------------------------------------------------
 * Framebuffer status codes
-----------------------------------------------------------------------------*/
enum fbo_status_t
{
    FBO_COMPLETE = GL_FRAMEBUFFER_COMPLETE,
    FBO_UNDEFINED = GL_FRAMEBUFFER_UNDEFINED,
    FBO_INCOMPLETE_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
    FBO_INCOMPLETE_MISSING_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
    FBO_UNSUPPORTED = GL_FRAMEBUFFER_UNSUPPORTED,
    FBO_INCOMPLETE_MULTISAMPLE = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
};



/**----------------------------------------------------------------------------
 * Bitmasks that can help determine what aspects of a framebuffer should be
 * modified during read/draw calls.
-----------------------------------------------------------------------------*/
enum fbo_mask_t
{
    FBO_DEPTH_BIT = GL_DEPTH_BUFFER_BIT,
    FBO_STENCIL_BIT = GL_STENCIL_BUFFER_BIT,
    FBO_COLOR_BIT = GL_COLOR_BUFFER_BIT,

    FBO_DEPTH_STENCIL_BIT = FBO_DEPTH_BIT | FBO_STENCIL_BIT,
    FBO_DEPTH_STENCIL_COLOR_BIT = FBO_DEPTH_BIT | FBO_STENCIL_BIT | FBO_COLOR_BIT,
    FBO_DEPTH_COLOR_BIT = FBO_DEPTH_BIT | FBO_COLOR_BIT,
    FBO_STENCIL_COLOR_BIT = FBO_STENCIL_BIT | FBO_COLOR_BIT,
};



/**----------------------------------------------------------------------------
 * Enumeration to help determine what type of render target is contained
 * within an FBOAttrib structure.
-----------------------------------------------------------------------------*/
enum fbo_target_t
{
    FBO_TARGET_TEXTURE,
    FBO_TARGET_RENDERBUFFER,
    FBO_TARGET_INVALID,
};



/**----------------------------------------------------------------------------
 * @brief Framebuffer Attributes
 *
 * While not directly affecting state, FBO attribs can provide meta-information
 * about framebuffers to help client code determine what to do with them.
-----------------------------------------------------------------------------*/
class FBOAttrib
{
  private:
    fbo_target_t targetType;

    fbo_attach_t attachType;

    fbo_texture_t texType;

    unsigned texLevel;

    unsigned texLayer;

    void* pTargetData; // subject to reinterpret_cast. beware

  public:
    ~FBOAttrib() noexcept;

    FBOAttrib() noexcept;

    FBOAttrib(const FBOAttrib&) noexcept;

    FBOAttrib(FBOAttrib&&) noexcept;

    FBOAttrib& operator=(const FBOAttrib&) noexcept;

    FBOAttrib& operator=(FBOAttrib&&) noexcept;

    void reset_attribs() noexcept;

    fbo_target_t get_target_type() const noexcept;

    void set_target_type(
        Texture& t,
        const fbo_texture_t type = fbo_texture_t::FBO_2D_TEXTURE,
        const unsigned mipmapLevel = 0,
        const unsigned arrayLayer = 0
    ) noexcept;

    void set_target_type(RenderBuffer& r) noexcept;

    fbo_texture_t get_texture_type() const noexcept;

    unsigned get_texture_level() const noexcept;

    unsigned get_texture_layer() const noexcept;

    fbo_attach_t get_attach_type() const noexcept;

    void set_attach_type(const fbo_attach_t a) noexcept;

    Texture* get_texture() const noexcept;

    RenderBuffer* get_renderbuffer() const noexcept;
};

/*-------------------------------------
-------------------------------------*/
inline fbo_texture_t FBOAttrib::get_texture_type() const noexcept
{
    return texType;
}

/*-------------------------------------
-------------------------------------*/
inline unsigned FBOAttrib::get_texture_level() const noexcept
{
    return texLevel;
}

/*-------------------------------------
-------------------------------------*/
inline unsigned FBOAttrib::get_texture_layer() const noexcept
{
    return texLayer;
}

/*-------------------------------------
-------------------------------------*/
inline fbo_target_t FBOAttrib::get_target_type() const noexcept
{
    return targetType;
}

/*-------------------------------------
-------------------------------------*/
inline fbo_attach_t FBOAttrib::get_attach_type() const noexcept
{
    return attachType;
}

/*-------------------------------------
-------------------------------------*/
inline void FBOAttrib::set_attach_type(const fbo_attach_t a) noexcept
{
    attachType = a;
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_FBO_ATTRIB_H__ */
