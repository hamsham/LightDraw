/*
 * File:   draw/BufferObject.h
 * Author: Miles Lacey
 *
 * Created on March 31, 2014, 8:00 PM
 */

#ifndef __LS_DRAW_BUFFER_OBJECT_H__
#define __LS_DRAW_BUFFER_OBJECT_H__

#include <cstddef> // ptrdiff_t

#include "ls/utils/Assertions.h"
#include "ls/utils/Pointer.h"

#include "ls/draw/Setup.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class BufferObject;
enum common_vertex_t : unsigned;
enum index_element_t : int;

/*-----------------------------------------------------------------------------
 * Enumerations
-----------------------------------------------------------------------------*/



/**
 * @brief This enumeration outlines the basic types of buffers are available
 * for use in a render context. It will be updated as more features are
 * written.
 */
enum buffer_use_t : GLenum {
    VBO_BUFFER_ARRAY = GL_ARRAY_BUFFER,
    VBO_BUFFER_ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
    VBO_BUFFER_TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
    VBO_BUFFER_UNIFORM_BUFFER = GL_UNIFORM_BUFFER,

    VBO_BUFFER_PIXEL_UPLOAD = GL_PIXEL_UNPACK_BUFFER,
    VBO_BUFFER_PIXEL_DOWNLOAD = GL_PIXEL_PACK_BUFFER,
};



/**
 * @brief Different usage parameters that determine how a buffer may be used.
 */
enum buffer_access_t : GLenum {
    VBO_STATIC_READ = GL_STATIC_READ,
    VBO_STATIC_DRAW = GL_STATIC_DRAW,
    VBO_STATIC_COPY = GL_STATIC_COPY,

    VBO_STREAM_READ = GL_STREAM_READ,
    VBO_STREAM_DRAW = GL_STREAM_DRAW,
    VBO_STREAM_COPY = GL_STREAM_COPY,

    VBO_DYNAMIC_READ = GL_DYNAMIC_READ,
    VBO_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    VBO_DYNAMIC_COPY = GL_DYNAMIC_COPY,

    VBO_COPY_READ = GL_COPY_READ_BUFFER,
    VBO_COPY_WRITE = GL_COPY_WRITE_BUFFER
};



/**
 * @brief Memory mapping bit flags used for DMA transfers between CPU and GPU
 * memory.
 */
enum buffer_map_t : GLbitfield {
    VBO_MAP_BIT_READ = GL_MAP_READ_BIT,
    VBO_MAP_BIT_WRITE = GL_MAP_WRITE_BIT,
    VBO_MAP_BIT_INVALIDATE_RANGE = GL_MAP_INVALIDATE_RANGE_BIT,
    VBO_MAP_BIT_INVALIDATE_BUFFER = GL_MAP_INVALIDATE_BUFFER_BIT,
    VBO_MAP_BIT_FLUSH_EXPLICIT = GL_MAP_FLUSH_EXPLICIT_BIT,
    VBO_MAP_BIT_UNSYNCHRONIZED = GL_MAP_UNSYNCHRONIZED_BIT
};



/**----------------------------------------------------------------------------
 * @brief GPU Buffer Object
 *
 * This object provides a more robust interface to OpenGL vertex buffer
 * objects. Different VBO types can be generated by modifying the template
 * parameter.
-----------------------------------------------------------------------------*/
class BufferObject {
  protected:
    /**
     * @brief A handle to the vertex buffer object residing on the GPU.
     */
    uint32_t gpuId;

    /**
     * @brief Release all resources used by an inherited object's
     * attributes.
     */
    virtual void terminate_attribs() noexcept = 0;

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
    virtual bool copy_attribs(const BufferObject& from) noexcept = 0;

