
#ifndef __LS_DRAW_RENDERBUFFER_H__
#define __LS_DRAW_RENDERBUFFER_H__

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Color.h"

namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * @brief rbo_format_t Allows for better type safety when creating or resizing
 * a renderbuffer.
-----------------------------------------------------------------------------*/
enum rbo_format_t : GLenum {
    RBO_FMT_DEPTH_STENCIL_24_8 = GL_DEPTH24_STENCIL8,
    RBO_FMT_DEPTH_STENCIL_32_8 = GL_DEPTH32F_STENCIL8,
    RBO_FMT_DEPTH_16 = GL_DEPTH_COMPONENT16,
    RBO_FMT_DEPTH_24 = GL_DEPTH_COMPONENT24,
    RBO_FMT_DEPTH_32 = GL_DEPTH_COMPONENT32F,
    RBO_FMT_STENCIL_8 = GL_STENCIL_INDEX8,

    RBO_FMT_R_8 = GL_R8,
    RBO_FMT_R_8I = GL_R8I,
    RBO_FMT_R_8U = GL_R8UI,
    RBO_FMT_R_16I = GL_R16I,
    RBO_FMT_R_16U = GL_R16UI,
    RBO_FMT_R_32I = GL_R32I,
    RBO_FMT_R_32U = GL_R32UI,

    RBO_FMT_RG_8 = GL_RG8,
    RBO_FMT_RG_8I = GL_RG8I,
    RBO_FMT_RG_8U = GL_RG8UI,
    RBO_FMT_RG_16I = GL_RG16I,
    RBO_FMT_RG_16U = GL_RG16UI,
    RBO_FMT_RG_32I = GL_RG32I,
    RBO_FMT_RG_32U = GL_RG32UI,

    RBO_FMT_RGB_8 = GL_RGB8,
    RBO_FMT_RGB_565 = GL_RGB565,

    RBO_FMT_RGBA_8 = GL_RGBA8,
    RBO_FMT_RGBA_8I = GL_RGBA8I,
    RBO_FMT_RGBA_8U = GL_RGBA8UI,
    RBO_FMT_RGBA_16I = GL_RGB16I,
    RBO_FMT_RGBA_16U = GL_RGB16UI,
    RBO_FMT_RGBA_32I = GL_RGB32I,
    RBO_FMT_RGBA_32U = GL_RGB32UI,

    RBO_FMT_RGBA_4 = GL_RGBA4,
    RBO_FMT_RGBA_5_1 = GL_RGB5_A1,
    RBO_FMT_RGBA_10_2 = GL_RGB10_A2,
    RBO_FMT_RGBA_10_2U = GL_RGB10_A2UI,

    RBO_FMT_SRGBA_8 = GL_SRGB8_ALPHA8,

    RBO_FMT_DEFAULT = GL_RGB8,
    RBO_FMT_INVALID = GL_INVALID_ENUM
};



/**----------------------------------------------------------------------------
 * RBO Attribs provide meta-data about a render buffer object without requiring
 * client code to query OpenGL.
-----------------------------------------------------------------------------*/
class RBOAttrib {
  private:
    color_type_t colorType;
    pixel_layout_t basicFormat;
    rbo_format_t internalFormat;


  public:
    ~RBOAttrib() noexcept;

    RBOAttrib() noexcept;

    RBOAttrib(const RBOAttrib&) noexcept;

    RBOAttrib(RBOAttrib&&) noexcept;

    RBOAttrib& operator=(const RBOAttrib&) noexcept;

    RBOAttrib& operator=(RBOAttrib&&) noexcept;

    void reset_attribs() noexcept;

    bool introspect_attribs() noexcept;

    color_type_t get_color_type() const noexcept;

    void set_internal_format(const rbo_format_t gpuFormat) noexcept;

    rbo_format_t get_internal_format() const noexcept;

    pixel_layout_t get_basic_format() const noexcept;
};



/**----------------------------------------------------------------------------
 * @brief The renderbuffer class is a GPU-only class which allows framebuffers
 * to render off-screen. This is useful for framebuffer operations which
 * will not be returned back to the CPU (such as for depth or stencil functions.
-----------------------------------------------------------------------------*/
class RenderBuffer {
    friend class RBOAssembly;

