
/*
 * File:   VertexBuffer.h
 * Author: miles
 *
 * Created on February 13, 2016, 3:15 PM
 */

#ifndef __LS_DRAW_VERTEXBUFFER_H__
#define __LS_DRAW_VERTEXBUFFER_H__



#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/BufferObject.h"
#include "lightsky/draw/VBOAttrib.h"


namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * @brief Vertex Buffer Class
 *
 * This class represents a Vertex Buffer Object from OpenGL. It can be used to
 * manage VBO data and attributes.
-----------------------------------------------------------------------------*/
class VertexBuffer final : public BufferObject {
    friend class BufferObject;

  private:
    /**
     * A bitfield of vertex types currently contained within *this.
     */
    common_vertex_t attribs;
    
    /**
     * @brief pAttribs contains a CPU-side representation of the per-vertex
     * attributes used by each element in *this object's GPU memory.
     */
    utils::Pointer<VBOAttrib[]> pAttribs;

    /**
     * @brief numAttribs should contain the number of attributes/members per
     * element in *this object's buffer.
     *
     * This member should directly correlate to the number of items in the
     * "pAttribs" member.
     */
    uint32_t numAttribs;

    /**
     * @brief Release all resources used by an inherited object's
     * attributes.
     */
    virtual void terminate_attribs() noexcept override;

    /**
     * @brief Copy attributes from the input object into *this.
     *
     * Buffer Attributes are a way of describing data within a GPU buffer
     * to client code.
     *
     * @param from
     * A constant reference to another BufferObject which contains one or
     * more attributes to copy.
     *
     * @return TRUE if *this object was able to copy all attributes from
     * the input parameter, FALSE if not.
     */
    virtual bool copy_attribs(const BufferObject& from) noexcept override;

  public:
    /**
     * @brief Destructor
     *
     * The destructor is only responsible for deleting CPU-side data as
     * multiple VertexBuffer objects can refer to the same data on a GPU.
     */
    ~VertexBuffer() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all internal values to their defaults.
     */
    VertexBuffer() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all CPU-side data from the input parameter into *this.
     */
    VertexBuffer(const VertexBuffer&) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves all CPU-side data from the input parameter into *this.
     */
    VertexBuffer(VertexBuffer&&) noexcept;

    /**
     * @brief Copy operator
     *
     * Copies all CPU-side data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    VertexBuffer& operator=(const VertexBuffer&) noexcept;

    /**
     * @brief Move operator
     *
     * Moves all CPU-side data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    VertexBuffer& operator=(VertexBuffer&&) noexcept;

    /**
     * @brief Retrieve the Usage of *this buffer on the GPU.
     *
     * This should help to determine if an OpenGL Buffer object is a
     * vertex, index, texture, uniform, or buffer object of another type.
     *
     * @return A value from the buffer_use_t enumeration which can help to
     * determine what type of buffer *this is at runtime.
     */
    virtual buffer_use_t get_type() const noexcept override;

    /**
     * @brief Retrieve the number of buffer attributes that are contained
     * within a buffer object.
     *
     * @return An unsigned integral type which contains the number of
     * attributes contained within a buffer object.
     */
    virtual unsigned get_num_attribs() const noexcept override;

    /**
     * @brief Setup all vertex attributes that will be associated with a
     * BufferObject.
     *
     * These attributes can be used to setup draw calls or be used as binding
     * information for a Vertex Array Object.
     *
     * @param attribs
     * A bitmask, containing all vertex attribute types which will be used for
     * a buffer object. The order in which the attributes are setup on the GPU
     * follows the same order in which they are defined in the
     * "common_vertex_t" enumeration (see VertexUtils.h).
     *
     * @return TRUE if all vertex attributes were able to be placed into the
     * BufferObject, FALSE if not.
     */
    bool setup_attribs(const common_vertex_t attribs) noexcept;

    /**
     * @brief Setup all vertex attributes that will be associated with a
     * BufferObject.
     *
     * These attributes can be used to setup draw calls or be used as binding
     * information for a Vertex Array Object.
     *
     * @param pAttribs
     * An array of bitmasks, containing all vertex attribute types which will
     * be used for a buffer object. The order in which the attributes are setup
     * on the GPU follows the same order in which they are defined in the
     * "common_vertex_t" enumeration (see VertexUtils.h).
     * 
     * @param attribCount
     * Contains the number of bitmasks contained in "pAttirbs."
     *
     * @return TRUE if all vertex attributes were able to be placed into the
     * BufferObject, FALSE if not.
     */
    bool setup_attribs(const common_vertex_t* const pAttribs, const unsigned attribCount) noexcept;
    
    /**
     * Retrieve a bitfield containing the type of vertex attributes contained
     * within *this.
     * 
     * @return A bitfield of common_vertex_t values which can provide
     * information about the types of vertices contained within *this.
     */
    common_vertex_t get_attrib_types() const noexcept;

    /**
     * @brief Retrieve a pointer to the list of internal VBOAttribs
     * contained within *this.
     *
     * @return A pointer to a constant array of VBOAttrib objects contained
     * within *this, or NULL if no attribs have been set up.
     */
    const VBOAttrib* get_attribs() const noexcept;

    /**
     * @brief Retrieve a reference to a single VBOAttrib contained within
     * *this.
     *
     * @param index
     * The array-offset to a requested VBOAttrib object.
     *
     * @return A constant reference to a single VBOAttrib object contained
     * within *this.
     */
    const VBOAttrib& get_attrib(const unsigned index) const noexcept;

    /**
     * @brief Retrieve a reference to a single VBOAttrib contained within
     * *this.
     *
     * @param index
     * The array-offset to a requested VBOAttrib object.
     *
     * @return A reference to a single VBOAttrib object contained within
     *this.
     */
    VBOAttrib& get_attrib(const unsigned index) noexcept;
};

/*-----------------------------------------------------------------------------
 * Inline Member Functions for a VertexBuffer.
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Retrieve the buffer type
-------------------------------------*/
inline buffer_use_t VertexBuffer::get_type() const noexcept {
    return buffer_use_t::VBO_BUFFER_ARRAY;
}

/*-------------------------------------
 * Retrieve the buffer's attribs
-------------------------------------*/
inline unsigned VertexBuffer::get_num_attribs() const noexcept {
    return numAttribs;
}

/*-------------------------------------
 * Retrieve a pointer to the list of internal VBOAttribs
-------------------------------------*/
inline const VBOAttrib* VertexBuffer::get_attribs() const noexcept {
    return pAttribs;
}

/*-------------------------------------
 * Retrieve the currently held vertex types
-------------------------------------*/
inline common_vertex_t VertexBuffer::get_attrib_types() const noexcept {
    return attribs;
}

/*-------------------------------------
 * Retrieve a reference to a single VBOAttrib contained within *this.
-------------------------------------*/
inline const VBOAttrib& VertexBuffer::get_attrib(const unsigned index) const noexcept {
    return pAttribs[index];
}

/*-------------------------------------
 * Retrieve a reference to a single VBOAttrib contained within *this.
-------------------------------------*/
inline VBOAttrib& VertexBuffer::get_attrib(const unsigned index) noexcept {
    return pAttribs[index];
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_VERTEXBUFFER_H__ */
