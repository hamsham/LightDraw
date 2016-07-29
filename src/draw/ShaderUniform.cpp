/*
 * File:   draw/ShaderUniform.cpp
 * Author: miles
 *
 * Created on July 7, 2015, 10:04 PM
 */

#include <cstring> // strlen
#include <utility> // std::move()

#include "lightsky//utils/Assertions.h"
#include "lightsky//utils/Copy.h"
#include "lightsky//utils/Hash.h"

#include "lightsky/draw/ShaderUniform.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Shader Uniform Object
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
ShaderUniform::~ShaderUniform() noexcept {
    terminate();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
ShaderUniform::ShaderUniform() noexcept :
    type{vertex_data_t::VERTEX_DATA_UNKNOWN},
    numElements{0},
    nameHash{0},
    name{nullptr},
    buffer{nullptr}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
ShaderUniform::ShaderUniform(const ShaderUniform& u) noexcept {
    *this = u;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ShaderUniform::ShaderUniform(ShaderUniform&& u) noexcept :
    type{u.type},
    numElements{u.numElements},
    nameHash{u.nameHash},
    name{std::move(u.name)},
    buffer{std::move(u.buffer)}
{
    u.type = vertex_data_t::VERTEX_DATA_UNKNOWN;
    u.numElements = 0;
    u.nameHash = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
ShaderUniform& ShaderUniform::operator=(const ShaderUniform& u) noexcept {
    if (u.buffer) {
        init(u.type, u.buffer.get(), numElements);
    }
    else {
        type = u.type;
        numElements = u.numElements;
        buffer.reset(nullptr);
    }
    
    set_name(u.name.get());
    
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
ShaderUniform& ShaderUniform::operator=(ShaderUniform&& u) noexcept {
    type = u.type;
    u.type = vertex_data_t::VERTEX_DATA_UNKNOWN;
    
    numElements = u.numElements;
    u.numElements = 0;
    
    nameHash = u.nameHash;
    u.nameHash = 0;
    
    name = std::move(u.name);
    
    buffer = std::move(u.buffer);
    
    return *this;
}

/*-------------------------------------
 * Initialization
-------------------------------------*/
bool ShaderUniform::init(const vertex_data_t dataType, const void* const pData, const unsigned elementCount) noexcept {
    const unsigned totalNumBytes = get_num_vertex_bytes(dataType) * elementCount;
    
    if (!totalNumBytes) {
        LS_DEBUG_ASSERT(false);
        buffer.reset(nullptr);
        return false;
    }
    
    type = dataType;
    numElements = elementCount;
    
    buffer.reset(new char[totalNumBytes]);
    utils::fast_memcpy(buffer.get(), pData, totalNumBytes);
    
    return true;
}

/*-------------------------------------
 * Termination
-------------------------------------*/
void ShaderUniform::terminate() noexcept {
    type = vertex_data_t::VERTEX_DATA_UNKNOWN;
    numElements = 0;
    nameHash = 0;
    name.reset(nullptr),
    buffer.reset(nullptr);
}

/*-------------------------------------
 * Validation
-------------------------------------*/
bool ShaderUniform::is_valid() const noexcept {
    return 1
    && type != vertex_data_t::VERTEX_DATA_UNKNOWN
    && numElements > 0
    && nameHash > 0
    && name.get() != nullptr
    && buffer.get() != nullptr;
}

/*-------------------------------------
 * Set the name of a uniform
-------------------------------------*/
bool ShaderUniform::set_name(const char* const uniformName, const unsigned numSrcChars) noexcept {
    unsigned numDestChars = numSrcChars ? numSrcChars : strlen(uniformName);
    
    if (!numDestChars) {
        name.reset(nullptr);
        return false;
    }
    
    name.reset(new(std::nothrow) GLchar[numDestChars+1]);
    
    if (!name) {
        return false;
    }
    
    utils::fast_memcpy(name.get(), uniformName, numDestChars);
    
    name[numDestChars] = '\0';
    
    nameHash = utils::string_hash(name.get());
    
    return true;
}


} // end draw namespace
} // end ls namespace