    /**
     * @brief Helper function to copy attributes from one VBO/IBO to another.
     *
     * @param from
     * A constant reference to a BufferObject from a derived class who's
     * data will be coped into another BufferObject of the same type.
     *
     * @param to
     * A reference to a BufferObject from a derived class who's data will
     * be replaced with a copy of the attribs within the "from" parameter.
     *
     * @return TRUE if the "to" parameter successfully had its attributes
     * replaced by copies of the data within the "from" parameter.
     */
    template <class bo_type, class attrib_type>
    static bool copy_attribs(const bo_type& from, bo_type& to);

  public:
    /**
     * @brief Destructor
     *
     * Releases all CPU-side resources used by *this.
     */
    virtual ~BufferObject() noexcept;

    /**
     * @brief Standard Constructor.
     */
    BufferObject() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all CPU-side data from the input parameter into *this. No
     * data on the GPU is copied in order to avoid hidden costs when *this
     * is used in an STL container.
     *
     * @param b
     * A buffer object whose data will be copied into *this object's GPU
     * memory.
     */
    BufferObject(const BufferObject& b) noexcept;

    /**
     * @brief Move Constructor
     *
     * This moves the data contained within the constructor parameter into
     * *this. No copies are performed.
     *
     * @param b
     * A buffer object whose data will be moved into *this.
     */
    BufferObject(BufferObject&& b) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all CPU-side data from the input parameter into *this. No
     * data on the GPU is copied in order to avoid hidden costs when *this
     * is used in an STL container.
     *
     * @param b
     * A buffer object whose data will be copied into *this object's CPU
     * memory.
     *
     * @return A reference to *this.
     */
    BufferObject& operator=(const BufferObject& b) noexcept;

    /**
     * @brief Move Operator
     *
     * This moves the data contained within the method parameter into *this.
     * No copies are performed.
     *
     * @param b
     * A vertex buffer object whose data will be moved into *this.
     *
     * @return A reference to *this.
     */
    BufferObject& operator=(BufferObject&& b) noexcept;

    /**
     * @brief Buffer initialization.
     *
     * Generates a vertex buffer object on the GPU that will be used to
     * contain a large pool of data.
     *
     * @param usage
     * Indicates the intended usage of the buffer object being initialized.
     * This parameter can impact context-sensitive GL functionality such as
     * binding a VBO to a VAO or draw calls.
     *
     * @return TRUE if the buffer was successfully created, FALSE if not.
     */
    bool init() noexcept;

    /**
     * @brief Terminate the vertex buffer and release all of its resources back
     * to the GPU.
     *
     * This function implicitly calls the virtual function
     * "terminate_attribs()".
     */
    void terminate() noexcept;

    /**
     * Get the GPU-Assigned ID used by *this.
     *
     * @return An unsigned integral type that correlates to a buffer object on
     * the GPU.
     */
    unsigned gpu_id() const noexcept;

    /**
     * @brief Determine if there is data used by this object
     *
     * @returns true if this object has data residing on GPU memory, false
     * if not.
     */
    bool is_valid() const noexcept;

    /**
     * @brief Bind a vertex buffer to the current global rendering context.
     */
    void bind() const noexcept;

    /**
     * @brief Bind a transform feedback or uniform buffer range to the current
     * global rendering context.
     * 
     * This function is only valid for transform feedback and uniform buffer
     * objects. An assertion will be thrown for all other buffer types.
     * 
     * @param index
     * Specifies a particular index within the buffer's array on the GPU.
     * 
     * @param offset
     * Specifies of offset into the internal buffer's memory store where the
     * binding point will be referenced.
     * 
     * @param numBytes
     * The number of bytes that will be bound to the current rendering context.
     */
    void bind_range(const unsigned index, const ptrdiff_t offset, const ptrdiff_t numBytes) const noexcept;

    /**
     * @brief Bind a transform feedback or uniform buffer to the current global
     * rendering context using a shader-defined index.
     * 
     * This function is only valid for transform feedback and uniform buffer
     * objects. An assertion will be thrown for all other buffer types.
     * 
     * When binding a transform feedback buffer or uniform buffer a fully
     * compiled and linked shader program will have indexed all uniform blocks
     * contained within it. Use the shader-defined index to bind a UBO with
     * this function.
     * 
     * @param index
     * Specifies a particular index within the buffer's array on the GPU. This
     * is usually defined by the GPU and can be retrieved through
     * introspection.
     * 
     * @param bindSlot
     * Specifies of user-defined binding point at which GLSL can reference
     * *this buffer. Each shader will manually require a binding point
     * specified before you can use this binding point.
     */
    void bind_base(const unsigned index) const noexcept;

