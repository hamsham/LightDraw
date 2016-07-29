/*
 * File:   draw/vertexArray.h
 * Author: Miles Lacey
 *
 * Created on February 7, 2014, 8:01 PM
 */

#ifndef __LS_DRAW_VERTEX_ARRAY_H__
#define __LS_DRAW_VERTEX_ARRAY_H__

#include "ls/utils/Pointer.h"

#include "ls/draw/Setup.h"
#include "ls/draw/VAOAttrib.h"
#include "VertexBuffer.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class VAOAssembly;



/*-----------------------------------------------------------------------------
 * @brief Vertex Array Object
 *
 * Represents a single VAO within OpenGL. VAO object store the state of
 * multiple vertex buffer object attributes into one container.
-----------------------------------------------------------------------------*/
class VertexArray {
    friend class VAOAssembly;

  private:
    /**
     * @brief A handle to the vertex array object residing on the GPU.
     */
    unsigned gpuId;

    /**
     * The "attribs" parameter acts as a descriptor for the GPU-side VAO
     * attributes bound by a VAO.
     */
    VAOAttrib attribs;

    /**
     * @brief Set the memory layout/offset of all attributes in the vertex
     * array using the validated attribs from a VAO assembly.
     *
     * @param index
     * Specifies a particular VAO attribute to initialize.
     *
     * @param attrib
     * A constant reference to a VBOAttrib object which contains validated
     * parameters required to determine the array positions, offsets, types
     * strides, and normalization parameters for multiple types contained
     * within a VAO object.
     */
    bool setup_attrib(const unsigned index, const VBOAttrib& attrib) noexcept;

  public:
    /**
     * @brief Destructor
     *
     * Releases all CPU-side resources used by *this.
     *
     * It is up to client code to call "vao.terminate()" in order to
     * release GPU-side resources. This is due to the fact that a VAO can
     * be placed into a container such as a std::vector() which
     * automatically calls destructors when reallocating.
     */
    ~VertexArray() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all variables to their default states.
     */
    VertexArray() noexcept;

    /**
     * @brief Assembly Constructor
     *
     * Initializes a VAO object using resources found in a VAOAssembly
     * object.
     */
    VertexArray(const VAOAssembly& assembly) noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all CPu-side data from the input parameter into *this.
     *
     * @param va
     * A vertexArray object whose data will be copied into *this.
     */
    VertexArray(const VertexArray& va) noexcept;

    /**
     * @brief Move Constructor
     *
     * This moves the data contained within the constructor parameter into
     * *this. No copies are performed.
     *
     * @param va
     * A vertexArray object whose data will be moved into *this.
     */
    VertexArray(VertexArray&& va) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all CPu-side data from the input parameter into *this.
     *
     * @param va
     * A vertexArray object whose data will be copied into *this.
     *
     * @return A reference to *this.
     */
    VertexArray& operator=(const VertexArray& va) noexcept;

    /**
     * @brief Move Operator
     *
     * This moves the data contained within the method parameter into *this.
     * No copies are performed.
     *
     * @param va
     * A vertexArray object whose data will be moved into *this.
     *
     * @return A reference to *this.
     */
    VertexArray& operator=(VertexArray&& va) noexcept;

    /**
     * @brief Terminate the vertex array and release all of its resources
     * on the GPU.
     *
     * This function is not called by the destructor.
     */
    void terminate() noexcept;

    /**
     * @brief Determine if there is data used by this object
     *
     * @returns true if this object has data residing on GPU memory, false
     * if not.
     */
    bool is_valid() const noexcept;

    /**
     * @brief Get the GPU-assigned ID for this VAO
     *
     * @return an unsigned integral type representing this VAO.
     */
    unsigned gpu_id() const noexcept;

    /**
     * @brief Enable an attribute contained within the vertex array.
     *
     * @param index
     * The array-index to an attribute that is to be enabled.
     */
    void enable_attrib(const unsigned index) noexcept;

    /**
     * @brief Disable an attribute contained in the array.
     *
     * @param index
     * The array-index to an attribute that is to be disabled.
     */
    void disable_attrib(const unsigned index) noexcept;

    /**
     * Determine if a single VAO attribute has been enabled or disabled
     * in *this.
     *
     * This function will call glGetVertexAttrib(...), possibly causing a
     * stall to the OpenGL pipeline. It is up to client code to make sure
     * *this VAO has been bound to the current OpenGL context in order to
     * return a valid value.
     *
     * @param index
     * The array-index to an attribute that is to be queried.
     *
     * @return GL_TRUE if the selected attribute is enabled for use in a
     * shader, GL_FALSE if not.
     */
    GLboolean is_attrib_enabled(const unsigned index) const noexcept;

    /**
     * Retrieve the VAO attrib object associated with *this.
     *
     * @return A constant reference to a VAOAttrib object which contains
     * CPU-side descriptions of the attribs of *this VAO's GPU data.
     */
    const VAOAttrib& get_attribs() const noexcept;

