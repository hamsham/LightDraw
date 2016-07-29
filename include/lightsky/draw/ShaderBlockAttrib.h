
#ifndef __LS_DRAW_SHADER_BLOCK_ATTRIB_H__
#define __LS_DRAW_SHADER_BLOCK_ATTRIB_H__

#include <string>

#include "lightsky/utils/Pointer.h"

#ifdef LS_DEBUG
    #include "lightsky/utils/Assertions.h"
#endif

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/BufferObject.h"
#include "lightsky/draw/Vertex.h" // get_num_vertex_bytes(), get_vertex_base_type()



namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * Shader Block Attributes allow for Uniform Buffer Objects to easily manage
 * the structures and members of a uniform block within GLSL. They function
 * similar to the way Vertex Buffers and VBOAttribs operator.
-----------------------------------------------------------------------------*/
class ShaderBlockAttrib {
    
    // Allow only the shader assembly and uniform buffer object to manage
    // internal data on uniform blocks.
    friend class ShaderAssembly;
    friend class UniformBuffer;
    
    // Private member variables
    private:
        GLint numBytes;
        
        GLint binding;
        
        GLint index;
        
        std::string name;
        
        unsigned numMembers;

        utils::Pointer<std::string[]> memberNames;
        
        utils::Pointer<unsigned[]> memberElements;
        
        utils::Pointer<vertex_data_t[]> memberTypes;
        
        utils::Pointer<unsigned[]> memberIndices;
        
        utils::Pointer<unsigned[]> memberOffsets;
        
        utils::Pointer<unsigned[]> memberStrides;
        
    // Private member functions
    public:
        // Intended for use by the ShaderAssembly. Implemented here
        bool run_block_introspection(const GLuint shaderId, unsigned shaderBlockIndex) noexcept;
        
        // not intended for use by the shader assembly.
        bool extract_uniform_block_members(
            const GLuint shaderId,
            utils::Pointer<GLchar[]>& nameBuffer,
            const unsigned nameBufferLen
        ) noexcept;
      
    public:
        ~ShaderBlockAttrib() noexcept;
        
        ShaderBlockAttrib() noexcept;
        
        ShaderBlockAttrib(const ShaderBlockAttrib& ubo)  noexcept;
        
        ShaderBlockAttrib(ShaderBlockAttrib&& ubo)  noexcept;
        
        ShaderBlockAttrib& operator=(const ShaderBlockAttrib& ubo) noexcept;
        
        ShaderBlockAttrib& operator=(ShaderBlockAttrib&& ubo) noexcept;

        void reset() noexcept;
        
        unsigned get_num_bytes() const noexcept;
        
        GLint get_block_binding() const noexcept;
        
        GLint get_block_index() const noexcept;
        
        const std::string& get_block_name() const noexcept;
        
        unsigned get_num_members() const noexcept;
        
        const std::string& get_member_name(const unsigned memberIndex) const noexcept;
        
        unsigned get_num_member_elements(const unsigned memberIndex) const noexcept;
        
        vertex_data_t get_member_type(const unsigned memberIndex) const noexcept;
        
        vertex_data_t get_member_base_type(const unsigned memberIndex) const noexcept;
        
        unsigned get_member_index(const unsigned memberIndex) const noexcept;
        
        unsigned get_member_offset(const unsigned memberIndex) const noexcept;
        
        unsigned get_member_size(const unsigned memberIndex) const noexcept;
};



/*-------------------------------------
 * Retrieve a Shader Block's total byte count
-------------------------------------*/
inline unsigned ShaderBlockAttrib::get_num_bytes() const noexcept {
    return numBytes;
}

/*-------------------------------------
 * Get the binding point of a shader block
-------------------------------------*/
inline GLint ShaderBlockAttrib::get_block_binding() const noexcept {
    return binding;
}

/*-------------------------------------
 * Get the GPU-assigned index of a shader block
-------------------------------------*/
inline GLint ShaderBlockAttrib::get_block_index() const noexcept {
    return index;
}

/*-------------------------------------
 * Get the name of a shader block
-------------------------------------*/
inline const std::string& ShaderBlockAttrib::get_block_name() const noexcept {
    return name;
}

/*-------------------------------------
 * Retrieve the number of members in a block
-------------------------------------*/
inline unsigned ShaderBlockAttrib::get_num_members() const noexcept {
    return numMembers;
}

/*-------------------------------------
 * Get the name of a block member
-------------------------------------*/
inline const std::string& ShaderBlockAttrib::get_member_name(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return memberNames[memberIndex];
}

/*-------------------------------------
 * Get the element count of a block member
-------------------------------------*/
inline unsigned ShaderBlockAttrib::get_num_member_elements(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return memberElements[memberIndex];
}

/*-------------------------------------
 * Get the type of a block member
-------------------------------------*/
inline vertex_data_t ShaderBlockAttrib::get_member_type(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return memberTypes[memberIndex];
}

/*-------------------------------------
 * Get the sub-type of a block member
-------------------------------------*/
inline vertex_data_t ShaderBlockAttrib::get_member_base_type(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return draw::get_vertex_base_type(memberTypes[memberIndex]);
}

/*-------------------------------------
 * Get the GPU-assigned index of a block member
-------------------------------------*/
inline unsigned ShaderBlockAttrib::get_member_index(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return memberIndices[memberIndex];
}

/*-------------------------------------
 * Get the byte-offset of a block member
-------------------------------------*/
inline unsigned ShaderBlockAttrib::get_member_offset(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return memberOffsets[memberIndex];
}

/*-------------------------------------
 * Get the number of bytes occupied by a block member
-------------------------------------*/
inline unsigned ShaderBlockAttrib::get_member_size(const unsigned memberIndex) const noexcept {
    LS_DEBUG_ASSERT(memberIndex < numMembers);
    return draw::get_num_vertex_bytes(memberTypes[memberIndex]);
}


} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SHADER_BLOCK_ATTRIB_H__ */