    /**
     * Query OpenGL to determine if the Buffer Object referenced by *this
     * is bound to the current rendering context.
     *
     * @return TRUE if *this is currently bound to OpenGL's rendering
     * context, FALSE if not.
     */
    bool is_bound() const noexcept;

    /**
     * @brief Unbind a vertex buffer object from the current render context.
     */
    void unbind() const noexcept;

    /**
     * @brief Set the data within the buffer to whatever is set at "pData."
     *
     * @param size
     * The size, in bytes, of the data that the buffer should contain.
     *
     * @param pData
     * A pointer to the data contained within the buffer. This argument can
     * be NULL to indicate that an empty buffer should be allocated and
     * will have its contents filled later.
     *
     * @param usage
     * The usage of this buffer. This can indicate that the buffer is
     * static, should be used to stream data occasionally, or the data is
     * dynamic and will be updated on a regular basis.
     *
     * @return TRUE if the buffer was successfully allocated, FALSE if not.
     */
    void set_data(
        const ptrdiff_t size,
        const void* const pData,
        const buffer_access_t usage
    ) const noexcept;

    /**
     * @brief Modify the data within a buffer to contain whatever is in
     * "pData." at an offset of a specified amount of bytes.
     *
     * @param size
     * The size, in bytes, of the data that the buffer should contain.
     *
     * @param offset
     * The offset, in bytes, to where the data should be placed in *this.
     *
     * @param pData
     * A pointer to the data contained within the buffer.
     *
     */
    void modify(
        const ptrdiff_t offset,
        const ptrdiff_t size,
        const void* const pData
    ) noexcept;

    /**
     * @brief Determine how much memory (in bytes) is currently being used by a
     * buffer on the GPU.
     *
     * @return An unsigned integer containing the number of bytes used by a
     * buffer object on the GPU.
     */
    ptrdiff_t get_size() const noexcept;

    /**
     * @brief Retrieve the current usage of a BufferObject.
     *
     * This usage can help to determine if the data in a buffer is static,
     * dynamic, streamed, readable, writable, copy-able, etc.
     *
     * @return An enumeration of the buffer_access_t type.
     */
    buffer_access_t get_usage() const noexcept;

    /**
     * @brief Map the contents of the buffer into memory in order to perform a
     * DMA transfer of data to the GPU.
     *
     * @param offset
     * The offset, in bytes, to where the data should be placed in *this.
     *
     * @param range
     * The range of bytes that will be mapped to memory.
     *
     * @param access
     * Specifies a combination of access flags indicating the desired
     * access to the range of memory.
     *
     * @returns void*
     * A pointer to the location in memory where the data will be mapped.
     * A NULL pointer may be returned, indicating an error that may have
     * occurred while attempting to map the buffer to memory.
     *
     */
    void* map_data(
        const ptrdiff_t offset,
        const ptrdiff_t range,
        const buffer_map_t access
        ) const noexcept;

    /**
     * @brief Notify the GPU that all data has been uploaded to the requested
     * location in memory and that the DMA transfer can now be performed.
     */
    bool unmap_data() const noexcept;

    /**
     * @brief Copy the GPU data from one buffer object into another.
     *
     * This function implicitly calls the virtual function
     * "copy_attribs()".
     *
     * @param from
     * A reference to a constant buffer object from which *this will have
     * it's data copied.
     */
    bool copy_data(const BufferObject& from) noexcept;

    /**
     * @brief Retrieve the Usage of *this buffer on the GPU.
     *
     * This should help to determine if an OpenGL Buffer object is a
     * vertex, index, texture, uniform, or buffer object of another type.
     *
     * @return A value from the buffer_use_t enumeration which can help to
     * determine what type of buffer *this is at runtime.
     */
    virtual buffer_use_t get_type() const noexcept = 0;

