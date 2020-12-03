
/* 
 * File:   ShaderAttribArray.h
 * Author: geeds
 *
 * @date June 30, 2016 4:38 PM
 */

#ifndef __LS_DRAW_SHADER_ATTRIB_ARRAY_H__
#define __LS_DRAW_SHADER_ATTRIB_ARRAY_H__

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/ShaderAttrib.h"



namespace ls
{
namespace draw
{



/**----------------------------------------------------------------------------
 * Shader Object Attribute
 *
 * During shader compilation, attributes from the shader object are either
 * queried from OpenGL or extracted from the shader source code. These
 * attributes can later be used for GLSL introspection.
-----------------------------------------------------------------------------*/
class ShaderAttribArray
{
  private:
    /**
     * This member contains an array of Shader input or output attributes
     * which may help an input (vertex) or output (fragment) shader provide
     * information about the data types they require to operate as part of
     * a rendering pipeline.
     */
    utils::Pointer<ShaderAttrib[]> attribs;

    /**
     * This parameter contains the total number of Shader input/output
     * variables which can be provided to client code.
     */
    unsigned numAttribs;

  public:
    /**
     * Destructor
     *
     * Cleans up all CPU-side memory from *this.
     */
    ~ShaderAttribArray() noexcept;

    /**
     * Constructor
     *
     * Initializes all internal data to 0 or NULL.
     */
    ShaderAttribArray() noexcept;

    /**
     * Preallocation Constructor
     *
     * Preallocates the internal array of shader attributes in *this so
     * no further allocations are necessary.
     *
     * @param numPreallocAttribs
     * Contains the number of CPU-side shader attribs which an instance of
     * *this should preallocate.
     */
    ShaderAttribArray(const unsigned numPreallocAttribs) noexcept;

    /**
     * Preallocation & assignment constructor
     *
     * @param numPreallocAttribs
     * Contains the number of CPU-side shader attribs which an instance of
     * *this should preallocate.
     *
     * @param preallocAttribs
     * Contains a dynamic pointer to an array of CPU-side shader attrib
     * attributes which *this will inherit.
     */
    ShaderAttribArray(
        const unsigned preallocCount,
        utils::Pointer<ShaderAttrib[]>&& preallocAttribs
    ) noexcept;

    /**
     * Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param s
     * A constant reference to another ShaderAttrib object who's contents
     * will be copied into *this.
     */
    ShaderAttribArray(const ShaderAttribArray& s) noexcept;

    /**
     * Move Constructor
     *
     * Moves all data from the input parameter into *this.
     *
     * @param s
     * An r-value reference to another ShaderAttrib object who's contents
     * will be moved into *this.
     */
    ShaderAttribArray(ShaderAttribArray&& s) noexcept;

    /**
     * Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param s
     * A constant reference to another ShaderAttrib object who's contents
     * will be copied into *this.
     *
     * @return A reference to *this.
     */
    ShaderAttribArray& operator=(const ShaderAttribArray& s) noexcept;

    /**
     * Move Operator
     *
     * Moves all data from the input parameter into *this.
     *
     * @param s
     * An r-value reference to another ShaderAttrib object who's contents
     * will be moved into *this.
     *
     * @return A reference to *this.
     */
    ShaderAttribArray& operator=(ShaderAttribArray&& s) noexcept;

    /**
     * Reallocate the current list of ShaderAttribs contained in *this.
     *
     * Requesting room for more attributes will keep the properties of all
     * currently held attributes. Requesting a number smaller than what is
     * currently allocated will remove attributes from the end of the
     * internal list.
     *
     * This function will keep the current number of attirbutes, and their
     * properties, if no allocation could be successfully performed.
     *
     * @param numAttribs
     * Specifies the number of ShaderAttribs to be kept within *this.
     *
     * @return TRUE if *this was successfully able to allocate a specific
     * number of attributes, FALSE if not.
     */
    bool set_num_attribs(const unsigned numAttribs) noexcept;

    /**
     * Reallocate the current list of ShaderAttribs contained in *this and
     * clear the internal ShaderAttrib array of any old attributes.
     *
     * Even if no reallocation is performed, all internal ShaderAttrib
     * objects will be reset to their defaults.
     *
     * @param numAttribs
     * Specifies the number of ShaderAttribs to be kept within *this.
     *
     * @return TRUE if *this was successfully able to allocate a specific
     * number of attributes, FALSE if not.
     */
    bool reset_num_attribs(const unsigned numAttribs) noexcept;

    /**
     * Retrieve the number of ShaderAttrib object in *this.
     *
     * @return An unsigned integer containing the number of ShaderAttrib
     * objects managed by *this.
     */
    unsigned get_num_attribs() const noexcept;

