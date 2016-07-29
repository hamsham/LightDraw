
/*
 * File:   VAOAssembly.cpp
 * Author: miles
 *
 * Created on February 14, 2016, 2:40 PM
 */

#include <memory> // std::nothrow

#include "ls/utils/Log.h"

#include "ls/draw/VertexArray.h"
#include "ls/draw/VAOAssembly.h"



namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
VAOAssembly::~VAOAssembly() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
VAOAssembly::VAOAssembly() noexcept :
    pIndexBuffer {nullptr}
{
    for (vbo_assembly_type::value_type& vboIndex : vboIndices) {
        vboIndex.first = nullptr;
        vboIndex.second = 0;
    }
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
VAOAssembly::VAOAssembly(const VAOAssembly& va) noexcept {
    *this = va;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
VAOAssembly::VAOAssembly(VAOAssembly&& va) noexcept {
    *this = std::move(va);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
VAOAssembly& VAOAssembly::operator =(const VAOAssembly& va) noexcept {
    pIndexBuffer = va.pIndexBuffer;

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        const vbo_assembly_type::value_type& inAttrib = va.vboIndices[i];
        vbo_assembly_type::value_type& outAttrib = this->vboIndices[i];

        outAttrib.first = inAttrib.first;
        outAttrib.second = inAttrib.second;
    }

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        attribNames[i] = va.attribNames[i];
    }

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
VAOAssembly& VAOAssembly::operator =(VAOAssembly&& va) noexcept {
    pIndexBuffer = va.pIndexBuffer;
    va.pIndexBuffer = nullptr;

    // std::array is an aggregate class, it has no move operations available.
    // Manually move each element out of the input VAOAssembly.
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        vbo_assembly_type::value_type& inVboMeta = va.vboIndices[i];
        vbo_assembly_type::value_type& outVboMeta = this->vboIndices[i];

        outVboMeta.first = inVboMeta.first;
        inVboMeta.first = nullptr;

        outVboMeta.second = inVboMeta.second;
        inVboMeta.second = 0;
    }

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        attribNames[i] = std::move(va.attribNames[i]);
    }

    return *this;
}

/*-------------------------------------
 * Pack valid VBO attribs into the front if "vboIndices" and "outAttribs"
-------------------------------------*/
void VAOAssembly::pack_vbo_attribs() noexcept {
    LS_LOG_MSG("Repacking VAO Assembly attributes.");

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        vbo_assembly_type::value_type& current = vboIndices[i];

        if (current.first != nullptr) {
            continue;
        }

        for (unsigned j = i + 1; j < VAO_MAX_VERTEX_ATTRIBS; ++j) {
            vbo_assembly_type::value_type& next = vboIndices[j];

            // Skip NULL values but clear the name
            if (next.first == nullptr) {
                attribNames[i].clear();
                continue;
            }

            LS_LOG_MSG("\tMoving VBO Attrib ", j, " to index ", i, '.');

            current.first = next.first;
            next.first = nullptr;

            current.second = next.second;
            next.second = 0;

            attribNames[i] = std::move(attribNames[j]);

            break;
        }
    }

    LS_LOG_MSG("\tDone.\n");
}

/*-------------------------------------
 * Assign a VBO's VBOAttrib to a VAO Attribute location.
-------------------------------------*/
void VAOAssembly::set_vbo_attrib(const unsigned vaoAttribIndex, const VertexBuffer& vbo, const unsigned vboAttribIndex) noexcept {
    LS_LOG_MSG("Attaching VBO Attrib ", &vbo, '-', vboAttribIndex, " to VAO Attrib ", vaoAttribIndex, ".");

    LS_DEBUG_ASSERT(vboAttribIndex < vbo.get_num_attribs());
    LS_DEBUG_ASSERT(vaoAttribIndex < VAO_MAX_VERTEX_ATTRIBS);

    vboIndices[vaoAttribIndex].first = &vbo;
    vboIndices[vaoAttribIndex].second = vboAttribIndex;

    if (vaoAttribIndex > 0 && vboIndices[vaoAttribIndex - 1].first == nullptr) {
        pack_vbo_attribs();
    }

    LS_LOG_MSG("\tDone.\n");
}

/*-------------------------------------
 * Assign all of a VBO's attributes to an attrib location.
-------------------------------------*/
void VAOAssembly::set_vbo_attribs(const VertexBuffer& vbo) noexcept {
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        if (i < vbo.get_num_attribs()) {
            set_vbo_attrib(i, vbo, i);
            continue;
        }

        if (vboIndices[i].first != nullptr) {
            clear_vbo(i);
        }
    }
}

/*-------------------------------------
 * Assign a VAO attrib a name.
-------------------------------------*/
bool VAOAssembly::set_attrib_name(const unsigned attribIndex, const char* const name) noexcept {
    // delegate to the std::string version
    std::string str {name};
    set_attrib_name(attribIndex, str);
    return true;
}

