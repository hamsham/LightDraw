/*
 * File:   draw/framebuffer.h
 * Author: Miles Lacey
 *
 * Created on Jult 30, 2014, 10:02 PM
 */

#ifndef __LS_DRAW_FRAMEBUFFER_H__
#define __LS_DRAW_FRAMEBUFFER_H__

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Color.h"
#include "lightsky/draw/GLQuery.h"
#include "lightsky/draw/TextureAttrib.h"
#include "lightsky/draw/FBOAttrib.h"

namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * @brief Framebuffer Object
 *
 * A Framebuffer Object represents a canvas for which all draw operations will
 * be placed on. When a Framebuffer is not active, OpenGL will render to the
 * default buffer residing within a render context. Framebuffer objects can
 * be used to render to a texture as well.
-----------------------------------------------------------------------------*/
class FrameBuffer final {
    friend class FBOAssembly;

  public:
    /**
     * Get the maximum number of supported color attachments from the
     * current render context.
     * OpenGL requires at least 8 attachments are supported.
     *
     * @return A signed integral type representing the number of currently
     * supported render targets.
     */
    static int get_max_num_attachments() noexcept;

    /**
     * Bind the default read/write buffer
     *
     * @param fbo_access_t
     */
    static void bind_default_framebuffer(
        const fbo_access_t access = FBO_ACCESS_RW,
        const bool rasterize = true
        ) noexcept;

    /**
     * @brief Blit (draw) this frame buffer onto another.
     *
     * This buffer must be bound as LS_READ_BUFFER and the destination FBO must
     * be bound as LS_DRAW_BUFFER in order to work properly.
     *
     * @param dest
     * A constant reference to an FBO currently bound as a draw buffer.
     *
     * @param srcOffset
     * A point that specifies the bottom-left corner of *this from which
     * to blit onto another framebuffer.
     *
     * @param srcSize
     * A point that specifies the top-right corner of *this to blit onto
     * another framebuffer.
     *
     * @param dstOffset
     * A point that specifies the bottom-left corner of the target
     * framebuffer at which to start blitting to.
     *
     * @param dstSize
     * A point that specifies the top-right corner of the target
     * framebuffer at which to end the blitting.
     *
     * @param mask
     * Used to determine if *this framebuffer's depth, stencil, and/or
     * color buffers should be copied to the target framebuffer.
     *
     * @param filtr
     * Used to determine the type of filtering that should be performed
     * during the blitting operation. Valid values for this parameter include
     * only TEX_FILTER_NEAREST and TEX_FILTER_LINEAR.
     */
    static void blit(
        const math::vec2i& srcOffset, const math::vec2i& srcSize,
        const math::vec2i& dstOffset, const math::vec2i& dstSize,
        const fbo_mask_t mask = (fbo_mask_t) (fbo_mask_t::FBO_COLOR_BIT | fbo_mask_t::FBO_DEPTH_BIT),
        const tex_filter_t filtr = TEX_FILTER_LINEAR
    ) noexcept;

    /**
     * Clear the color attachment indicated by the input parameter.
     *
     * @param A bitmask of type fbo_mask_t.
     */
    static void clear_buffers(
        const GLbitfield mask = (fbo_mask_t) fbo_mask_t::FBO_COLOR_BIT | fbo_mask_t::FBO_DEPTH_STENCIL_BIT
    ) noexcept;

    static void clear_color_buffer(const fbo_attach_t attachIndex, const color::color& clearVal = color::black) noexcept;

    static void clear_depth_buffer(const float clearVal) noexcept;

    static void clear_stencil_buffer(const int clearVal) noexcept;

    static void clear_depth_stencil_buffer(const float depthClearVal, const int stencilClearVal) noexcept;

  private:
    /**
     * Access determines if a framebuffer should be rendered to, read from,
     * or both.
     */
    fbo_access_t access = FBO_ACCESS_RW;

    /**
     * gpuId is a GPU-assigned handle to a framebuffer object within
     * OpenGL.
     */
    unsigned gpuId = 0;

    float clearDepthVal;

    int clearStencilVal;

    color::colorf_t clearColorVal;

    math::vec3i largestSize;

    unsigned numAttribs;

    utils::Pointer<FBOAttrib[]> attribs;

