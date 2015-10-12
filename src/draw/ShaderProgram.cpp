/* 
 * File:   draw/shaderProgram.cpp
 * Author: Miles Lacey
 * 
 * Created on January 21, 2014, 6:54 PM
 */

#include <utility>

#include "lightsky/draw/ShaderProgram.h"
#include "lightsky/draw/VertexAttrib.h"

namespace ls {
namespace draw {

/*-------------------------------------
    Constructor
-------------------------------------*/
ShaderProgram::ShaderProgram() :
    gpuId{0}
{}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
ShaderProgram::ShaderProgram(ShaderProgram&& sp) :
    gpuId{sp.gpuId}
{
    sp.gpuId = 0;
}

/*-------------------------------------
    Destructor
-------------------------------------*/
ShaderProgram::~ShaderProgram() {
    terminate();
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& sp) {
    gpuId = sp.gpuId;
    sp.gpuId = 0;
    return *this;
}

/*-------------------------------------
    Termination
-------------------------------------*/
void ShaderProgram::terminate() {
    glDeleteProgram(gpuId);
    gpuId = 0;
}

/*-------------------------------------
    Attaching Shaders
-------------------------------------*/
bool ShaderProgram::init(
    const vertexShader& vs,
    const fragmentShader& fs
) {
    if (gpuId != 0) {
        glDeleteProgram(gpuId);
    }
    else {
        gpuId = glCreateProgram();
        if (gpuId == 0) {
            LS_LOG_ERR("Unable to create a GLSL Program Handle.\n");
            return false;
        }
    }
    
    glAttachShader(gpuId, vs.gpuId);
    glAttachShader(gpuId, fs.gpuId);
    
    return true;
}

/*-------------------------------------
    Linking
-------------------------------------*/
bool ShaderProgram::link() {
    GLint linkResult = 0;
    
    glLinkProgram(gpuId);
    glGetProgramiv(gpuId, GL_LINK_STATUS, &linkResult);
    
    if (linkResult != GL_TRUE) {
        GLint logLength = 0;
        GLchar* logData = nullptr;
        glGetProgramiv(gpuId, GL_INFO_LOG_LENGTH, &logLength);
        logData = new GLchar[logLength+1];
        logData[logLength] = '\0';

        glGetProgramInfoLog(gpuId, logLength, nullptr, logData);
        LS_LOG_ERR("Program linkage error:\n", logData, '\n');

        delete [] logData;
        terminate();
        
        return false;
    }
    
    return true;
}

/*-------------------------------------
 * Shader Uniform information
-------------------------------------*/
std::string ShaderProgram::get_attrib_name(
    const vertex_attrib_t attribType,
    const GLint index,
    GLint* const outVarSize,
    GLenum* const outVarType
) const {
    GLint maxVarNameLen = 0;
    
    const GLenum paramType = attribType == vertex_attrib_t::UNIFORM_ATTRIB
        ? GL_ACTIVE_UNIFORM_MAX_LENGTH
        : GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
    
    glGetProgramiv(gpuId, paramType, &maxVarNameLen);
    
    if (maxVarNameLen < 1) {
        return std::string{};
    }
    
    std::string varName;
    GLsizei varNameLen = 0;
    varName.resize(maxVarNameLen);
    
    if (attribType == vertex_attrib_t::UNIFORM_ATTRIB) {
        glGetActiveUniform(gpuId, index, maxVarNameLen, &varNameLen, outVarSize, outVarType, &varName[0]);
    }
    else {
        glGetActiveAttrib(gpuId, index, maxVarNameLen, &varNameLen, outVarSize, outVarType, &varName[0]);
    }
    
    varName.resize(varNameLen);
    return varName;
}

/*-------------------------------------
 * Get all shader uniform names
-------------------------------------*/
std::vector<VertexAttrib> ShaderProgram::get_attribs(const vertex_attrib_t attribType) const {
    const GLenum paramType = attribType == vertex_attrib_t::UNIFORM_ATTRIB
        ? GL_ACTIVE_UNIFORMS
        : GL_ACTIVE_ATTRIBUTES;
    GLint total = 0;
    glGetProgramiv(gpuId, paramType, &total);
    
    std::vector<VertexAttrib> ret{(unsigned)total};
    
    
    for (int i = 0; i < total; ++i) {
        VertexAttrib& attrib = ret[i];
        
        attrib.name = std::move(get_attrib_name(
            attribType, i, &attrib.components, (GLenum*)&attrib.type
        ));
    }
    
    return ret;
}

/*-------------------------------------
    Uniform information
-------------------------------------*/
std::string ShaderProgram::get_uniform_name(
    int index,
    GLint* const varSize,
    GLenum* const varType
) const {
    GLint maxVarNameLen = 0;
    
    glGetProgramiv(gpuId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxVarNameLen);
    
    if (maxVarNameLen < 1) {
        return std::string{};
    }
    
    GLchar* varName = nullptr;
    GLsizei* varNameLen = nullptr;
    
    glGetActiveUniform(
        gpuId, index, maxVarNameLen, varNameLen, varSize, varType, varName
    );
    
    if (varNameLen == nullptr || *varNameLen < 1 || varName == nullptr) {
        return std::string{};
    }
    
    return std::string{varName, (std::size_t)varNameLen};
}

} // end draw namespace
} // end ls namespace
