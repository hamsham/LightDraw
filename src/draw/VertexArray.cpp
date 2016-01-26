/* 
 * File:   vertexArray.cpp
 * Author: Miles Lacey
 * 
 * Created on February 7, 2014, 8:01 PM
 */

#include "lightsky/draw/VertexArray.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Constructor
-------------------------------------*/
VertexArray::VertexArray() {
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VertexArray::VertexArray(VertexArray&& va) :
    vaoId{va.vaoId}
{
    va.vaoId = 0;
}

/*-------------------------------------
 * Destructor
-------------------------------------*/
VertexArray::~VertexArray() {
    terminate();
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VertexArray& VertexArray::operator=(VertexArray&& va) {
    vaoId = va.vaoId;
    va.vaoId = 0;
    return *this;
}

/*-------------------------------------
 * Array initialization.
-------------------------------------*/
bool VertexArray::init() {
    if (vaoId != 0) {
        terminate();
    }

    glGenVertexArrays(1, &vaoId);
    return vaoId != 0;
}

/*-------------------------------------
 * Set the memory layout/offset of an attribute in the vertex array.
-------------------------------------*/
void VertexArray::set_attrib_offsets(
    VertexAttrib* const pAttribs,
    const unsigned numAttribs,
    const unsigned vertStride
) {
    
    unsigned offset = 0;
    const unsigned stride = vertStride;
    
    for (unsigned i = 0; i < numAttribs; ++i) {
        VertexAttrib& attrib = pAttribs[i];
        
        attrib.index = i;
        attrib.offset = reinterpret_cast<void*>(offset);
        attrib.stride = stride;
        offset += get_num_attrib_bytes(attrib.type);
        
        this->set_attrib_offset(attrib);
    }
}

} // end draw namespace
} // end ls namespace