  public:
    /**
     * @brief Destructor
     *
     * Frees all CPU-side memory used by *this. A manual call to "terminate()"
     * is required to free all GPU data.
     */
    ~FrameBuffer() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all values within *this to their default values. This
     * does not initialize a framebuffer on the GPU.
     */
    FrameBuffer() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all CPU-side data from the input parameter into *this.
     *
     * @param fbo
     * A constant reference to another framebuffer object.
     */
    FrameBuffer(const FrameBuffer& fbo) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves all data from the input parameter into *this without
     * performing any copies.
     *
     * @param fbo
     * An r-value reference to another Framebuffer Object.
     */
    FrameBuffer(FrameBuffer&& fbo) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all CPU-side data from the input parameter into *this.
     *
     * @param fbo
     * A constant reference to another framebuffer object.
     *
     * @return A reference to *this.
     */
    FrameBuffer& operator=(const FrameBuffer& fbo) noexcept;

    /**
     * @brief Move Operator
     *
     * Moves all data from the input parameter into *this without
     * performing any copies.
     *
     * @param fbo
     * An r-value reference to another Framebuffer Object.
     *
     * @return A reference to *this.
     */
    FrameBuffer& operator=(FrameBuffer&& fbo) noexcept;

    /**
     * Get the GPU-Assigned ID that this object references.
     *
     * @return A non-zero unsigned integer that is used as a handle to the
     * framebuffer object represented by this object.
     *
     * This value returns 0 if nothing is referenced by *this.
     */
    unsigned gpu_id() const noexcept;

    /**
     * Determine if *this represents a valid framebuffer which can be rendered
     * to.
     *
     * @return TRUE if *this contains a valid handle to a framebuffer object
     * on the GPU, FALSE if not.
     */
    bool is_valid() const noexcept;

    /**
     * Bind the current framebuffer to OpenGL
     *
     * @param a
     * An enumeration that can help make perform framebuffer read/write
     * operations.
     */
    void bind(const fbo_access_t a) noexcept;

    /**
     * Unbind the current framebuffer to OpenGL
     */
    void unbind() const noexcept;

    /**
     * Release all resources used by this framebuffer object (CPU & GPU).
     */
    void terminate() noexcept;

    /**
     * Get the framebuffer acccess type.
     *
     * @return fbo_access_t
     * An enumeration that can help make perform framebuffer read/write
     * operations
     */
    fbo_access_t get_access_type() const noexcept;

    /**
     * Retrieve the value of *this FBO's depth clear color.
     *
     * @return The value used by *this FBO to set all pixels in the depth
     * buffer during a call to "clear_buffers()"
     */
    float get_depth_clear_value() const noexcept;

    /**
     * Assign the value of *this FBO's depth clear color.
     *
     * @param v
     * A value which will be used by *this FBO to set all pixels in the depth
     * buffer during a call to "clear_buffers()"
     */
    void set_depth_clear_value(const float v) noexcept;

    /**
     * Retrieve the value of *this FBO's stencil clear color.
     *
     * @return The value used by *this FBO to set all pixels in the stencil
     * buffer during a call to "clear_buffers()"
     */
    float get_stencil_clear_value() const noexcept;

    /**
     * Assign the value of *this FBO's stencil clear color.
     *
     * @param v
     * A value which will be used by *this FBO to set all pixels in the stencil
     * buffer during a call to "clear_buffers()"
     */
    void set_stencil_clear_value(const int v) noexcept;

    /**
     * Retrieve the value of *this FBO's color attachment clear color.
     *
     * @return The value used by *this FBO to set all pixels in the color
     * buffer during a call to "clear_buffers()"
     */
    const color::color& get_color_clear_value() const noexcept;

    /**
     * Assign the value of *this FBO's color attachment clear color.
     *
     * @param v
     * A value which will be used by *this FBO to set all pixels in the color
     * buffer during a call to "clear_buffers()"
     */
    void set_color_clear_value(const color::color& v) noexcept;

    /**
     * Retrieve the size of the largest framebuffer attachment in *this.
     *
     * Other attachment sizes can be queried by retrieving the list of
     * attachments in *this.
     *
     * @return A constant reference to a 3D integral vector which contains
     * the size of the largest attachment in *this.
     */
    const math::vec3i& get_size() const noexcept;

    /**
     * Retrieve the current number of framebuffer attributes from *this.
     *
     * @return The number of attributes in the internal attrib list.
     */
    unsigned get_num_attribs() const noexcept;

