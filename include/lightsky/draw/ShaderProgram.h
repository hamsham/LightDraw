/* 
 * File:   draw/shaderProgram.h
 * Author: Miles Lacey
 *
 * Created on January 21, 2014, 6:54 PM
 */

#ifndef __LS_DRAW_SHADER_PROGRAM_H__
#define	__LS_DRAW_SHADER_PROGRAM_H__

#include <string>
#include <vector>

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/ShaderObject.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum class vertex_attrib_t : int;
struct VertexAttrib;

/**----------------------------------------------------------------------------
 * @brief Shader Program
 * 
 * Represents a combination of OpenGL vertex, fragment, and geometry shader
 * objects.
-----------------------------------------------------------------------------*/
class ShaderProgram {
    private:
        /**
         * A handle to the GPU-side shader program within OpenGL.
         */
        GLuint gpuId = 0;

    public:
        /**
         * @brief Constructor
         */
        ShaderProgram();
        
        /**
         * @brief Copy Constructor -- Deleted
         */
        ShaderProgram(const ShaderProgram&) = delete;
        
        /**
         * @brief Move Constructor
         * 
         * Copy the handle to the shader object owned by the input argument,
         * reset the moved object's shader handle to 0.
         * 
         * @param sp
         * A, r-value reference to a shader program object to move into *this.
         */
        ShaderProgram(ShaderProgram&& sp);
        
        /**
         * @brief Copy Operator -- Deleted
         */
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        
        /**
         * @brief Move Operator
         * 
         * Move the values of the input argument and reset the moved object's
         * values to 0.
         * 
         * @param sp
         * A, r-value reference to a shader program object to move into *this.
         * 
         * @return A reference to *this
         */
        ShaderProgram& operator=(ShaderProgram&& sp);
        
        /**
         * @brief Destructor
         * 
         * Destroy this object and free any GPU memory it uses.
         */
        ~ShaderProgram();
        
        /**
         * Free all memory used by this shader object.
         */
        void terminate();
        
        /**
         * Attach a set of shader units to the program
         * 
         * @param vs
         * A vertex shader unit that should be attached to *this.
         * 
         * @param fs
         * A fragment shader unit that should be attached to *this.
         * 
         * @return TRUE if the input shader units were successfully attached to
         * *this.
         */
        bool init(const vertexShader& vs, const fragmentShader& fs);
        
        /**
         * Link the attached shader units to this and create an entire GPU-side
         * shader program.
         * 
         * @return TRUE if *this object was successfully linked with its
         * attached shader units.
         */
        bool link();
        
        /**
         * @brief Retrieve the OpenGL-assigned ID associated with the shader
         * binary used by *this.
         * 
         * @return An unsigned integral type, identifying the shader program
         * used by OpenGL.
         */
        unsigned gpu_id() const;
        
        /**
         * Bind this program to the current context
         */
        void bind() const;
        
        /**
         * Unbind this program from the context.
         */
        void unbind() const;
        
        /**
         * Bind a vertex attribute to a shader.
         * 
         * @param index
         * An unsigned integral type which specifies the vertex array location
         * of an attachment to bind.
         * 
         * @param name
         * The exact name of the vertex array attribute in *this to bind.
         */
        void bind_attrib_location(GLuint index, const GLchar* const name) const;
        
        /**
         * Get the location of a vertex attribute
         * 
         * @param name
         * The exact name of the vertex array attribute in *this whose index
         * location should be retrieved.
         * 
         * @return GLint
         * A positive value to indicate the attribute's location in OpenGL or
         * -1 for an invalid index.
         */
        GLint get_attrib_location(const GLchar* const name) const;
        
        /**
         * @brief Get information about an active uniform located in a shader.
         * 
         * @param prog
         * A constant reference to successfully compiled a ShaderProgram object.
         * 
         * @param attribType
         * An enumeration which determines if the returned attribute should contain a
         * Vertex attribute or Uniform attribute.
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
         * @returns the name of a vertex or uniform attribute as it is known in a GLSL
         * shader.
         * 
         */
        std::string get_attrib_name(
            const vertex_attrib_t attribType,
            const GLint index,
            GLint* const outVarSize,
            GLenum* const outVarType
        ) const;
        
        /**
         * @brief Retrieve the meta-data about all shader uniform/attributes.
         * 
         * @param prog
         * A constant reference to successfully compiled a ShaderProgram object.
         * 
         * @param attribType
         * An enumeration which determines if the list of returned attributes should
         * contain Vertex attributes or Uniform attributes.
         * 
         * @return A std::vector of std::string objects, containing the names of all
         * shader attributes or uniforms within the input program object.
         */
        std::vector<VertexAttrib> get_attribs(const vertex_attrib_t attribType) const;
        
