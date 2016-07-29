/*
 * File:   draw/VertexAttrib.h
 * Author: miles
 *
 * Created on June 10, 2015, 8:52 PM
 */

#ifndef __LS_DRAW_VERTEX_ATTRIB_H__
#define __LS_DRAW_VERTEX_ATTRIB_H__

#include <utility> // std::forward
#include <string>

#include "ls/utils/Assertions.h"
#include "ls/utils/Pointer.h"

#include "ls/math/Math.h"

#include "ls/draw/Setup.h"
#include "ls/draw/Vertex.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum vertex_data_t : GLenum;
class VBOAttrib;



/**
 * @brief Vertex Attrib Types
 *
 * This type can be used to determine if an object is a vertex array attribute
 * or a shader uniform attribute.
 */
enum class vertex_attrib_t : int {
    UNIFORM_ATTRIB,
    VERTEX_ATTRIB
};



/**
 * Enumeration containing OpenGL's built-in limits for VAO-related tasks.
 */
enum vao_limits_t : unsigned {
    VAO_MAX_VERTEX_ATTRIBS = 16 // minimum set by the OpenGL standard.
};



/**----------------------------------------------------------------------------
 * @brief Vertex Attribute Array Object
 *
 * The Vertex Attrib Array structure helps to provide a layer of introspection
 * between OpenGL client code and GLSL shader code.
-----------------------------------------------------------------------------*/
class VAOAttrib {
  private:
    /**
     * Contains an array of names associated with vertex attributes.
     */
    utils::Pointer<std::string[]> names;

    /**
     * Contains an array of vertex buffer attributes,
     */
    utils::Pointer<VBOAttrib[]> vboAttribs;

    /**
     * Contains the total number of currently allocated vertex attributes
     * and names.
     */
    unsigned numAttribs;

  public:
    /**
     * Destructor
     */
    virtual ~VAOAttrib() noexcept;

    /**
     * Constructor
     *
     * Initializes all internal values and pointer to NULL or 0.
     */
    VAOAttrib() noexcept;

    /**
     * Preallocation Constructor
     *
     * Preallocates the internal arrays of Vertex Array attributes and
     * attribute names.
     *
     * @param numPreallocAttribs
     * An unsigned integer which will determine the maximum number of
     * vertex attributes to preallocate in *this.
     */
    VAOAttrib(const unsigned numPreallocAttribs) noexcept;

    /**
     * Preallocated initialization Constructor.
     *
     * Allows a VAO Attrib object to be constructed using preallocated
     * variables. It is up to client code to ensure the lengths of both
     * input arrays match the preallocation count exactly.
     *
     * @param preallocCount
     * Provides the number of preallocated elements in both input arrays.
     * Passing in a value of 0 will invoke the default constructor.
     *
     * @param preallocNames
     * Contains an array of names associated with the VBO attributes in the
     * input "preallocAttribs" array. This array must be of length
     * "preallocCount", otherwise it will produce undefined behavior.
     *
     * @param preallocAttribs
     * Contains an array of VBO attributes. This array must be of length
     * "preallocCount", otherwise it will produce undefined behavior.
     */
    VAOAttrib(
        const unsigned preallocCount,
        utils::Pointer<std::string[]>&& preallocNames,
        utils::Pointer<VBOAttrib[]>&& preallocAttribs
        ) noexcept;

    /**
     * Copy Constructor
     *
     * Copies all data from the input parameter into *this. Calling this
     * function will cause memory reallocations in *this if the number of
     * attributes in the input parameter does not match *this.
     *
     * @param vaoAttribs
     * A constant reference to a VAOAttrib object who's data will be copied
     * into *this.
     */
    VAOAttrib(const VAOAttrib& vaoAttribs) noexcept;

    /**
     * Move Constructor
     *
     * Moves all data from the input parameter into *this, leaving the
     * input VAOAttrib object in a null-state (all members are either 0 or
     * NULL).
     *
     * @param vaoAttribs
     * An r-value reference to a VAOAttrib object who's data will be pwned
     * by *this.
     */
    VAOAttrib(VAOAttrib&& vaoAttribs) noexcept;

    /**
     * Copy Operator
     *
     * Copies all data from the input parameter into *this. Calling this
     * function will cause memory reallocations in *this if the number of
     * attributes in the input parameter does not match *this.
     *
     * @param vaoAttribs
     * A constant reference to a VAOAttrib object who's data will be copied
     * into *this.
     *
     * @return a reference to *this.
     */
    VAOAttrib& operator=(const VAOAttrib& vaoAttribs) noexcept;

    /**
     * Move Operator
     *
     * Moves all data from the input parameter into *this, leaving the
     * input VAOAttrib object in a null-state (all members are either 0 or
     * NULL).
     *
     * @param vaoAttribs
     * An r-value reference to a VAOAttrib object who's data will be pwned
     * by *this.
     *
     * @return a reference to *this.
     */
    VAOAttrib& operator=(VAOAttrib&& vaoAttribs) noexcept;

    /**
     * Reallocate the number of attribs in *this to a specified size.
     *
     * This method does nothing if the input number matches the current
     * number of attributes. If "attribCount" is smaller than the current
     * number of attribs, only valid attributes up to "attribCount" will
     * be kept after a reallocation. A count larger than the current number
     * of attribs will keep the current valid attribs, but will reserve
     * space for additional attributes up to "attribCount".
     *
     * @param attribCount
     * An unsigned integer which will determine the maximum number of
     * vertex attributes to keep in *this.
     *
     * @return TRUE if the reallocation was successful, FALSE if not.
     */
    bool set_num_attribs(const unsigned attribCount) noexcept;

