
/*
 * File:   RenderValidation.h
 * Author: miles
 *
 * Created on February 22, 2016, 2:47 PM
 */

#ifndef __LS_DRAW_RENDER_VALIDATION_H__
#define __LS_DRAW_RENDER_VALIDATION_H__

namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class VertexArray;

class ShaderProgram;

class UniformBuffer;



/*-----------------------------------------------------------------------------
 * Functions to help determine what object can be rendered to/from
-----------------------------------------------------------------------------*/
/**
 * Determine if a VAO can have its data rendered with a particular shader
 * program by comparing the data in their attribute arrays.
 *
 * The requirements for a VAO to use a shader are very loose. As long as a VAO
 * and Shader have at least one attribute of the same base type with the same
 * number of components and subcomponents, this function will return true.
 *
 * @param prog
 * A constant reference to a valid, linked shader program.
 *
 * @param vao
 * A constant reference to a valid, assembled VertexArray object.
 *
 * @return TRUE if the vertex attributes in the input VAO match enough
 * attributes in the input program to allow for rendering.
 */
bool are_attribs_compatible(const ShaderProgram& prog, const VertexArray& vao) noexcept;



/**
 * Determine if a UBO can have its data rendered with a particular shader
 * program by comparing the data in their ShaderBlockAttrib arrays.
 *
 * The requirements for a UBO to use a shader stricter than with a VAO and a
 * shader. A UBO must contain the same size, number of members, member
 * elements, types, indices, offsets, and strides. The index and binding point
 * between a UBO block attribute and shader block attribute can differ.
 *
 * @param prog
 * A constant reference to a valid, linked shader program.
 * 
 * @param blockIndex
 * An unsigned integral type, containing the index of the input program's
 * ShaderBlockAttrib which will be compared to the input UniformBuffer.
 *
 * @param ubo
 * A constant reference to a valid, assembled UniformBuffer object.
 *
 * @return An integer greater than or equal to 0 if the shader block attributes
 * in the input UBO match enough attributes in the input program to allow for
 * rendering. This return value can be used to specify a binding index at which
 * the UBO can be bound to the input shader. A negative integer will be
 * returned if the input attributes do not match.
 */
int are_attribs_compatible(const ShaderProgram& prog, const unsigned blockIndex, const UniformBuffer& ubo) noexcept;
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_RENDER_VALIDATION_H__ */
