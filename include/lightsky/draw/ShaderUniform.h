/* 
 * File:   draw/ShaderUniform.h
 * Author: miles
 *
 * Created on July 7, 2015, 10:04 PM
 */

#ifndef __LS_DRAW_SHADER_UNIFORM_H__
#define __LS_DRAW_SHADER_UNIFORM_H__

#include <vector>

#include "lightsky/draw/setup.h"
#include "lightsky/draw/VertexAttrib.h"
#include "shaderProgram.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum vertex_data_t : GLenum;
class shaderProgram;

/*-----------------------------------------------------------------------------
 * Typedefs and Aliases
-----------------------------------------------------------------------------*/
typedef VertexAttrib ShaderUniform;

/*-----------------------------------------------------------------------------
 * Shader Uniform Functions
-----------------------------------------------------------------------------*/
/**------------------------------------
 * @brief Get information about an active uniform located in a shader.
 * 
 * @param prog
 * A constant reference to successfully compiled a ShaderProgram object.
 * 
 * @param index - indicates the index of the uniform to be queried.
 * 
 * @param varSize - indicates the number of elements contained within
 * the uniform. This will be a value of 1 for all variables that are not
 * arrays.
 * 
 * @param varType - used to determine the variable's data type; such as
 * an int, float, sampler, matrix, or sampler array.
 * 
 * @returns the name of the variable as it is known in the shader
 * source code.
 * 
-------------------------------------*/
std::string get_shader_uniform_name(
    const shaderProgram& prog,
    const GLint index,
    GLint* const outVarSize,
    GLenum* const outVarType
);

/**------------------------------------
 * @brief Retrieve a list of all shader uniform attributes.
 * 
 * @param prog
 * A constant reference to successfully compiled a ShaderProgram object.
 * 
 * @return A std::vector of std::string objects, containing the names of all
 * shader uniforms within the input program object.
-------------------------------------*/
std::vector<ShaderUniform> get_shader_uniforms(const shaderProgram& prog);

/**------------------------------------
 * @brief Get the location of a uniform variable.
 * 
 * @return GLint
 * A positive value to indicate the uniform's location in OpenGL or
 * -1 for an invalid uniform index.
-------------------------------------*/
inline GLint get_shader_uniform_location(const shaderProgram& sp, const GLchar* const name) {
    return glGetUniformLocation(sp.getId(), name);
}

/**------------------------------------
 * @brief Get the location of a uniform variable.
 * 
 * @return GLint
 * A positive value to indicate the uniform's location in OpenGL or
 * -1 for an invalid uniform index.
-------------------------------------*/
inline GLint get_shader_uniform_location(const shaderProgram& sp, const std::string& name) {
    return glGetUniformLocation(sp.getId(), name.c_str());
}

/**------------------------------------
 * @brief Set a single uniform integer variable
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, int val) {
    glUniform1i(uniformId, val);
}

/**------------------------------------
 * @brief Set two uniform integer variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, int val0, int val1) {
    glUniform2i(uniformId, val0, val1);
}

/**------------------------------------
 * @brief Set three uniform integer variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, int val0, int val1, int val2) {
    glUniform3i(uniformId, val0, val1, val2);
}

/**------------------------------------
 * @brief Set four uniform integer variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, int val0, int val1, int val2, int val3) {
    glUniform4i(uniformId, val0, val1, val2, val3);
}

/**------------------------------------
 * @brief Set a uniform 2d vector of integers
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec2i& val) {
    glUniform2iv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 3d vector of integers
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec3i& val) {
    glUniform3iv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 4d vector of integers
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec4i& val) {
    glUniform4iv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a single uniform unsigned int variable
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val) {
    glUniform1ui(uniformId, val);
}

/**------------------------------------
 * @brief Set two uniform unsigned int variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val0, unsigned val1) {
    glUniform2ui(uniformId, val0, val1);
}

/**------------------------------------
 * @brief Set three uniform unsigned int variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val0, unsigned val1, unsigned val2) {
    glUniform3ui(uniformId, val0, val1, val2);
}

/**------------------------------------
 * @brief Set four uniform unsigned int variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val0, unsigned val1, unsigned val2, unsigned val3) {
    glUniform4ui(uniformId, val0, val1, val2, val3);
}

/**------------------------------------
 * @brief Set a uniform 2d vector of unsigned ints
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec2ui& val) {
    glUniform2uiv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 3d vector of unsigned ints
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec3ui& val) {
    glUniform3uiv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 4d vector of unsigned ints
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec4ui& val) {
    glUniform4uiv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a single uniform float variable
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val) {
    glUniform1f(uniformId, val);
}

/**------------------------------------
 * @brief Set two uniform float variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val0, float val1) {
    glUniform2f(uniformId, val0, val1);
}

/**------------------------------------
 * @brief Set three uniform float variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val0, float val1, float val2) {
    glUniform3f(uniformId, val0, val1, val2);
}

/**------------------------------------
 * @brief Set four uniform float variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val0, float val1, float val2, float val3) {
    glUniform4f(uniformId, val0, val1, val2, val3);
}

/**------------------------------------
 * @brief Set a uniform 2d vector of floats
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec2& val) {
    glUniform2fv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 3d vector of floats
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec3& val) {
    glUniform3fv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 4d vector of floats
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec4& val) {
    glUniform4fv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 2d matrix
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::mat2& val, bool transpose) {
    glUniformMatrix2fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/**------------------------------------
 * @brief Set a uniform 3d matrix
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::mat3& val, bool transpose) {
    glUniformMatrix3fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/**------------------------------------
 * @brief Set a uniform 4d matrix
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::mat4& val, bool transpose) {
    glUniformMatrix4fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_SHADER_UNIFORM_H__ */

