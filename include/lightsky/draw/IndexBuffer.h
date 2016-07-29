
/*
 * File:   IndexBuffer.h
 * Author: miles
 *
 * Created on February 13, 2016, 3:19 PM
 */

#ifndef __LS_DRAW_INDEXBUFFER_H__
#define __LS_DRAW_INDEXBUFFER_H__



#include "ls/draw/BufferObject.h"


namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum index_element_t : GLint; // found in VertexUtils.h



/**----------------------------------------------------------------------------
 * @brief This structure contains basic attribute data management for Index
 * Buffer Objects.
-----------------------------------------------------------------------------*/
class IBOAttrib {
  private:
    /**
     * Total number of these attributes.
     */
    GLsizei count;

    /**
     * The generic makeup of each element in a buffer.
     */
    index_element_t indexType;

    /**
     * Byte-offset to the first index in the buffer (NOT MEMORY ADDRESS).
     */
    const GLvoid* offset;

  public:
    ~IBOAttrib() noexcept;

    IBOAttrib() noexcept;

    IBOAttrib(const IBOAttrib&) noexcept;

    IBOAttrib(IBOAttrib&&) noexcept;

    IBOAttrib& operator=(const IBOAttrib&) noexcept;

    IBOAttrib& operator=(IBOAttrib&&) noexcept;

    void set_attrib_count(const unsigned numIndices) noexcept;

    unsigned get_attrib_count() const noexcept;

    void set_attrib_type(const index_element_t) noexcept;

    index_element_t get_attrib_type() const noexcept;

    void set_attrib_offset(const GLvoid * const numOffsetBytes) noexcept;

    const GLvoid* get_attrib_offset() const noexcept;
};

/*-------------------------------------
 * Set the number of IBO indices
-------------------------------------*/
inline void IBOAttrib::set_attrib_count(const unsigned numIndices) noexcept {
    count = numIndices;
}

/*-------------------------------------
 * Get the current count of indices
-------------------------------------*/
inline unsigned IBOAttrib::get_attrib_count() const noexcept {
    return count;
}

/*-------------------------------------
 * Set the IBO attrib data type
-------------------------------------*/
inline void IBOAttrib::set_attrib_type(const index_element_t idxType) noexcept {
    indexType = idxType;
}

/*-------------------------------------
 * Get the current attrib type
-------------------------------------*/
inline index_element_t IBOAttrib::get_attrib_type() const noexcept {
    return indexType;
}

/*-------------------------------------
 * Assign the offset to the current IBO Attribute
-------------------------------------*/
inline void IBOAttrib::set_attrib_offset(const GLvoid * const numOffsetBytes) noexcept {
    offset = numOffsetBytes;
}

/*-------------------------------------
 * Retrieve the offset to the current IBO Attribute
-------------------------------------*/
inline const GLvoid* IBOAttrib::get_attrib_offset() const noexcept {
    return offset;
}



/**----------------------------------------------------------------------------
 * @Brief This class represents a Vertex Buffer Object from OpenGL. It can be
 * used to manage VBO data and attributes.
-----------------------------------------------------------------------------*/
class IndexBuffer final : public BufferObject {
    friend class BufferObject;

  protected:
    /**
     * @brief pAttribs contains a CPU-side representation of the per-index
     * attributes used by each element in *this object's GPU memory.
     */
    utils::Pointer<IBOAttrib[] > pAttribs;

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
     * multiple IndexBuffer objects can refer to the same data on a GPU.
     */
    ~IndexBuffer() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all internal values to their defaults.
     */
    IndexBuffer() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all CPU-side data from the input parameter into *this.
     */
    IndexBuffer(const IndexBuffer&) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves all CPU-side data from the input parameter into *this.
     */
    IndexBuffer(IndexBuffer&&) noexcept;

    /**
     * @brief Copy operator
     *
     * Copies all CPU-side data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    IndexBuffer& operator=(const IndexBuffer&) noexcept;

    /**
     * @brief Move operator
     *
     * Moves all CPU-side data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    IndexBuffer& operator=(IndexBuffer&&) noexcept;

    /**
     * @brief Retrieve the Usage of *this buffer on the GPU.
     *
     * @return buffer_use_t::VBO_BUFFER_INDEX
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
     * @brief Setup all index attributes that will be associated with a
     * BufferObject.
     *
     * These attributes can be used to setup draw calls for indexed
     * vertices.
     *
     * @param attribCount
     * An unsigned integral type containing the total number of attributes
     * which should be allocated for *this IBO.
     *
     * @return TRUE if all vertex attributes were able to be placed into the
     * BufferObject, FALSE if not.
     */
    bool setup_attribs(const unsigned attribCount) noexcept;

    /**
     * @brief Retrieve a pointer to the list of internal IndexAttrib
     * contained within *this.
     *
     * @return A pointer to a constant array of IndexAttrib objects contained
     * within *this, or NULL if no attribs have been set up.
     */
    const IBOAttrib* get_attribs() const noexcept;

    /**
     * @brief Retrieve a reference to a single IndexAttrib contained within
     * *this.
     *
     * @param index
     * The array-offset to a requested IndexAttrib object.
     *
     * @return A constant reference to a single IndexAttrib object contained
     * within *this.
     */
    const IBOAttrib& get_attrib(const unsigned index) const noexcept;

    /**
     * @brief Retrieve a reference to a single IndexAttrib contained within
     * *this.
     *
     * @param index
     * The array-offset to a requested IndexAttrib object.
     *
     * @return A reference to a single IndexAttrib object contained within
     *this.
     */
    IBOAttrib& get_attrib(const unsigned index) noexcept;
};

/*-----------------------------------------------------------------------------
 * Inline Member Functions for a VertextBuffer.
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Retrieve the buffer type
-------------------------------------*/
inline buffer_use_t IndexBuffer::get_type() const noexcept {
    return buffer_use_t::VBO_BUFFER_ELEMENT;
}

/*-------------------------------------
 * Retrieve the buffer's attribs
-------------------------------------*/
inline unsigned IndexBuffer::get_num_attribs() const noexcept {
    return numAttribs;
}

/*-------------------------------------
 * Retrieve a pointer to the list of internal IBOAttribs
-------------------------------------*/
inline const IBOAttrib* IndexBuffer::get_attribs() const noexcept {
    return pAttribs;
}

/*-------------------------------------
 * Retrieve a reference to a single IndexAttrib contained within *this.
-------------------------------------*/
inline const IBOAttrib& IndexBuffer::get_attrib(const unsigned index) const noexcept {
    return pAttribs[index];
}

/*-------------------------------------
 * Retrieve a reference to a single IndexAttrib contained within *this.
-------------------------------------*/
inline IBOAttrib& IndexBuffer::get_attrib(const unsigned index) noexcept {
    return pAttribs[index];
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_INDEXBUFFER_H__ */
