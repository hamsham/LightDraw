
/*
 * File:   VertexBuffer.cpp
 * Author: miles
 *
 * Created on February 13, 2016, 3:15 PM
 */

#include <utility> // std::move()

#include "lightsky/draw/VertexBuffer.h"
#include "lightsky/draw/VertexUtils.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Vertex Buffer Class
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
VertexBuffer::~VertexBuffer() noexcept
{
    terminate_attribs();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VertexBuffer::VertexBuffer() noexcept :
    BufferObject{},
    attribs{(common_vertex_t)0},
    pAttribs{nullptr},
    numAttribs{0}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
VertexBuffer::VertexBuffer(const VertexBuffer& b) noexcept :
    BufferObject{b},
    attribs{(common_vertex_t)0},
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
    attribs{b.attribs},
    pAttribs{b.pAttribs.release()},
    numAttribs{b.numAttribs}
{

    b.attribs = (common_vertex_t)0;
    b.numAttribs = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VertexBuffer& VertexBuffer::operator=(const VertexBuffer& b) noexcept
{
    BufferObject::operator=(b);

    copy_attribs(b);

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VertexBuffer& VertexBuffer::operator=(VertexBuffer&& b) noexcept
{
    BufferObject::operator=(std::move(b));

    attribs = b.attribs;
    b.attribs = (common_vertex_t)0;

    pAttribs.reset(b.pAttribs.release());

    numAttribs = b.numAttribs;
    b.numAttribs = 0;

    return *this;
}

/*-------------------------------------
 * Delete all attributes in use by *this.
-------------------------------------*/
void VertexBuffer::terminate_attribs() noexcept
{
    attribs = (common_vertex_t)0;
    pAttribs.reset();
    numAttribs = 0;
}

/*-------------------------------------
 * Dynamically create the vertex attributes required for a BufferObject.
-------------------------------------*/
bool VertexBuffer::setup_attribs(const common_vertex_t inAttribs) noexcept
{
    return setup_attribs(&inAttribs, 1);
}

/*-------------------------------------
 * Dynamically create the vertex attributes required for a BufferObject.
-------------------------------------*/
bool VertexBuffer::setup_attribs(const common_vertex_t* const pInAttribs, const unsigned attribCount) noexcept
{
    if (!pInAttribs || !attribCount)
    {
        terminate_attribs();
        return true;
    }

    const char* byteOffset = nullptr;
    unsigned totalNumAttribs = 0;

    for (unsigned i = 0; i < attribCount; ++i)
    {
        const common_vertex_t inAttribs = pInAttribs[i];

        totalNumAttribs += math::count_set_bits(inAttribs);
    }

    if (this->numAttribs != totalNumAttribs)
    {
        VBOAttrib* const pNewAttribs = new(std::nothrow) VBOAttrib[totalNumAttribs];

        if (!pNewAttribs)
        {
            return false;
        }

        this->pAttribs.reset(pNewAttribs);
        this->numAttribs = totalNumAttribs;
    }

    unsigned attribIndex = 0;

    for (unsigned i = 0; i < attribCount; ++i)
    {
        const common_vertex_t inAttribs = pInAttribs[i];
        const GLsizei byteStride = get_vertex_byte_size(inAttribs);

        for (unsigned j = 0; j < COMMON_VERTEX_FLAGS_COUNT; ++j)
        {
            if (0 == (inAttribs & COMMON_VERTEX_FLAGS_LIST[j]))
            {
                continue;
            }

            const vertex_data_t dataType = COMMON_VERTEX_TYPES_LIST[j];
            VBOAttrib& attrib = this->pAttribs[attribIndex];

            LS_LOG_MSG("\t\tSetting up attrib ", attribIndex, ':');

            attrib.set_num_elements(1);
            attrib.set_type(dataType);
            attrib.set_normalized(draw::get_vertex_normalization(dataType));
            attrib.set_byte_stride(byteStride);
            attrib.set_offset(byteOffset);

            LS_LOG_MSG(
                "\t\t\tIndex:         ", attribIndex,
                "\n\t\t\tComponents:    ", attrib.get_num_components(),
                "\n\t\t\tElements:      ", attrib.get_num_elements(),
                "\n\t\t\tBase Type:     ", attrib.get_base_type(),
                "\n\t\t\tNormalized:    ", attrib.is_normalized() == GL_TRUE,
                "\n\t\t\tStride:        ", attrib.get_byte_stride(),
                "\n\t\t\tOffset:        ", attrib.get_offset()
            );

            byteOffset += get_num_vertex_bytes(dataType);
            ++attribIndex;
        }

        this->attribs = (common_vertex_t)(this->attribs | inAttribs);
    }

    return true;
}

/*-------------------------------------
 * Copy attribs from the input parameter into *this.
-------------------------------------*/
bool VertexBuffer::copy_attribs(const BufferObject& from) noexcept
{
    LS_DEBUG_ASSERT(from.get_type() == buffer_use_t::VBO_BUFFER_ARRAY);

    const VertexBuffer* const pVbo = static_cast<const VertexBuffer*>(&from);

    attribs = pVbo->attribs;

    return BufferObject::copy_attribs<VertexBuffer, VBOAttrib>(*pVbo, *this);
}
} // end draw namespace
} // end ls namespace
