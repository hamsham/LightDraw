
#include "lightsky/draw/renderbuffer.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
renderbuffer::~renderbuffer() {
    terminate();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
renderbuffer::renderbuffer() :
    rboId{0}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
renderbuffer::renderbuffer(renderbuffer&& rbo) :
    rboId{rbo.rboId}
{
    rbo.rboId = 0;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
renderbuffer& renderbuffer::operator =(renderbuffer&& rbo) {
    rboId = rbo.rboId;
    rbo.rboId = 0;

    return *this;
}

/*-------------------------------------
 * Initialize a renderbuffer
-------------------------------------*/
bool renderbuffer::init() {
    if (!rboId) {
        glGenRenderbuffers(1, &rboId);
        LOG_GL_ERR();

        if (rboId == 0) {
            LS_LOG_ERR("Unable to generate a renderbuffer object");
            return false;
        }
    }

    return true;
}

/*-------------------------------------
 * Initialize a renderbuffer with storage
-------------------------------------*/
bool renderbuffer::init(rbo_format_t internalFormat, const math::vec2i& size) {
    // render buffers cannot be resized unless first deleted.
    terminate();

    if (!init()) {
        return false;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    LOG_GL_ERR();

    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size[0], size[1]);
    LOG_GL_ERR();

    return true;
}

/*-------------------------------------
 * Release resources
-------------------------------------*/
void renderbuffer::terminate() {
    if (rboId) {
        glDeleteRenderbuffers(1, &rboId);
        rboId = 0;
    }
}

} // end draw namespace
} // end ls namespace
