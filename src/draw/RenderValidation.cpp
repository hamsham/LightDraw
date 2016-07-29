
/*
 * File:   RenderValidation.cpp
 * Author: miles
 *
 * Created on February 22, 2016, 2:47 PM
 */

#include "ls/utils/Assertions.h"
#include "ls/utils/Log.h"

#include "ls/draw/RenderValidation.h"
#include "ls/draw/VertexArray.h"
#include "ls/draw/VertexBuffer.h"
#include "ls/draw/ShaderProgram.h"
#include "ls/draw/UniformBuffer.h"

namespace ls {

/*-------------------------------------
 * Determine if a VAO can be rendered with a particular shader.
-------------------------------------*/
bool draw::are_attribs_compatible(const ShaderProgram& prog, const VertexArray& vao) noexcept {
    if (!vao.is_valid() || !prog.is_valid()) {
        return false;
    }

    const VAOAttrib& vAttribs = vao.get_attribs();
    const ShaderAttribArray& sAttribs = prog.get_vertex_attribs();

    for (unsigned vIter = 0; vIter < vAttribs.get_num_attribs(); ++vIter) {
        const VBOAttrib& vAttrib = vao.get_attrib(vIter);

        for (unsigned sIter = 0; sIter < sAttribs.get_num_attribs(); ++sIter) {
            const ShaderAttrib& sAttrib = sAttribs.get_attrib(sIter);
            
            if (vAttrib.get_num_elements() != sAttrib.get_num_elements()) {
                return false;
            }

            if (vAttrib.get_base_type() != sAttrib.get_base_type()) {
                continue;
            }

            if (vAttrib.get_num_components() != sAttrib.get_num_components()) {
                continue;
            }

            if (vAttrib.get_num_subcomponents() != sAttrib.get_num_subcomponents()) {
                continue;
            }

            return true;
        }
    }

    return false;
}

/*-------------------------------------
 * Determine if a UBO can be used with a Shader
-------------------------------------*/
int draw::are_attribs_compatible(
    const ShaderProgram& p,
    const unsigned blockIndex,
    const UniformBuffer& ubo
) noexcept {
    if (!p.is_valid() || !ubo.is_valid()) {
        LS_DEBUG_ASSERT(false);
        return -1;
    }
    
    if (!p.get_num_uniform_blocks() || blockIndex >= p.get_num_uniform_blocks()) {
        LS_DEBUG_ASSERT(false);
        return -1;
    }
    
    const ls::utils::Pointer<ShaderBlockAttrib[]>& progBlocks = p.get_uniform_blocks();
    const ShaderBlockAttrib& progAttribs = progBlocks[blockIndex];
    
    if (ubo.get_num_attribs() < progAttribs.get_num_members()) {
        LS_DEBUG_ASSERT(false);
        return -1;
    }
    
    const ShaderBlockAttrib& uboAttribs = ubo.get_attribs();
    
    if (progAttribs.get_block_name() != uboAttribs.get_block_name()
    || progAttribs.get_num_bytes() != uboAttribs.get_num_bytes()
    || progAttribs.get_num_members() != uboAttribs.get_num_members()
    ) {
        LS_DEBUG_ASSERT(false);
        return -1;
    }
    
    // Not currently going to bother with member compatibility. Comparing the
    // number of bytes, block name, and number of members seems like enough to
    // prevent someone from shooting themselves in the foot.
    
    return progAttribs.get_block_index();
}



} // end ls namespace
