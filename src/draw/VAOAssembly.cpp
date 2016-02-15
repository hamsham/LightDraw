
/* 
 * File:   VAOAssembly.cpp
 * Author: miles
 * 
 * Created on February 14, 2016, 2:40 PM
 */

#include <memory> // std::nothrow

#include "lightsky/utils/Log.h"

#include "lightsky/draw/VAOAssembly.h"



namespace ls {
namespace draw {



/*-------------------------------------
-------------------------------------*/
VAOAssembly::~VAOAssembly() noexcept {
}

/*-------------------------------------
-------------------------------------*/
VAOAssembly::VAOAssembly() noexcept :
    pIndexBuffer{nullptr}
{
    for (vbo_assembly_type::value_type& vboIndex : vboIndices) {
        vboIndex.first = nullptr;
        vboIndex.second = 0;
    }
}

/*-------------------------------------
-------------------------------------*/
VAOAssembly::VAOAssembly(const VAOAssembly& va) noexcept {
    *this = va;
}

/*-------------------------------------
-------------------------------------*/
VAOAssembly::VAOAssembly(VAOAssembly&& va) noexcept {
    *this = std::move(va);
}

/*-------------------------------------
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
        outAttribs[i] = va.outAttribs[i];
    }
    
    return *this;
}

/*-------------------------------------
-------------------------------------*/
VAOAssembly& VAOAssembly::operator =(VAOAssembly&& va) noexcept {
    pIndexBuffer = va.pIndexBuffer;
    va.pIndexBuffer = nullptr;
    
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        vbo_assembly_type::value_type& inVboMeta = va.vboIndices[i];
        vbo_assembly_type::value_type& outVboMeta = this->vboIndices[i];
        
        outVboMeta.first = inVboMeta.first;
        inVboMeta.first = nullptr;
        
        outVboMeta.second = inVboMeta.second;
        inVboMeta.second = 0;
    }
    
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        outAttribs[i] = std::move(va.outAttribs[i]);
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
        
        for (unsigned j = i+1; j < VAO_MAX_VERTEX_ATTRIBS; ++j) {
            vbo_assembly_type::value_type& next = vboIndices[j];
            
            if (next.first == nullptr) {
                continue;
            }
            
            LS_LOG_MSG("\tMoving VBO Attrib ", j, " to index ", i, '.');
            
            current.first = next.first;
            next.first = nullptr;
            
            current.second = next.second;
            next.second = 0;
            
            outAttribs[i] = std::move(outAttribs[j]);
            
            break;
        }
    }
    
    LS_LOG_MSG("\tDone.\n");
}

/*-------------------------------------
-------------------------------------*/
void VAOAssembly::set_vbo_attrib(const VertexBuffer& vbo, unsigned vboAttribIndex, unsigned vaoAttribIndex) noexcept {
    LS_LOG_MSG("Attaching VBO Attrib ", vboAttribIndex, " to VAO Attrib ", vaoAttribIndex, ".");
    
    LS_DEBUG_ASSERT(vboAttribIndex < vbo.get_num_attribs());
    LS_DEBUG_ASSERT(vaoAttribIndex < VAO_MAX_VERTEX_ATTRIBS);
    
    vboIndices[vaoAttribIndex].first = &vbo;
    vboIndices[vaoAttribIndex].second = vboAttribIndex;
    
    if (vaoAttribIndex > 0 && vboIndices[vaoAttribIndex-1].first == nullptr) {
        pack_vbo_attribs();
    }
    
    LS_LOG_MSG("\tDone.\n");
}

/*-------------------------------------
-------------------------------------*/
void VAOAssembly::set_vbo_attribs(const VertexBuffer& vbo) noexcept {
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        if (i < vbo.get_num_attribs()) {
            set_vbo_attrib(vbo, i, i);
            continue;
        }
        
        if (vboIndices[i].first != nullptr) {
            clear_vbo(i);
        }
    }
}

