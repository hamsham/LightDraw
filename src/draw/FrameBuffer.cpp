/* 
 * File:   draw/framebuffer.cpp
 * Author: Miles Lacey
 * 
 * Created on Jult 30, 2014, 10:02 PM
 */

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/RenderBuffer.h"
#include "lightsky/draw/FrameBuffer.h"

namespace ls {
namespace draw {

/*-------------------------------------
    FBO Error handling
-------------------------------------*/
void FrameBuffer::print_status(const FrameBuffer& fbo) {
    LOG_GL_ERR();
    
    const fbo_status_t err = get_status(fbo);
    
    switch (err) {
        case FBO_COMPLETE:
            LS_LOG_MSG("Framebuffer ", fbo.gpu_id(), " completed.");
            break;
        case FBO_UNDEFINED:
            LS_LOG_ERR("Framebuffer ", fbo.gpu_id(), " undefined.");
            break;
        case FBO_INCOMPLETE_ATTACHMENT:
            LS_LOG_ERR("Framebuffer ", fbo.gpu_id(), " contains an incomplete attachment.");
            break;
        case FBO_INCOMPLETE_MISSING_ATTACHMENT:
            LS_LOG_ERR("Framebuffer ", fbo.gpu_id(), " is missing attachments.");
            break;
        case FBO_UNSUPPORTED:
            LS_LOG_ERR("Framebuffer ", fbo.gpu_id(), " uses unsupported formats.");
            break;
        case FBO_INCOMPLETE_MULTISAMPLE:
            LS_LOG_ERR("Framebuffer ", fbo.gpu_id(), " uses alternating samples.");
            break;
    }
}

/*-------------------------------------
    Move constructor
-------------------------------------*/
FrameBuffer::FrameBuffer(FrameBuffer&& fb) :
    access{fb.access},
    gpuId{fb.gpuId}
{
    fb.access = FBO_ACCESS_RW;
    fb.gpuId = 0;
}

/*-------------------------------------
    Destructor
-------------------------------------*/
FrameBuffer::~FrameBuffer() {
    terminate();
}

/*-------------------------------------
    constructor
-------------------------------------*/
FrameBuffer::FrameBuffer() {
}

/*-------------------------------------
    Move operator
-------------------------------------*/
FrameBuffer& FrameBuffer::operator=(FrameBuffer&& fb) {
    access = fb.access;
    fb.access = FBO_ACCESS_RW;
    
    gpuId = fb.gpuId;
    fb.gpuId = 0;
    
    return *this;
}

/*-------------------------------------
    Initialization
-------------------------------------*/
bool FrameBuffer::init() {
    terminate();
    glGenFramebuffers(1, &gpuId);
    return gpuId != 0;
}

/*-------------------------------------
    Termination
-------------------------------------*/
void FrameBuffer::terminate() {
    access = FBO_ACCESS_RW;
    glDeleteFramebuffers(1, &gpuId);
    gpuId = 0;
}

/*-------------------------------------
    Attach a texture to the currently bound framebuffer
-------------------------------------*/
void FrameBuffer::attach_render_target(
    fbo_attach_t attachment,
    texture_target_t target,
    const Texture& tex,
    int mipmapLevel,
    int layer
) {
    const tex_desc_t desc = tex.get_texture_type();
    const unsigned texId = tex.gpu_id();
    
    if (desc == TEX_DESC_2D) {
        glFramebufferTexture2D(access, attachment, target, texId, mipmapLevel);
    }
    else if (desc == TEX_DESC_3D || desc == TEX_DESC_2D_ARRAY || desc == TEX_DESC_CUBE) {
        glFramebufferTextureLayer(access, attachment, texId, mipmapLevel, layer);
    }
    else {
        LS_LOG_ERR("Attempting to load an unsupported texture type into a framebuffer.");
    }
    LOG_GL_ERR();
}

/*-------------------------------------
    Attach a texture to the currently bound framebuffer
-------------------------------------*/
void FrameBuffer::attach_render_target(fbo_attach_t attachment, const Texture& tex) {
    const tex_desc_t desc = tex.get_texture_type();
    
    if (desc == TEX_DESC_2D) {
        attach_render_target(attachment, texture_target_t::FBO_2D_TARGET, tex);
        return;
    }
    else if (desc == TEX_DESC_3D || desc == TEX_DESC_2D_ARRAY || desc == TEX_DESC_CUBE) {
        attach_render_target(attachment, texture_target_t::FBO_3D_TARGET, tex);
        return;
    }
    else {
        LS_LOG_ERR("Attempting to load an unsupported texture type into a framebuffer.");
    }
}

/*-------------------------------------
    Attach a renderbuffer to the currently bound framebuffer
-------------------------------------*/
void FrameBuffer::attach_render_target(fbo_attach_t attachment, const RenderBuffer& rbo) {
    const unsigned rboId = rbo.gpu_id();
    glFramebufferRenderbuffer(access, attachment, GL_RENDERBUFFER, rboId);
    LOG_GL_ERR();
}

} // end draw namespace
} // end ls namespace
