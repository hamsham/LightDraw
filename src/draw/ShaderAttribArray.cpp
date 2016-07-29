
/* 
 * @file: ShaderAttribArray.cpp
 * @author geeds
 * 
 * @date June 30, 2016 4:38 PM
 */

#include "ls/draw/ShaderAttribArray.h"
#include "ls/draw/ShaderObject.h" // shader_stage_t
#include "ls/draw/ShaderProgram.h"
#include "ls/draw/VAOAttrib.h" // vertex_attrib_t

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Shader Attrib Array
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
ShaderAttribArray::~ShaderAttribArray() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
ShaderAttribArray::ShaderAttribArray() noexcept :
    attribs {nullptr},
    numAttribs {0}
{}

/*-------------------------------------
 * Preallocation Constructor
-------------------------------------*/
ShaderAttribArray::ShaderAttribArray(const unsigned allocCount) noexcept :
    // Delegated constructor
    ShaderAttribArray::ShaderAttribArray {
        allocCount, utils::Pointer < ShaderAttrib[]> {allocCount ? new(std::nothrow) ShaderAttrib[allocCount] : nullptr}
    }
{}

/*-------------------------------------
 * Allocation & Fill Constructor
-------------------------------------*/
ShaderAttribArray::ShaderAttribArray(
    const unsigned preallocCount,
    utils::Pointer<ShaderAttrib[]>&& preallocAttribs
) noexcept :
    attribs{nullptr},
    numAttribs {0}
{
    if (preallocCount) {
        if (!preallocAttribs) {
            constexpr char errMsg[] = "Unable to allocate memory to copy VAO Attributes.";
            utils::runtime_assert(false, utils::error_t::LS_ERROR, errMsg);
        }

        attribs = std::move(preallocAttribs);
        numAttribs = preallocCount;
    }
}

