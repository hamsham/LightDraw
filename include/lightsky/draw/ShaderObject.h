/*
 * File:   draw/shaderObject.h
 * Author: Miles Lacey
 *
 * Created on January 21, 2014, 6:54 PM
 */

#ifndef __LS_DRAW_SHADER_OBJECT_H__
#define __LS_DRAW_SHADER_OBJECT_H__

#include <string>

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/ShaderAttribArray.h"



namespace ls
{
namespace draw
{


/*-----------------------------------------------------------------------------
 * Forward declarations & Typedefs
-----------------------------------------------------------------------------*/
enum vertex_data_t : GLenum;

class ShaderProgram;



/**----------------------------------------------------------------------------
 * Enumeration to determine the type of shader that should be created on the GPU
-----------------------------------------------------------------------------*/
enum shader_stage_t : GLint
{
    SHADER_STAGE_INVALID = 0,

#if defined(LS_DRAW_BACKEND_GLES)
    SHADER_STAGE_VERTEX = GL_VERTEX_SHADER,
    SHADER_STAGE_FRAGMENT = GL_FRAGMENT_SHADER,
    SHADER_STAGE_MAX = 2

#else
    SHADER_STAGE_VERTEX = GL_VERTEX_SHADER,
    SHADER_STAGE_GEOMETRY = GL_GEOMETRY_SHADER,
    SHADER_STAGE_FRAGMENT = GL_FRAGMENT_SHADER,
    SHADER_STAGE_MAX = 3
#endif
};



enum shader_string_t : GLenum
{
    SHADER_STRING_LOG = GL_INFO_LOG_LENGTH,
    SHADER_STRING_SOURCE = GL_SHADER_SOURCE_LENGTH,
};



/**----------------------------------------------------------------------------
 * Shader Object Base Class
 *
 * Shader objects are attached to programs in order to manipulate rendering on
 * the GPU
-----------------------------------------------------------------------------*/
class ShaderObject
{
  private:
    /**
     * Handle to the GPU-side shader object.
     */
    GLuint gpuId;

    /**
     * Determines the type of shader to instantiate.
     */
    shader_stage_t shaderStage;

    /**
     * Member to hold all meta-info for the vertex shader inputs.
     */
    ShaderAttribArray attribs;

    /**
     * Query OpenGL for some string information about a particular shader.
     *
     * @param shaderId
     * A GPU-provided handle to an OpenGL shader object.
     *
     * @param stringType
     * A value from the shader_string_t enumeration which determines the
     * type of string data which will be returned from OpenGL.
     *
     * @return A unique pointer to a C-style string which contains the
     * requested string data from OpenGL.
     */
    static utils::Pointer<GLchar[]> get_shader_string(
        const GLuint shaderId,
        const shader_string_t stringType
    ) noexcept;

    /**
     * Perform some introspection on the shader source code and retrieve a
     * list of attributes associated with it.
     */
    bool introspect_attributes() noexcept;

  public:
    /**
     * Destroy this object and free any CPU memory it uses.
     *
     * The destructor does not delete any handles to OpenGL data as
     * multiple ShaderObject instances can have handles to the same data.
     */
    ~ShaderObject() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all members within *this.
     */
    ShaderObject() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param s
     * A constant reference to another ShaderObject who's data is to be
     * copied into *this.
     */
    ShaderObject(const ShaderObject& s) noexcept;

    /**
     * @brief Move Constructor
     *
     * Copy the handle to the shader object owned by the input argument,
     * reset the moved object's shader handle to 0.
     */
    ShaderObject(ShaderObject&& s) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param s
     * A constant reference to another ShaderObject who's data is to be
     * copied into *this.
     *
     * @return A reference to *this.
     */
    ShaderObject& operator=(const ShaderObject& s) noexcept;

    /**
     * @brief Move Operator
     *
     * Move the values of the input argument and reset the moved object's
     * values to 0.
     *
     * @param s
     * A shaderProgram to move into *this.
     *
     * @return A reference to *this
     */
    ShaderObject& operator=(ShaderObject&& so) noexcept;

    /**
     * Free all memory used by this shader object.
     */
    void terminate() noexcept;

    /**
     * Compile a shader and put it into GPU memory.
     *
     * Initializing a shader does not delete the previous OpenGL handle if
     * one exists.
     *
     * @param shaderSourceType
     * A value from the "shader_stage_t" enumeration which determines if
     * the input source code is for a vertxe, fragment, or any other type
     * of shader.
     *
     * @param data A pointer to a string which contains the shader's
     * textual content.
     *
     * @param size
     * the size, in bytes, of the shader's text data. Let this remain zero
     * if you are sure that the data string is null-terminated.
     *
     * @return TRUE if the shader was successfully compiled, FALSE if
     * otherwise.
     */
    bool init(
        const shader_stage_t shaderSourceType,
        const char* const data,
        const int size
    ) noexcept;

