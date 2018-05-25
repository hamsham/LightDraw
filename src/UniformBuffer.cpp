
#include <utility> // std::move

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/ShaderProgram.h"
#include "lightsky/draw/UniformBuffer.h"



namespace ls {
namespace draw {



/*-------------------------------------
 * Destructor
-------------------------------------*/
UniformBuffer::~UniformBuffer() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
UniformBuffer::UniformBuffer() noexcept :
    BufferObject{},
    attribs{}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
UniformBuffer::UniformBuffer(const UniformBuffer& ubo) noexcept :
    BufferObject{ubo},
    attribs{ubo.attribs}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
UniformBuffer::UniformBuffer(UniformBuffer&& ubo) noexcept :
    BufferObject{std::move(ubo)},
    attribs{std::move(ubo.attribs)}
{}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
UniformBuffer& UniformBuffer::operator=(const UniformBuffer& ubo) noexcept {
    BufferObject::operator=(ubo);
    attribs = ubo.attribs;
    return *this;
}

/*-------------------------------------
-------------------------------------*/
UniformBuffer& UniformBuffer::operator=(UniformBuffer&& ubo) noexcept {
    BufferObject::operator=(std::move(ubo));
    attribs = std::move(ubo.attribs);
    return *this;
}

/*-------------------------------------
 * Copy internal attributes
-------------------------------------*/
bool UniformBuffer::copy_attribs(const BufferObject& from) noexcept {
    LS_DEBUG_ASSERT(from.get_type() == buffer_use_t::VBO_BUFFER_UNIFORM_BUFFER);
    const UniformBuffer& u = static_cast<const UniformBuffer&>(from);
    attribs = u.attribs; // eew dynamic memory reallocations
    return true;
}

/*-------------------------------------
 * Terminate all internal attributes
-------------------------------------*/
bool UniformBuffer::setup_attribs(const ShaderProgram& p, const unsigned blockIndex) noexcept {
    if (!p.is_valid() || !p.get_num_uniform_blocks()) {
        LS_DEBUG_ASSERT(false);
        return false;
    }
    
    const ls::utils::Pointer<ShaderBlockAttrib[]>& inBlocks = p.get_uniform_blocks();
    const ShaderBlockAttrib& inAttribs = inBlocks[blockIndex];
    
    set_data(inAttribs.get_num_bytes(), nullptr, buffer_access_t::VBO_DYNAMIC_DRAW);
    LS_LOG_GL_ERR();
    
    // eeeeeeewwww, more dynamic memories
    attribs = inAttribs;
    
    set_block_index(0);
    set_block_binding(0);
    
    return true;
}


} // end draw namespace
} // end ls namespace
