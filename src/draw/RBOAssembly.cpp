
/*
 * File:   RBOAssembly.cpp
 * Author: miles
 *
 * Created on March 3, 2016, 9:52 PM
 */

#include <utility>

#include "ls/draw/RBOAssembly.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Assembler of Render Buffer Objects
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
RBOAssembly::~RBOAssembly() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
RBOAssembly::RBOAssembly() noexcept :
size {
    0
},
attribs {}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
RBOAssembly::RBOAssembly(const RBOAssembly& rbo) noexcept :
size {
    rbo.size
},
attribs {rbo.attribs}
{
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
RBOAssembly::RBOAssembly(RBOAssembly&& a) noexcept :
size {
    std::move(a.size)
},
attribs {std::move(a.attribs)}
{
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
RBOAssembly& RBOAssembly::operator =(const RBOAssembly& a) noexcept {
    size = a.size;
    attribs = a.attribs;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
RBOAssembly& RBOAssembly::operator =(RBOAssembly&& a) noexcept {
    size = std::move(a.size);
    attribs = std::move(a.attribs);

    return *this;
}

/*-------------------------------------
-------------------------------------*/
bool RBOAssembly::set_attribs(const RBOAttrib& a) noexcept {
    attribs = a;
    return true;
}

/*-------------------------------------
-------------------------------------*/
bool RBOAssembly::set_format_attrib(const rbo_format_t internalFormat) noexcept {
    attribs.set_internal_format(internalFormat);

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool RBOAssembly::set_size_attrib(const math::vec2i& texSize) noexcept {
    size[0] = texSize[0];
    size[1] = texSize[1];

    return true;
}

/*-------------------------------------
-------------------------------------*/
void RBOAssembly::clear() noexcept {
    size = math::vec2i {0};
    attribs.reset_attribs();
}

/*-------------------------------------
-------------------------------------*/
bool RBOAssembly::is_assembly_valid() const noexcept {
    LS_LOG_MSG("Validating a render buffer assembly");

    LS_LOG_MSG("\tVerifying Renderbuffer size.");
    if (size[0] <= 0 || size[1] <= 0) {
        LS_LOG_ERR("\t\tInvalid render buffer size!\n");
        return false;
    }
    LS_LOG_MSG("\t\tDone.");

    LS_LOG_MSG("\tVerifying renderbuffer data format.");
    const rbo_format_t internalFormat = attribs.get_internal_format();

    switch (internalFormat) {
        case RBO_FMT_DEPTH_STENCIL_24_8:
        case RBO_FMT_DEPTH_STENCIL_32_8:
        case RBO_FMT_DEPTH_16:
        case RBO_FMT_DEPTH_24:
        case RBO_FMT_DEPTH_32:
        case RBO_FMT_STENCIL_8:
        case RBO_FMT_R_8:
        case RBO_FMT_R_8I:
        case RBO_FMT_R_8U:
        case RBO_FMT_R_16I:
        case RBO_FMT_R_16U:
        case RBO_FMT_R_32I:
        case RBO_FMT_R_32U:
        case RBO_FMT_RG_8:
        case RBO_FMT_RG_8I:
        case RBO_FMT_RG_8U:
        case RBO_FMT_RG_16I:
        case RBO_FMT_RG_16U:
        case RBO_FMT_RG_32I:
        case RBO_FMT_RG_32U:
        case RBO_FMT_RGB_8:
        case RBO_FMT_RGB_565:
        case RBO_FMT_RGBA_8:
        case RBO_FMT_RGBA_8I:
        case RBO_FMT_RGBA_8U:
        case RBO_FMT_RGBA_16I:
        case RBO_FMT_RGBA_16U:
        case RBO_FMT_RGBA_32I:
        case RBO_FMT_RGBA_32U:
        case RBO_FMT_RGBA_4:
        case RBO_FMT_RGBA_5_1:
        case RBO_FMT_RGBA_10_2:
        case RBO_FMT_RGBA_10_2U:
        case RBO_FMT_SRGBA_8:
            break;
        default:
            LS_LOG_ERR("\t\tInvalid renderbuffer data format: ", internalFormat, ".\n");
            return false;
    }

    LS_LOG_MSG("\t\tSuccessfully validated a render buffer assembly.\n");

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool RBOAssembly::assemble(RenderBuffer& rbo) const noexcept {
    if (!is_assembly_valid()) {
        return false;
    }

    LS_LOG_MSG("Attempting to assemble a render buffer object.");
    GLuint gpuId = 0;

    if (!rbo.gpu_id()) {
        LS_LOG_MSG("\tGenerating a handle to a new render buffer object on the GPU.");
        glGenRenderbuffers(1, &gpuId);
        LS_LOG_GL_ERR();

        if (!gpuId) {
            LS_LOG_ERR("\tFailed to generate a render buffer object on the GPU.");
            return false;
        }

        LS_LOG_MSG("\t\tDone. Successfully generated a render buffer on the GPU: ", gpuId);
    }
    else {
        gpuId = rbo.gpu_id();
        LS_LOG_MSG("\tAssembling data for a preexisting render buffer: ", gpuId);
    }

    LS_LOG_MSG("\tAllocating space for RBO data using render buffer ", gpuId, '.');
    glBindRenderbuffer(GL_RENDERBUFFER, gpuId);
    glRenderbufferStorage(GL_RENDERBUFFER, attribs.get_internal_format(), size[0], size[1]);
    LS_LOG_GL_ERR();
    LS_LOG_MSG("\t\tDone.");

    LS_LOG_MSG("\tApplying attributes.");
    rbo.gpuId = gpuId;
    rbo.size = size;
    rbo.attribs = attribs;
    LS_LOG_MSG("\t\tDone.");

    LS_LOG_MSG(
        "\tSuccessfully assembled a render buffer object:",
        "\n\t\tGPU ID:        ", rbo.gpuId,
        "\n\t\tPixel Format:  ", rbo.attribs.get_internal_format(),
        "\n\t\tDimensions:    ", rbo.size[0], " x ", rbo.size[1],
        "\n"
        );

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return true;
}



} // end draw namespace
} // end ls namespace
