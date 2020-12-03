/*
 * File:   vertexArray.cpp
 * Author: Miles Lacey
 *
 * Created on February 7, 2014, 8:01 PM
 */

#include <utility> // std::move()

#include "lightsky/draw/VertexArray.h"
#include "lightsky/draw/VAOAssembly.h"



namespace ls
{
namespace draw
{

/*-------------------------------------
 * Destructor
-------------------------------------*/
VertexArray::~VertexArray() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VertexArray::VertexArray() noexcept :
    gpuId{
        0
    },
    attribs{}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
VertexArray::VertexArray(const VertexArray& va) noexcept :
    gpuId{
        va.gpuId
    },
    attribs{va.attribs}
{
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VertexArray::VertexArray(VertexArray&& va) noexcept :
    gpuId{
        va.gpuId
    },
    attribs{std::move(va.attribs)}
{
    va.gpuId = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VertexArray& VertexArray::operator=(const VertexArray& va) noexcept
{
    gpuId = va.gpuId;
    attribs = va.attribs;
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
{
    gpuId = va.gpuId;
    va.gpuId = 0;

    attribs = std::move(va.attribs);

    return *this;
}

/*-------------------------------------
 Determine if an attrib is active or not within OpenGL.
-------------------------------------*/
GLboolean VertexArray::is_attrib_enabled(const unsigned index) const noexcept
{
    // State management error checking.
    LS_DEBUG_ASSERT(this->is_bound() == true);

    GLint isEnabled = GL_FALSE;
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isEnabled);
    return isEnabled == GL_TRUE;
}

/*-------------------------------------
 * Set the memory layout/offset of an attribute in the vertex array.
-------------------------------------*/
bool VertexArray::setup_attrib(const unsigned index, const VBOAttrib& attrib) noexcept
{
    enable_attrib(index);
    glVertexAttribPointer(
        index,
        attrib.get_num_components(),
        attrib.get_base_type(),
        attrib.is_normalized(),
        attrib.get_byte_stride(),
        attrib.get_offset()
    );

    if (attrib.get_instance_rate() > 0)
    {
        glVertexAttribDivisor(index, attrib.get_instance_rate());
    }

    return true;
}
} // end draw namespace
} // end ls namespace
