
#include <utility>

#include "lightsky/draw/TextureAttrib.h" // get_texture_int/float()
#include "lightsky/draw/RenderBuffer.h"



namespace ls
{
namespace draw
{

/*-----------------------------------------------------------------------------
 * Render Buffer Attributes
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
RBOAttrib::~RBOAttrib() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
RBOAttrib::RBOAttrib() noexcept
{
    reset_attribs();
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
RBOAttrib::RBOAttrib(const RBOAttrib& a) noexcept
{
    *this = a;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
RBOAttrib::RBOAttrib(RBOAttrib&& a) noexcept
{
    *this = std::move(a);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
RBOAttrib& RBOAttrib::operator=(const RBOAttrib& a) noexcept
{
    colorType = a.colorType;
    basicFormat = a.basicFormat;
    internalFormat = a.internalFormat;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
RBOAttrib& RBOAttrib::operator=(RBOAttrib&& a) noexcept
{
    *this = a;
    a.reset_attribs();

    return *this;
}

/*-------------------------------------
 * Reset all attributes to their OpenGL defaults
-------------------------------------*/
void RBOAttrib::reset_attribs() noexcept
{
    colorType = color_type_t::COLOR_TYPE_DEFAULT;
    basicFormat = pixel_layout_t::COLOR_LAYOUT_DEFAULT_RGB;
    internalFormat = rbo_format_t::RBO_FMT_DEFAULT;
}

/*-------------------------------------
 * Introspection (most) of a Texture Attributes
-------------------------------------*/
bool RBOAttrib::introspect_attribs() noexcept
{
    GLint fmt = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &fmt);

    if (!fmt)
    {
        return false;
    }

    set_internal_format((rbo_format_t)fmt);

    return true;
}

/*-------------------------------------
 * Retrieve the coloring information of an RBO's pixels
-------------------------------------*/
color_type_t RBOAttrib::get_color_type() const noexcept
{
    return colorType;
}

/*-------------------------------------
 * Set the data format of an RBO's pixels
-------------------------------------*/
void RBOAttrib::set_internal_format(const rbo_format_t gpuFormat) noexcept
{
    internalFormat = gpuFormat;
    basicFormat = draw::get_color_layout((pixel_format_t)internalFormat);
    colorType = draw::get_color_type((pixel_format_t)internalFormat);
}

/*-------------------------------------
 * Retrieve the data format of an RBO's pixels
-------------------------------------*/
rbo_format_t RBOAttrib::get_internal_format() const noexcept
{
    return internalFormat;
}

/*-------------------------------------
 * Retrieve the layout of an RBO's pixels.
-------------------------------------*/
pixel_layout_t RBOAttrib::get_basic_format() const noexcept
{
    return basicFormat;
}




/*-----------------------------------------------------------------------------
 * Render Buffer Objects
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
RenderBuffer::~RenderBuffer() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
RenderBuffer::RenderBuffer() noexcept :
    gpuId{
        0
    },
    size{0},
    attribs{}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
RenderBuffer::RenderBuffer(const RenderBuffer& rbo) noexcept :
    gpuId{
        rbo.gpuId
    },
    size{rbo.size},
    attribs{rbo.attribs}
{
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
RenderBuffer::RenderBuffer(RenderBuffer&& rbo) noexcept :
    gpuId{
        rbo.gpuId
    },
    size{std::move(rbo.size)},
    attribs{std::move(rbo.attribs)}
{
    rbo.gpuId = 0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
RenderBuffer& RenderBuffer::operator=(const RenderBuffer& rbo) noexcept
{
    gpuId = rbo.gpuId;
    size = rbo.size;
    attribs = rbo.attribs;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
RenderBuffer& RenderBuffer::operator=(RenderBuffer&& rbo) noexcept
{
    gpuId = rbo.gpuId;
    rbo.gpuId = 0;

    size = std::move(rbo.size);

    attribs = std::move(rbo.attribs);

    return *this;
}

/*-------------------------------------
 * Release resources
-------------------------------------*/
void RenderBuffer::terminate() noexcept
{
    if (gpuId)
    {
        glDeleteRenderbuffers(1, &gpuId);
        gpuId = 0;
    }

    size = math::vec2i{0};
    attribs.reset_attribs();
}
} // end draw namespace
} // end ls namespace