    /**
     * Retrieve a pointer to the list of attributes used by *this.
     *
     * @return A constant pointer to a constant list of framebuffer attributes.
     */
    const FBOAttrib* get_attribs() const noexcept;

    /**
     * Set the current draw targets to be used by this.
     *
     * @param numTargets
     * The number of draw targets contained within the array specified by
     * "targets."
     *
     * @param targets
     * The set of all render targets that should be rendered to by this.
     */
    void set_draw_targets(unsigned numTargets, const fbo_attach_t* targets) const noexcept;

    /**
     * Set all attachments in *this as the current draw targets to be used by
     * OpenGL.
     */
    void set_draw_targets() const noexcept;

    /**
     * Set a single fbo attachment to be used as the target to read data from.
     *
     * @param attribIndex
     * An array index to the FBOAttrib which will be used as the color
     * attachment to read data from.
     */
    void set_read_target(const unsigned attribIndex) const noexcept;
};

/*-------------------------------------
    Blit this framebuffer to the current draw buffer.
-------------------------------------*/
inline void FrameBuffer::blit(
    const math::vec2i& srcOrig, const math::vec2i& srcSize,
    const math::vec2i& dstOrig, const math::vec2i& dstSize,
    fbo_mask_t mask, tex_filter_t filter
) noexcept {
    glBlitFramebuffer(
        srcOrig[0], srcOrig[1], srcSize[0], srcSize[1],
        dstOrig[0], dstOrig[1], dstSize[0], dstSize[1],
        (GLbitfield) mask, (GLenum) filter
    );
}

/*-------------------------------------
    Get the maximum number of supported color attachments.
-------------------------------------*/
inline int FrameBuffer::get_max_num_attachments() noexcept {
    int numAttachments;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &numAttachments);
    return numAttachments;
}

/*-------------------------------------
    Clear the color attachment indicated by the input parameter.
-------------------------------------*/
inline void FrameBuffer::clear_buffers(const GLbitfield mask) noexcept {
    glClear(mask);
}

/*-------------------------------------
-------------------------------------*/
inline void FrameBuffer::clear_color_buffer(const fbo_attach_t attachIndex, const color::color& clearVal) noexcept {
    glClearBufferfv(GL_COLOR, attachIndex-fbo_attach_t::FBO_ATTACHMENT_0, clearVal.v);
}

/*-------------------------------------
-------------------------------------*/
inline void FrameBuffer::clear_depth_buffer(const float clearVal) noexcept {
    glClearBufferfv(GL_DEPTH, 0, &clearVal);
}

/*-------------------------------------
-------------------------------------*/
inline void FrameBuffer::clear_stencil_buffer(const int clearVal) noexcept {
    glClearBufferiv(GL_STENCIL, 0, &clearVal);
}

/*-------------------------------------
-------------------------------------*/
inline void FrameBuffer::clear_depth_stencil_buffer(const float depthClearVal, const int stencilClearVal) noexcept {
    glClearBufferfi(GL_DEPTH_STENCIL, 0, depthClearVal, stencilClearVal);
}

/*-------------------------------------
    Get the GPU-Assigned ID that this object references.
-------------------------------------*/
inline unsigned FrameBuffer::gpu_id() const noexcept {
    return gpuId;
}

/*-------------------------------------
 * Determine if *this is a valid FBO
-------------------------------------*/
inline bool FrameBuffer::is_valid() const noexcept {
    return gpu_id() != 0;
}

/*-------------------------------------
    Bind the current framebuffer to OpenGL
-------------------------------------*/
inline void FrameBuffer::bind(const fbo_access_t a) noexcept {
    access = a;
    glBindFramebuffer(access, gpuId);
}

/*-------------------------------------
    Unbind the current framebuffer to OpenGL
-------------------------------------*/
inline void FrameBuffer::unbind() const noexcept {
    glBindFramebuffer(access, 0);
}

/*-------------------------------------
    Get the framebuffer acccess type.
-------------------------------------*/
inline fbo_access_t FrameBuffer::get_access_type() const noexcept {
    return access;
}

/*-------------------------------------
 * get the depth clear color value
-------------------------------------*/
inline float FrameBuffer::get_depth_clear_value() const noexcept {
    return clearDepthVal;
}

