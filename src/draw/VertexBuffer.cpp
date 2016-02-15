
/* 
 * File:   VertexBuffer.cpp
 * Author: miles
 * 
 * Created on February 13, 2016, 3:15 PM
 */

#include <utility> // std::move()

#include "lightsky/draw/VertexBuffer.h"
#include "lightsky/draw/VertexUtils.h"



namespace ls {
namespace draw {



/*-------------------------------------
 * Destructor
-------------------------------------*/
VertexBuffer::~VertexBuffer() noexcept {
    terminate_attribs();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VertexBuffer::VertexBuffer() noexcept :
    BufferObject{},
    pAttribs{nullptr},
    numAttribs{0}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
VertexBuffer::VertexBuffer(const VertexBuffer& b) noexcept :
    BufferObject{b},
    pAttribs{nullptr},
    numAttribs{0}
{
    copy_attribs(b);
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VertexBuffer::VertexBuffer(VertexBuffer&& b) noexcept :
    BufferObject{std::move(b)},
    pAttribs{b.pAttribs.release()},
    numAttribs{b.numAttribs}
{
    b.numAttribs = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VertexBuffer& VertexBuffer::operator =(const VertexBuffer& b) noexcept {
    BufferObject::operator=(b);
    
    copy_attribs(b);
    
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VertexBuffer& VertexBuffer::operator =(VertexBuffer&& b) noexcept {
    BufferObject::operator=(std::move(b));
    
    pAttribs.reset(b.pAttribs.release());
    
    numAttribs = b.numAttribs;
    b.numAttribs = 0;
    
    return *this;
}

/*-------------------------------------
 * Delete all attributes in use by *this.
-------------------------------------*/
void VertexBuffer::terminate_attribs() noexcept {
    pAttribs.reset();
    numAttribs = 0;
}

/*-------------------------------------
 * Dynamically create the vertex attributes required for a BufferObject.
-------------------------------------*/
bool VertexBuffer::setup_attribs(const common_vertex_t attribs) noexcept {
    if (attribs == 0) {
        pAttribs.reset();
        numAttribs = 0;
        return true;
    }
    
    const GLsizei   byteStride  = get_vertex_byte_size(attribs);
    const char*     byteOffset  = nullptr;
    const unsigned  attribCount = math::count_set_bits(attribs);
    
    if (this->numAttribs != attribCount) {
        VBOAttrib* const pNewAttribs = new(std::nothrow) VBOAttrib[attribCount];
        
        if (!pNewAttribs) {
            return false;
        }
        
        this->pAttribs.reset(pNewAttribs);
        this->numAttribs = attribCount;
    }
    
    unsigned attribIndex = 0;
    
    for (unsigned i = 0; i < COMMON_VERTEX_FLAGS_COUNT; ++i) {
        if (0 == (attribs & COMMON_VERTEX_FLAGS_LIST[i])) {
            continue;
        }
        
        const vertex_data_t dataType = COMMON_VERTEX_TYPES_LIST[i];
        
        VBOAttrib& attrib = pAttribs[attribIndex];
        attrib.count      = 0;
        attrib.components = get_num_attrib_components(dataType);
        attrib.baseType   = get_attrib_base_type(dataType);
        attrib.normalized = get_attrib_normalization(dataType);
        attrib.stride     = byteStride;
        attrib.offset     = byteOffset; byteOffset += get_num_attrib_bytes(dataType);

        LS_LOG_MSG(
            "\t\tSetting up attrib ", attribIndex, ':',
            "\n\t\t\tIndex:         ", attribIndex,
            "\n\t\t\tComponents:    ", attrib.components,
            "\n\t\t\tBase Type:     ", attrib.baseType,
            "\n\t\t\tNormalized:    ", attrib.normalized == GL_TRUE,
            "\n\t\t\tStride:        ", attrib.stride,
            "\n\t\t\tOffset:        ", attrib.offset
        );
        
        ++attribIndex;
    }
    
    return true;
}

/*-------------------------------------
 * Copy attribs from the input parameter into *this.
-------------------------------------*/
bool VertexBuffer::copy_attribs(const BufferObject& from) noexcept {
    LS_DEBUG_ASSERT(from.get_type() == buffer_use_t::VBO_BUFFER_ARRAY);
    
    const VertexBuffer* const pVbo = static_cast<const VertexBuffer*>(&from);
    
    return BufferObject::copy_attribs<VertexBuffer, VBOAttrib>(*pVbo, *this);
}



} // end draw namespace
} // end ls namespace