    /**
     * @brief Retrieve the number of buffer attributes that are contained
     * within a buffer object.
     *
     * @return An unsigned integral type which contains the number of
     * attributes contained within a buffer object.
     */
    virtual unsigned get_num_attribs() const noexcept = 0;
};


/*-----------------------------------------------------------------------------
 * Static & Inline Member Functions for a BufferObject.
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Copy attribs from one BufferObject to another.
-------------------------------------*/
template <class bo_type, class attrib_type>
bool BufferObject::copy_attribs(const bo_type& from, bo_type& to) {
    if (!from.pAttribs) {
        if (to.pAttribs) {
            to.pAttribs.reset();
            to.numAttribs = 0;
        }

        return true;
    }

    attrib_type * const pNewAttribs{new(std::nothrow) attrib_type[from.numAttribs]};

    if (!pNewAttribs) {
        return false;
    }

    to.pAttribs.reset(pNewAttribs);

    std::copy(from.pAttribs.get(), from.pAttribs.get() + from.numAttribs, to.pAttribs.get());

    to.numAttribs = from.numAttribs;

    return true;
}

/*-------------------------------------
 * Get the GPU-Assigned ID used by *this.
-------------------------------------*/
inline unsigned BufferObject::gpu_id() const noexcept {
    return gpuId;
}

/*-------------------------------------
 * Determine if there is data used by this object
-------------------------------------*/
inline bool BufferObject::is_valid() const noexcept {
    return this->gpuId != 0;
}

/*-------------------------------------
 * Bind a vertex buffer to the current global rendering context.
-------------------------------------*/
inline void BufferObject::bind() const noexcept {
    glBindBuffer(get_type(), this->gpuId);
}

/*-------------------------------------
 * Bind a transform feedback or uniform buffer index to the current global rendering context.
-------------------------------------*/
inline void BufferObject::bind_range(const unsigned index, const ptrdiff_t offset, const ptrdiff_t numBytes) const noexcept {
    const buffer_use_t type = get_type();
    // Transform feedback offsets must be in multiples of 4, according to the GL spec.
    LS_DEBUG_ASSERT(
        type == buffer_use_t::VBO_BUFFER_UNIFORM_BUFFER ||
        (type == buffer_use_t::VBO_BUFFER_TRANSFORM_FEEDBACK && (offset % 4 == 0))
    );
    glBindBufferRange(type, index, this->gpuId, offset, numBytes);
}

/*-------------------------------------
 * Bind a transform feedback or uniform buffer to a shader uniform index.
-------------------------------------*/
inline void BufferObject::bind_base(const unsigned index) const noexcept {
    const buffer_use_t type = get_type();
    // Transform feedback offsets must be in multiples of 4, according to the GL spec.
    LS_DEBUG_ASSERT(
        type == buffer_use_t::VBO_BUFFER_UNIFORM_BUFFER ||
        type == buffer_use_t::VBO_BUFFER_TRANSFORM_FEEDBACK
    );
    glBindBufferBase(type, index, gpuId);
}

/*-------------------------------------
 * @brief Unbind a vertex buffer object from the current render context.
-------------------------------------*/
inline void BufferObject::unbind() const noexcept {
    glBindBuffer(get_type(), 0);
}

/*-------------------------------------
 * Set the data within the buffer to whatever is set at "pData."
-------------------------------------*/
inline void BufferObject::set_data(const ptrdiff_t size, const void* const pData, const buffer_access_t usage) const noexcept {
    glBufferData(get_type(), size, pData, usage);
}

/*-------------------------------------
 * Modify a subset of data within *this.
-------------------------------------*/
inline void BufferObject::modify(const ptrdiff_t offset, const ptrdiff_t size, const void* const pData) noexcept {
    glBufferSubData(get_type(), offset, size, pData);
}

