
/*
 * File:   ShaderProgramAssembly.cpp
 * Author: miles
 *
 * Created on February 21, 2016, 4:03 PM
 */

#include <memory> // std::nothrow
#include <utility> // std::move

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/ShaderAttrib.h"
#include "lightsky/draw/ShaderObject.h"
#include "lightsky/draw/ShaderProgram.h"
#include "lightsky/draw/ShaderAssembly.h"

namespace ls {
namespace draw {

#ifdef LS_DRAW_BACKEND_GL
/*-------------------------------------
 * Destructor
-------------------------------------*/
ShaderProgramAssembly::~ShaderProgramAssembly() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
ShaderProgramAssembly::ShaderProgramAssembly() noexcept :
    pVertShader{},
    pFragShader{},
    pGeomShader{}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
ShaderProgramAssembly::ShaderProgramAssembly(const ShaderProgramAssembly& a) noexcept :
    pVertShader{a.pVertShader},
    pFragShader{a.pFragShader},
    pGeomShader{a.pGeomShader}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ShaderProgramAssembly::ShaderProgramAssembly(ShaderProgramAssembly&& a) noexcept :
    pVertShader{a.pVertShader},
    pFragShader{a.pFragShader},
    pGeomShader{a.pGeomShader}
{
    a.pVertShader = nullptr;
    a.pFragShader = nullptr;
    a.pGeomShader = nullptr;
}

#else
/*-------------------------------------
 * Destructor
-------------------------------------*/
ShaderProgramAssembly::~ShaderProgramAssembly() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
ShaderProgramAssembly::ShaderProgramAssembly() noexcept :
    pVertShader{},
    pFragShader{}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
ShaderProgramAssembly::ShaderProgramAssembly(const ShaderProgramAssembly& a) noexcept :
    pVertShader{a.pVertShader},
    pFragShader{a.pFragShader}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ShaderProgramAssembly::ShaderProgramAssembly(ShaderProgramAssembly&& a) noexcept :
    pVertShader{a.pVertShader},
    pFragShader{a.pFragShader}
{
    a.pVertShader = nullptr;
    a.pFragShader = nullptr;
}
#endif /* LS_DRAW_BACKEND_GL */

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
ShaderProgramAssembly& ShaderProgramAssembly::operator =(const ShaderProgramAssembly& a) noexcept {
    pVertShader = a.pVertShader;
    pFragShader = a.pFragShader;

    #ifdef LS_DRAW_BACKEND_GL
        pGeomShader = a.pGeomShader;
    #endif
    
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
ShaderProgramAssembly& ShaderProgramAssembly::operator =(ShaderProgramAssembly&& a) noexcept {
    pVertShader = a.pVertShader;
    a.pVertShader = nullptr;

    pFragShader = a.pFragShader;
    a.pFragShader = nullptr;

    #ifdef LS_DRAW_BACKEND_GL
        pGeomShader = a.pGeomShader;
        a.pGeomShader = nullptr;
    #endif

    return *this;
}

/*-------------------------------------
 * Attach a vertex shader to *this.
-------------------------------------*/
bool ShaderProgramAssembly::set_vertex_shader(const ShaderObject& vs) noexcept {
    if (vs.get_shader_type() != shader_stage_t::SHADER_STAGE_VERTEX) {
        LS_DEBUG_ASSERT(false);
        return false;
    }
    
    if (!vs.is_valid()) {
        LS_DEBUG_ASSERT(false);
        return false;
    }

    pVertShader = &vs;

    return true;
}

/*-------------------------------------
 * Determine if a vertex shader was attached.
-------------------------------------*/
bool ShaderProgramAssembly::has_vertex_shader() const noexcept {
    return pVertShader != nullptr &&
        pVertShader->get_shader_type() == shader_stage_t::SHADER_STAGE_VERTEX &&
        pVertShader->is_valid();
}

/*-------------------------------------
 * Remove the currently attached vertex shader
-------------------------------------*/
void ShaderProgramAssembly::clear_vertex_shader() noexcept {
    pVertShader = nullptr;
}

/*-------------------------------------
 * Assign a fragment shader to *this.
-------------------------------------*/
bool ShaderProgramAssembly::set_fragment_shader(const ShaderObject& fs) noexcept {
    if (fs.get_shader_type() != shader_stage_t::SHADER_STAGE_FRAGMENT) {
        LS_DEBUG_ASSERT(false);
        return false;
    }
    
    if (!fs.is_valid()) {
        LS_DEBUG_ASSERT(false);
        return false;
    }

    pFragShader = &fs;

    return true;
}

/*-------------------------------------
 * Determine if a fragment shader has been attached.
-------------------------------------*/
bool ShaderProgramAssembly::has_fragment_shader() const noexcept {
    return pFragShader != nullptr &&
        pFragShader->get_shader_type() == shader_stage_t::SHADER_STAGE_FRAGMENT &&
        pFragShader->is_valid();
}

/*-------------------------------------
 * Clear an attached fragment shader.
-------------------------------------*/
void ShaderProgramAssembly::clear_fragment_shader() noexcept {
    pFragShader = nullptr;
}

#ifdef LS_DRAW_BACKEND_GL
/*-------------------------------------
 * Assign a geometry shader to *this.
-------------------------------------*/
bool ShaderProgramAssembly::set_geometry_shader(const ShaderObject& gs) noexcept {
    if (gs.get_shader_type() != shader_stage_t::SHADER_STAGE_GEOMETRY) {
        LS_DEBUG_ASSERT(false);
        return false;
    }
    
    if (!gs.is_valid()) {
        LS_DEBUG_ASSERT(false);
        return false;
    }

    pGeomShader = &gs;

    return true;
}

/*-------------------------------------
 * Determine if a geometry shader has been attached.
-------------------------------------*/
bool ShaderProgramAssembly::has_geometry_shader() const noexcept {
    if (pGeomShader != nullptr) {
        return
            pGeomShader->get_shader_type() == shader_stage_t::SHADER_STAGE_GEOMETRY &&
            pGeomShader->is_valid();
    }
    
    return true;
}

/*-------------------------------------
 * Clear an attached geometry shader.
-------------------------------------*/
void ShaderProgramAssembly::clear_geometry_shader() noexcept {
    pGeomShader = nullptr;
}
#endif

/*-------------------------------------
 * Clear all pre-link shader attachments
-------------------------------------*/
void ShaderProgramAssembly::clear() noexcept {
    clear_vertex_shader();
    clear_fragment_shader();

    #ifdef LS_DRAW_BACKEND_GL
        clear_geometry_shader();
    #endif
}

/*-------------------------------------
 * Determine if *this vertex assembly is valid
-------------------------------------*/
bool ShaderProgramAssembly::is_assembly_valid() noexcept {
    #ifdef LS_DRAW_BACKEND_GLES
        return has_vertex_shader() && has_fragment_shader();
    #else
        return has_vertex_shader() && has_geometry_shader() && has_fragment_shader();
    #endif
}

/*-------------------------------------
 * Set the vertex layout locations for an unlinked shader program
-------------------------------------*/
bool ShaderProgramAssembly::assign_shader_layouts(
    const ShaderProgram& prog,
    const ShaderObject& shdr
) const noexcept {

    if (prog.get_attached_shader_id(shdr.get_shader_type()) != shdr.gpu_id()) {
        return false;
    }

    // fragment shaders in OpenGLES 3.0 can't have their location manually
    // specified as glBindFragDataLocation is unavailable. Geometry shaders on
    // desktop GL versions should not have attribs as it increases the amount
    // of required maintenance for a feature with limited support.
    if (shdr.get_shader_type() != shader_stage_t::SHADER_STAGE_VERTEX) {
        return true;
    }

    // Manually assign layout locations so OpenGL doesn't optimize out
    // perfectly valid locations of variables.
    const ShaderAttribArray& attribs = shdr.get_attribs();
    
    for (unsigned i = 0; i < attribs.get_num_attribs(); ++i) {
        const ShaderAttrib& attrib = attribs.get_attrib(i);
        glBindAttribLocation(prog.gpu_id(), attrib.get_location(), attrib.get_name().get());
        LS_LOG_GL_ERR();
    }

    return true;
}

/*-------------------------------------
 * Setup a shader's attributes
-------------------------------------*/
bool ShaderProgramAssembly::setup_program_attribs(ShaderProgram& outProg) const noexcept {
    outProg.uniforms = draw::get_shader_attribs(outProg, vertex_attrib_t::UNIFORM_ATTRIB);
    outProg.vertAttribs = get_linked_shader_attribs(outProg, shader_stage_t::SHADER_STAGE_VERTEX);
    outProg.fragAttribs = get_linked_shader_attribs(outProg, shader_stage_t::SHADER_STAGE_FRAGMENT);
    // not bothering to support geometry shader attributes.

    // Shader stages all need attributes
    if (!outProg.get_vertex_attribs().get_num_attribs()) {
        LS_LOG_ERR("\tFailed to introspect the vertex shader of ", outProg.gpu_id(), ".\n");
        LS_DEBUG_ASSERT(false);
        return false;
    }

    if (!outProg.get_fragment_attribs().get_num_attribs()) {
        LS_LOG_ERR("\tFailed to introspect the fragment shader of ", outProg.gpu_id(), ".\n");
        LS_DEBUG_ASSERT(false);
        return false;
    }
    
    const GLuint shaderId = outProg.gpu_id();
    GLint totalUniformBlocks = 0;
    
    glGetProgramiv(shaderId, GL_ACTIVE_UNIFORM_BLOCKS, &totalUniformBlocks);
    LS_LOG_GL_ERR();
    
    LS_LOG_MSG("\tLocated ", totalUniformBlocks, " uniform blocks in shader ", shaderId, '.');
    outProg.uniformBlocks.reset(new(std::nothrow) ShaderBlockAttrib[totalUniformBlocks]);
    
    if (!outProg.uniformBlocks) {
        LS_LOG_ERR("\tFailed to allocate space for ", totalUniformBlocks, " uniform blocks!");
        LS_DEBUG_ASSERT(false);
        return false;
    }
    
    for (int i = 0; i < totalUniformBlocks; ++i) {
        const bool isIntrospected = outProg.uniformBlocks[i].run_block_introspection(shaderId, i);
        
        if (!isIntrospected) {
            LS_LOG_ERR("\tFailed to introspect uniform block ", i, '.');
            outProg.uniformBlocks.reset();
            LS_DEBUG_ASSERT(false);
            return false;
        }
    }
    
    outProg.numUniformBlocks = totalUniformBlocks;
    
    return true;
}

/*-------------------------------------
 * Attach shader objects to a program
-------------------------------------*/
bool ShaderProgramAssembly::assemble(ShaderProgram& outProg, bool runLinker) noexcept {
    LS_LOG_MSG("Attempting to assemble a Shader Program.");

    if (!is_assembly_valid()) {
        LS_LOG_ERR("\tFailed to assemble a Shader Program. Invalid inputs shader detected.\n");
        return false;
    }

    if (outProg.gpu_id() != 0) {
        LS_LOG_ERR("\tAttempted to assemble a preexisting program object.\n");
        return false;
    }
    else {
        const GLuint progId = glCreateProgram();

        LS_LOG_GL_ERR();

        if (progId == 0) {
            LS_LOG_ERR("\tUnable to create a handle to an OpenGL Shader Program.\n");
            return false;
        }

        outProg.gpuId = progId;
    }

    glAttachShader(outProg.gpu_id(), pVertShader->gpu_id());
    LS_LOG_GL_ERR();

    glAttachShader(outProg.gpu_id(), pFragShader->gpu_id());
    LS_LOG_GL_ERR();
    
    #ifdef LS_DRAW_BACKEND_GL
        if (pGeomShader && pGeomShader->is_valid()) {
            glAttachShader(outProg.gpu_id(), pGeomShader->gpu_id());
            LS_LOG_GL_ERR();
        }
    #endif

    // Ensure all shaders attached to the program correctly
    if (!assign_shader_layouts(outProg, *pVertShader)
    || !assign_shader_layouts(outProg, *pFragShader)
    ) {
        LS_LOG_ERR(
            "\tAn Error occurred while attaching a shader to the shader program ",
            outProg.gpu_id(), " during assembly. The shader program will be destroyed.\n"
        );
        outProg.terminate();
        return false;
    }

    LS_LOG_MSG("\tSuccessfully assembled the Shader Program ", outProg.gpu_id(), ".\n");

    if (runLinker) {
        return link(outProg);
    }

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool ShaderProgramAssembly::link(ShaderProgram& outProg) const noexcept {
    LS_LOG_MSG("Attempting to link the shader ", outProg.gpu_id(), '.');

    GLint linkResult = 0;

    glLinkProgram(outProg.gpu_id());
    LS_LOG_GL_ERR();

    glGetProgramiv(outProg.gpu_id(), GL_LINK_STATUS, &linkResult);
    LS_LOG_GL_ERR();

    GLint logLength = 0;

    glGetProgramiv(outProg.gpu_id(), GL_INFO_LOG_LENGTH, &logLength);
    LS_LOG_GL_ERR();

    utils::Pointer <GLchar[]> logData {new GLchar[logLength + 1]};
    logData[logLength] = '\0';

    glGetProgramInfoLog(outProg.gpu_id(), logLength, nullptr, logData.get());
    LS_LOG_GL_ERR();
    LS_LOG_MSG("\tProgram linker log:\n", logData.get(), '\n');

    if (linkResult != GL_TRUE) {
        LS_DEBUG_ASSERT(false);
        return false;
    }

    LS_LOG_MSG("\tSuccessfully linked shader ", outProg.gpu_id(), ". Now running introspection\n");
    if (!setup_program_attribs(outProg)) {
        LS_LOG_ERR("\tIntrospection of shader", outProg.gpu_id(), "failed during post-link setup.\n");
        return false;
    }

    LS_LOG_MSG(
        "\tDone. Successfully assembled and linked shader ", outProg.gpu_id(), ":",
        "\n\t\tUniforms:            ", outProg.uniforms.get_num_attribs(),
        "\n\t\tVertex Attribs:      ", outProg.vertAttribs.get_num_attribs(),
        "\n\t\tFragment Attribs:    ", outProg.fragAttribs.get_num_attribs(),
        '\n'
    );

    return true;
}



} // end draw namespace
} // end ls namespace