/*-------------------------------------
-------------------------------------*/
bool VAOAssembly::set_attrib_name(unsigned attribIndex, const std::string& name) noexcept {
    LS_LOG_MSG("Assigning the name \"", name, "\" to VAO Assembly attribute ", attribIndex, '.');
    
    LS_DEBUG_ASSERT(attribIndex < VAO_MAX_VERTEX_ATTRIBS);
    
    if (vboIndices[attribIndex].first == nullptr) {
        LS_LOG_MSG("\tFailed. No attribute exists at index ", attribIndex, ".\n");
        return false;
    }
    
    outAttribs[attribIndex] = name;
    
    LS_LOG_MSG("\tSuccessfully named VAO Assembly attrib ", attribIndex, " to \"", name, "\".\n");
    
    return true;
}

/*-------------------------------------
-------------------------------------*/
bool VAOAssembly::set_attrib_name(unsigned attribIndex, const char* const name) noexcept {
    LS_LOG_MSG("Assigning the name \"", name, "\" to VAO Assembly attribute ", attribIndex, '.');
    
    LS_DEBUG_ASSERT(attribIndex < VAO_MAX_VERTEX_ATTRIBS);
    
    if (vboIndices[attribIndex].first == nullptr) {
        LS_LOG_MSG("\tFailed. No attribute exists at index ", attribIndex, ".\n");
        return false;
    }
    
    outAttribs[attribIndex] = name;
    
    LS_LOG_MSG("\tSuccessfully named VAO Assembly attrib ", attribIndex, " to \"", name, "\".\n");
    
    return true;
}

/*-------------------------------------
-------------------------------------*/
void VAOAssembly::swap_vbo_attribs(unsigned indexA, unsigned indexB) noexcept {
    LS_LOG_MSG("Swapping VAO Assembly attributes ", indexA, " and ", indexB, '.');
    
    LS_DEBUG_ASSERT(indexA < VAO_MAX_VERTEX_ATTRIBS);
    LS_DEBUG_ASSERT(indexB < VAO_MAX_VERTEX_ATTRIBS);
    
    if (indexA == indexB) {
        LS_LOG_MSG("\tIndex values are the same. Nothing to do.\n");
    }
    
    const vbo_assembly_type::value_type tempAttrib = vboIndices[indexA];
    vboIndices[indexA] = vboIndices[indexB];
    vboIndices[indexB] = tempAttrib;
    
    outAttribs[indexA].swap(outAttribs[indexB]);
    
    LS_LOG_MSG("\tDone.\n");
    
    // Keep all valid attribs at the front of the storage array.
    pack_vbo_attribs();
}

/*-------------------------------------
-------------------------------------*/
void VAOAssembly::clear_vbo(unsigned attribIndex) noexcept {
    LS_LOG_MSG("Removing vertex attribute ", attribIndex, " from a VAO Assembly.");
    
    LS_DEBUG_ASSERT(attribIndex < VAO_MAX_VERTEX_ATTRIBS);
    
    vboIndices[attribIndex].first = nullptr;
    vboIndices[attribIndex].second = 0;
    outAttribs[attribIndex].clear();
    
    
    LS_LOG_MSG("\tDone.\n");
    
    // Keep the array packed with no empty slots between vboAttrib indices.
    pack_vbo_attribs();
}

/*-------------------------------------
-------------------------------------*/
void VAOAssembly::clear_vbos() noexcept {
    LS_LOG_MSG("Removing all vertex attributes from a VAO Assembly.");
    
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        vboIndices[i].first = nullptr;
        vboIndices[i].second = 0;
    }
    
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        outAttribs[i].clear();
    }
    
    LS_LOG_MSG("\tDone.\n");
}

/*-------------------------------------
-------------------------------------*/
void VAOAssembly::clear() noexcept {
    clear_ibo();
    clear_vbos();
}

/*-------------------------------------
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
-------------------------------------*/
bool VAOAssembly::is_assembly_valid() const noexcept {
    LS_LOG_MSG("Validating a VAO Assembly.");
    
    for (unsigned i = 0; i < VAO_MAX_VERTEX_ATTRIBS; ++i) {
        const vbo_assembly_type::value_type& current = vboIndices[i];
        const VertexBuffer* const pVbo = current.first;
        
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
        if (outAttribs[i].empty()) {
            LS_LOG_MSG("\tInvalid VAO Assembly found. Attrin ", i, " has no name.\n");
            return false;
        }
    }
    
    LS_LOG_MSG("\tDone.\n");
    
    return true;
}