        /**
         * Get the location of a uniform variable.
         * 
         * @return GLint
         * A positive value to indicate the uniform's location in OpenGL or
         * -1 for an invalid uniform index.
         */
        GLint get_uniform_location(const GLchar* const name) const;
        
        /**
         * Get information about an active uniform located in a shader.
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
         */
        std::string get_uniform_name(int index, GLint* const varSize, GLenum* const varType) const;
        
        /**
         * Set a single uniform integer variable
         */
        void set_uniform_value(GLint uniformId, int val) const;
        
        /**
         * Set two uniform integer variables
         */
        void set_uniform_value(GLint uniformId, int val0, int val1) const;
        
        /**
         * Set three uniform integer variables
         */
        void set_uniform_value(GLint uniformId, int val0, int val1, int val2) const;
        
        /**
         * Set four uniform integer variables
         */
        void set_uniform_value(GLint uniformId, int val0, int val1, int val2, int val3) const;
        
        /**
         * Set a uniform 2d vector of integers
         */
        void set_uniform_value(GLint uniformId, const math::vec2i& val) const;
        
        /**
         * Set a uniform 3d vector of integers
         */
        void set_uniform_value(GLint uniformId, const math::vec3i& val) const;
        
        /**
         * Set a uniform 4d vector of integers
         */
        void set_uniform_value(GLint uniformId, const math::vec4i& val) const;
        
        /**
         * Set a single uniform unsigned int variable
         */
        void set_uniform_value(GLint uniformId, unsigned val) const;
        
        /**
         * Set two uniform unsigned int variables
         */
        void set_uniform_value(GLint uniformId, unsigned val0, unsigned val1) const;
        
        /**
         * Set three uniform unsigned int variables
         */
        void set_uniform_value(GLint uniformId, unsigned val0, unsigned val1, unsigned val2) const;
        
        /**
         * Set four uniform unsigned int variables
         */
        void set_uniform_value(GLint uniformId, unsigned val0, unsigned val1, unsigned val2, unsigned val3) const;
        
        /**
         * Set a uniform 2d vector of unsigned ints
         */
        void set_uniform_value(GLint uniformId, const math::vec2ui& val) const;
        
        /**
         * Set a uniform 3d vector of unsigned ints
         */
        void set_uniform_value(GLint uniformId, const math::vec3ui& val) const;
        
        /**
         * Set a uniform 4d vector of unsigned ints
         */
        void set_uniform_value(GLint uniformId, const math::vec4ui& val) const;
        
        /**
         * Set a single uniform float variable
         */
        void set_uniform_value(GLint uniformId, float val) const;
        
        /**
         * Set two uniform float variables
         */
        void set_uniform_value(GLint uniformId, float val0, float val1) const;
        
        /**
         * Set three uniform float variables
         */
        void set_uniform_value(GLint uniformId, float val0, float val1, float val2) const;
        
        /**
         * Set four uniform float variables
         */
        void set_uniform_value(GLint uniformId, float val0, float val1, float val2, float val3) const;
        
        /**
         * Set a uniform 2d vector of floats
         */
        void set_uniform_value(GLint uniformId, const math::vec2& val) const;
        
        /**
         * Set a uniform 3d vector of floats
         */
        void set_uniform_value(GLint uniformId, const math::vec3& val) const;
        
        /**
         * Set a uniform 4d vector of floats
         */
        void set_uniform_value(GLint uniformId, const math::vec4& val) const;
        
        /**
         * Set a uniform 2d matrix
         */
        void set_uniform_value(GLint uniformId, const math::mat2& val, bool transpose = false) const;
        
        /**
         * Set a uniform 3d matrix
         */
        void set_uniform_value(GLint uniformId, const math::mat3& val, bool transpose = false) const;
        
        /**
         * Set a uniform 4d matrix
         */
        void set_uniform_value(GLint uniformId, const math::mat4& val, bool transpose = false) const;
        
        /**
         * Query the bindings of color numbers to user-defined varying out variables
         */
        GLint get_frag_data_location(const GLchar* const name) const;
};

/*-------------------------------------
    Get the OpenGL ID used by *this.
-------------------------------------*/
inline unsigned ShaderProgram::gpu_id() const {
    return gpuId;
}

/*-------------------------------------
    Bind this program to the current context
-------------------------------------*/
inline void ShaderProgram::bind() const {
    glUseProgram(gpuId);
}