    /**
     * Reallocate the current number of attributes to "attribCount" and
     * invalidate all current attributes. No original values will be kept.
     *
     * @param attribCount
     * An unsigned integer which will determine the maximum number of
     * vertex attributes to reserve in *this.
     *
     * @return TRUE if the reallocation was successful, FALSE if not.
     */
    bool reset_num_attribs(const unsigned attribCount) noexcept;

    /**
     * Retrieve the current number of total attributes in *this.
     *
     * @return The number of attribute descriptors currently managed by
     * *this.
     */
    unsigned get_num_attribs() const noexcept;

    /**
     * Retrieve the current number of total valid attributes in *this.
     *
     * Attributes are only valid if they have been provided a name.
     * Additional validity may be left up to OpenGL's
     * "glVertexAttribPointer(...)" and "glVertexAttribDivisor(...)"
     * functions.
     *
     * @return The total number of valid attributes in *this.
     */
    unsigned get_num_valid_attribs() const noexcept;

    /**
     * Determine if a single vertex array attribute is valid.
     *
     * Attributes are only valid if they have been provided a name.
     * Additional validity may be left up to OpenGL's
     * "glVertexAttribPointer(...)" and "glVertexAttribDivisor(...)"
     * functions.
     *
     * @return TRUE if the requested attribute has a name, FALSE if not.
     */
    bool is_attrib_valid(const unsigned attribIndex) const noexcept;

    /**
     * Invalidate a single attribute in *this.
     *
     * @param attribIndex
     * Determines the attribute to invalidate in *this.
     */
    void invalidate_attrib(const unsigned attribIndex) noexcept;

    /**
     * Invalidate all attributes in *this.
     */
    void invalidate_attribs() noexcept;

    /**
     * Set the name of a single attribute in *this.
     *
     * @param index
     * An array index which will be used to determine the attribute in
     * *this to name.
     *
     * @param attribName
     * A reference (R-value or L-value) to an std::string which will be
     * used to set the name of the attrib in *this to name.
     */
    template <class std_string_type>
    void set_attrib_name(const unsigned index, std_string_type&& attribName) noexcept;

    /**
     * Retrieve the name of a single attrib in *this.
     *
     * @param index
     * An array index which will be used to retrieve the name of a single
     * attribute in *this.
     *
     * @return A constant reference to an std::string object containing
     * the name of a VAO Attrib in *this.
     */
    const std::string& get_attrib_name(const unsigned index) const noexcept;

    /**
     * Retrieve the name of a single attrib in *this.
     *
     * @param index
     * An array index which will be used to retrieve the name of a single
     * attribute in *this.
     *
     * @return A reference to an std::string object containing the name
     * of a VAO Attrib in *this.
     */
    std::string& get_attrib_name(const unsigned index) noexcept;

    /**
     * Assign the contents of a single VBOAttib in *this.
     *
     * @param index
     * An array index which will be used to determine the attribute in
     * *this to assign a value to.
     *
     * @param attrib
     * A reference (R-value or L-value) to a VBOAttrib object in *this to
     * use as a data source.
     */
    template <class vbo_attrib_type>
    void set_vbo_attrib(const unsigned index, vbo_attrib_type&& attrib) noexcept;

    /**
     * Retrieve a VBOAttrib in *this.
     *
     * @param index
     * An array index which will be used to retrieve the data of a single
     * attribute in *this.
     *
     * @return A const reference to VBOAttrib object in *this.
     */
    const VBOAttrib& get_attrib(const unsigned index) const noexcept;

    /**
     * Retrieve a VBOAttrib in *this.
     *
     * @param index
     * An array index which will be used to retrieve the data of a single
     * attribute in *this.
     *
     * @return A reference to VBOAttrib object in *this.
     */
    VBOAttrib& get_attrib(const unsigned index) noexcept;
};

/*-------------------------------------
-------------------------------------*/
inline unsigned VAOAttrib::get_num_attribs() const noexcept {
    return numAttribs;
}

/*-------------------------------------
-------------------------------------*/
inline bool VAOAttrib::is_attrib_valid(const unsigned attribIndex) const noexcept {
    // TODO: Add more validations within each VBO Attribute
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    return !names[attribIndex].empty();
}

/*-------------------------------------
-------------------------------------*/
inline void VAOAttrib::invalidate_attrib(const unsigned attribIndex) noexcept {
    LS_DEBUG_ASSERT(attribIndex < get_num_attribs());
    names[attribIndex].clear();
}

/*-------------------------------------
-------------------------------------*/
template <class std_string_type>
inline void VAOAttrib::set_attrib_name(const unsigned index, std_string_type&& attribName) noexcept {
    LS_DEBUG_ASSERT(index < get_num_attribs());
    names[index] = std::forward<std_string_type>(attribName);
}

/*-------------------------------------
-------------------------------------*/
inline const std::string& VAOAttrib::get_attrib_name(const unsigned index) const noexcept {
    LS_DEBUG_ASSERT(index < get_num_attribs());
    return names[index];
}

/*-------------------------------------
-------------------------------------*/
inline std::string& VAOAttrib::get_attrib_name(const unsigned index) noexcept {
    LS_DEBUG_ASSERT(index < get_num_attribs());
    return names[index];
}

/*-------------------------------------
 * Set a VBO Attrib
-------------------------------------*/
template <class vbo_attrib_type>
inline void VAOAttrib::set_vbo_attrib(const unsigned index, vbo_attrib_type&& attrib) noexcept {
    LS_DEBUG_ASSERT(index < get_num_attribs());
    vboAttribs[index] = std::forward(attrib);
}



/*-----------------------------------------------------------------------------
 * Vertex Attrib Helper Functions
-----------------------------------------------------------------------------*/
VAOAttrib create_vertex_attrib(const vertex_data_t type);



} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_VERTEX_ATTRIB_H__ */
