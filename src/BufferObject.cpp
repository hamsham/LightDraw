/*
 * File:   draw/BufferObject.cpp
 * Author: Miles Lacey
 *
 * Created on March 31, 2014, 8:00 PM
 */

#include <algorithm>
#include <new>

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Log.h"

#include "lightsky/draw/BufferObject.h"
#include "lightsky/draw/VertexUtils.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * BufferObject Member Functions
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
BufferObject::~BufferObject() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
BufferObject::BufferObject() noexcept :
    gpuId {0}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
BufferObject::BufferObject(const BufferObject& b) noexcept :
    gpuId {b.gpuId}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
BufferObject::BufferObject(BufferObject&& b) noexcept :
gpuId {b.gpuId}
{
    b.gpuId = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
BufferObject& BufferObject::operator =(const BufferObject& b) noexcept {
    this->gpuId = b.gpuId;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
BufferObject& BufferObject::operator =(BufferObject&& b) noexcept {
    this->gpuId = b.gpuId;
    b.gpuId = 0;

    return *this;
}

/*-------------------------------------
 * Buffer initialization
-------------------------------------*/
bool BufferObject::init() noexcept {
    LS_DEBUG_ASSERT(this->is_valid() == false); // insurance
    
    if (!gpuId) {
        glGenBuffers(1, &gpuId);
    }

    return gpuId != 0;
}

/*-------------------------------------
 * Terminate the vertex buffer and release all of its resources back to the GPU.
-------------------------------------*/
void BufferObject::terminate() noexcept {
    if (gpuId) {
        glDeleteBuffers(1, &gpuId);
        gpuId = 0;
    }

    terminate_attribs();
}

/*-------------------------------------
    Determine if *this Buffer Object is currently active.
-------------------------------------*/
bool BufferObject::is_bound() const noexcept {
    GLint typeToCheck = 0;

    switch (get_type()) {
        case buffer_use_t::VBO_BUFFER_ARRAY:
            typeToCheck = GL_ARRAY_BUFFER_BINDING;
            break;

        case buffer_use_t::VBO_BUFFER_ELEMENT:
            typeToCheck = GL_ELEMENT_ARRAY_BUFFER_BINDING;
            break;

        case buffer_use_t::VBO_BUFFER_TRANSFORM_FEEDBACK:
            typeToCheck = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
            break;

        case buffer_use_t::VBO_BUFFER_UNIFORM_BUFFER:
            typeToCheck = GL_UNIFORM_BUFFER_BINDING;
            break;

        default:
            LS_LOG_ERR("Forgot to implement an enumeration for the current buffer type!");
            break;
    }

    // just fail, no room for debug statements here.
    LS_ASSERT(typeToCheck != 0);

    GLint currentBuffer = 0;
    glGetIntegerv(typeToCheck, &currentBuffer);
    return currentBuffer == (GLint)this->gpuId;
}

/*-------------------------------------
 * Copy data from one buffer into another.
-------------------------------------*/
bool BufferObject::copy_data(const BufferObject& from) noexcept {
    LS_DEBUG_ASSERT(from.gpuId != this->gpuId);
    LS_DEBUG_ASSERT(this->is_valid());
    LS_DEBUG_ASSERT(from.is_valid());
    LS_DEBUG_ASSERT(from.get_type() == this->get_type());

    // bind the buffers to OGL's read/write targets to prevent a pipeline stall.
    glBindBuffer(VBO_COPY_READ, from.gpuId);

    GLint numBytes = 0;
    buffer_access_t usage = buffer_access_t::VBO_STATIC_DRAW;

    glGetBufferParameteriv(VBO_COPY_READ, GL_BUFFER_SIZE, &numBytes);
    glGetBufferParameteriv(VBO_COPY_READ, GL_BUFFER_USAGE, (GLint*) & usage);

    if (numBytes != 0 && !this->gpuId) {
        if (!init()) {
            terminate();
            return false;
        }
    }

    glBindBuffer(VBO_COPY_WRITE, this->gpuId);
    glBufferData(VBO_COPY_WRITE, numBytes, nullptr, usage);

    glCopyBufferSubData(VBO_COPY_READ, VBO_COPY_WRITE, 0, 0, numBytes);

    // better safe than sorry
    glBindBuffer(VBO_COPY_READ, 0);
    glBindBuffer(VBO_COPY_WRITE, 0);

    copy_attribs(from);

    return true;
}



} // end draw namespace
/*-----------------------------------------------------------------------------
 * Loose utility functions to run with BufferObjects. All of these functions
 * rely on the currently active buffer.
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Get the read/write access pattern of a buffer.
-------------------------------------*/
GLint draw::get_buffer_access(const buffer_use_t bufferType) {
    GLint usage = 0;
    glGetBufferParameteriv(bufferType, GL_BUFFER_ACCESS_FLAGS, &usage);
    return usage;
}

/*-------------------------------------
 * Determine if a buffer is currently mapped to the CPU.
-------------------------------------*/
bool draw::is_buffer_mapped(const buffer_use_t bufferType) {
    GLint isMapped = 0;
    glGetBufferParameteriv(bufferType, GL_BUFFER_MAPPED, &isMapped);
    return isMapped == GL_TRUE;
}

/*-------------------------------------
 * Get the amount of bytes that have been mapped from the GPU to CPU.
-------------------------------------*/
GLint64 draw::get_buffer_map_length(const buffer_use_t bufferType) {
    GLint64 mapLen = 0;
    glGetBufferParameteri64v(bufferType, GL_BUFFER_MAP_LENGTH, &mapLen);
    return mapLen;
}

/*-------------------------------------
 * Query the offset from the start of a mapped buffer.
-------------------------------------*/
GLint64 draw::get_buffer_map_offset(const buffer_use_t bufferType) {
    GLint64 offset = 0;
    glGetBufferParameteri64v(bufferType, GL_BUFFER_MAP_OFFSET, &offset);
    return offset;
}

/*-------------------------------------
 * Query a buffer's size on the GPU.
-------------------------------------*/
GLint64 draw::get_buffer_size(const buffer_use_t bufferType) {
    GLint64 size = 0;
    glGetBufferParameteri64v(bufferType, GL_BUFFER_SIZE, &size);
    return size;
}

/*-------------------------------------
 * Retrieve a buffer's usage pattern.
-------------------------------------*/
draw::buffer_access_t draw::get_buffer_usage(const buffer_use_t bufferType) {
    GLint usage = 0;
    glGetBufferParameteriv(bufferType, GL_BUFFER_USAGE, &usage);
    return (buffer_access_t)usage;
}

} // end ls namespace
