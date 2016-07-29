
/*
 * File:   VAOAssembly.h
 * Author: miles
 *
 * Created on February 14, 2016, 2:40 PM
 */

#ifndef __LS_DRAW_VAO_ASSEMBLY_H__
#define __LS_DRAW_VAO_ASSEMBLY_H__

#include <array>
#include <string>
#include <utility> // std::forward

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/VertexBuffer.h"
#include "lightsky/draw/IndexBuffer.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class VAOAttrib;
class VertexArray;



/**----------------------------------------------------------------------------
 * The VAOAssembly class helps to generate Vertex Array Objects for an OpenGL
 * rendering pipeline. It allows client code to easily generate VAO objects
 * and assign them values so they can be manipulated or queried much easier
 * than using standard OpenGL.
-----------------------------------------------------------------------------*/
class VAOAssembly {
    /**
     * Typedef to declare the type which manages a VBO and its index within a
     * VAO.
     */
    typedef std::array<std::pair<const VertexBuffer*, unsigned>, VAO_MAX_VERTEX_ATTRIBS> vbo_assembly_type;

  private:
    /**
     * Pointer to an Index Buffer Object which will be bound to a VAO upon
     * assembly.
     */
    const IndexBuffer* pIndexBuffer;

    /**
     * Array of VBO objects and the indices of the attributes within them.
     * Each attrib (referenced by the second std::pair value) within the
     * appointed VBO (first std::pair value) will be bound to a VAO upon
     * assembly.
     */
    vbo_assembly_type vboIndices;

    /**
     * This array contains the name of each VBO Attribute which will be
     * bound to a VAO upon assembly. These names will help to make bindings
     * of VAO attribs to Shader attribs much easier in the rendering
     * pipeline.
     */
    std::array<std::string, VAO_MAX_VERTEX_ATTRIBS> attribNames;

    /**
     * Reorder the internal VBO objects so all non-NULL VBO pointers in
     * "vboIndices" are at the front of the array.
     *
     * Only values within the "vboIndices" array that are not NULL are
     * bound to the output VAO. to make management of internal resources
     * easier, all non-NULL VBO pointers within "vboIndices" are packed
     * into the front of the array.
     *
     * Vertex names are moved by this function. If a VBO at a particular
     * array index is NULL, the name at the corresponding array index is
     * cleared.
     */
    void pack_vbo_attribs() noexcept;

  public:
    /**
     * Destructor
     *
     * Cleans up and deletes all currently managed CPU memory. Pointers to
     * VBO and IBO objects referenced by *this are not deleted as those
     * are merely file handles.
     */
    ~VAOAssembly() noexcept;

    /**
     * Constructor
     *
     * Initializes all internal pointers and arrays to NULL.
     */
    VAOAssembly() noexcept;

    /**
     * Copy Constructor
     *
     * The copy operations of this class will copy all arrays from the
     * input parameters into *this. It is up to client code to determine
     * whether these functions are called as they will cause memory
     * allocations/
     *
     * @param va
     * A constant reference to a VAOAssembly object who's data will be
     * copied into *this.
     */
    VAOAssembly(const VAOAssembly& va) noexcept;

    /**
     * Move Constructor
     *
     * No memory reallocations are performed by this function.
     *
     * @param va
     * An r-value reference to a VAOAssembly object who's data will be
     * moved into *this.
     */
    VAOAssembly(VAOAssembly&& va) noexcept;

    /**
     * Copy Operator
     *
     * The copy operations of this class will copy all arrays from the
     * input parameters into *this. It is up to client code to determine
     * whether these functions are called as they will cause memory
     * allocations/
     *
     * @param va
     * A constant reference to a VAOAssembly object who's data will be
     * copied into *this.
     *
     * @return A reference to *this.
     */
    VAOAssembly& operator=(const VAOAssembly& va) noexcept;

    /**
     * Move Operator
     *
     * No memory reallocations are performed by this function.
     *
     * @param va
     * An r-value reference to a VAOAssembly object who's data will be
     * moved into *this.
     *
     * @return A reference to *this.
     */
    VAOAssembly& operator=(VAOAssembly&& va) noexcept;

    /**
     * Determine if an IBO object has been added to *this in order to bind
     * it to a VAO object during assembly.
     *
     * @return TRUE if *this references an IBO object, FALSE if not.
     */
    bool is_ibo_set() const noexcept;

    /**
     * Determine if a VBO object has been added to *this in order to bind
     * it to a VAO object during assembly.
     *
     * @return TRUE if *this references a particular VBO object at an
     * assignable VAO attribute index, FALSE if not.
     */
    bool is_vbo_set(unsigned vaoIndex) const noexcept;

