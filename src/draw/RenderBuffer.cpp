
#include "lightsky/draw/RenderBuffer.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
RenderBuffer::~RenderBuffer() {
    terminate();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
RenderBuffer::RenderBuffer() :
    gpuId{0}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
RenderBuffer::RenderBuffer(RenderBuffer&& rbo) :
    gpuId{rbo.gpuId}
{
    rbo.gpuId = 0;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
RenderBuffer& RenderBuffer::operator =(RenderBuffer&& rbo) {
    gpuId = rbo.gpuId;
    rbo.gpuId = 0;

    return *this;
}

/*-------------------------------------
 * Initialize a renderbuffer
-------------------------------------*/
bool RenderBuffer::init() {
    if (!gpuId) {
        glGenRenderbuffers(1, &gpuId);
        LS_LOG_GL_ERR();

        if (gpuId == 0) {
            LS_LOG_ERR("Unable to generate a renderbuffer object");
            return false;
        }
    }

    return true;
}

/*-------------------------------------
 * Initialize a renderbuffer with storage
-------------------------------------*/
bool RenderBuffer::init(rbo_format_t internalFormat, const math::vec2i& size) {
    // render buffers cannot be resized unless first deleted.
    terminate();

    if (!init()) {
        return false;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, gpuId);
    LS_LOG_GL_ERR();

    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size[0], size[1]);
    LS_LOG_GL_ERR();

    return true;
}

/*-------------------------------------
 * Release resources
-------------------------------------*/
void RenderBuffer::terminate() {
    if (gpuId) {
        glDeleteRenderbuffers(1, &gpuId);
        gpuId = 0;
    }
}

} // end draw namespace
} // end ls namespace