/*-------------------------------------
 * Copy constructor
-------------------------------------*/
ShaderAttribArray::ShaderAttribArray(const ShaderAttribArray& shaderAttrib) noexcept {
    *this = shaderAttrib;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ShaderAttribArray::ShaderAttribArray(ShaderAttribArray&& shaderAttrib) noexcept {
    *this = std::move(shaderAttrib);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
ShaderAttribArray& ShaderAttribArray::operator =(const ShaderAttribArray& shaderAttrib) noexcept {
    // Check for an early exit by querying for 0 attributes
    if (!shaderAttrib.numAttribs) {
        if (this->numAttribs) {
            attribs.reset();
            numAttribs = 0;
        }
        return *this;
    }

    // Determine if a reallocation is necessary
    if (shaderAttrib.numAttribs != this->numAttribs) {
        const unsigned allocCount = shaderAttrib.numAttribs;

        attribs.reset(new(std::nothrow) ShaderAttrib[allocCount]);

        if (allocCount && !attribs) {
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
        attribs[i] = shaderAttrib.attribs[i];
    }

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
ShaderAttribArray& ShaderAttribArray::operator =(ShaderAttribArray&& shaderAttrib) noexcept {
    attribs = std::move(shaderAttrib.attribs);
    numAttribs = shaderAttrib.numAttribs;

    shaderAttrib.numAttribs = 0;

    return *this;
}

/*-------------------------------------
 * Attribute Reallocation
-------------------------------------*/
bool ShaderAttribArray::set_num_attribs(const unsigned attribCount) noexcept {
    if (attribCount == numAttribs) {
        return true;
    }

    if (!attribCount) {
        attribs.reset();
        numAttribs = 0;
        return true;
    }

    ShaderAttribArray tempAttrib {attribCount, std::move(attribs)};

    *this = ShaderAttribArray {attribCount};

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
        attribs[i] = std::move(tempAttrib.attribs[i]);
    }

    return true;
}

/*-------------------------------------
 * Reallocation & Invalidation
-------------------------------------*/
bool ShaderAttribArray::reset_num_attribs(const unsigned attribCount) noexcept {
    if (attribCount == this->numAttribs) {
        for (unsigned i = 0; i < attribCount; ++i) {
            this->attribs[i] = ShaderAttrib {};
        }
    }
    else {
        ShaderAttribArray tempAttrib {attribCount};

        if (tempAttrib.numAttribs != attribCount) {
            return false;
        }

        *this = std::move(tempAttrib);
    }

    return true;
}



/*-----------------------------------------------------------------------------
 * Utility functions for Shader Attributes
-----------------------------------------------------------------------------*/
} // end draw namespace

/*-------------------------------------
 * Get all shader attrib/uniform names
-------------------------------------*/
draw::ShaderAttribArray draw::get_shader_attribs(
    const ShaderProgram& prog,
    const vertex_attrib_t attribType
) noexcept {
    LS_DEBUG_ASSERT(
        attribType == vertex_attrib_t::UNIFORM_ATTRIB ||
        attribType == vertex_attrib_t::VERTEX_ATTRIB
    );

    const GLenum paramType = (attribType == vertex_attrib_t::UNIFORM_ATTRIB)
        ? GL_ACTIVE_UNIFORMS
        : GL_ACTIVE_ATTRIBUTES;

    GLint totalAttribs = 0;
    glGetProgramiv(prog.gpu_id(), paramType, &totalAttribs);
    LS_LOG_GL_ERR();

    ShaderAttribArray ret {(unsigned)totalAttribs};

    for (int i = 0; i < totalAttribs; ++i) {
        GLint outVarSize = 0;
        GLenum outVarType = 0;
        ShaderAttrib& attrib = ret.get_attrib(i);
        
        const int inAttribType = attribType == vertex_attrib_t::UNIFORM_ATTRIB
            ? GL_ACTIVE_UNIFORM_MAX_LENGTH
            : GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
        
        utils::Pointer<GLchar[]>&& attribName = draw::get_attrib_name(prog, i, outVarSize, outVarType, inAttribType);
        attrib.set_name(std::move(attribName));
            
        if (attribType == vertex_attrib_t::UNIFORM_ATTRIB) {
            attrib.set_location(glGetUniformLocation(prog.gpu_id(), attribName.get()));
        }
        else {
            attrib.set_location(glGetAttribLocation(prog.gpu_id(), attribName.get()));
        }

        attrib.set_type((vertex_data_t)outVarType);
    }

    return ret;
}

/*-------------------------------------
 * Perform introspection to retrieve all current vertex attributes.
-------------------------------------*/
draw::ShaderAttribArray draw::get_linked_shader_attribs(
    const ShaderProgram& prog,
    const shader_stage_t shaderType
) noexcept {
    // Vertex shader introspection can be run through routines shared with
    // uniform introspection routines.
    if (shaderType != shader_stage_t::SHADER_STAGE_FRAGMENT) {
        return draw::get_shader_attribs(prog, vertex_attrib_t::VERTEX_ATTRIB);
    }

    const GLint shaderId = prog.get_attached_shader_id(shaderType);

    LS_LOG_MSG("Attempting to introspect a fragment shader.");

    if (!shaderId) {
        LS_LOG_ERR("\tFailed introspect a fragment shader. No ID available.\n");
        return false;
    }

    ShaderObject shaderObj {};

    if (!shaderObj.recreate_from_id(shaderId)) {
        LS_LOG_ERR("\tFailed recreate a fragment shader.\n");
        return false;
    }

    // Begin the introspection
    const ShaderAttribArray& shaderAttribs = shaderObj.get_attribs();
    unsigned numCurrentAttribs = 0;

    // determine how many attributes currently exist in *this.
    for (unsigned i = 0; i < shaderAttribs.get_num_attribs(); ++i) {
        const utils::Pointer<GLchar[]>& attribName = shaderAttribs.get_attrib_name(i);
        if (glGetFragDataLocation(prog.gpu_id(), attribName.get()) >= 0) {
            ++numCurrentAttribs;
        }
        LS_LOG_GL_ERR();
    }

    LS_LOG_MSG("\tFound ", numCurrentAttribs, " attributes in the recreated fragment shader.");

    // preallocate some space
    ShaderAttribArray outAttribs;

    if (!outAttribs.reset_num_attribs(numCurrentAttribs)) {
        LS_LOG_ERR("\tFailed to preallocate room in a ShaderAttribArray for ", numCurrentAttribs, " attributes.\n");
        return false;
    }

    // fill the current attribute list
    for (unsigned i = 0; i < numCurrentAttribs; ++i) {
        ShaderAttrib& outAttrib = outAttribs.get_attrib(i);

        // Fragment shader attributes require special care
        for (unsigned j = 0; j < shaderAttribs.get_num_attribs(); ++j) {

            // Check if an output attribute made it past the linking/optimizing
            // stage of the GLSL compiler.
            const utils::Pointer<GLchar[]>& attribName = shaderAttribs.get_attrib_name(j);
            const GLint attribLoc = glGetFragDataLocation(prog.gpu_id(), attribName.get());
            LS_LOG_GL_ERR();

            if (attribLoc < 0) {
                continue;
            }

            outAttrib.set_name(shaderAttribs.get_attrib_name(j));
            outAttrib.set_location(attribLoc);
            outAttrib.set_type(shaderAttribs.get_attrib(j).get_type());
        }
    }

    LS_LOG_MSG("\tSuccessfully found ", numCurrentAttribs, " linked attributes.\n");

    return outAttribs;
}



} // end ls namespace