    /**
     * Assign or overwrite a particular IBO in *this so it becomes bound to
     * a VAO object during assembly.
     *
     * It is up to client code to manage the lifetime of the input IBO's
     * memory. Each VAOAssembly object will hang onto a pointer to the
     * input IBO until client code determines otherwise.
     *
     * @param ibo
     * A constant reference to an Index Buffer Object which will be bound
     * to a VAO during assembly.
     */
    void set_ibo_attrib(const IndexBuffer& ibo) noexcept;

    /**
     * Assign a VBO to a particular vertex array index during assembly.
     *
     * Calling this function may cause the requested input VBO attrib
     * index to be reassigned. VBOs are tightly packed into the front of
     * the "vboIndices" array. Any NULL values in the "vboAttribs" array
     * are immediately overwritten by the next available VBO pointer.
     *
     * It is up to client code to manage the lifetime of the input VBO's
     * memory. Each VAOAssembly object will hang onto a pointer to the
     * input VBO until client code determines otherwise.
     *
     * @param vaoAttribIndex
     * The suggested vertex array index which the input VBOAttrib from
     * "vbo" will be assigned to. If there are any unassigned attrib
     * indices, this value will change to the first available index.
     *
     * @param vbo
     * A constant reference to a VBO object which will both be bound and
     * have its requested attributes bound to the output VAO during
     * assembly.
     *
     * @param vboAttribIndex
     * An array-offset to the VBOAttrib within "vbo" which will be bound to
     * an output VAO during assembly.
     */
    void set_vbo_attrib(const unsigned vaoAttribIndex, const VertexBuffer& vbo, const unsigned vboAttribIndex) noexcept;

    /**
     * Reassign all internal VAO attrib slots and assign all VBOAttribs to
     * the current set of attribs.
     *
     * @param vbo
     * A constant reference to a Vertex Buffer Object who's attribs will be
     * the only attribs bound to the output VAO during assembly.
     */
    void set_vbo_attribs(const VertexBuffer& vbo) noexcept;

    /**
     * Set the name of a particular VBOAttribue to be bound to a VAO during
     * assembly.
     *
     * Only attributes in *this which have been assigned a VBO and index to
     * a VBOAttrib can be named. If a VBOAttrib at "attribIndex" has not
     * been assigned, this function will do nothing.
     *
     * @param attribIndex
     * The index of the output VAO attrib to be named.
     *
     * @param name
     * Either a constant reference or an r-value reference to an
     * std::string object which will be used as a name for a single vertex
     * attribute.
     *
     * @return TRUE if the attribute at "attribIndex" was successfully
     * named, FALSE if no attribute was currently assigned to the VAO
     * attrib location at "attribIndex".
     */
    template <typename std_string_type>
    bool set_attrib_name(const unsigned attribIndex, std_string_type&& name) noexcept;

    /**
     * Set the name of a particular VBOAttribue to be bound to a VAO during
     * assembly.
     *
     * Only attributes in *this which have been assigned a VBO and index to
     * a VBOAttrib can be named. If a VBOAttrib at "attribIndex" has not
     * been assigned, this function will do nothing.
     *
     * @param attribIndex
     * The index of the output VAO attrib to be named.
     *
     * @param name
     * A constant reference to a C-style string which will be used as a
     * name for a single vertex attribute.
     *
     * @return TRUE if the attribute at "attribIndex" was successfully
     * named, FALSE if no attribute was currently assigned to the VAO
     * attrib location at "attribIndex".
     */
    bool set_attrib_name(const unsigned attribIndex, const char* const pName) noexcept;

    /**
     * Swap vertex array attributes.
     *
     * This function will raise an assertion if either one if the input
     * indices are out of the range of available VAO attribute index
     * values.
     *
     * @param indexA
     * An unsigned integer which references a valid, assigned VBO
     * attribute.
     *
     * @param indexB
     * An unsigned integer which references a valid, assigned VBO
     * attribute.
     */
    void swap_vbo_attribs(const unsigned indexA, const unsigned indexB) noexcept;

    /**
     * Remove the currently referenced Index Buffer Object from *this.
     *
     * This function does nothing if no IBO object had been referenced by
     * *this.
     */
    void clear_ibo() noexcept;

    /**
     * Clear a particular VBO & VBOAttrib reference from *this.
     *
     * This function does nothing if no VBO object had been referenced by
     * *this. Calling this function may cause any attributes beyond
     * "attribIndex" to be moved into the position at "attribIndex".
     *
     * @param attribIndex
     * An array-offset to a particular VBO & VBOAttrib index in *this which
     * will be removed from *this.
     */
    void clear_vbo(const unsigned attribIndex) noexcept;