    /**
     * Retrieve current number of attribs held by *this.
     *
     * @return An integer containing the current number of VAO attribs.
     */
    unsigned get_num_attribs() const noexcept;

    /**
     * Convenience method to retrieve a single VBO attrib descriptor in
     * *this.
     *
     * @param attribIndex
     * The array index of the VBOAttrib object descriptor contained in
     * *this.
     *
     * @return A constant reference to a single VBOAttrib object.
     */
    const VBOAttrib& get_attrib(const unsigned attribIndex) const noexcept;

    /**
     * @brief Retrieve the rate at which an attribute should repeat during
     * instanced draw calls on the GPU.
     *
     * @param index
     * The attribute's index within the array.
     *
     * @return An integer which can be used to determine how many vertices
     * are rendered before an instance of a specific attribute is drawn.
     */
    void get_attrib_instance_rate(const unsigned index) const noexcept;

    /**
     * @brief Set the rate at which an attribute should repeat during
     * instanced draw calls on the GPU.
     *
     * @param index
     * The attribute's index within the array.
     *
     * @param instancesPerAttrib
     * The number of instances that should be drawn before repeating
     * another render of the array attribute.
     */
    void set_attrib_instance_rate(const unsigned index, const unsigned instancesPerAttrib) noexcept;

    /**
     * @brief Bind this vertex array to the current global rendering
     * context.
     *
     * Binding a VAO with a gpu_id() of 0 will actually unbind the active
     * VAO from the current context.
     */
    void bind() const noexcept;

    /**
     * Query OpenGL to determine if the VAO referenced by *this is bound to
     * the current rendering context.
     *
     * @return TRUE if *this VAO is currently bound to OpenGL's rendering
     * context, FALSE if not.
     */
    bool is_bound() const noexcept;

    /**
     * @brief Unbind this vertex array object from the current render
     * context.
     */
    void unbind() const noexcept;
};

/*-------------------------------------
    Terminate the vertex array and release all of its resources back to
    the GPU.
-------------------------------------*/
inline void VertexArray::terminate() noexcept {
    glDeleteVertexArrays(1, &gpuId);
    gpuId = 0;
}

/*-------------------------------------
    Determine if there is data used by this object
-------------------------------------*/
inline bool VertexArray::is_valid() const noexcept {
    return gpuId != 0 && attribs.get_num_attribs() > 0;
}

/*-------------------------------------
    Get the GPU-assigned ID for this VAO
-------------------------------------*/
inline unsigned VertexArray::gpu_id() const noexcept {
    return gpuId;
}

/*-------------------------------------
    Enable an attribute contained within the vertex array.
-------------------------------------*/
inline void VertexArray::enable_attrib(const unsigned index) noexcept {
    glEnableVertexAttribArray(index);
}

/*-------------------------------------
    Disable an attribute contained in the array.
-------------------------------------*/
inline void VertexArray::disable_attrib(const unsigned index) noexcept {
    glDisableVertexAttribArray(index);
}

/*-------------------------------------
 * Retrieve the VAO attrib object associated with *this.
-------------------------------------*/
inline const VAOAttrib& VertexArray::get_attribs() const noexcept {
    return attribs;
}

/*-------------------------------------
 * Convenience method to retrieve the current attrib count.
-------------------------------------*/
inline unsigned VertexArray::get_num_attribs() const noexcept {
    return attribs.get_num_attribs();
}

/*-------------------------------------
 * Convenience method to retrieve a single VBO attrib descriptor in *this.
-------------------------------------*/
inline const VBOAttrib& VertexArray::get_attrib(const unsigned attribIndex) const noexcept {
    LS_DEBUG_ASSERT(attribIndex < attribs.get_num_attribs());
    return attribs.get_attrib(attribIndex);
}

/*-------------------------------------
    Set the rate at which an attribute should repeat during instanced
    draw calls on the GPU.
-------------------------------------*/
inline void VertexArray::set_attrib_instance_rate(const unsigned index, const unsigned instancesPerAttrib) noexcept {
    attribs.get_attrib(index).set_instance_rate(instancesPerAttrib);
    glVertexAttribDivisor(index, instancesPerAttrib);
}

/*-------------------------------------
    Bind this vertex array to the current global rendering context.
-------------------------------------*/
inline void VertexArray::bind() const noexcept {
    glBindVertexArray(gpuId);
}

/*-------------------------------------
    Determine if *this VAO is currently active.
-------------------------------------*/
inline bool VertexArray::is_bound() const noexcept {
    GLint currentVao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);
    return currentVao == (GLint)this->gpuId;
}

/*-------------------------------------
    Unbind this vertex array object from the current render context.
-------------------------------------*/
inline void VertexArray::unbind() const noexcept {
    glBindVertexArray(0);
}

} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_VERTEX_ARRAY_H__ */
