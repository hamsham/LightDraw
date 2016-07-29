
/*
 * File:   FBOAssembly.cpp
 * Author: miles
 *
 * Created on March 4, 2016, 4:30 PM
 */

#include <new>

#include "ls/setup/Macros.h"

#include "ls/utils/Assertions.h"
#include "ls/utils/Log.h"

#include "ls/draw/Color.h"
#include "ls/draw/Texture.h"
#include "ls/draw/RenderBuffer.h"
#include "ls/draw/FrameBuffer.h"
#include "ls/draw/FBOAssembly.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
FBOAssembly::~FBOAssembly() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
FBOAssembly::FBOAssembly() noexcept {
    clear();
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
FBOAssembly::FBOAssembly(const FBOAssembly& a) noexcept {
    *this = a;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
FBOAssembly::FBOAssembly(FBOAssembly&& a) noexcept {
    *this = std::move(a);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
FBOAssembly& FBOAssembly::operator =(const FBOAssembly& a) noexcept {
    for (unsigned i = 0; i < LS_ARRAY_SIZE(a.attribs); ++i) {
        attribs[i] = a.attribs[i];
    }

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
FBOAssembly& FBOAssembly::operator =(FBOAssembly&& a) noexcept {
    for (unsigned i = 0; i < LS_ARRAY_SIZE(a.attribs); ++i) {
        attribs[i] = a.attribs[i];
        a.attribs[i].reset_attribs();
    }

    return *this;
}

/*-------------------------------------
 * Pack all internal attributes to the front of the attrib array.
-------------------------------------*/
void FBOAssembly::pack_vbo_attribs() noexcept {
    LS_LOG_MSG("Repacking FBO Assembly attributes.");

    // can't use the "is_attrib_valid()" function here. That function also
    // checks for valid attachment types, which is not necessary here.
    const auto isVerifiable = [&](const FBOAttrib & a)->bool {
        if (a.get_texture() != nullptr && a.get_texture()->is_valid()) {
            return true;
        }

        if (a.get_renderbuffer() != nullptr && a.get_renderbuffer()->is_valid()) {
            return true;
        }

        return false;
    };

    for (unsigned i = 0; i < get_num_attribs(); ++i) {
        FBOAttrib& current = attribs[i];

        // don't bother modifying valid attribs.
        if (isVerifiable(current)) {
            continue;
        }

        for (unsigned j = i + 1; j < get_num_attribs(); ++j) {
            FBOAttrib& next = attribs[j];

            if (isVerifiable(next) == false) {
                continue;
            }

            LS_LOG_MSG("\tMoving FBO Attrib ", j, " to index ", i, '.');

            current = std::move(next);

            break;
        }
    }

    LS_LOG_MSG("\tDone.\n");

}

/*-------------------------------------
 * Validate the render target in an attribute
-------------------------------------*/
bool FBOAssembly::attrib_has_targets(const FBOAttrib& a) noexcept {
    return (a.get_texture() != nullptr && a.get_texture()->is_valid())
        || (a.get_renderbuffer() != nullptr && a.get_renderbuffer()->is_valid());
}

/*-------------------------------------
 * Validate the color attachment in an attribute
-------------------------------------*/
bool FBOAssembly::attrib_has_attachments(const FBOAttrib& a) noexcept {
    const fbo_attach_t attachment = a.get_attach_type();

    return (attachment == fbo_attach_t::FBO_ATTACHMENT_DEPTH)
        || (attachment == fbo_attach_t::FBO_ATTACHMENT_DEPTH_STENCIL)
        || (attachment == fbo_attach_t::FBO_ATTACHMENT_STENCIL)
        || (attachment >= fbo_attach_t::FBO_ATTACHMENT_0 && attachment < fbo_attach_t::FBO_ATTACHMENT_0 + draw::get_max_fbo_attachments());
}

/*-------------------------------------
    Attach a texture to the currently bound framebuffer
-------------------------------------*/
bool FBOAssembly::attach_target_texture(const FBOAttrib& attrib) const noexcept {
    const fbo_attach_t attach = attrib.get_attach_type();
    const unsigned level = attrib.get_texture_level();
    const Texture& tex = *attrib.get_texture();
    const tex_type_t texType = tex.get_texture_type();
    const unsigned texId = tex.gpu_id();
    bool ret = true;

    tex.bind();
    LS_LOG_GL_ERR();

    if (texType == tex_type_t::TEX_TYPE_2D || texType == tex_type_t::TEX_TYPE_CUBE) {
        glFramebufferTexture2D(fbo_access_t::FBO_ACCESS_RW, attach, attrib.get_texture_type(), texId, level);
        LS_LOG_GL_ERR();
    }
    else if (texType == tex_type_t::TEX_TYPE_3D || texType == tex_type_t::TEX_TYPE_2D_ARRAY) {
        glFramebufferTextureLayer(fbo_access_t::FBO_ACCESS_RW, attach, texId, level, attrib.get_texture_layer());
        LS_LOG_GL_ERR();
    }
    else {
        // sunspots made this branch happen.
        LS_DEBUG_ASSERT(false);
        ret = false;
    }

    tex.unbind();
    LS_LOG_GL_ERR();

    return ret;
}

/*-------------------------------------
    Attach a renderbuffer to the currently bound framebuffer
-------------------------------------*/
bool FBOAssembly::attach_target_renderbuffer(const FBOAttrib& attrib) const noexcept {
    const RenderBuffer& rbo = *attrib.get_renderbuffer();
    const unsigned rboId = rbo.gpu_id();

    rbo.bind();
    LS_LOG_GL_ERR();

    glFramebufferRenderbuffer(fbo_access_t::FBO_ACCESS_RW, attrib.get_attach_type(), GL_RENDERBUFFER, rboId);
    LS_LOG_GL_ERR();

    rbo.unbind();
    LS_LOG_GL_ERR();

    return true;
}

/*-------------------------------------
 * Check the framebuffer's completion
-------------------------------------*/
bool FBOAssembly::check_fbo_completion(const unsigned fboId) const noexcept {
    const GLenum err = glCheckFramebufferStatus(fbo_access_t::FBO_ACCESS_RW);
    LS_LOG_GL_ERR();

    switch (err) {
        case FBO_COMPLETE:
            break;

        case FBO_UNDEFINED:
            LS_LOG_ERR("\t\tFramebuffer ", fboId, " undefined.");
            return false;

        case FBO_INCOMPLETE_ATTACHMENT:
            LS_LOG_ERR("\t\tFramebuffer ", fboId, " contains an incomplete attachment.");
            return false;

        case FBO_INCOMPLETE_MISSING_ATTACHMENT:
            LS_LOG_ERR("\t\tFramebuffer ", fboId, " is missing attachments.");
            return false;

        case FBO_UNSUPPORTED:
            LS_LOG_ERR("\t\tFramebuffer ", fboId, " uses unsupported formats.");
            return false;

        case FBO_INCOMPLETE_MULTISAMPLE:
            LS_LOG_ERR("\t\tFramebuffer ", fboId, " uses alternating samples.");
            return false;

        default:
            LS_LOG_ERR("\t\tFramebuffer ", fboId, " has an unknown completion status: ", err);
            return false;
    }

    LS_LOG_MSG("\t\tDone. Successfully attached all targets to the framebuffer ", fboId, '.');

    return true;
}

/*-------------------------------------
 * Get the total number of verifiable attributes
-------------------------------------*/
unsigned FBOAssembly::get_num_verifiable_attribs() const noexcept {
    unsigned i = 0;

    for (; i < get_num_attribs(); ++i) {
        const FBOAttrib& a = attribs[i];

        if (!attrib_has_targets(a) && !attrib_has_attachments(a)) {
            return i;
        }
    }

    return i;
}

/*-------------------------------------
 * Assign a texture as an FBO Attribute
-------------------------------------*/
bool FBOAssembly::set_attrib(const unsigned attribIndex, Texture& attachment, const fbo_attach_t type) noexcept {
    if (attribIndex >= get_num_attribs()) {
        LS_DEBUG_ASSERT(attribIndex >= get_num_attribs());
        return false;
    }

    FBOAttrib temp;
    temp.set_target_type(attachment);
    temp.set_attach_type(type);

    return set_attrib(attribIndex, temp);
}

/*-------------------------------------
 * Assign a render buffer as an FBO Attribute
-------------------------------------*/
bool FBOAssembly::set_attrib(const unsigned attribIndex, RenderBuffer& attachment, const fbo_attach_t type) noexcept {
    if (attribIndex >= get_num_attribs()) {
        LS_DEBUG_ASSERT(attribIndex >= get_num_attribs());
        return false;
    }

    FBOAttrib temp;
    temp.set_target_type(attachment);
    temp.set_attach_type(type);

    return set_attrib(attribIndex, temp);
}

/*-------------------------------------
 * Assign a pre-assembled attrib as an attribute
-------------------------------------*/
bool FBOAssembly::set_attrib(const unsigned attribIndex, const FBOAttrib& attachment) noexcept {
    if (attribIndex >= get_num_attribs()) {
        LS_DEBUG_ASSERT(attribIndex >= get_num_attribs());
        return false;
    }

    if (!attrib_has_attachments(attachment) || !attrib_has_targets(attachment)) {
        return false;
    }
    else {
        attribs[attribIndex] = attachment;
    }

    pack_vbo_attribs();

    return true;
}

/*-------------------------------------
 * Retrieve an attrib
-------------------------------------*/
const FBOAttrib& FBOAssembly::get_attrib(const unsigned attribIndex) const noexcept {
    LS_ASSERT(attribIndex < get_num_attribs());

    return attribs[attribIndex];
}

/*-------------------------------------
 * Retrieve an attrib's attachment type
-------------------------------------*/
fbo_attach_t FBOAssembly::get_attrib_attach_type(const unsigned attribIndex) const noexcept {
    if (attribIndex >= get_num_attribs()) {
        LS_DEBUG_ASSERT(attribIndex >= get_num_attribs());
        return fbo_attach_t::FBO_ATTACHMENT_INVALID;
    }

    if (!is_attrib_valid(attribIndex)) {
        return fbo_attach_t::FBO_ATTACHMENT_INVALID;
    }

    return attribs[attribIndex].get_attach_type();
}

/*-------------------------------------
 * Set an attrib's attachment type
-------------------------------------*/
bool FBOAssembly::set_attrib_attach_type(const unsigned attribIndex, const fbo_attach_t type) noexcept {
    if (attribIndex >= get_num_attribs()) {
        LS_DEBUG_ASSERT(attribIndex >= get_num_attribs());
        return false;
    }
    else {
        attribs[attribIndex].set_attach_type(type);
    }

    return true;
}

/*-------------------------------------
 * Get an attrib's attachment type
-------------------------------------*/
fbo_target_t FBOAssembly::get_attrib_target_type(const unsigned attribIndex) const noexcept {
    if (attribIndex >= get_num_attribs()) {
        LS_DEBUG_ASSERT(attribIndex >= get_num_attribs());
        return fbo_target_t::FBO_TARGET_INVALID;
    }

    return attribs[attribIndex].get_target_type();
}

/*-------------------------------------
 * Check to see if an attribute can be used at all
-------------------------------------*/
bool FBOAssembly::is_attrib_valid(const unsigned attribIndex) const noexcept {
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());

    const FBOAttrib& attrib = attribs[attribIndex];
    return attrib_has_targets(attrib) && attrib_has_attachments(attrib);
}

/*-------------------------------------
 * Remove all data from *this.
-------------------------------------*/
void FBOAssembly::clear() noexcept {
    for (FBOAttrib& attrib : attribs) {
        attrib = FBOAttrib {};
    }
}

/*-------------------------------------
 * Validate the current assembly
-------------------------------------*/
bool FBOAssembly::is_assembly_valid() const noexcept {
    // using first FBO_ATTACHMENT_MAX_COLORS for set bits and the last bit to
    // indicate a failure.
    uint32_t validatedColors = 0x0;
    constexpr unsigned failedColor = 1u << draw::get_max_fbo_attachments();
    unsigned depthCount = 0;
    unsigned stencilCount = 0;
    bool ret = true;

    LS_LOG_MSG("Validating an FBO Assembly.");

    const unsigned availableAttribs = get_num_verifiable_attribs();

    if (availableAttribs < 1) {
        LS_LOG_ERR("\tNo valid attributes are currently available in an FBO Assembly.\n");
        return false;
    }

    for (unsigned i = 0; i < availableAttribs; ++i) {
        const FBOAttrib& attrib = attribs[i];
        const fbo_attach_t attachment = attrib.get_attach_type();

        if (attachment == fbo_attach_t::FBO_ATTACHMENT_DEPTH) {
            ++depthCount;

            if (depthCount > 1) {
                LS_LOG_ERR("\tDuplicate depth attachment found in an FBO Assembly at attrib index ", i, '.');
                ret = false;
            }

            continue;
        }

        if (attachment == fbo_attach_t::FBO_ATTACHMENT_DEPTH_STENCIL) {
            ++depthCount;
            ++stencilCount;

            if (depthCount > 1) {
                LS_LOG_ERR("\tDuplicate depth attachment found in an FBO Assembly at attrib index ", i, '.');
                ret = false;
            }

            if (stencilCount > 1) {
                LS_LOG_ERR("\tDuplicate stencil attachment found in an FBO Assembly at attrib index ", i, '.');
                ret = false;
            }

            continue;
        }

        if (attachment == fbo_attach_t::FBO_ATTACHMENT_STENCIL) {
            ++stencilCount;
            if (stencilCount > 1) {
                LS_LOG_ERR("\tDuplicate stencil attachment found in an FBO Assembly at attrib index ", i, '.');
                ret = false;
            }
            continue;
        }

        // see if the current attachment is within the range of maximum color attachments
        // Get the enumeration of the highest color attachment + 1
        constexpr unsigned maxColAttachment = fbo_attach_t::FBO_ATTACHMENT_0 + draw::get_max_fbo_attachments();

        if (attachment >= fbo_attach_t::FBO_ATTACHMENT_0 && attachment < maxColAttachment) {
            unsigned colorBit = 1 << (attachment - fbo_attach_t::FBO_ATTACHMENT_0);

            if (validatedColors & colorBit) {
                LS_LOG_ERR("\tDuplicate color attachment found in an FBO Assembly at attrib index ", i, '.');
                validatedColors |= failedColor;
                ret = false;
            }
            else {
                validatedColors |= colorBit;
            }

            continue;
        }

        LS_LOG_ERR("\tInvalid FBO attachment located at index ", i, ". Unknown attachment type: ", attachment);
        ret = false;
    }

    if (depthCount < 1) {
        LS_LOG_ERR("\tNo depth buffer is currently available in an FBO assembly.");
        ret = false;
    }

    if (ret == true) {
        LS_LOG_MSG(
            "\tSuccessfully validated an FBO assembly:",
            "\n\t\tDepth Attachments:   ", depthCount,
            "\n\t\tStencil Attachments: ", stencilCount,
            "\n\t\tColor Attachments:   ", math::count_set_bits(validatedColors),
            '\n'
            );
    }
    else {
        LS_LOG_ERR("\tFailed to validate an FBO assembly.\n");
    }

    return ret;
}

/*-------------------------------------
 * Assemble a framebuffer object using all of the internal attachments.
-------------------------------------*/
bool FBOAssembly::assemble(FrameBuffer& fbo) const noexcept {
    if (fbo.gpu_id() != 0) {
        LS_LOG_ERR("Attempted to assemble a preexisting FBO object.\n");
        return false;
    }

    if (!is_assembly_valid()) {
        return false;
    }

    LS_LOG_MSG("Assembling a FrameBuffer object.");

    GLuint fboId = 0;
    glGenFramebuffers(1, &fboId);
    LS_LOG_GL_ERR();

    if (!fboId) {
        LS_LOG_ERR("\tFailed to create a new framebuffer during assembly.\n");
        return false;
    }

    glBindFramebuffer(fbo_access_t::FBO_ACCESS_RW, fboId);

    const unsigned attribCount = get_num_verifiable_attribs();
    math::vec3i size {0, 0, 1};

    for (unsigned i = 0; i < attribCount; ++i) {
        const FBOAttrib& attrib = attribs[i];
        bool didAttach = false;

        if (attrib.get_target_type() == fbo_target_t::FBO_TARGET_TEXTURE) {
            didAttach = attach_target_texture(attrib);

            const math::vec3i& texSize = attrib.get_texture()->get_size();
            size[0] = math::max(size[0], texSize[0]);
            size[1] = math::max(size[1], texSize[1]);
            size[2] = math::max(size[2], texSize[2]);
        }
        else {
            didAttach = attach_target_renderbuffer(attrib);

            const math::vec2i& texSize = attrib.get_renderbuffer()->get_size();
            size[0] = math::max(size[0], texSize[0]);
            size[1] = math::max(size[1], texSize[1]);
        }

        if (!didAttach) {
            LS_LOG_ERR("\tAn error occurred while attempting to attach FBOAttrib ", i, " to a framebuffer.\n");
            glDeleteFramebuffers(1, &fboId);
            return false;
        }
    }

    LS_LOG_MSG("\tQuerying OpenGL to check FBO completion.");
    if (!check_fbo_completion(fboId)) {
        glDeleteFramebuffers(1, &fboId);
        LS_LOG_GL_ERR();
        return false;
    }

    glBindFramebuffer(fbo_access_t::FBO_ACCESS_RW, 0);
    LS_LOG_GL_ERR();

    LS_LOG_MSG("\tCopying attributes from an FBO assembly into a new framebuffer object.");
    utils::Pointer < FBOAttrib[] > tempAttribs {new(std::nothrow) FBOAttrib[attribCount]};
    if (!tempAttribs) {
        LS_LOG_ERR("\t\tAn error occurred while allocating room in a FBO for attribute information.\n");
        glDeleteFramebuffers(1, &fboId);
        return false;
    }
    else {
        for (unsigned i = 0; i < attribCount; ++i) {
            tempAttribs[i] = attribs[i];
        }
        LS_LOG_MSG("\t\tDone. Copied ", attribCount, " attributes into a new framebuffer object.");
    }

    fbo.access = FBO_ACCESS_RW;
    fbo.gpuId = fboId;
    fbo.clearDepthVal = 0.f;
    fbo.clearStencilVal = 0;
    fbo.clearColorVal = color::blank;
    fbo.largestSize = size;
    fbo.numAttribs = attribCount;
    fbo.attribs.swap(tempAttribs);

    LS_LOG_MSG(
        "\tSuccessfully assembled a framebuffer object:",
        "\n\t\tGPU ID:              ", fbo.gpuId,
        "\n\t\tTotal Attachments:   ", attribCount,
        "\n\t\tDimensions:          ", size[0], " x ", size[1], " x ", size[2],
        '\n'
        );

    return true;
}



} // end draw namespace
} // end ls namespace
