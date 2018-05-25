/*
 * File:   draw/shaderProgram.cpp
 * Author: Miles Lacey
 *
 * Created on January 21, 2014, 6:54 PM
 */

#include <memory> // std::nothrow
#include <utility> // std::move, std::forward

#include "lightsky/utils/Log.h"
#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/ShaderObject.h"
#include "lightsky/draw/ShaderProgram.h"
#include "lightsky/draw/ShaderAttrib.h"
#include "lightsky/draw/UniformBuffer.h"
#include "lightsky/draw/VertexUtils.h"

namespace ls {
namespace draw {



/*-------------------------------------
    Destructor
-------------------------------------*/
ShaderProgram::~ShaderProgram() noexcept {
}



/*-------------------------------------
    Constructor
-------------------------------------*/
ShaderProgram::ShaderProgram() noexcept :
    gpuId{0},
    numUniformBlocks{0},
    uniformBlocks{nullptr},
    uniforms{},
    vertAttribs{},
    fragAttribs{}
{}



/*-------------------------------------
    Copy Constructor
-------------------------------------*/
ShaderProgram::ShaderProgram(const ShaderProgram& sp) noexcept {
    *this = sp;
}



/*-------------------------------------
    Move Constructor
-------------------------------------*/
ShaderProgram::ShaderProgram(ShaderProgram&& sp) noexcept {
    *this = std::move(sp);
}



/*-------------------------------------
    Copy Operator
-------------------------------------*/
ShaderProgram& ShaderProgram::operator =(const ShaderProgram& sp) noexcept {
    gpuId = sp.gpuId;
    numUniformBlocks = sp.numUniformBlocks;
    
    if (numUniformBlocks) {
        uniformBlocks.reset(new(std::nothrow) ShaderBlockAttrib[numUniformBlocks]);
        for (unsigned i = 0; i < numUniformBlocks; ++i) {
            uniformBlocks[i] = sp.uniformBlocks[i];
        }
    }
    else {
        uniformBlocks.reset();
    }
    
    uniforms = sp.uniforms;
    vertAttribs = sp.vertAttribs;
    fragAttribs = sp.fragAttribs;

    return *this;
}



/*-------------------------------------
    Move Operator
-------------------------------------*/
ShaderProgram& ShaderProgram::operator =(ShaderProgram&& sp) noexcept {
    gpuId = sp.gpuId;
    sp.gpuId = 0;
    
    numUniformBlocks = sp.numUniformBlocks;
    sp.numUniformBlocks = 0;
    
    uniformBlocks = std::move(sp.uniformBlocks);
    uniforms = std::move(sp.uniforms);
    vertAttribs = std::move(sp.vertAttribs);
    fragAttribs = std::move(sp.fragAttribs);

    return *this;
}



/*-------------------------------------
    Termination
-------------------------------------*/
void ShaderProgram::terminate() noexcept {
    glDeleteProgram(gpuId);
    gpuId = 0;
    numUniformBlocks = 0;
    uniformBlocks.reset();
    uniforms.reset_num_attribs(0);
    vertAttribs.reset_num_attribs(0);
    fragAttribs.reset_num_attribs(0);
}



/*-------------------------------------
 * Retrieve an attached shader object ID
-------------------------------------*/
GLuint ShaderProgram::get_attached_shader_id(const shader_stage_t shaderType) const noexcept {
    if (!gpu_id()) {
        return 0;
    }

#ifdef LS_DRAW_BACKEND_GL
    LS_DEBUG_ASSERT(
        shaderType == shader_stage_t::SHADER_STAGE_VERTEX ||
        shaderType == shader_stage_t::SHADER_STAGE_GEOMETRY ||
        shaderType == shader_stage_t::SHADER_STAGE_FRAGMENT
    );
#else
    LS_DEBUG_ASSERT(
        shaderType == shader_stage_t::SHADER_STAGE_VERTEX ||
        shaderType == shader_stage_t::SHADER_STAGE_FRAGMENT
    );
#endif

    GLint maxNumShaders = 0;
    GLint numShaders = 0;
    GLuint allShaders[shader_stage_t::SHADER_STAGE_MAX] = {0};

    glGetProgramiv(gpu_id(), GL_ATTACHED_SHADERS, &maxNumShaders);
    LS_LOG_GL_ERR();
    if (maxNumShaders == 0) {
        return 0;
    }

    glGetAttachedShaders(gpu_id(), maxNumShaders, &numShaders, allShaders);
    if (numShaders == 0) {
        return 0;
    }

    while (numShaders--) {
        GLint typeQuery = 0;
        glGetShaderiv(allShaders[numShaders], GL_SHADER_TYPE, &typeQuery);
        LS_LOG_GL_ERR();

        if (typeQuery == shaderType) {
            return allShaders[numShaders];
        }
    }

    return 0;
}



/*-------------------------------------
 * Retrieve the index of a CPU-Side shader block attribute (C-String)
-------------------------------------*/
int ShaderProgram::get_matching_uniform_block_index(const char* const blockName) const noexcept {
    if (!numUniformBlocks || !blockName) {
        return -1;
    }
    
    for (unsigned i = 0; i < numUniformBlocks; ++i) {
        if (uniformBlocks[i].get_block_name() == blockName) {
            return (int)i;
        }
    }
    
    return -1;
}



/*-------------------------------------
 * Retrieve the index of a CPU-Side shader block attribute
-------------------------------------*/
int ShaderProgram::get_matching_uniform_block_index(const std::string& blockName) const noexcept {
    if (!numUniformBlocks) {
        return -1;
    }
    
    for (unsigned i = 0; i < numUniformBlocks; ++i) {
        if (uniformBlocks[i].get_block_name() == blockName) {
            return (int)i;
        }
    }
    
    return -1;
}



/*-------------------------------------
 * Retrieve the index of a CPU-Side shader block attribute
-------------------------------------*/
int ShaderProgram::get_matching_uniform_block_index(const UniformBuffer& ubo) const noexcept {
    return get_matching_uniform_block_index(ubo.get_attribs().get_block_name());
}



} // end draw namespace
} // end ls namespace