/*-------------------------------------
 * Get the current byte usage.
-------------------------------------*/
inline ptrdiff_t BufferObject::get_size() const noexcept {
    GLint numBytes = 0;
    glGetBufferParameteriv(get_type(), GL_BUFFER_SIZE, &numBytes);
    return numBytes;
}

/*-------------------------------------
 * Retrieve the current usage of a BufferObject.
-------------------------------------*/
inline buffer_access_t BufferObject::get_usage() const noexcept {
    GLuint usage = 0;
    glGetBufferParameteriv(get_type(), GL_BUFFER_USAGE, (GLint*) & usage);
    return (buffer_access_t) usage;
}

/*-------------------------------------
 * Map the contents of the buffer into memory
-------------------------------------*/
inline void* BufferObject::map_data(const ptrdiff_t offset, const ptrdiff_t range, const buffer_map_t access) const noexcept {
    return glMapBufferRange(get_type(), offset, range, access);
}

/*-------------------------------------
 * Unmap the current buffer
-------------------------------------*/
inline bool BufferObject::unmap_data() const noexcept {
    return glUnmapBuffer(get_type());
}



/*-----------------------------------------------------------------------------
 * Loose utility functions to run with BufferObjects. All of these functions
 * rely on the currently active buffer.
-----------------------------------------------------------------------------*/
/**
 * @brief Retrieve the access pattern used for a type of buffer.
 *
 * @param bufferType
 * A value from the buffer_use_t enumeration which determines the OpenGL buffer
 * binding point to query.
 *
 * @return A GLint, representing the access pattern of "bufferType" The initial
 * return value is GL_READ_WRITE.
 */
GLint get_buffer_access(const buffer_use_t bufferType);

/**
 * @brief Determine if a buffer at a particular binding point is currently
 * mapped for DMA read/write operations.
 *
 * @param bufferType
 * A value from the buffer_use_t enumeration which determines the OpenGL buffer
 * binding point to query.
 *
 * @return TRUE if a buffer bound to the current binding point, specified by
 * "bufferType", is currently mapped for DMA transfers.
 */
bool is_buffer_mapped(const buffer_use_t bufferType);

/**
 * @brief Retrieve the number of bytes of a particular buffer are currently
 * mapped into memory by the GPU.
 *
 * @param bufferType
 * A value from the buffer_use_t enumeration which determines the OpenGL buffer
 * binding point to query.
 *
 * @return A 64-bit integer which determines the number of bytes that a
 * particular buffer type has currently mapped into CPU memory from the GPU.
 */
GLint64 get_buffer_map_length(const buffer_use_t bufferType);

/**
 * @brief Retrieve the number of bytes which have been offset from the
 * starting location of a currently mapped GPU buffer.
 *
 * @param bufferType
 * A value from the buffer_use_t enumeration which determines the OpenGL buffer
 * binding point to query.
 *
 * @return A 64-bit integer which determines the number of bytes that have been
 * offset from the beginning location of a GPU-mapped buffer.
 */
GLint64 get_buffer_map_offset(const buffer_use_t bufferType);

/**
 * @brief Retrieve the total number of bytes that have been allocated for a
 * buffer on the GPU.
 *
 * @param bufferType
 * A value from the buffer_use_t enumeration which determines the OpenGL buffer
 * binding point to query.
 *
 * @return A 64-bit integer which can be used to determine the number of bytes
 * that have been allocated on the GPU to hold a buffer's data.
 */
GLint64 get_buffer_size(const buffer_use_t bufferType);

/**
 * @brief Retrieve the usage pattern of a particular buffer on the GPU.
 *
 *
 * @param bufferType
 * A value from the buffer_use_t enumeration which determines the OpenGL buffer
 * binding point to query.
 *
 * @return A value from the buffer_access_t enumeration which can be used to
 * determine if a buffer is being used to stream, store, read, write, or copy
 * data.
 */
buffer_access_t get_buffer_usage(const buffer_use_t bufferType);



} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_BUFFER_OBJECT_H__ */
