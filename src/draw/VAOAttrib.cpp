/*
 * File:   draw/vertexAttrib.cpp
 * Author: miles
 *
 * Created on June 10, 2015, 8:52 PM
 */

#include <new> // std::nothrow

#include "lightsky/draw/VAOAttrib.h"
#include "lightsky/draw/VertexBuffer.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Vertex Attrib Object
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
VAOAttrib::~VAOAttrib() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VAOAttrib::VAOAttrib() noexcept :
    names{nullptr},
    vboAttribs{nullptr},
    numAttribs{0}
{}

/*-------------------------------------
 * Preallocation Constructor
-------------------------------------*/
VAOAttrib::VAOAttrib(const unsigned allocCount) noexcept :
// Delegated constructor
    VAOAttrib::VAOAttrib {
        allocCount,
        utils::Pointer <std::string[]> {allocCount ? new(std::nothrow) std::string[allocCount] : nullptr},
        utils::Pointer <VBOAttrib[]> {allocCount ? new(std::nothrow) VBOAttrib[allocCount] : nullptr}
    }
{}

/*-------------------------------------
 * Allocation & Fill Constructor
-------------------------------------*/
VAOAttrib::VAOAttrib(
    const unsigned preallocCount,
    utils::Pointer<std::string[]>&& preallocNames,
    utils::Pointer<VBOAttrib[]>&& preallocAttribs
) noexcept :
    names{nullptr},
    vboAttribs {nullptr},
    numAttribs {0}
{
    if (preallocCount) {
        if (!(preallocNames && preallocAttribs)) {
            constexpr char errMsg[] = "Unable to allocate memory to copy VAO Attributes.";
            utils::runtime_assert(false, utils::error_t::LS_ERROR, errMsg);
        }

        names = std::move(preallocNames);
        vboAttribs = std::move(preallocAttribs);
        numAttribs = preallocCount;
    }
}

/*-------------------------------------
 * Copy constructor
-------------------------------------*/
VAOAttrib::VAOAttrib(const VAOAttrib& vaoAttrib) noexcept {
    *this = vaoAttrib;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VAOAttrib::VAOAttrib(VAOAttrib&& vaoAttrib) noexcept {
    *this = std::move(vaoAttrib);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VAOAttrib& VAOAttrib::operator =(const VAOAttrib& vaoAttrib) noexcept {
    // Check for an early exit by querying for 0 attributes
    if (!vaoAttrib.numAttribs) {
        if (this->numAttribs) {
            names.reset();
            vboAttribs.reset();
            numAttribs = 0;
        }
        return *this;
    }

    // Determine if a reallocation is necessary
    if (vaoAttrib.numAttribs != this->numAttribs) {
        const unsigned allocCount = vaoAttrib.numAttribs;

        names.reset(new(std::nothrow) std::string[allocCount]);
        vboAttribs.reset(new(std::nothrow) VBOAttrib[allocCount]);

        if (allocCount && !(names && vboAttribs)) {
            names.reset();
            vboAttribs.reset();
            numAttribs = 0;

            // Assertion goes here for reentrancy.
            constexpr char errMsg[] = "Unable to allocate memory to copy VAO Attributes.";
            utils::runtime_assert(false, utils::error_t::LS_ERROR, errMsg);
        }
        else {
            numAttribs = allocCount;
        }
    }

    // COPY!
    for (unsigned i = 0; i < numAttribs; ++i) {
        names[i] = vaoAttrib.names[i];
        vboAttribs[i] = vaoAttrib.vboAttribs[i];
    }

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VAOAttrib& VAOAttrib::operator =(VAOAttrib&& vaoAttrib) noexcept {
    names = std::move(vaoAttrib.names);
    vboAttribs = std::move(vaoAttrib.vboAttribs);
    numAttribs = vaoAttrib.numAttribs;

    vaoAttrib.numAttribs = 0;

    return *this;
}

/*-------------------------------------
 * Attribute Reallocation
-------------------------------------*/
bool VAOAttrib::set_num_attribs(const unsigned attribCount) noexcept {
    if (attribCount == numAttribs) {
        return true;
    }

    if (!attribCount) {
        names.reset();
        vboAttribs.reset();
        numAttribs = 0;
        return true;
    }

    VAOAttrib tempAttrib {attribCount, std::move(names), std::move(vboAttribs)};

    *this = VAOAttrib {attribCount};

    // noexcept guarantee
    if (!this->numAttribs) {
        *this = std::move(tempAttrib);
        return false;
    }

    // Keep track of the number of attribs to copy over
    const unsigned maxAttribCount = this->numAttribs < tempAttrib.numAttribs
        ? this->numAttribs
        : tempAttrib.numAttribs;

    // Move old attribs into *this
    for (unsigned i = 0; i < maxAttribCount; ++i) {
        names[i] = std::move(tempAttrib.names[i]);
        vboAttribs[i] = std::move(tempAttrib.vboAttribs[i]);
    }

    return true;
}

/*-------------------------------------
 * Reallocation & Invalidation
-------------------------------------*/
bool VAOAttrib::reset_num_attribs(const unsigned attribCount) noexcept {
    VAOAttrib tempAttrib {attribCount};

    if (tempAttrib.numAttribs != attribCount) {
        return false;
    }
    else {
        *this = std::move(tempAttrib);
    }

    return true;
}

/*-------------------------------------
 * Retrieve the number of valid attribs
-------------------------------------*/
unsigned VAOAttrib::get_num_valid_attribs() const noexcept {
    unsigned totalValid = 0;

    for (unsigned i = 0; i < numAttribs; ++i) {
        if (is_attrib_valid(i)) {
            ++totalValid;
        }
    }

    return totalValid;
}

/*-------------------------------------
 * Invalidate all attribs.
-------------------------------------*/
void VAOAttrib::invalidate_attribs() noexcept {
    for (unsigned i = 0; i < numAttribs; ++i) {
        invalidate_attrib(i);
    }
}

/*-------------------------------------
 * Set a VBO Attrib (const)
-------------------------------------*/
const VBOAttrib& VAOAttrib::get_attrib(const unsigned index) const noexcept {
    LS_DEBUG_ASSERT(index < get_num_attribs());
    return vboAttribs[index];
}

/*-------------------------------------
 * Get a VBO Attrib
-------------------------------------*/
VBOAttrib& VAOAttrib::get_attrib(const unsigned index) noexcept {
    LS_DEBUG_ASSERT(index < get_num_attribs());
    return vboAttribs[index];
}



} // end draw namespace

/*-----------------------------------------------------------------------------
 * Vertex Attrib Helper Functions
-----------------------------------------------------------------------------*/
draw::VAOAttrib draw::create_vertex_attrib(const vertex_data_t type) {
    VAOAttrib attrib {1};
    VBOAttrib& vbo = attrib.get_attrib(0);

    vbo.set_num_elements(1);
    vbo.set_type(type);

    return attrib;
}



} // end ls namespace