/*-------------------------------------
-------------------------------------*/
bool VAOAssembly::assemble_attrib_list(
    unsigned& outNumAttribs,
    utils::Pointer<VertexAttrib[]>& outAttribList
) const noexcept {
    
    if (!is_assembly_valid()) {
        outNumAttribs = 0;
        return false;
    }
    
    LS_LOG_MSG("Assembling a list of VAO attributes.");
    
    outNumAttribs = get_num_vbo_attribs();
    outAttribList.reset(new(std::nothrow) VertexAttrib[outNumAttribs]);
    
    if (!outAttribList) {
        LS_LOG_MSG("Unable to allocate memory for a VAO attribute list.");
        outNumAttribs = 0;
        return false;
    }
    
    // Create the attrib list
    for (unsigned i = 0; i < outNumAttribs; ++i) {
        const VertexBuffer& vbo     = *vboIndices[i].first;
        const VBOAttrib& vboAttrib  = vbo.get_attrib(vboIndices[i].second);
        VertexAttrib& vaoAttrib     = outAttribList[i];
        
        vaoAttrib.index         = i;
        vaoAttrib.components    = vboAttrib.components;
        vaoAttrib.type          = vboAttrib.baseType;
        vaoAttrib.normalized    = vboAttrib.normalized;
        vaoAttrib.stride        = vboAttrib.stride;
        vaoAttrib.offset        = vboAttrib.offset;
        vaoAttrib.instanceRate  = 0;
        vaoAttrib.name          = outAttribs[i];
    }
    
    LS_LOG_MSG("\tDone. Assembled ", outNumAttribs, " VAO attributes.\n");
    
    return true;
}
        
/*-------------------------------------
-------------------------------------*/
bool VAOAssembly::assemble_vao(
    unsigned& outNumAttribs,
    utils::Pointer<VertexAttrib[]>& outAttribList,
    VertexArray& outVao
) const noexcept {
    
    if (!assemble_attrib_list(outNumAttribs, outAttribList)) {
        return false;
    }
    
    LS_LOG_MSG("Creating a VAO to contain assembled CPU-side attributes.");
    
    if (!outVao.init()) {
        outNumAttribs = 0;
        outAttribList.reset();
        
        LS_LOG_MSG("\tUnable to create a VAO on the GPU.\n");
        
        return false;
    }
    
    outVao.bind();
    LS_LOG_GL_ERR();
    
    if (pIndexBuffer) {
        pIndexBuffer->bind();
        LS_LOG_GL_ERR();
    }
    
    const VertexBuffer* pVbo = nullptr;
    
    for (unsigned i = 0; i < outNumAttribs; ++i) {
        if (pVbo != vboIndices[i].first) {
            pVbo = vboIndices[i].first;
            pVbo->bind();
            LS_LOG_GL_ERR();
        }
        
        const VertexAttrib& attrib = outAttribList[i];
        
        LS_LOG_MSG(
            "\t\tSetting up attrib ", i, ':',
            "\n\t\t\tIndex:         ", attrib.index,
            "\n\t\t\tComponents:    ", attrib.components,
            "\n\t\t\tBase Type:     ", attrib.type,
            "\n\t\t\tNormalized:    ", attrib.normalized == GL_TRUE,
            "\n\t\t\tStride:        ", attrib.stride,
            "\n\t\t\tOffset:        ", attrib.offset,
            "\n\t\t\tName:          ", attrib.name
        );
        
        outVao.set_attrib_offset(attrib);
        LS_LOG_GL_ERR();
    }

    outVao.unbind();
    LS_LOG_GL_ERR();
    
    pVbo->unbind();
    LS_LOG_GL_ERR();
    
    if (pIndexBuffer) {
        pIndexBuffer->unbind();
    }
    
    LS_LOG_MSG("\tDone.\n");
    
    return true;
}



} // end draw namespace
} // end ls namespace