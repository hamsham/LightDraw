
/*
 * File:   ShaderAttrib.cpp
 * Author: Miles
 *
 * Created on February 18, 2016, 2:30 PM
 */

#include <cstring> // strlen
#include <utility> // std::move
#include <new> // std::nothrow

#include "lightsky/utils/Copy.h"
#include "lightsky/utils/Hash.h"
#include "lightsky/utils/Log.h"

#include "lightsky/draw/Vertex.h"
#include "lightsky/draw/ShaderAttrib.h"
#include "lightsky/draw/ShaderObject.h"
#include "lightsky/draw/ShaderProgram.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Shader Attrib Object
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
ShaderAttrib::~ShaderAttrib() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
ShaderAttrib::ShaderAttrib() noexcept :
    location{GLSL_INVALID_LOCATION},
    components{0},
    type{VERTEX_DATA_UNKNOWN},
    numElements{1},
    nameHash{0},
    name{nullptr}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
ShaderAttrib::ShaderAttrib(const ShaderAttrib& v) noexcept :
    location{v.location},
    components{v.components},
    type{v.type},
    numElements{v.numElements},
    nameHash{0},
    name{nullptr}
{
    set_name(v.name);
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ShaderAttrib::ShaderAttrib(ShaderAttrib&& v) noexcept :
    location {v.location},
    components {v.components},
    type {v.type},
    numElements{v.numElements},
    nameHash{v.nameHash},
    name{std::move(v.name)}
{
    v.location = GLSL_INVALID_LOCATION;
    v.components = 0;
    v.type = VERTEX_DATA_UNKNOWN;
    v.nameHash = 0;
    v.numElements = 1;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
ShaderAttrib& ShaderAttrib::operator =(const ShaderAttrib& v) noexcept {
    location = v.location;
    components = v.components;
    type = v.type;
    numElements = v.numElements;
    
    set_name(v.name);

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
ShaderAttrib& ShaderAttrib::operator =(ShaderAttrib&& v) noexcept {
    location = v.location;
    v.location = GLSL_INVALID_LOCATION;

    components = v.components;
    v.components = 0;

    type = v.type;
    v.type = VERTEX_DATA_UNKNOWN;
    
    numElements = v.numElements;
    v.numElements = 1;

    nameHash = v.nameHash;
    v.nameHash = 0;
    
    name = std::move(v.name);

    return *this;
}

/*-------------------------------------
 * Compare matching attribs
-------------------------------------*/
bool ShaderAttrib::operator==(const ShaderAttrib& s) const noexcept {
    return 1
    && location == s.location
    && components == s.components
    && type == s.type
    && numElements == s.numElements
    && nameHash == s.nameHash;
    // Don't compare the name member, the hash must match
}

/*-------------------------------------
 * Compare mismatched attribs
-------------------------------------*/
bool ShaderAttrib::operator!=(const ShaderAttrib& s) const noexcept {
    return !(*this == s);
}

/*-------------------------------------
 * Set the name of an attribute
-------------------------------------*/
bool ShaderAttrib::set_name(const char* const attribName, const unsigned numSrcChars) noexcept {
    unsigned numDestChars = numSrcChars ? numSrcChars : strlen(attribName);
    
    if (!numDestChars) {
        name.reset(nullptr);
        return false;
    }
    
    name.reset(new(std::nothrow) GLchar[numDestChars+1]);
    
    if (!name) {
        return false;
    }
    
    utils::fast_memcpy(name.get(), attribName, numDestChars);
    
    name[numDestChars] = '\0';
    
    nameHash = utils::string_hash(name.get());
    
    return true;
}



/*-----------------------------------------------------------------------------
 * Utility functions for Shader Attributes
-----------------------------------------------------------------------------*/
} // end draw namespace

/*-------------------------------------
 * Shader Uniform information
-------------------------------------*/
utils::Pointer<GLchar[]> draw::get_attrib_name(
    const ShaderProgram& prog,
    const GLint index,
    GLint& outVarSize,
    GLenum& outVarType,
    const GLint attribMaxLenFlag
    ) noexcept {
    LS_DEBUG_ASSERT(
        attribMaxLenFlag == GL_ACTIVE_UNIFORM_MAX_LENGTH ||
        attribMaxLenFlag == GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
    );

    GLint maxVarNameLen = 0;
    glGetProgramiv(prog.gpu_id(), attribMaxLenFlag, &maxVarNameLen);
    LS_LOG_GL_ERR();

    if (maxVarNameLen < 1) {
        return utils::Pointer<GLchar[]>{nullptr};
    }
    else {
        // OpenGL doesn't account for the null-terminator when retrieving the
        // length of a variable's name.
        maxVarNameLen += 1;
    }

    GLsizei varNameLen = 0;
    utils::Pointer<GLchar[]> varName{new GLchar[maxVarNameLen]};
    
    utils::fast_memset(varName.get(), '\0', maxVarNameLen);

    if (attribMaxLenFlag == GL_ACTIVE_UNIFORM_MAX_LENGTH) {
        glGetActiveUniform(prog.gpu_id(), index, maxVarNameLen, &varNameLen, &outVarSize, &outVarType, varName.get());
    }
    else {
        glGetActiveAttrib(prog.gpu_id(), index, maxVarNameLen, &varNameLen, &outVarSize, &outVarType, varName.get());
    }

    LS_LOG_GL_ERR();

    return varName;
}



} // end ls namespace
