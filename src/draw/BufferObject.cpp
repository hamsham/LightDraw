/* 
 * File:   draw/BufferObject.cpp
 * Author: Miles Lacey
 * 
 * Created on March 31, 2014, 8:00 PM
 */

#include <algorithm>
#include <new>

#include "lightsky/draw/BufferObject.h"
#include "lightsky/draw/VertexUtils.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * BufferObject Member Functions
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Constructor
-------------------------------------*/
BufferObject::BufferObject() :
    gpuId{0},
    bufferType{buffer_use_t::VBO_BUFFER_ARRAY},
    numAttribs{0},
    pAttribs{nullptr}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
BufferObject::BufferObject(const BufferObject& b) :
    gpuId{b.gpuId},
    bufferType{b.bufferType},
    numAttribs{b.numAttribs},
    pAttribs{new(std::nothrow) VertexAttrib[b.numAttribs]}
{   
    if (!this->pAttribs) {
        gpuId = 0;
        bufferType = buffer_use_t::VBO_BUFFER_ARRAY; // default
        numAttribs = 0;
    }
    else {
        std::copy(b.pAttribs.get(), b.pAttribs.get()+b.numAttribs, this->pAttribs.get());
    }
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
BufferObject::BufferObject(BufferObject&& b) :
    gpuId{b.gpuId},
    bufferType{b.bufferType},
    numAttribs{b.numAttribs},
    pAttribs{std::move(b.pAttribs)}
{
    b.gpuId = 0;
    b.bufferType = buffer_use_t::VBO_BUFFER_ARRAY; // default
    b.numAttribs = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
BufferObject& BufferObject::operator=(const BufferObject& b) {
    this->gpuId = b.gpuId;
    this->bufferType = b.bufferType;
    this->numAttribs = b.numAttribs;
    this->pAttribs.reset(new(std::nothrow) VertexAttrib[b.numAttribs]);
    
    if (!this->pAttribs) {
        gpuId = 0;
        bufferType = buffer_use_t::VBO_BUFFER_ARRAY;
        numAttribs = 0;
    }
    else {
        std::copy(b.pAttribs.get(), b.pAttribs.get()+b.numAttribs, this->pAttribs.get());
    }
    
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
BufferObject& BufferObject::operator=(BufferObject&& b) {
    this->gpuId = b.gpuId;
    b.gpuId = 0;
    
    this->bufferType = b.bufferType;
    b.bufferType = buffer_use_t::VBO_BUFFER_ARRAY;
    
    this->numAttribs = b.numAttribs;
    b.numAttribs = 0;
    
    this->pAttribs = std::move(b.pAttribs);
    
    return *this;
}

/*-----------------------------------------------------------------------------
 * Buffer Object Loose Functions
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Buffer initialization
-------------------------------------*/
bool init_buffer(BufferObject& buf) {
    if (!buf.gpuId) {
        glGenBuffers(1, &buf.gpuId);
    }
    
    return buf.gpuId != 0;
}

/*-------------------------------------
 * Terminate the vertex buffer and release all of its resources back to the GPU.
-------------------------------------*/
void terminate_buffer(BufferObject& buf) {
    if (buf.gpuId) {
        glDeleteBuffers(1, &buf.gpuId);
        buf.gpuId = 0;
    }
    
    buf.bufferType = buffer_use_t::VBO_BUFFER_ARRAY;
    buf.numAttribs = 0;
    buf.pAttribs.release();
}

/*-------------------------------------
 * Copy data from one buffer into another.
-------------------------------------*/
bool copy_buffer_data(const BufferObject& from, BufferObject& to) {
    LS_DEBUG_ASSERT(to.gpuId != from.gpuId);
    
    // bind the buffers to OGL's read/write targets to prevent a pipeline stall.
    glBindBuffer(VBO_COPY_READ, from.gpuId);
    
    GLint numBytes = 0;
    buffer_access_t usage = buffer_access_t::VBO_STATIC_DRAW;
    
    glGetBufferParameteriv(VBO_COPY_READ, GL_BUFFER_SIZE, &numBytes);
    glGetBufferParameteriv(VBO_COPY_READ, GL_BUFFER_USAGE, (GLint*)&usage);
    
    if (numBytes != 0 && !to.gpuId) {
        glGenBuffers(1, &to.gpuId);
        if (!to.gpuId) {
            terminate_buffer(to);
            return false;
        }
    }
    
    glBindBuffer(VBO_COPY_WRITE, to.gpuId);
    glBufferData(VBO_COPY_WRITE, numBytes, nullptr, usage);
    
    glCopyBufferSubData(VBO_COPY_READ, VBO_COPY_WRITE, 0, 0, numBytes);
    
    // better safe than sorry
    glBindBuffer(VBO_COPY_READ, 0);
    glBindBuffer(VBO_COPY_WRITE, 0);
    
    // last few bits of data to copy
    to.bufferType = from.bufferType;
    to.numAttribs = from.numAttribs;
    to.pAttribs.reset(new(std::nothrow) VertexAttrib[to.numAttribs]);
    
    if (!to.pAttribs) {
        terminate_buffer(to);
    }
    else {
        std::copy(
            from.pAttribs.get(),
            from.pAttribs.get()+from.numAttribs,
            to.pAttribs.get()
        );
    }
    
    return true;
}

/*-------------------------------------
 * Dynamically create the vertex attributes required for a BufferObject.
-------------------------------------*/
bool setup_vertex_buffer_attribs(BufferObject& buf, const common_vertex_t attribs) {
    const unsigned numAttribs = math::count_set_bits(attribs);
    VertexAttrib* pAttribs = nullptr;
    unsigned attribIndex = 0;
    const GLsizei byteStride = get_vertex_byte_size(attribs);
    char* byteOffset = nullptr;
    
    if (buf.numAttribs != numAttribs) {
        buf.numAttribs = 0;
        buf.pAttribs.release();
        pAttribs = new(std::nothrow) VertexAttrib[numAttribs];
        
        if (!pAttribs) {
            return false;
        }
    }
    else {
        pAttribs = buf.pAttribs.get();
    }
    
    auto setBufferAttrib = [&](const unsigned components, const vertex_data_t dataType, const char* const name)->void {
        VertexAttrib& attrib = pAttribs[attribIndex];
        
        attrib.index      = attribIndex; ++attribIndex;
        attrib.components = components;
        attrib.type       = get_attrib_base_type(dataType);
        attrib.normalized = GL_FALSE;
        attrib.stride     = byteStride;
        attrib.offset     = byteOffset; byteOffset += get_num_attrib_bytes(dataType);
        attrib.name       = name;
    };
    
    if (attribs & common_vertex_t::POSITION_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::POSITION_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_position);
    }
    
    if (attribs & common_vertex_t::TEXTURE_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::TEXTURE_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_uv);
    }
    
    if (attribs & common_vertex_t::NORMAL_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::NORMAL_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_normal);
    }
    
    if (attribs & common_vertex_t::TANGENT_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::TANGENT_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_tangent);
    }
    
    if (attribs & common_vertex_t::BITANGENT_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::BITANGENT_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_bitangent);
    }
    
    if (attribs & common_vertex_t::MODEL_MAT_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::MODEL_MAT_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_model_matrix);
    }
    
    if (attribs & common_vertex_t::BONE_ID_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::BONE_ID_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_boneId);
    }
    
    if (attribs & common_vertex_t::BONE_WEIGHT_VERTEX) {
        const vertex_data_t cmp = vertex_data_t::BONE_WEIGHT_VERTEX_TYPE;
        setBufferAttrib(get_num_attrib_components(cmp), cmp, ls::draw::vertex_bone_weight);
    }
    
    return true;
}

/*-------------------------------------
 * Dynamically create the indxe attributes required for a BufferObject.
-------------------------------------*/
bool setup_index_buffer_attribs(BufferObject& buf, const index_element_t indexType) {
    utils::pointer<VertexAttrib[]> pAttrib = nullptr;
    
    if (buf.numAttribs != 1) {
        pAttrib.reset(new(std::nothrow) VertexAttrib[1]);
        
        if (!pAttrib) {
            return false;
        }
        
        buf.pAttribs.swap(pAttrib);
    }
    
    pAttrib->index      = 0;
    pAttrib->components = get_num_attrib_components((vertex_data_t)indexType);
    pAttrib->type       = get_attrib_base_type((vertex_data_t)indexType);
    pAttrib->normalized = GL_FALSE;
    pAttrib->stride     = get_num_attrib_bytes((vertex_data_t)indexType);
    pAttrib->offset     = 0;
    pAttrib->name       = "";
    
    return true;
}

} // end draw namespace
} // end ls namespace
