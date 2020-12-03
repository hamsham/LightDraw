/*
 * File:   draw/shaderProgram.h
 * Author: Miles Lacey
 *
 * Created on January 21, 2014, 6:54 PM
 */

#ifndef __LS_DRAW_SHADER_PROGRAM_H__
#define __LS_DRAW_SHADER_PROGRAM_H__

#include <string>
#include <vector>

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/VAOAttrib.h" // vertex_attrib_t
#include "lightsky/draw/ShaderBlockAttrib.h"
#include "lightsky/draw/ShaderAttribArray.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum class vertex_attrib_t : int;
enum shader_stage_t : GLint;

class ShaderObject;

class ShaderProgramAssembly;

class UniformBuffer;

class VAOAttrib;



/**----------------------------------------------------------------------------
 * @brief Shader Program
 *
 * Represents a combination of OpenGL vertex, fragment, and geometry shader
 * objects.
-----------------------------------------------------------------------------*/
class ShaderProgram
{
    friend class ShaderProgramAssembly;

  private:
    /**
     * A handle to the GPU-side shader program within OpenGL.
     */
    GLuint gpuId;

    unsigned numUniformBlocks;

    /**
     * Array of CPU-side references to uniform blocks in *this shader.
     */
    utils::Pointer<ShaderBlockAttrib[]> uniformBlocks;

    /**
     * Member to hold all meta-info for the program uniforms.
     */
    ShaderAttribArray uniforms;

    /**
     * Member to hold all meta-info for the vertex shader inputs.
     */
    ShaderAttribArray vertAttribs;

    /**
     * Member to hold all meta-info for the vertex shader inputs.
     */
    ShaderAttribArray fragAttribs;

  public:
    /**
     * @brief Destructor
     *
     * Destroy this object and free any CPU memory it uses. No GPU-side
     * data will be deleted unless "terminate()".
     */
    ~ShaderProgram() noexcept;

    /**
     * @brief Constructor
     */
    ShaderProgram() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     */
    ShaderProgram(const ShaderProgram&) noexcept;

    /**
     * @brief Move Constructor
     *
     * Move all data from the input parameter into *this without performing
     * any copies. All member variables within the input parameter will be
     * reset to their defaults.
     *
     * @param sp
     * An r-value reference to a shader program object to move into *this.
     */
    ShaderProgram(ShaderProgram&& sp) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    ShaderProgram& operator=(const ShaderProgram&) noexcept;

    /**
     * @brief Move Operator
     *
     * Move all data from the input parameter into *this without performing
     * any copies. All member variables within the input parameter will be
     * reset to their defaults.
     *
     * @param sp
     * An r-value reference to a shader program object to move into *this.
     *
     * @return A reference to *this
     */
    ShaderProgram& operator=(ShaderProgram&& sp) noexcept;

    /**
     * Free all memory used by this shader object.
     */
    void terminate() noexcept;

    /**
     * @brief Retrieve the OpenGL-assigned ID associated with the shader
     * binary used by *this.
     *
     * @return An unsigned integral type, identifying the shader program
     * used by OpenGL.
     */
    unsigned gpu_id() const noexcept;

    /**
     * Determine if *this object represents a valid OpenGL shader program which
     * can be rendered to.
     *
     * @return TRUE if *this can be used as a management container for an OpenGL
     * shader program, FALSE if not.
     */
    bool is_valid() const noexcept;

    /**
     * Bind this program to the current context
     */
    void bind() const noexcept;

    /**
     * Unbind this program from the context.
     */
    void unbind() const noexcept;

    /**
     * Retrieve an array of all attributes which can be passed into a
     * vertex shader.
     *
     * The returned ShaderAttribArray will only contain values if the
     * current shader has successfully been linked.
     *
     * @return A constant reference to a ShaderAttribArray object which
     * represents all input vertex shader attributes found after linking.
     */
    const ShaderAttribArray& get_vertex_attribs() const noexcept;

    /**
     * Retrieve an array of all attributes which can output from a fragment
     * shader.
     *
     * The returned ShaderAttribArray will only contain values if the
     * current shader has successfully been linked.
     *
     * @return A constant reference to a ShaderAttribArray object which
     * represents all output fragment shader attributes found after
     * linking.
     */
    const ShaderAttribArray& get_fragment_attribs() const noexcept;

    /**
     * Retrieve the number of uniform blocks active within *this Shader
     * program.
     * 
     * @return The current number of uniform blocks in the Shader program
     * represented by *this.
     */
    unsigned get_num_uniform_blocks() const noexcept;

    /**
     * Retrieve a list of all currently active Uniform Blocks contained within
     * *this.
     * 
     * @return A reference to the ShaderBlockAttrib array used by *this for
     * referencing uniform blocks in the shader represented by *this.
     */
    const utils::Pointer<ShaderBlockAttrib[]>& get_uniform_blocks() const noexcept;

    /**
     * @brief Retrieve a list of all shader uniform attributes.
     *
     * @return A ShaderAttribArray containing a list of all uniforms which
     * were found in the current shader after linking *this with a valid
     * vertex and fragment shader.
     */
    const ShaderAttribArray& get_uniforms() const noexcept;

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
    GLint get_uniform_location(const std::string& name) const noexcept;

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
    GLint get_uniform_location(const GLchar* const name) const noexcept;