    /**
     * Compile a set of shader strings and put them into GPU memory.
     *
     * Initializing a shader does not delete the previous OpenGL handle if
     * one exists.
     *
     * @param shaderSourceType
     * A value from the "shader_stage_t" enumeration which determines if
     * the input source code is for a vertxe, fragment, or any other type
     * of shader.
     *
     * @param numStrings
     * The number of shader strings which are to be passed to the GPU for
     * compilation.
     *
     * @param data
     * A pointer to an array of strings which contain the shader's textual
     * content.
     *
     * @param sizes
     * An array of integers, specifying the sizes, in bytes, of each
     * shader's textual data. Set this to NULL if you are sure that the
     * data in each string is null-terminated.
     *
     * @return TRUE if the shader was successfully compiled, FALSE if
     * otherwise.
     */
    bool init(
        const shader_stage_t shaderSourceType,
        const unsigned numStrings,
        const char* const* data,
        const int* const sizes
    ) noexcept;

    /**
     * Get a shader's GPU-assigned ID
     *
     * @return An unsigned integer which contains a handle to the shader
     * that *this object references on the GPU. A value of 0 is returned
     * if *this object does not contain a handle to a valid OpenGL shader.
     */
    GLuint gpu_id() const noexcept;

    /**
     * Retrieve the type of shader object which *this represents.
     *
     * @return A value from the "shader_stage_t" enumeration which can be
     * used to determine if *this object represents a vertex, fragment, or
     * any other type of shader.
     */
    shader_stage_t get_shader_type() const noexcept;

    /**
     * Retrieve the log data of a shader.
     *
     * This usually returns nothing unless a shader object failed to
     * compile.
     *
     * @return A unique pointer to a C-style string which contains the
     * information log about a shader object's compilation.
     */
    utils::Pointer<GLchar[]> get_shader_info_log() const noexcept;

    /**
     * Retrieve the source code for a shader object.
     *
     * @return A unique pointer to a C-style string containing the source code
     * of a shader object, or an empty string if the shader failed to compile.
     */
    utils::Pointer<GLchar[]> get_shader_source() const noexcept;

    /**
     * Determine if this is a valid shader which has been validated through
     * compilation.
     *
     * @return TRUE if this is a valid shader which has been compiled and
     * can be attached to a ShaderProgram or queried for data, FALSE if
     * otherwise.
     */
    bool is_valid() const noexcept;

    /**
     * Retrieve a list of Shader Attributes which were found after a
     * successful compilation.
     *
     * This array will represent input attributes for a vertex shader and
     * output attributes for a fragment shader.
     *
     * @return A constant reference to a ShaderAttribArray object which
     * contains either input vertex attribute information or output
     * fragment attribute information.
     */
    const ShaderAttribArray& get_attribs() const noexcept;

    /**
     * Attempt to recreate a ShaderObject from a vertex or fragment shader
     * ID.
     *
     * @param shaderId
     * An unsigned integer provided by OpenGL which is being used as a
     * handle to a shader object on the GPU.
     *
     * @return TRUE if *this ShaderObject and all attrib information could
     * successfully be regenerated by querying OpenGL, FALSE if not.
     */
    bool recreate_from_id(const GLuint shaderId) noexcept;
};

/*-------------------------------------
    Shader Loading (single shader).
-------------------------------------*/
inline bool ShaderObject::init(
    const shader_stage_t shaderSourceType,
    const char* const data,
    const int size
) noexcept
{
    return init(shaderSourceType, 1, &data, size ? &size : nullptr);
}

/*-------------------------------------
    Get a shader's GPU-assigned ID
-------------------------------------*/
inline GLuint ShaderObject::gpu_id() const noexcept
{
    return gpuId;
}

/*-------------------------------------
    Get a shader's type
-------------------------------------*/
inline shader_stage_t ShaderObject::get_shader_type() const noexcept
{
    return shaderStage;
}

/*-------------------------------------
    Get a shader's log data
-------------------------------------*/
inline utils::Pointer<GLchar[]> ShaderObject::get_shader_info_log() const noexcept
{
    return get_shader_string(gpu_id(), shader_string_t::SHADER_STRING_LOG);
}

/*-------------------------------------
    Get a shader's source code
-------------------------------------*/
inline utils::Pointer<GLchar[]> ShaderObject::get_shader_source() const noexcept
{
    return get_shader_string(gpu_id(), shader_string_t::SHADER_STRING_SOURCE);
}

/*-------------------------------------
 Determine if *this is a valid shader
-------------------------------------*/
inline bool ShaderObject::is_valid() const noexcept
{
    return gpu_id() != 0;
}

/*-------------------------------------
 Retrieve information about the current shader's attributes.
-------------------------------------*/
inline const ShaderAttribArray& ShaderObject::get_attribs() const noexcept
{
    return attribs;
}
} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_SHADER_OBJECT_H__ */
