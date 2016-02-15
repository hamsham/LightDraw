
/* 
 * File:   VAOAssembly.h
 * Author: miles
 *
 * Created on February 14, 2016, 2:40 PM
 */

#ifndef __LS_DRAW_VAOASSEMBLY_H__
#define __LS_DRAW_VAOASSEMBLY_H__

#include <array>
#include <string>

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/VertexBuffer.h"
#include "lightsky/draw/IndexBuffer.h"
#include "lightsky/draw/VertexArray.h"



namespace ls {
namespace draw {



enum vao_limits_t : unsigned {
    VAO_MAX_VERTEX_ATTRIBS = 16 // minimum set by the OpenGL standard.
};



class VAOAssembly {
    
    typedef std::array<std::pair<const VertexBuffer*, unsigned>, VAO_MAX_VERTEX_ATTRIBS> vbo_assembly_type;
    
    private:
        const IndexBuffer* pIndexBuffer;
        
        vbo_assembly_type vboIndices;
        
        std::array<std::string, VAO_MAX_VERTEX_ATTRIBS> outAttribs;
        
        void pack_vbo_attribs() noexcept;

    public:
        ~VAOAssembly() noexcept;

        VAOAssembly() noexcept;

        VAOAssembly(const VAOAssembly&) noexcept;

        VAOAssembly(VAOAssembly&&) noexcept;

        VAOAssembly& operator=(const VAOAssembly&) noexcept;

        VAOAssembly& operator=(VAOAssembly&&) noexcept;
        
        bool is_ibo_set() const noexcept;
        
        bool is_vbo_set(unsigned vaoIndex) const noexcept;
        
        void set_ibo_attrib(const IndexBuffer& ibo) noexcept;
        
        void set_vbo_attrib(const VertexBuffer& vbo, unsigned vboAttribIndex, unsigned vaoAttribIndex) noexcept;
        
        void set_vbo_attribs(const VertexBuffer& vbo) noexcept;
        
        bool set_attrib_name(unsigned attribIndex, const std::string& name) noexcept;
        
        bool set_attrib_name(unsigned attribIndex, const char* const pName) noexcept;
        
        void swap_vbo_attribs(unsigned indexA, unsigned indexB) noexcept;
        
        void clear_ibo() noexcept;
        
        void clear_vbo(unsigned attribIndex) noexcept;
        
        void clear_vbos() noexcept;
        
        void clear() noexcept;
        
        unsigned get_num_vbo_attribs() const noexcept;
        
        bool is_assembly_valid() const noexcept;
        
        bool assemble_attrib_list(
            unsigned& outNumAttribs,
            utils::Pointer<VertexAttrib[]>& outAttribList
        ) const noexcept;
        
        bool assemble_vao(
            unsigned& outNumAttribs,
            utils::Pointer<VertexAttrib[]>& outAttribList,
            VertexArray& outVao
        ) const noexcept;
};

/*-----------------------------------------------------------------------------
 * Member functions for the VAOAssembly class.
-----------------------------------------------------------------------------*/
/*-------------------------------------
-------------------------------------*/
inline bool VAOAssembly::is_ibo_set() const noexcept {
    return pIndexBuffer != nullptr;
}

/*-------------------------------------
-------------------------------------*/
inline bool VAOAssembly::is_vbo_set(unsigned vaoIndex) const noexcept {
    LS_DEBUG_ASSERT(vaoIndex < VAO_MAX_VERTEX_ATTRIBS);
    return vboIndices[vaoIndex].first != nullptr;
}

/*-------------------------------------
-------------------------------------*/
inline void VAOAssembly::set_ibo_attrib(const IndexBuffer& ibo) noexcept {
    pIndexBuffer = &ibo;
}

/*-------------------------------------
-------------------------------------*/
inline void VAOAssembly::clear_ibo() noexcept {
    pIndexBuffer = nullptr;
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_VAOASSEMBLY_H__ */
