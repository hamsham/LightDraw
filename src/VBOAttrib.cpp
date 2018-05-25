
/*
 * File:   VBOAttrib.cpp
 * Author: miles
 *
 * Created on March 9, 2016, 11:06 AM
 */

#include "lightsky/draw/VAOAttrib.h" // draw::get_attrib_base_type()
#include "lightsky/draw/VBOAttrib.h"
#include "lightsky/draw/VertexUtils.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * VBOAttrib Class
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
VBOAttrib::~VBOAttrib() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VBOAttrib::VBOAttrib() noexcept :
    numElements {0},
    components {0},
    vertType {vertex_data_t::VERTEX_DATA_UNKNOWN},
    normalized {GL_FALSE},
    stride {0},
    offset {nullptr},
    vertsPerAttrib {0}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
VBOAttrib::VBOAttrib(const VBOAttrib& attrib) noexcept :
    numElements {attrib.numElements},
    components {attrib.components},
    vertType {attrib.vertType},
    normalized {attrib.normalized},
    stride {attrib.stride},
    offset {attrib.offset},
    vertsPerAttrib {attrib.vertsPerAttrib}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VBOAttrib::VBOAttrib(VBOAttrib&& attrib) noexcept :
    numElements {attrib.numElements},
    components {attrib.components},
    vertType {attrib.vertType},
    normalized {attrib.normalized},
    stride {attrib.stride},
    offset {attrib.offset},
    vertsPerAttrib {attrib.vertsPerAttrib}
{
    attrib.numElements = 0;
    attrib.components = 0;
    attrib.vertType = vertex_data_t::VERTEX_DATA_UNKNOWN;
    attrib.normalized = GL_FALSE;
    attrib.stride = 0;
    attrib.offset = nullptr;
    attrib.vertsPerAttrib = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VBOAttrib& VBOAttrib::operator =(const VBOAttrib& attrib) noexcept {
    numElements = attrib.numElements;
    components = attrib.components;
    vertType = attrib.vertType;
    normalized = attrib.normalized;
    stride = attrib.stride;
    offset = attrib.offset;
    vertsPerAttrib = attrib.vertsPerAttrib;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VBOAttrib& VBOAttrib::operator =(VBOAttrib&& attrib) noexcept {
    numElements = attrib.numElements;
    attrib.numElements = 0;

    components = attrib.components;
    attrib.components = 0;

    vertType = attrib.vertType;
    attrib.vertType = vertex_data_t::VERTEX_DATA_UNKNOWN;

    normalized = attrib.normalized;
    attrib.normalized = GL_FALSE;

    stride = attrib.stride;
    attrib.stride = 0;

    offset = attrib.offset;
    attrib.offset = nullptr;

    vertsPerAttrib = attrib.vertsPerAttrib;
    attrib.vertsPerAttrib = 0;

    return *this;
}

/*-------------------------------------
 * Set the vertex type and base type.
-------------------------------------*/
void VBOAttrib::set_type(const vertex_data_t vertexType) noexcept {
    vertType = vertexType;
    components = draw::get_vertex_components(vertexType);
}

/*-------------------------------------
 * Retrieve the generic make-up of the current vertex type
-------------------------------------*/
vertex_data_t VBOAttrib::get_base_type() const noexcept {
    return draw::get_vertex_base_type(vertType);
}

/*-------------------------------------
 * Retrieve the number of sub-components for the current vertex type
-------------------------------------*/
unsigned VBOAttrib::get_num_subcomponents() const noexcept {
    return draw::get_vertex_subcomponents(vertType);
}



} // end draw namespace
} // end ls namespace
