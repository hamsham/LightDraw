/* 
 * File:   draw/ShaderUniform.cpp
 * Author: miles
 * 
 * Created on July 7, 2015, 10:04 PM
 */

#include <utility>

#include "lightsky/draw/shaderProgram.h"
#include "lightsky/draw/ShaderUniform.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Shader Uniform Functions
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Get active uniform info from a shader.
-------------------------------------*/
std::string get_shader_uniform_name(
    const shaderProgram& prog,
    const GLint index,
    GLint* const outVarSize,
    GLenum* const outVarType
) {
    return prog.getAttribName(
        vertex_attrib_t::UNIFORM_ATTRIB,
        index,
        outVarSize,
        outVarType
    );
}

/*-------------------------------------
 * Get all shader uniform attributes.
-------------------------------------*/
std::vector<ShaderUniform> get_shader_uniforms(const shaderProgram& prog) {
    return prog.getAttribs(vertex_attrib_t::UNIFORM_ATTRIB);
}

} // end draw namespace
} // end ls namespace