    /**
     * Retrieve a single ShaderAttrib from *this.
     *
     * @param attribIndex
     * An array index of the shader attrib object to be retrieved.
     *
     * @return A constant reference to a ShaderAttrib object in *this.
     */
    const ShaderAttrib& get_attrib(const unsigned attribIndex) const noexcept;

    /**
     * Retrieve a single ShaderAttrib from *this.
     *
     * @param attribIndex
     * An array index of the shader attrib object to be retrieved.
     *
     * @return A reference to a ShaderAttrib object in *this.
     */
    ShaderAttrib& get_attrib(const unsigned attribIndex) noexcept;

    /**
     * Set the data for a specific ShaderAttrib in *this.
     *
     * @param attribIndex
     * The index offset to a single ShaderAttrib object contained in
     * *this who's data should be overwritten.
     *
     * @param attrib
     * An l-value or r-value reference to a shader object which contains
     * data that should overwrite the contents of an internal ShaderAttrib
     * object.
     */
    template <typename shader_attrib_type>
    void set_attrib(const unsigned attribIndex, shader_attrib_type&& attrib) noexcept;

    /**
     * Get the name of a specific ShaderAttrib in *this.
     *
     * @param attribIndex
     * The index offset to a single ShaderAttrib object contained in
     * *this who's name should be retrieved.
     *
     * @return A constant reference to a utils::Pointer<GLchar[]> object
     * containing the name of the ShaderAttrib object requested.
     */
    const utils::Pointer<GLchar[]>& get_attrib_name(const unsigned attribIndex) const noexcept;

    /**
     * Set the name of a specific ShaderAttrib in *this.
     *
     * @param attribIndex
     * The index offset to a single ShaderAttrib object contained in
     * *this who's name should be retrieved.
     *
     * @param attribName
     * A C-style string object containing the name of the ShaderAttrib
     * object requested.
     */
    void set_attrib_name(const unsigned attribIndex, const char* const attribName) noexcept;
};

/*-------------------------------------
 * Get the current number of attributes
-------------------------------------*/
inline unsigned ShaderAttribArray::get_num_attribs() const noexcept
{
    return numAttribs;
}

/*-------------------------------------
 * Get a single attribute
-------------------------------------*/
inline const ShaderAttrib& ShaderAttribArray::get_attrib(const unsigned attribIndex) const noexcept
{
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    return attribs[attribIndex];
}

/*-------------------------------------
 * Get a single attribute
-------------------------------------*/
inline ShaderAttrib& ShaderAttribArray::get_attrib(const unsigned attribIndex) noexcept
{
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    return attribs[attribIndex];
}

/*-------------------------------------
 * Reassign a shader attribute
-------------------------------------*/
template <typename shader_attrib_type>
inline void ShaderAttribArray::set_attrib(const unsigned attribIndex, shader_attrib_type&& attrib) noexcept
{
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    return attribs[attribIndex] = std::forward<shader_attrib_type>(attrib);
}

/*-------------------------------------
 * Get the name of a shader attribute
-------------------------------------*/
inline const utils::Pointer<GLchar[]>& ShaderAttribArray::get_attrib_name(const unsigned attribIndex) const noexcept
{
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    return get_attrib(attribIndex).get_name();
}

/*-------------------------------------
 * Reassign the name of a shader attribute.
-------------------------------------*/
inline void ShaderAttribArray::set_attrib_name(const unsigned attribIndex, const char* const attribName) noexcept
{
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    attribs[attribIndex].set_name(attribName);
}



/*-----------------------------------------------------------------------------
 * Utility functions for Shader Attributes
-----------------------------------------------------------------------------*/
/**------------------------------------
 * @brief Retrieve the meta-data about all shader uniform/attributes.
 *
 * @param prog
 * A constant reference to a ShaderProgram object which is to be
 * queried for attribute/uniform information.
 *
 * @param attribType
 * An enumeration which determines if the list of returned attributes should
 * contain Vertex attributes or Uniform attributes.
 *
 * @return A ShaderAttribArray object, containing the names of all
 * shader attributes or uniforms within the input program object.
-------------------------------------*/
ShaderAttribArray get_shader_attribs(
    const ShaderProgram& prog,
    const vertex_attrib_t attribType
) noexcept;



/**------------------------------------
 * Perform a shader inspection to retrieve information about a shader program's
 * attributes after it's been linked and optimized.
 *
 * @return TRUE if all vertex shader attributes could be retrieved, FALSE if
 * not.
-------------------------------------*/
ShaderAttribArray get_linked_shader_attribs(
    const ShaderProgram& prog,
    const shader_stage_t shaderType
) noexcept;
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SHADER_ATTRIB_ARRAY_H__ */
