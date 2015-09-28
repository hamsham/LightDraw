/* 
 * File:   draw/setup.cpp
 * Author: Miles Lacey
 *
 * Created on November 15, 2013, 8:28 PM
 */

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * OpenGL Errors
-------------------------------------*/
#ifdef LS_DEBUG
void printGlError(int line, const char* file) {
    GLenum errorCode = glGetError();
    
    while (errorCode != GL_NO_ERROR) {
        switch(errorCode) {
            case GL_INVALID_ENUM:
                LS_LOG_ERR("OpenGL Error: GL_INVALID_ENUM On line ", line, " of file ", file, '\n');
                break;
            case GL_INVALID_VALUE:
                LS_LOG_ERR("OpenGL Error: GL_INVALID_VALUE On line ", line, " of file ", file, '\n');
                break;
            case GL_INVALID_OPERATION:
                LS_LOG_ERR("OpenGL Error: GL_INVALID_OPERATION On line ", line, " of file ", file, '\n');
                break;
            case GL_OUT_OF_MEMORY:
                LS_LOG_ERR("OpenGL Error: GL_OUT_OF_MEMORY On line ", line, " of file ", file, '\n');
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                LS_LOG_ERR("OpenGL Error: GL_INVALID_FRAMEBUFFER_OPERATION On line ", line, " of file ", file, '\n');
                break;
        }
        
        errorCode = glGetError();
    }
}
#endif

} // end draw namespace
} // end ls namespace
