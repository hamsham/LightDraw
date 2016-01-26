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
#include "VertexAttrib.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum class vertex_attrib_t : int;
struct VertexAttrib;

/*-----------------------------------------------------------------------------
 * Typedefs and Aliases
-----------------------------------------------------------------------------*/
typedef VertexAttrib ShaderUniform;

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
        
        /**
         * @brief Retrieve the meta-data about all shader uniform/attributes.
         * 
         * @param attribType
         * An enumeration which determines if the list of returned attributes should
         * contain Vertex attributes or Uniform attributes.
         * 
         * @return A std::vector of std::string objects, containing the names of all
         * shader attributes or uniforms within the input program object.
         */
        std::vector<VertexAttrib> get_attribs_impl(const vertex_attrib_t attribType = vertex_attrib_t::VERTEX_ATTRIB) const;

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
         * @param index
         * Indicates the index of the uniform to be queried.
         * 
         * @param varSize
         * Indicates the number of elements contained within the uniform. This
         * will be a value of 1 for all variables that are not arrays.
         * 
         * @param varType
         * Used to determine the variable's data type; such as an int, float,
         * sampler, matrix, or sampler array.
         * 
         * @returns the name of a vertex or uniform attribute as it is known in a GLSL
         * shader.
         * 
         */
        std::string get_attrib_name(
            const GLint index,
            GLint* const outVarSize,
            GLenum* const outVarType
        ) const;
        
        /**
         * @brief Retrieve the meta-data about all shader uniform/attributes.
         * 
         * @return A std::vector of std::string objects, containing the names
         * of all shader attributes within the input program object.
         */
        std::vector<VertexAttrib> get_attribs() const;

        /*---------------------------------------------------------------------
         * Shader Uniform Functions
        ---------------------------------------------------------------------*/
        /**
         * @brief Get information about an active uniform located in a shader.
         * 
         * @param index
         * Indicates the index of the uniform to be queried.
         * 
         * @param varSize
         * indicates the number of elements contained within the uniform. This
         * will be a value of 1 for all variables that are not arrays.
         * 
         * @param varType
         * Used to determine the variable's data type; such as an int, float,
         * sampler, matrix, or sampler array.
         * 
         * @returns the name of the variable as it is known in the shader
         * source code.
         * 
        */
        std::string get_uniform_name(
            const GLint index,
            GLint* const outVarSize,
            GLenum* const outVarType
        );

        /**
         * @brief Retrieve a list of all shader uniform attributes.
         * 
         * @return A std::vector of std::string objects, containing the names of all
         * shader uniforms within the input program object.
        */
        std::vector<ShaderUniform> get_uniforms() const;

        /**
         * @brief Get the location of a uniform variable.
         * 
         * @param name
         * A constant reference to an std::string object which contains the
         * name of a uniform to query within the currently active shader.
         * 
         * @return GLint
         * A positive value to indicate the uniform's location in OpenGL or
         * -1 for an invalid uniform index.
         */
        GLint get_uniform_location(const std::string& name) const;
        
        /**
         * Get the location of a uniform variable.
         * 
         * @param name
         * A constant pointer to a constant C-style string which contains the
         * name of a uniform to query within the currently active shader.
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
        std::string get_uniform_name(
            int index,
            GLint* const varSize,
            GLenum* const varType
        ) const;
        
        /*---------------------------------------------------------------------
         * Fragment Shader Info
        ---------------------------------------------------------------------*/
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
 * Get all shader uniform names
-------------------------------------*/
inline std::vector<ShaderUniform> ShaderProgram::get_attribs() const {
    return get_attribs_impl(vertex_attrib_t::VERTEX_ATTRIB);
}

/*-------------------------------------
    Get the location of a uniform variable.
-------------------------------------*/
inline GLint ShaderProgram::get_uniform_location(const GLchar* const name) const {
    return glGetUniformLocation(gpuId, name);
}

/*-------------------------------------
    Get the location of a uniform variable.
-------------------------------------*/
inline GLint ShaderProgram::get_uniform_location(const std::string& name) const {
    return glGetUniformLocation(gpuId, name.c_str());
}

/*-------------------------------------
 * Get all shader uniform names
-------------------------------------*/
inline std::vector<ShaderUniform> ShaderProgram::get_uniforms() const {
    return get_attribs_impl(vertex_attrib_t::UNIFORM_ATTRIB);
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