/*-------------------------------------
    Unbind this program from the context.
-------------------------------------*/
inline void ShaderProgram::unbind() const {
    glUseProgram(0);
}

/*-------------------------------------
    Bind a vertex attribute to a shader
-------------------------------------*/
inline void ShaderProgram::bind_attrib_location(GLuint index, const GLchar* const name) const {
    glBindAttribLocation(gpuId, index, name);
}

/*-------------------------------------
    Get the location of a vert6ex attribute
-------------------------------------*/
inline GLint ShaderProgram::get_attrib_location(const GLchar* const name) const {
    return glGetAttribLocation(gpuId, name);
}

/*-------------------------------------
    Get the location of a uniform variable.
    
    @return GLint
    A positive value to indicate the uniform's location in OpenGL or
    -1 for an invalid uniform index.
-------------------------------------*/
inline GLint ShaderProgram::get_uniform_location(const GLchar* const name) const {
    return glGetUniformLocation(gpuId, name);
}

/*-------------------------------------
    Set a single uniform integer variable
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, int val) const {
    glUniform1i(uniformId, val);
}

/*-------------------------------------
    Set two uniform integer variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, int val0, int val1) const {
    glUniform2i(uniformId, val0, val1);
}

/*-------------------------------------
    Set three uniform integer variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, int val0, int val1, int val2) const {
    glUniform3i(uniformId, val0, val1, val2);
}

/*-------------------------------------
    Set four uniform integer variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, int val0, int val1, int val2, int val3) const {
    glUniform4i(uniformId, val0, val1, val2, val3);
}

/*-------------------------------------
    Set a uniform 2d vector of integers
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec2i& val) const {
    glUniform2iv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 3d vector of integers
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec3i& val) const {
    glUniform3iv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 4d vector of integers
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec4i& val) const {
    glUniform4iv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a single uniform unsigned int variable
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, unsigned val) const {
    glUniform1ui(uniformId, val);
}

/*-------------------------------------
    Set two uniform unsigned int variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, unsigned val0, unsigned val1) const {
    glUniform2ui(uniformId, val0, val1);
}

/*-------------------------------------
    Set three uniform unsigned int variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, unsigned val0, unsigned val1, unsigned val2) const {
    glUniform3ui(uniformId, val0, val1, val2);
}

/*-------------------------------------
    Set four uniform unsigned int variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, unsigned val0, unsigned val1, unsigned val2, unsigned val3) const {
    glUniform4ui(uniformId, val0, val1, val2, val3);
}

/*-------------------------------------
    Set a uniform 2d vector of unsigned ints
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec2ui& val) const {
    glUniform2uiv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 3d vector of unsigned ints
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec3ui& val) const {
    glUniform3uiv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 4d vector of unsigned ints
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec4ui& val) const {
    glUniform4uiv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a single uniform float variable
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, float val) const {
    glUniform1f(uniformId, val);
}

/*-------------------------------------
    Set two uniform float variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, float val0, float val1) const {
    glUniform2f(uniformId, val0, val1);
}

/*-------------------------------------
    Set three uniform float variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, float val0, float val1, float val2) const {
    glUniform3f(uniformId, val0, val1, val2);
}

/*-------------------------------------
    Set four uniform float variables
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, float val0, float val1, float val2, float val3) const {
    glUniform4f(uniformId, val0, val1, val2, val3);
}

/*-------------------------------------
    Set a uniform 2d vector of floats
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec2& val) const {
    glUniform2fv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 3d vector of floats
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec3& val) const {
    glUniform3fv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 4d vector of floats
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::vec4& val) const {
    glUniform4fv(uniformId, 1, val.v);
}

/*-------------------------------------
    Set a uniform 2d matrix
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::mat2& val, bool transpose) const {
    glUniformMatrix2fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/*-------------------------------------
    Set a uniform 3d matrix
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::mat3& val, bool transpose) const {
    glUniformMatrix3fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/*-------------------------------------
    Set a uniform 4d matrix
-------------------------------------*/
inline void ShaderProgram::set_uniform_value(GLint uniformId, const math::mat4& val, bool transpose) const {
    glUniformMatrix4fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/*-------------------------------------
    Query the bindings of color numbers to user-defined varying out variables
-------------------------------------*/
inline GLint ShaderProgram::get_frag_data_location(const GLchar* const name) const {
    return glGetFragDataLocation(gpuId, name);
}

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_SHADER_PROGRAM_H__ */

