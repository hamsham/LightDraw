
/*
 * File:   VBOAttrib.h
 * Author: miles
 *
 * Created on March 9, 2016, 11:06 AM
 */

#ifndef __LS_DRAW_VBO_ATTRIB_H__
#define __LS_DRAW_VBO_ATTRIB_H__

#include "lightsky/draw/Setup.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum vertex_data_t : GLenum; // found in VertexUtils.h



/*-----------------------------------------------------------------------------
 * @brief The VBOAttrib class contains basic attribute data management for
 * VBOs and validation for VAOs.
-----------------------------------------------------------------------------*/
class VBOAttrib
{
  private:
    /**
     * Total number of these attributes.
     */
    unsigned numElements;

    /**
     * Number of elements per vertex type (1 for scalars, 2-4 for vectors,
     * 2-4 for matrices).
     */
    GLint components;

    /**
     * The primary makeup of each vertex.
     */
    vertex_data_t vertType;

    /**
     * Determines if this is fixed-point data that needs normalizing.
     */
    GLboolean normalized;

    /**
     * Stride between each vertex element.
     */
    GLsizei stride;

    /**
     * Byte-offset to the first vertex in the buffer (NOT MEMORY ADDRESS).
     */
    const GLvoid* offset;

    /**
     * Sets the number of vertices which can be drawn before this attribute
     * does (a.k.a. instance rate).
     */
    unsigned vertsPerAttrib;

  public:
    ~VBOAttrib() noexcept;

    VBOAttrib() noexcept;

    VBOAttrib(const VBOAttrib&) noexcept;

    VBOAttrib(VBOAttrib&&) noexcept;

    VBOAttrib& operator=(const VBOAttrib&) noexcept;

    VBOAttrib& operator=(VBOAttrib&&) noexcept;

    void set_num_elements(const unsigned elementsInArray) noexcept;

    unsigned get_num_elements() const noexcept;

    unsigned get_num_components() const noexcept;

    unsigned get_num_subcomponents() const noexcept;

    void set_type(const vertex_data_t vertexType) noexcept;

    vertex_data_t get_type() const noexcept;

    vertex_data_t get_base_type() const noexcept;

    void set_normalized(const GLboolean normalize) noexcept;

    GLboolean is_normalized() const noexcept;

    void set_byte_stride(const unsigned vertStride) noexcept;

    unsigned get_byte_stride() const noexcept;

    void set_offset(const GLvoid* numOffsetBytes) noexcept;

    const GLvoid* get_offset() const noexcept;

    void set_instance_rate(const unsigned instanceRate = 0) noexcept;

    unsigned get_instance_rate() const noexcept;
};

/*-------------------------------------
 * Set the number of vertex array elements
-------------------------------------*/
inline void VBOAttrib::set_num_elements(const unsigned elementsInArray) noexcept
{
    numElements = elementsInArray;
}

/*-------------------------------------
 * Get the number of vertex array elements
-------------------------------------*/
inline unsigned VBOAttrib::get_num_elements() const noexcept
{
    return numElements;
}

/*-------------------------------------
 * Set the number of array-element components for *this
-------------------------------------*/
inline unsigned VBOAttrib::get_num_components() const noexcept
{
    return components;
}

/*-------------------------------------
 * Retrieve the GLSL-based data type
-------------------------------------*/
inline vertex_data_t VBOAttrib::get_type() const noexcept
{
    return vertType;
}

/*-------------------------------------
 * Set *this to a normalized data type.
-------------------------------------*/
inline void VBOAttrib::set_normalized(const GLboolean normalize) noexcept
{
    normalized = normalize;
}

/*-------------------------------------
 * Determine if *this represents a normalized data type.
-------------------------------------*/
inline GLboolean VBOAttrib::is_normalized() const noexcept
{
    return normalized;
}

/*-------------------------------------
 * Set the byte padding between consecutive vertex elements
-------------------------------------*/
inline void VBOAttrib::set_byte_stride(const unsigned vertStride) noexcept
{
    stride = vertStride;
}

/*-------------------------------------
 * Get the byte padding between consecutive vertex elements
-------------------------------------*/
inline unsigned VBOAttrib::get_byte_stride() const noexcept
{
    return stride;
}

/*-------------------------------------
 * Set the offset to the first vertex element
-------------------------------------*/
inline void VBOAttrib::set_offset(const GLvoid* numOffsetBytes) noexcept
{
    offset = numOffsetBytes;
}

/*-------------------------------------
 * Get the offset to the first vertex element
-------------------------------------*/
inline const GLvoid* VBOAttrib::get_offset() const noexcept
{
    return offset;
}

/*-------------------------------------
 * Set the number of consecutive elements between each new rendered instance
-------------------------------------*/
inline void VBOAttrib::set_instance_rate(const unsigned instanceRate) noexcept
{
    vertsPerAttrib = instanceRate;
}

/*-------------------------------------
 * Get the number of consecutive elements between each new rendered instance
-------------------------------------*/
inline unsigned VBOAttrib::get_instance_rate() const noexcept
{
    return vertsPerAttrib;
}
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_VBO_ATTRIB_H__ */