  private:
    /**
     * @brief gpuId
     *
     * A handle to the GPU-side render buffer object.
     */
    unsigned gpuId;

    /**
     * Dimensional information about a render buffer's size.
     */
    math::vec2i size;

    /**
     * Meta-data about the current render buffer object's GPU information.
     */
    RBOAttrib attribs;

  public:
    /**
     * @brief Destructor
     *
     * Frees all CPU-side memory from *this.
     */
    ~RenderBuffer() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all local variables.
     */
    RenderBuffer() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param rbo
     * A constant reference to a constant RBO object who's data will be copied
     * into *this. No GPU-side copies are performed.
     */
    RenderBuffer(const RenderBuffer& rbo) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves the GPU handle from the input parameter into *this. The rbo
     * object passed into *this will then point to nothing.
     *
     * @param rbo
     * An r-value reference to a renderbuffer object who's GPU handle will
     * be transferred into *this.
     */
    RenderBuffer(RenderBuffer&& rbo) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param rbo
     * A constant reference to a constant RBO object who's data will be copied
     * into *this. No GPU-side copies are performed.
     *
     * @return A reference to *this.
     */
    RenderBuffer& operator=(const RenderBuffer& rbo) noexcept;

    /**
     * @brief Move Operator
     *
     * Moves the GPU handle from the input parameter into *this. The rbo
     * object passed into *this will then point to nothing.
     *
     * @param rbo
     * An r-value reference to a renderbuffer object who's GPU handle will
     * be transferred into *this.
     *
     * @return A reference to *this.
     */
    RenderBuffer& operator=(RenderBuffer&& rbo) noexcept;

    /**
     * Bind the current renderbuffer to OpenGL
     */
    void bind() const noexcept;

    /**
     * Unbind the current renderbuffer to OpenGL
     */
    void unbind() const noexcept;

    /**
     * @brief Terminate all GPU-side resources used by *this renderbuffer.
     */
    void terminate() noexcept;

    /**
     * Get the GPU-Assigned ID that this object references.
     *
     * @return A non-zero unsigned integer that is used as a handle to the
     * renderbuffer object represented by this object. This value returns 0
     * if nothing is referenced by *this.
     */
    unsigned gpu_id() const noexcept;

    /**
     * @brief Determine if *this is able to be used for rendering.
     *
     * @return TRUE if *this contains a valid RBO ID, FALSE if not.
     */
    bool is_valid() const noexcept;

    /**
     * Retrieve all current attributes to *this.
     *
     * @return A constant reference to a constant RBOAttrib object, containing
     * cached values of the current texture object's attributes.
     */
    const RBOAttrib& get_attribs() const noexcept;

    /**
     * Retrieve the current render buffer object's size.
     *
     * @return A constant reference to a 3D integer vector containing the
     * dimensions of the current texture object.
     */
    const math::vec2i& get_size() const noexcept;
};

/*-------------------------------------
 * Get the GPU-Assigned ID that this object references.
-------------------------------------*/
inline unsigned RenderBuffer::gpu_id() const noexcept {
    return gpuId;
}

/*-------------------------------------
 Determine if *this can be used during rendering operations.
-------------------------------------*/
inline bool RenderBuffer::is_valid() const noexcept {
    return gpu_id() != 0;
}

/*-------------------------------------
 * Get current RBO's dimentsions.
-------------------------------------*/
inline const math::vec2i& RenderBuffer::get_size() const noexcept {
    return size;
}

/*-------------------------------------
 * Get the current attributes.
-------------------------------------*/
inline const RBOAttrib& RenderBuffer::get_attribs() const noexcept {
    return attribs;
}

/*-------------------------------------
 * Bind the current renderbuffer to OpenGL
-------------------------------------*/
inline void RenderBuffer::bind() const noexcept {
    glBindRenderbuffer(GL_RENDERBUFFER, gpuId);
}

/*-------------------------------------
 * Unbind the current renderbuffer to OpenGL
-------------------------------------*/
inline void RenderBuffer::unbind() const noexcept {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_RENDERBUFFER_H__
