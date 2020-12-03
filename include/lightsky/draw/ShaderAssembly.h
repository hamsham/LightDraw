
/*
 * File:   ShaderAssembly.h
 * Author: miles
 *
 * Created on February 21, 2016, 4:03 PM
 */

#ifndef __LS_DRAW_SHADER_ASSEMBLY_H__
#define __LS_DRAW_SHADER_ASSEMBLY_H__


#include "lightsky/draw/Setup.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class ShaderObject;

class ShaderProgram;

class ShaderAttribArray;



/**----------------------------------------------------------------------------
 * Shader Assembly Class
-----------------------------------------------------------------------------*/
class ShaderProgramAssembly
{
  private:
    /**
     * Pointer of the input vertex shader object which will be used during
     * assembly to generate a ShaderProgram.
     */
    const ShaderObject* pVertShader;

    /**
     * Pointer of the input fragment shader object which will be used
     * during assembly to generate a ShaderProgram.
     */
    const ShaderObject* pFragShader;

#ifdef LS_DRAW_BACKEND_GL

    /**
     * Geometry shaders are supported only on Desktop versions of the
     * renderer. This was initially implemented to help debug vertex
     * normals and tangents.
     */
    const ShaderObject* pGeomShader;

#endif

    /**
     * Assign the layout locations for an unlinked shader program.
     *
     * @param prog
     * A constant reference to a shader program which has not yet been
     * linked. This program will have its layout locations automatically
     * specified based on the meta-data retrieved from shader introspection.
     *
     * @param shdr
     * A constant reference to a Shader Object which contains the layout
     * locations of vertex/fragment shader attributes.
     *
     * @return TRUE if the input shader could have its attributes manually
     * specified, or FALSE if not.
     */
    bool assign_shader_layouts(const ShaderProgram& prog, const ShaderObject& shdr) const noexcept;

    /**
     * Setup all shader attributes and uniforms upon linking a shader
     * program.
     *
     * @return TRUE if a shader was successfully setup with all of its
     * attributes, FALSE if not.
     */
    bool setup_program_attribs(ShaderProgram& outProg) const noexcept;

  public:
    /**
     * Destructor
     */
    ~ShaderProgramAssembly() noexcept;

    /**
     * Constructor
     *
     * Initializes all pointers and values to NULL/0.
     */
    ShaderProgramAssembly() noexcept;

    /**
     * Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another ShaderProgramAssembly object which
     * will be used to initialize *this.
     */
    ShaderProgramAssembly(const ShaderProgramAssembly& a) noexcept;

    /**
     * Move Constructor
     *
     * Moves all data from the input parameter into *this, then resets all
     * values within the input parameter to NULL/0.
     */
    ShaderProgramAssembly(ShaderProgramAssembly&& a) noexcept;

    /**
     * Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another ShaderProgramAssembly object which
     * will be used to set the data in *this.
     *
     * @return A reference to *this.
     */
    ShaderProgramAssembly& operator=(const ShaderProgramAssembly& a) noexcept;

    /**
     * Move Operator
     *
     * Moves all data from the input parameter into *this, then resets all
     * values within the input parameter to NULL/0.
     *
     * @return A reference to *this.
     */
    ShaderProgramAssembly& operator=(ShaderProgramAssembly&& a) noexcept;

    /**
     * Assign a vertex shader to be used in the assembly of a shader
     * program.
     *
     * @param vs
     * A constant reference to a valid, compiled, ShaderObject which
     * represents a vertex shader.
     *
     * @return TRUE if the input parameter is a valid, compiled vertex
     * shader, FALSE if not.
     */
    bool set_vertex_shader(const ShaderObject& vs) noexcept;

    /**
     * Determine if a vertex shader has been assigned to *this assembly.
     *
     * @return TRUE if a vertex shader has been assigned for assembly,
     * FALSE if not.
     */
    bool has_vertex_shader() const noexcept;

    /**
     * Remove the currently attached vertex shader from *this.
     *
     * This function does nothing if no vertex shader has been attached.
     */
    void clear_vertex_shader() noexcept;

    /**
     * Assign a fragment shader to be used in the assembly of a shader
     * program.
     *
     * @param fs
     * A constant reference to a valid, compiled, ShaderObject which
     * represents a fragment shader.
     *
     * @return TRUE if the input parameter is a valid, compiled fragment
     * shader, FALSE if not.
     */
    bool set_fragment_shader(const ShaderObject& fs) noexcept;

    /**
     * Determine if a fragment shader has been assigned to *this assembly.
     *
     * @return TRUE if a fragment shader has been assigned for assembly,
     * FALSE if not.
     */
    bool has_fragment_shader() const noexcept;

    /**
     * Remove the currently attached fragment shader from *this.
     *
     * This function does nothing if no fragment shader has been attached.
     */
    void clear_fragment_shader() noexcept;

#ifdef LS_DRAW_BACKEND_GL

    /**
     * Assign a geometry shader to be used in the assembly of a shader
     * program.
     *
     * @param gs
     * A constant reference to a valid, compiled, ShaderObject which
     * represents a geometry shader.
     *
     * @return TRUE if the input parameter is a valid, compiled geometry
     * shader, FALSE if not.
     */
    bool set_geometry_shader(const ShaderObject& gs) noexcept;

    /**
     * Determine if a geometry shader has been assigned to *this assembly.
     *
     * @return TRUE if a geometry shader has been assigned for assembly,
     * FALSE if not.
     */
    bool has_geometry_shader() const noexcept;

    /**
     * Remove the currently attached geometry shader from *this.
     *
     * This function does nothing if no geometry shader has been attached.
     */
    void clear_geometry_shader() noexcept;

#endif

    /**
     * Remove both the current vertex and fragment shaders from *this
     * assembly (and potentially a geometry shader).
     */
    void clear() noexcept;

    /**
     * Determine if *this assembly has all of the necessary components to
     * generate a ShaderProgram.
     *
     * @return TRUE if a valid, compiled vertex and fragment shader have
     * been attached, FALSE if not.
     */
    bool is_assembly_valid() noexcept;

    /**
     * Generate a ShaderProgram and fill it with valid attribute meta-data
     * which can be used for introspection.
     *
     * @param runLinker
     * This parameter will determine if the generated ShaderProgram object
     * should be both attached to the input ShaderObjects and linked to
     * them or simply attached without linking.
     *
     * @return TRUE if a valid ShaderProgram object was successfully
     * created, FALSE if not.
     */
    bool assemble(ShaderProgram& outProg, bool runLinker = true) noexcept;

    /**
     * Run the GLSL linker & optimizer for a Shader Program which has
     * been assembled but not yet linked.
     *
     * @param outProg
     *
     * @return TRUE if the shader program could successfully link to its
     * compiled shader objects, FALSE if not.
     */
    bool link(ShaderProgram& outProg) const noexcept;
};
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SHADER_ASSEMBLY_H__ */
