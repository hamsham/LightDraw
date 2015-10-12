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
    Constructor
-------------------------------------*/
VertexArray::VertexArray() {
}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
VertexArray::VertexArray(VertexArray&& va) :
    vaoId{va.vaoId}
{
    va.vaoId = 0;
}

/*-------------------------------------
    Destructor
-------------------------------------*/
VertexArray::~VertexArray() {
    terminate();
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
VertexArray& VertexArray::operator=(VertexArray&& va) {
    vaoId = va.vaoId;
    va.vaoId = 0;
    return *this;
}

/*-------------------------------------
    Array initialization.
-------------------------------------*/
bool VertexArray::init() {
    if (vaoId != 0) {
        terminate();
    }

    glGenVertexArrays(1, &vaoId);
    return vaoId != 0;
}

} // end draw namespace
} // end ls namespace
