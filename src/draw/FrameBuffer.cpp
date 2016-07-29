/*
 * File:   draw/framebuffer.cpp
 * Author: Miles Lacey
 *
 * Created on Jult 30, 2014, 10:02 PM
 */

#include <new>
#include <utility>

#include "ls/utils/Assertions.h"

#include "ls/draw/Setup.h"
#include "ls/draw/RenderBuffer.h"
#include "ls/draw/Texture.h"
#include "ls/draw/FrameBuffer.h"

namespace ls {
namespace draw {

/*-------------------------------------
    Destructor
-------------------------------------*/
FrameBuffer::~FrameBuffer() noexcept {
}

/*-------------------------------------
    constructor
-------------------------------------*/
FrameBuffer::FrameBuffer() noexcept :
    access {fbo_access_t::FBO_ACCESS_RW},
    gpuId {0},
    clearDepthVal {0.f},
    clearStencilVal {0},
    clearColorVal {color::blank},
    largestSize {0},
    numAttribs {0},
    attribs {nullptr}
{}

/*-------------------------------------
    Copy constructor
-------------------------------------*/
FrameBuffer::FrameBuffer(const FrameBuffer& fb) noexcept {
    *this = fb;
}

/*-------------------------------------
    Move constructor
-------------------------------------*/
FrameBuffer::FrameBuffer(FrameBuffer&& fb) noexcept {
    *this = std::move(fb);
}

/*-------------------------------------
    Copy operator
-------------------------------------*/
FrameBuffer& FrameBuffer::operator =(const FrameBuffer& fb) noexcept {
    utils::Pointer < FBOAttrib[] > tempAttribs {new(std::nothrow) FBOAttrib[fb.numAttribs]};

    if (tempAttribs) {
        access = fb.access;
        gpuId = fb.gpuId;
        clearDepthVal = fb.clearDepthVal;
        clearStencilVal = fb.clearStencilVal;
        clearColorVal = fb.clearColorVal;
        largestSize = fb.largestSize;
        numAttribs = fb.numAttribs;

        attribs.swap(tempAttribs);
        for (unsigned i = 0; i < numAttribs; ++i) {
            attribs[i] = fb.attribs[i];
        }
    }
    else {
        access = fbo_access_t::FBO_ACCESS_RW;
        gpuId = 0;
        clearDepthVal = 0.f;
        clearStencilVal = 0;
        clearColorVal = color::blank;
        largestSize = 0;
        numAttribs = 0;
    }

    return *this;
}

/*-------------------------------------
    Move operator
-------------------------------------*/
FrameBuffer& FrameBuffer::operator =(FrameBuffer&& fb) noexcept {
    access = fb.access;
    fb.access = FBO_ACCESS_RW;

    gpuId = fb.gpuId;
    fb.gpuId = 0;

    clearDepthVal = fb.clearDepthVal;
    fb.clearDepthVal = 0.f;

    clearStencilVal = fb.clearStencilVal;
    fb.clearStencilVal = 0;

    clearColorVal = std::move(fb.clearColorVal);
    fb.clearColorVal = color::blank;

    largestSize = std::move(fb.largestSize);

    numAttribs = fb.numAttribs;
    fb.numAttribs = 0;

    attribs = std::move(fb.attribs);

    return *this;
}

/*-------------------------------------
    Bind the default read/write buffer
-------------------------------------*/
void FrameBuffer::bind_default_framebuffer(const fbo_access_t access, const bool rasterize) noexcept {
    glBindFramebuffer(access, 0);
    LS_LOG_GL_ERR();

    if (access == fbo_access_t::FBO_ACCESS_R || fbo_access_t::FBO_ACCESS_RW) {
        glReadBuffer(rasterize ? GL_BACK : GL_NONE);
        LS_LOG_GL_ERR();
    }

    if ((access == fbo_access_t::FBO_ACCESS_W || fbo_access_t::FBO_ACCESS_RW) && !rasterize) {
        constexpr GLenum discardMode = GL_NONE;
        glDrawBuffers(1, &discardMode);
        LS_LOG_GL_ERR();
    }
}

/*-------------------------------------
    Termination
-------------------------------------*/
void FrameBuffer::terminate() noexcept {
    glDeleteFramebuffers(1, &gpuId);

    *this = FrameBuffer {};
}

/*-------------------------------------
    Set tattachmentsattachmentshe current draw targets to be used by this.
-------------------------------------*/
void FrameBuffer::set_draw_targets() const noexcept {
    fbo_attach_t attachments[fbo_attach_t::FBO_ATTACHMENT_MAX_COLORS] = {fbo_attach_t::FBO_ATTACHMENT_INVALID};

    unsigned numColorBuffers = 0;
    constexpr fbo_attach_t maxAttachment = (fbo_attach_t)(fbo_attach_t::FBO_ATTACHMENT_0 + fbo_attach_t::FBO_ATTACHMENT_MAX_COLORS);

    for (unsigned i = 0; i < numAttribs; ++i) {
        const FBOAttrib& attrib = attribs[i];
        const fbo_attach_t attachment = attrib.get_attach_type();

        if (attachment >= fbo_attach_t::FBO_ATTACHMENT_0 && attachment < maxAttachment) {
            attachments[numColorBuffers++] = attachment;
        }
    }

    set_draw_targets(numColorBuffers, attachments);
}

/*-------------------------------------
    Set the current read target to be used by this.
-------------------------------------*/
void FrameBuffer::set_read_target(const unsigned attribIndex) const noexcept {
    LS_DEBUG_ASSERT(attribIndex < numAttribs);

    constexpr fbo_attach_t maxAttachment = (fbo_attach_t)(fbo_attach_t::FBO_ATTACHMENT_0 + fbo_attach_t::FBO_ATTACHMENT_MAX_COLORS);
    const FBOAttrib& attrib = attribs[attribIndex];
    const fbo_attach_t attachment = attrib.get_attach_type();

    if (attachment >= fbo_attach_t::FBO_ATTACHMENT_0 && attachment < maxAttachment) {
        glReadBuffer(attachment);
    }
    else {
        glReadBuffer(GL_BACK);
    }
    
    LS_LOG_GL_ERR();
}

} // end draw namespace
} // end ls namespace