/*-------------------------------------
 * set the depth clear color value
-------------------------------------*/
inline void FrameBuffer::set_depth_clear_value(const float v) noexcept {
    clearDepthVal = v;
}

/*-------------------------------------
 * get the stencil clear color value
-------------------------------------*/
inline float FrameBuffer::get_stencil_clear_value() const noexcept {
    return clearStencilVal;
}

/*-------------------------------------
 * set the stencil clear color value
-------------------------------------*/
inline void FrameBuffer::set_stencil_clear_value(const int v) noexcept {
    clearStencilVal = v;
}

/*-------------------------------------
 * get the clear color value
-------------------------------------*/
inline const color::color& FrameBuffer::get_color_clear_value() const noexcept {
    return clearColorVal;
}

/*-------------------------------------
 * set the clear color value
-------------------------------------*/
inline void FrameBuffer::set_color_clear_value(const color::color& v) noexcept {
    clearColorVal = v;
}

/*-------------------------------------
 * Get the size of the largest attachment
-------------------------------------*/
inline const math::vec3i& FrameBuffer::get_size() const noexcept {
    return largestSize;
}

/*-------------------------------------
 Get the current number of attribs.
-------------------------------------*/
inline unsigned FrameBuffer::get_num_attribs() const noexcept {
    return numAttribs;
}

/*-------------------------------------
 Get the current attrib list.
-------------------------------------*/
inline const FBOAttrib* FrameBuffer::get_attribs() const noexcept {
    return attribs.get();
}

/*-------------------------------------
    Set the current draw targets to be used by this.
-------------------------------------*/
inline void FrameBuffer::set_draw_targets(unsigned numTargets, const fbo_attach_t* targets) const noexcept {
    glDrawBuffers(numTargets, (const GLenum*) targets);
    LS_LOG_GL_ERR();
}

/*-----------------------------------------------------------------------------
 * Framebuffer Object Utility functions
-----------------------------------------------------------------------------*/
inline GLuint get_current_read_fbo() noexcept {
    return get_gl_int(GL_READ_FRAMEBUFFER_BINDING);
}

inline GLuint get_current_write_fbo() noexcept {
    return get_gl_int(GL_DRAW_FRAMEBUFFER_BINDING);
}

inline bool is_fbo_raster_enabled() noexcept {
    return get_gl_bool(GL_RASTERIZER_DISCARD);
}

constexpr GLuint get_max_fbo_attachments() noexcept {
    // Defined in Desktop GL as a minimum of 16
    // Defined in Mobile GL as a minimum of 4
    return fbo_attach_t::FBO_ATTACHMENT_MAX_COLORS;
}

constexpr GLint get_max_fbo_width() noexcept {
    //return get_gl_int(GL_MAX_FRAMEBUFFER_WIDTH);
    return 16384; // GLES 3.0 spec minimum
}

constexpr GLint get_max_fbo_height() noexcept {
    //return get_gl_int(GL_MAX_FRAMEBUFFER_HEIGHT);
    return 16384; // GLES 3.0 spec minimum
}

constexpr math::vec2i get_max_fbo_size() noexcept {
    return math::vec2i{get_max_fbo_width(), get_max_fbo_height()};
}

constexpr GLuint get_max_fbo_samples() noexcept {
    //return get_gl_uint(GL_MAX_FRAMEBUFFER_SAMPLES);
    return 4; // GLES 3.0 spec minimum
}

inline GLuint get_current_fbo_red_bits() noexcept {
    return get_gl_uint(GL_RED_BITS);
}

inline GLuint get_current_fbo_green_bits() noexcept {
    return get_gl_uint(GL_GREEN_BITS);
}

inline GLuint get_current_fbo_blue_bits() noexcept {
    return get_gl_uint(GL_BLUE_BITS);
}

inline GLuint get_current_fbo_alpha_bits() noexcept {
    return get_gl_uint(GL_ALPHA_BITS);
}

inline GLuint get_current_fbo_depth_bits() noexcept {
    return get_gl_uint(GL_DEPTH_BITS);
}

inline GLuint get_current_fbo_stencil_bits() noexcept {
    return get_gl_uint(GL_STENCIL_BITS);
}

inline GLuint get_current_fbo_sample_buffers() noexcept {
    return get_gl_uint(GL_SAMPLE_BUFFERS);
}


} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_FRAMEBUFFER_H__ */