    /**
     * Remove all references to VBO objects and their attributes from
     *this.
     */
    void clear_vbos() noexcept;

    /**
     * Remove all VBO, VBOAttrib, and IBO references from *this.
     */
    void clear() noexcept;

    /**
     * Retrieve the current number of referenced VBO attributes from *this.
     *
     * @return An unsigned integer which can be used to determine the
     * number of attributes which will be bound to a VAO object upon
     * assembly.
     */
    unsigned get_num_vbo_attribs() const noexcept;

    /**
     * Get the total number of VBO attrib components which will be bound to
     * a VAO object upon construction.
     *
     * For example, scalars and vectors take up a single component
     * (a single layout location in GLSL). Matrices can take up 2-3
     * components depending on their number of rows.
     *
     * @return An integral value which determines the number of total
     * components required of a VAO object.
     */
    unsigned get_num_vbo_attrib_components() const noexcept;

    /**
     * Determine if a VAO can be assembled by *this.
     *
     * This function will fail if the currently referenced VBOs and their
     * VBOAttirb indices have not yet been assigned a name.
     *
     * @return TRUE if *this can assembly all of the currently referenced
     * VBO, IBO, and VBOAttrib objects into a VAO, FALSE if not.
     */
    bool is_assembly_valid() const noexcept;

    /**
     * Assemble the currently reference VBO, IBO, and VBOAttrib values into
     * a CPU-side VertexAttrib object.
     *
     * @param outAttribs
     * A reference to a VertexAttrib object which will be used to contain
     * meta-data about vertex attributes that could be used to create a
     * single VAO.
     *
     * @return TRUE if the output VertexAttrib object was successfully
     * assembled, FALSE if not.
     */
    bool assemble(VAOAttrib& outAttribs) const noexcept;

    /**
     * Assemble the currently reference VBO, IBO, and VBOAttrib values into
     * a VertexArray object which can be instantly used by OpenGL.
     *
     * @param outVAO
     * A reference to a VertexArray object which will be used to contain
     * bindings to the currently reference VBO, IBO, and VBOAttribs.
     *
     * @return TRUE if the output VertexArray object was successfully
     * assembled, FALSE if not.
     */
    bool assemble(VertexArray& outVao) const noexcept;
};

/*-----------------------------------------------------------------------------
 * Member functions for the VAOAssembly class.
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Check if an IBO has been assigned to a VAO attrib location.
-------------------------------------*/
inline bool VAOAssembly::is_ibo_set() const noexcept {
    return pIndexBuffer != nullptr;
}

/*-------------------------------------
 * Check if a VBO has been assigned to a VAO attrib location.
-------------------------------------*/
inline bool VAOAssembly::is_vbo_set(unsigned vaoIndex) const noexcept {
    LS_DEBUG_ASSERT(vaoIndex < VAO_MAX_VERTEX_ATTRIBS);
    return vboIndices[vaoIndex].first != nullptr;
}

/*-------------------------------------
 * Assign an index buffer object to be used with a VAO during assembly
-------------------------------------*/
inline void VAOAssembly::set_ibo_attrib(const IndexBuffer& ibo) noexcept {
    pIndexBuffer = &ibo;
}

/*-------------------------------------
 * Assign a VAO attrib a name.
-------------------------------------*/
template<typename std_string_type>
bool VAOAssembly::set_attrib_name(const unsigned attribIndex, std_string_type&& name) noexcept {
    LS_LOG_MSG("Assigning the name \"", name, "\" to VAO Assembly attribute ", attribIndex, '.');

    LS_DEBUG_ASSERT(attribIndex < VAO_MAX_VERTEX_ATTRIBS);

    if (name.empty()) {
        LS_LOG_MSG("\tFailed. Attempted to use an empty string as the name for ", attribIndex, ".\n");
        return false;
    }

    if (vboIndices[attribIndex].first == nullptr) {
        LS_LOG_MSG("\tFailed. No attribute exists at index ", attribIndex, ".\n");
        return false;
    }

    attribNames[attribIndex] = std::forward<std_string_type>(name);

    LS_LOG_MSG("\tSuccessfully named VAO Assembly attrib ", attribIndex, " to \"", name, "\".\n");

    return true;
}

/*-------------------------------------
 * Remove the currently bound index buffer object
-------------------------------------*/
inline void VAOAssembly::clear_ibo() noexcept {
    pIndexBuffer = nullptr;
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_VAO_ASSEMBLY_H__ */
