
/*
 * File:   IndexBuffer.cpp
 * Author: miles
 *
 * Created on February 13, 2016, 3:19 PM
 */

#include "lightsky/draw/VertexUtils.h"
#include "lightsky/draw/IndexBuffer.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * IndexBuffer Attributes
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
IBOAttrib::~IBOAttrib() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
IBOAttrib::IBOAttrib() noexcept :
    count{0},
    indexType{index_element_t::INDEX_TYPE_DEFAULT},
    offset{nullptr}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
IBOAttrib::IBOAttrib(const IBOAttrib& attrib) noexcept :
    count{attrib.count},
    indexType{attrib.indexType},
    offset{attrib.offset}
{
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
IBOAttrib::IBOAttrib(IBOAttrib&& attrib) noexcept :
    count{attrib.count},
    indexType{attrib.indexType},
    offset{attrib.offset}
{
    attrib.count = 0;
    attrib.indexType = index_element_t::INDEX_TYPE_DEFAULT;
    attrib.offset = nullptr;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
IBOAttrib& IBOAttrib::operator=(const IBOAttrib& attrib) noexcept
{
    count = attrib.count;
    indexType = attrib.indexType;
    offset = attrib.offset;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
IBOAttrib& IBOAttrib::operator=(IBOAttrib&& attrib) noexcept
{
    count = attrib.count;
    attrib.count = 0;

    indexType = attrib.indexType;
    attrib.indexType = index_element_t::INDEX_TYPE_DEFAULT;

    offset = attrib.offset;
    attrib.offset = nullptr;

    return *this;
}



/*-----------------------------------------------------------------------------
 * IndexBuffer Objects
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
IndexBuffer::~IndexBuffer() noexcept
{
    terminate_attribs();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
IndexBuffer::IndexBuffer() noexcept :
    BufferObject{},
    pAttribs{nullptr},
    numAttribs{0}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
IndexBuffer::IndexBuffer(const IndexBuffer& b) noexcept :
    BufferObject{b},
    pAttribs{nullptr},
    numAttribs{0}
{
    copy_attribs(b);
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
IndexBuffer::IndexBuffer(IndexBuffer&& b) noexcept :
    BufferObject{std::move(b)},
    pAttribs{b.pAttribs.release()},
    numAttribs{b.numAttribs}
{
    b.numAttribs = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
IndexBuffer& IndexBuffer::operator=(const IndexBuffer& b) noexcept
{
    BufferObject::operator=(b);

    copy_attribs(b);

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
IndexBuffer& IndexBuffer::operator=(IndexBuffer&& b) noexcept
{
    BufferObject::operator=(std::move(b));

    pAttribs.reset(b.pAttribs.release());

    numAttribs = b.numAttribs;
    b.numAttribs = 0;

    return *this;
}

/*-------------------------------------
 * Delete all attributes in use by *this.
-------------------------------------*/
void IndexBuffer::terminate_attribs() noexcept
{
    pAttribs.reset();
    numAttribs = 0;
}

/*-------------------------------------
 * Dynamically create the index attributes required for a BufferObject.
-------------------------------------*/
bool IndexBuffer::setup_attribs(const unsigned attribCount) noexcept
{
    if (attribCount == 0)
    {
        pAttribs.reset();
        numAttribs = 0;
        return true;
    }

    if (this->numAttribs != attribCount)
    {
        IBOAttrib* const pNewAttribs = new(std::nothrow) IBOAttrib[attribCount];

        if (!pNewAttribs)
        {
            return false;
        }

        this->pAttribs.reset(pNewAttribs);
        this->numAttribs = attribCount;
    }

    for (unsigned i = 0; i < numAttribs; ++i)
    {
        IBOAttrib& pAttrib = pAttribs[i];
        pAttrib.set_attrib_count(0);
        pAttrib.set_attrib_type(index_element_t::INDEX_TYPE_DEFAULT);
        pAttrib.set_attrib_offset(nullptr);
    }

    return true;
}

/*-------------------------------------
 * Copy attribs from the input parameter into *this.
-------------------------------------*/
bool IndexBuffer::copy_attribs(const BufferObject& from) noexcept
{
    LS_DEBUG_ASSERT(from.get_type() == buffer_use_t::VBO_BUFFER_ELEMENT);
    const IndexBuffer* const pIbo = static_cast<const IndexBuffer*>(&from);
    return BufferObject::copy_attribs<IndexBuffer, IBOAttrib>(*pIbo, *this);
}
} // end draw namespace
} // end ls namespace
