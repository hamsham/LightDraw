
/*
 * File:   RBOAssembly.h
 * Author: miles
 *
 * Created on March 3, 2016, 9:52 PM
 */

#ifndef __LS_DRAW_RBO_ASSEMBLY_H__
#define __LS_DRAW_RBO_ASSEMBLY_H__

#include "ls/draw/Setup.h"
#include "ls/draw/Color.h"
#include "ls/draw/RenderBuffer.h"



namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * Assembler of Render Buffer Objects
-----------------------------------------------------------------------------*/
class RBOAssembly {
  private:
    math::vec2i size;

    RBOAttrib attribs;

  public:
    ~RBOAssembly() noexcept;

    RBOAssembly() noexcept;

    RBOAssembly(const RBOAssembly& a) noexcept;

    RBOAssembly(RBOAssembly&& a) noexcept;

    RBOAssembly& operator=(const RBOAssembly& a) noexcept;

    RBOAssembly& operator=(RBOAssembly&& a) noexcept;

    bool set_attribs(const RBOAttrib& texAttribs) noexcept;

    bool set_format_attrib(const rbo_format_t internalFormat = rbo_format_t::RBO_FMT_DEFAULT) noexcept;

    bool set_size_attrib(const math::vec2i& size) noexcept;

    void clear() noexcept;

    bool is_assembly_valid() const noexcept;

    bool assemble(RenderBuffer& rbo) const noexcept;
};



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_RBO_ASSEMBLY_H__ */