/*-------------------------------------
 * Swap two vertex attrib locations
-------------------------------------*/
void VAOAssembly::swap_vbo_attribs(const unsigned indexA, const unsigned indexB) noexcept {
    LS_LOG_MSG("Swapping VAO Assembly attributes ", indexA, " and ", indexB, '.');

    LS_ASSERT(indexA < VAO_MAX_VERTEX_ATTRIBS);
    LS_ASSERT(indexB < VAO_MAX_VERTEX_ATTRIBS);

    if (indexA == indexB) {
        LS_LOG_MSG("\tIndex values are the same. Nothing to do.\n");
    }

    const vbo_assembly_type::value_type tempAttrib = vboIndices[indexA];
    vboIndices[indexA] = vboIndices[indexB];
    vboIndices[indexB] = tempAttrib;

    attribNames[indexA].swap(attribNames[indexB]);

    LS_LOG_MSG("\tDone.\n");

    // Keep all valid attribs at the front of the storage array.
    pack_vbo_attribs();
}

/*-------------------------------------
 * clear a single VAO attribute
-------------------------------------*/
void VAOAssembly::clear_vbo(const unsigned attribIndex) noexcept {
    LS_LOG_MSG("Removing vertex attribute ", attribIndex, " from a VAO Assembly.");

    LS_DEBUG_ASSERT(attribIndex < VAO_MAX_VERTEX_ATTRIBS);

    vboIndices[attribIndex].first = nullptr;
    vboIndices[attribIndex].second = 0;
    attribNames[attribIndex].clear();


    LS_LOG_MSG("\tDone.\n");

    // Keep the array packed with no empty slots between vboAttrib indices.
    pack_vbo_attribs();
}

/*-------------------------------------
 * Clear all Vertex attributes
-------------------------------------*/
void VAOAssembly::clear_vbos() noexcept {
    LS_LOG_MSG("Removing all vertex attributes from a VAO Assembly.");

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        vboIndices[i].first = nullptr;
        vboIndices[i].second = 0;
    }

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        attribNames[i].clear();
    }

    LS_LOG_MSG("\tDone.\n");
}

/*-------------------------------------
 * Clear all vertex and index attibutes
-------------------------------------*/
void VAOAssembly::clear() noexcept {
    clear_ibo();
    clear_vbos();
}

/*-------------------------------------
 * get the current number of vertex attribs
-------------------------------------*/
unsigned VAOAssembly::get_num_vbo_attribs() const noexcept {
    unsigned numAttribs = 0;

    for (const vbo_assembly_type::value_type& attrib : vboIndices) {
        if (attrib.first == nullptr) {
            break;
        }
        else {
            ++numAttribs;
        }
    }

    return numAttribs;
}

/*-------------------------------------
 * Get the current number of VAO attribs
-------------------------------------*/
unsigned VAOAssembly::get_num_vbo_attrib_components() const noexcept {
    unsigned numAttribs = 0;

    for (const vbo_assembly_type::value_type& attrib : vboIndices) {
        if (attrib.first == nullptr) {
            break;
        }
        else {
            const VertexBuffer * const pVbo = attrib.first;
            const VBOAttrib& vboAttrib = pVbo->get_attrib(attrib.second);
            //const unsigned numComponents = vboAttrib.get_num_attrib_components();
            const unsigned numSubcomponents = vboAttrib.get_num_subcomponents();

            numAttribs += numSubcomponents;
            //numAttribs += numSubcomponents * numComponents;;
        }
    }

    return numAttribs;
}

/*-------------------------------------
 * Determine if a VAO can be generated
-------------------------------------*/
bool VAOAssembly::is_assembly_valid() const noexcept {
    LS_LOG_MSG("Validating a VAO Assembly.");

    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        const vbo_assembly_type::value_type& current = vboIndices[i];
        const VertexBuffer * const pVbo = current.first;

        // Iterate until the first empty attribute
        if (!pVbo) {
            if (i == 0) {
                LS_LOG_MSG("\tInvalid VAO Assembly found. No vertex attribs exist.\n");
                return false;
            }
            else {
                break;
            }

            if (pVbo->get_num_attribs() == 0) {
                LS_LOG_MSG("\tInvalid VAO Assembly found. No VBO ", i, " contains no attributes.\n");
                return false;
            }
        }

        // ensure all attribs have a name
        if (attribNames[i].empty()) {
            LS_LOG_MSG("\tInvalid VAO Assembly found. Attrib ", i, " has no name.\n");
            return false;
        }
    }

    LS_LOG_MSG("\tDone.\n");

    return true;
}

