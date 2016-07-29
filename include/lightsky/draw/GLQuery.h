
/*
 * File:   GlUtils.h
 * Author: miles
 *
 * Created on February 29, 2016, 9:55 AM
 */

#ifndef __LS_DRAW_GL_UTILS_H__
#define __LS_DRAW_GL_UTILS_H__

#include "lightsky/draw/Setup.h"

namespace ls {
namespace draw {



/**
 * Retrieve a boolean value from the OpenGL state machine.
 *
 * @param param
 * An OpenGL enumeration, describing the value which should be retrieved from
 * an OpenGL "glGet*" function.
 *
 * @return A boolean value retrieved from the OpenGL state machine.
 */
bool get_gl_bool(const GLenum param) noexcept;

/**
 * Retrieve an integer value from the OpenGL state machine.
 *
 * @param param
 * An OpenGL enumeration, describing the value which should be retrieved from
 * an OpenGL "glGet*" function.
 *
 * @return A integer value retrieved from the OpenGL state machine.
 */
GLint get_gl_int(const GLenum param) noexcept;

/**
 * Retrieve an unsigned integer value from the OpenGL state machine.
 *
 * @param param
 * An OpenGL enumeration, describing the value which should be retrieved from
 * an OpenGL "glGet*" function.
 *
 * @return An unsigned integer value retrieved from the OpenGL state machine.
 */
GLuint get_gl_uint(const GLenum param) noexcept;

/**
 * Retrieve a 64-bit integer value from the OpenGL state machine.
 *
 * @param param
 * An OpenGL enumeration, describing the value which should be retrieved from
 * an OpenGL "glGet*" function.
 *
 * @return A 64-bit integer value retrieved from the OpenGL state machine.
 */
GLint64 get_gl_int64(const GLenum param) noexcept;

/**
 * Retrieve a 64-bit unsined int value from the OpenGL state machine.
 *
 * @param param
 * An OpenGL enumeration, describing the value which should be retrieved from
 * an OpenGL "glGet*" function.
 *
 * @return A 64-bit unsined int value retrieved from the OpenGL state machine.
 */
GLuint64 get_gl_uint64(const GLenum param) noexcept;

/**
 * Retrieve a float value from the OpenGL state machine.
 *
 * @param param
 * An OpenGL enumeration, describing the value which should be retrieved from
 * an OpenGL "glGet*" function.
 *
 * @return A float value retrieved from the OpenGL state machine.
 */
GLfloat get_gl_float(const GLenum param) noexcept;



} // end draw namespace
} // end ls namespace



#endif /* __LS_DRAW_GL_UTILS_H__ */