    /**
     * Query the bindings of color numbers to user-defined varying out variables
     *
     * @param name
     * The name of the fragment shader output attribute to be queried.
     *
     * @return An GLint containing the output index of the attribute which was
     * found by "name". A value of -1 is returned if an object by the name of
     * "name" doesn't exist.
     */
    GLint get_frag_data_location(const GLchar* const name) const noexcept;

    /**
     * Retrieve the ID of a currently attached shader.
     *
     * @param shaderType
     * A value from the shader_stage_t enumeration, representing the type
     * of shader to query *this for and have it's ID returned.
     *
     * @return The ID of a shader attached to this at the "shaderType"
     * binding point.
     */
    GLuint get_attached_shader_id(const shader_stage_t shaderType) const noexcept;

    /**
     * Retrieve the index of a uniform block which matches the attributes in
     * *this ShaderProgram's array of uniform attributes.
     * 
     * @param blockName
     * A constant pointer to a constant c-style string which will be used to
     * lookup and possibly return the index of a ShaderBlockAttrib in *this.
     * 
     * @return An integer which identifies one of the ShaderBlockAttribs in
     * *this. A negative value will be returned if the input string does not
     * match any of the block attributes in *this.
     */
    int get_matching_uniform_block_index(const char* const blockName) const noexcept;

    /**
     * Retrieve the index of a uniform block which matches the attributes in
     * *this ShaderProgram's array of uniform attributes.
     * 
     * @param blockName
     * A constant reference to an std::string object which will be used to
     * lookup and possibly return the index of a ShaderBlockAttrib in *this.
     * 
     * @return An integer which identifies one of the ShaderBlockAttribs in
     * *this. A negative value will be returned if the input string does not
     * match any of the block attributes in *this.
     */
    int get_matching_uniform_block_index(const std::string& blockName) const noexcept;

    /**
     * Retrieve the index of a uniform block which matches the attributes in a
     * UniformBuffer object.
     * 
     * @param ubo
     * A constant reference to a UniformBuffer object whose index should be
     * retrieved from *this shader.
     * 
     * @return An integer which identifies one of the ShaderBlockAttribs in
     * *this. A negative value will be returned if the input UBO does not
     * match any of the block attributes in *this.
     */
    int get_matching_uniform_block_index(const UniformBuffer& ubo) const noexcept;
};

/*-------------------------------------
    Get the OpenGL ID used by *this.
-------------------------------------*/
inline unsigned ShaderProgram::gpu_id() const noexcept
{
    return gpuId;
}

/*-------------------------------------
 Determine if *this is a valid shader object.
-------------------------------------*/
inline bool ShaderProgram::is_valid() const noexcept
{
    return gpu_id() != 0
           && vertAttribs.get_num_attribs() > 0
           && fragAttribs.get_num_attribs() > 0;
}

/*-------------------------------------
    Bind this program to the current context
-------------------------------------*/
inline void ShaderProgram::bind() const noexcept
{
    glUseProgram(gpu_id());
}

/*-------------------------------------
    Unbind this program from the context.
-------------------------------------*/
inline void ShaderProgram::unbind() const noexcept
{
    glUseProgram(0);
}

/*-------------------------------------
 * Get all vertex shader attributes
-------------------------------------*/
inline const ShaderAttribArray& ShaderProgram::get_vertex_attribs() const noexcept
{
    return vertAttribs;
}

/*-------------------------------------
 * Get all fragment shader attributes
-------------------------------------*/
inline const ShaderAttribArray& ShaderProgram::get_fragment_attribs() const noexcept
{
    return fragAttribs;
}

/*-------------------------------------
 * Get the current number of uniform blocks
-------------------------------------*/
inline unsigned ShaderProgram::get_num_uniform_blocks() const noexcept
{
    return numUniformBlocks;
}

/*-------------------------------------
 * Get all uniform blocks in *this.
-------------------------------------*/
inline const utils::Pointer<ShaderBlockAttrib[]>& ShaderProgram::get_uniform_blocks() const noexcept
{
    return uniformBlocks;
}

/*-------------------------------------
 * Get all shader uniform names
-------------------------------------*/
inline const ShaderAttribArray& ShaderProgram::get_uniforms() const noexcept
{
    return uniforms;
}

/*-------------------------------------
    Get the location of a uniform variable.
-------------------------------------*/
inline GLint ShaderProgram::get_uniform_location(const GLchar* const name) const noexcept
{
    return glGetUniformLocation(gpu_id(), name);
}

/*-------------------------------------
    Get the location of a uniform variable.
-------------------------------------*/
inline GLint ShaderProgram::get_uniform_location(const std::string& name) const noexcept
{
    return glGetUniformLocation(gpu_id(), name.c_str());
}

/*-------------------------------------
    Query the bindings of color numbers to user-defined varying out variables
-------------------------------------*/
inline GLint ShaderProgram::get_frag_data_location(const GLchar* const name) const noexcept
{
    return glGetFragDataLocation(gpu_id(), name);
}
} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_SHADER_PROGRAM_H__ */