/*-------------------------------------
 * Assemble CPU-side Vertex Attributes
-------------------------------------*/
bool VAOAssembly::assemble(VAOAttrib& outVaoAttrib) const noexcept {
    // Don't touch the output list for reentrancy
    if (!is_assembly_valid()) {
        return false;
    }

    const unsigned numAttribs = get_num_vbo_attribs();
    const unsigned numSubComponents = get_num_vbo_attrib_components();

    LS_LOG_MSG(
        "Assembling a list of ", numAttribs, " VAO attributes with ",
        numSubComponents, " components."
    );

    if (!outVaoAttrib.reset_num_attribs(numSubComponents)) {
        LS_LOG_MSG("Unable to allocate memory for a VAO attribute list.");
        return false;
    }

    // Create the attrib list
    for (unsigned i = 0; i < numAttribs; ++i) {
        const VertexBuffer& vbo     = *vboIndices[i].first;
        const VBOAttrib& vboAttrib  = vbo.get_attrib(vboIndices[i].second);
        VBOAttrib& outAttrib        = outVaoAttrib.get_attrib(i);

        for (unsigned j = 0; j < vboAttrib.get_num_subcomponents(); ++j) {
            outAttrib.set_num_elements  (vboAttrib.get_num_elements());
            outAttrib.set_type          (draw::get_vertex_subcomponent_type(vboAttrib.get_type()));
            outAttrib.set_normalized    (vboAttrib.is_normalized());
            outAttrib.set_byte_stride   (vboAttrib.get_byte_stride());
            outAttrib.set_offset        (vboAttrib.get_offset());
            outAttrib.set_instance_rate (vboAttrib.get_instance_rate());
        }

        outVaoAttrib.set_attrib_name(i, attribNames[i]);
    }

    LS_LOG_MSG("\tDone. Assembled ", numAttribs, " VAO attributes.\n");

    return true;
}

/*-------------------------------------
 * Assembly a GPU-side VAO
-------------------------------------*/
bool VAOAssembly::assemble(VertexArray& vao) const noexcept {
    if (vao.gpu_id() != 0) {
        LS_LOG_ERR("\tAttempted to assemble a preexisting VAO object.\n");
        return false;
    }

    VAOAttrib outVaoAttrib;

    if (!assemble(outVaoAttrib)) {
        return false;
    }

    LS_LOG_MSG("Creating a VAO to contain assembled CPU-side attributes.");

    GLuint vaoId = 0;
    glGenVertexArrays(1, &vaoId);

    if (!vaoId) {
        LS_LOG_MSG("\tUnable to create a VAO on the GPU.\n");
        return false;
    }
    else {
        vao.gpuId = vaoId;
        vao.attribs = std::move(outVaoAttrib);
    }

    vao.bind();
    LS_LOG_GL_ERR();

    if (pIndexBuffer) {
        pIndexBuffer->bind();
        LS_LOG_GL_ERR();
    }

    const VertexBuffer* pVbo = nullptr;
    unsigned currentAttrib = 0;

    // The number of Primary VBO attribs varies depending on the number of
    // sub-attributes in each. For example, vectors & scalars have 1
    // sub-component but matrices can be 2-4 sub-components.
    for (unsigned i = 0; i < get_num_vbo_attribs(); ++i) {
        const std::string& attribName = attribNames[i];
        const VBOAttrib& vboAttrib = vao.get_attrib(i);

        if (pVbo != vboIndices[i].first) {
            pVbo = vboIndices[i].first;

            LS_DEBUG_ASSERT(pVbo != nullptr);

            pVbo->bind();
            LS_LOG_GL_ERR();
        }

        // Pack all sub-attributes from the current VBO Attribute
        for (unsigned j = 0; j < vboAttrib.get_num_subcomponents(); ++j) {
            LS_LOG_MSG(
                "\t\tAssembling a VAO attribute using a VAOAssembly:",
                "\n\t\t\tIndex:         ", currentAttrib,
                "\n\t\t\tComponents:    ", vboAttrib.get_num_components(),
                "\n\t\t\tSubComponents: ", vboAttrib.get_num_subcomponents(),
                "\n\t\t\tVertex Type:   ", vboAttrib.get_type(),
                "\n\t\t\tBase Type:     ", vboAttrib.get_base_type(),
                "\n\t\t\tNormalized:    ", vboAttrib.is_normalized() == GL_TRUE,
                "\n\t\t\tStride:        ", vboAttrib.get_byte_stride(),
                "\n\t\t\tOffset:        ", vboAttrib.get_offset(),
                "\n\t\t\tName:          ", attribName
            );

            vao.setup_attrib(currentAttrib, vboAttrib);
            LS_LOG_GL_ERR();
            ++currentAttrib;
        }
    }

    // Make sure the current OpenGL context doesn't have any stragglers
    vao.unbind();
    LS_LOG_GL_ERR();

    pVbo->unbind();
    LS_LOG_GL_ERR();

    if (pIndexBuffer) {
        pIndexBuffer->unbind();
    }

    LS_LOG_MSG("\tDone. Successfully assembled VAO ", vao.gpu_id(), ".\n");

    return true;
}



} // end draw namespace
} // end ls namespace
