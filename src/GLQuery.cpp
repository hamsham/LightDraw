
#include "lightsky/draw/GLQuery.h"



namespace ls
{

/*-------------------------------------
 * Retrieve a bool from OpenGL.
-------------------------------------*/
bool draw::get_gl_bool(const GLenum param) noexcept
{
    return get_gl_int(param) == GL_TRUE;
}

/*-------------------------------------
 * Retrieve an integer from OpenGL.
-------------------------------------*/
GLint draw::get_gl_int(const GLenum param) noexcept
{
    GLint retVal;
    glGetIntegerv(param, &retVal);
    return retVal;
}

/*-------------------------------------
 * Retrieve an unsigned integer from OpenGL.
-------------------------------------*/
GLuint draw::get_gl_uint(const GLenum param) noexcept
{
    return (GLuint)get_gl_int(param);
}

/*-------------------------------------
 * Retrieve a 64-bit integer from OpenGL.
-------------------------------------*/
GLint64 draw::get_gl_int64(const GLenum param) noexcept
{
    GLint64 retVal;
    glGetInteger64v(param, &retVal);
    return (GLint64)retVal;
}

/*-------------------------------------
 * Retrieve a 64-bit unsigned integer from OpenGL.
-------------------------------------*/
GLuint64 draw::get_gl_uint64(const GLenum param) noexcept
{
    return (GLuint64)get_gl_int64(param);
}

/*-------------------------------------
 * Retrieve a float from OpenGL.
-------------------------------------*/
GLfloat draw::get_gl_float(const GLenum param) noexcept
{
    GLfloat retVal;
    glGetFloatv(param, &retVal);
    return retVal;
}
} // end ls namespace
