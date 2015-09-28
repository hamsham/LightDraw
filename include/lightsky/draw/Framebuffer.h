/* 
 * File:   draw/framebuffer.h
 * Author: Miles Lacey
 *
 * Created on Jult 30, 2014, 10:02 PM
 */

#ifndef __LS_DRAW_FRAMEBUFFER_H__
#define	__LS_DRAW_FRAMEBUFFER_H__

#include "lightsky/draw/setup.h"
#include "lightsky/draw/renderbuffer.h"
#include "lightsky/draw/texture.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * Draw buffers for fbo render targets
-----------------------------------------------------------------------------*/
enum fbo_attach_t : int {
    FBO_ATTACHMENT_0            = GL_COLOR_ATTACHMENT0,
    FBO_ATTACHMENT_1            = GL_COLOR_ATTACHMENT1,
    FBO_ATTACHMENT_2            = GL_COLOR_ATTACHMENT2,
    FBO_ATTACHMENT_3            = GL_COLOR_ATTACHMENT3,
    FBO_ATTACHMENT_4            = GL_COLOR_ATTACHMENT4,
    FBO_ATTACHMENT_5            = GL_COLOR_ATTACHMENT5,
    FBO_ATTACHMENT_6            = GL_COLOR_ATTACHMENT6,
    FBO_ATTACHMENT_7            = GL_COLOR_ATTACHMENT7,
    FBO_ATTACHMENT_8            = GL_COLOR_ATTACHMENT8,
    FBO_ATTACHMENT_9            = GL_COLOR_ATTACHMENT9,
    FBO_ATTACHMENT_10           = GL_COLOR_ATTACHMENT10,
    FBO_ATTACHMENT_11           = GL_COLOR_ATTACHMENT11,
    FBO_ATTACHMENT_12           = GL_COLOR_ATTACHMENT12,
    FBO_ATTACHMENT_13           = GL_COLOR_ATTACHMENT13,
    FBO_ATTACHMENT_14           = GL_COLOR_ATTACHMENT14,
    FBO_ATTACHMENT_15           = GL_COLOR_ATTACHMENT15,
    
    FBO_ATTACHMENT_DEPTH            = GL_DEPTH_ATTACHMENT,
    FBO_ATTACHMENT_STENCIL          = GL_STENCIL_ATTACHMENT,
    FBO_ATTACHMENT_DEPTH_STENCIL    = GL_DEPTH_STENCIL_ATTACHMENT
};

/**----------------------------------------------------------------------------
 * Texture targets that can be bound to an FBO
-----------------------------------------------------------------------------*/
enum texture_target_t : int {
    FBO_2D_TARGET           = GL_TEXTURE_2D,
    
    FBO_3D_TARGET           = GL_TEXTURE_3D,
    
    FBO_POS_X_TARGET        = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    FBO_NEG_X_TARGET        = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    FBO_POS_Y_TARGET        = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    FBO_NEG_Y_TARGET        = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    FBO_POS_Z_TARGET        = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    FBO_NEG_Z_TARGET        = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

/**----------------------------------------------------------------------------
 * Draw/Read access control for framebuffer objects.
-----------------------------------------------------------------------------*/
enum fbo_access_t : unsigned {
    FBO_ACCESS_RW   = GL_FRAMEBUFFER,
    FBO_ACCESS_W    = GL_DRAW_FRAMEBUFFER,
    FBO_ACCESS_R    = GL_READ_FRAMEBUFFER
};

/**----------------------------------------------------------------------------
 * Framebuffer status codes
-----------------------------------------------------------------------------*/
enum fbo_status_t : int {
    FBO_COMPLETE                        = GL_FRAMEBUFFER_COMPLETE,
    FBO_UNDEFINED                       = GL_FRAMEBUFFER_UNDEFINED,
    FBO_INCOMPLETE_ATTACHMENT           = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
    FBO_INCOMPLETE_MISSING_ATTACHMENT   = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
    FBO_UNSUPPORTED                     = GL_FRAMEBUFFER_UNSUPPORTED,
    FBO_INCOMPLETE_MULTISAMPLE          = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
};

/**----------------------------------------------------------------------------
 * Bitmasks that can help determine what aspects of a framebuffer should be
 * modified during read/draw calls.
-----------------------------------------------------------------------------*/
enum fbo_mask_t : int {
    FBO_DEPTH_BIT   = GL_DEPTH_BUFFER_BIT,
    FBO_STENCIL_BIT = GL_STENCIL_BUFFER_BIT,
    FBO_COLOR_BIT   = GL_COLOR_BUFFER_BIT,
};

/**----------------------------------------------------------------------------
 * @brief Framebuffer Object
 * 
 * A Framebuffer Object represents a canvas for which all draw operations will
 * be placed on. When a Framebuffer is not active, OpenGL will render to the
 * default buffer residing within a render context. Framebuffer objects can
 * be used to render to a texture as well.
-----------------------------------------------------------------------------*/
class framebuffer final {
    public:
        /**
         * Get the maximum number of supported color attachments from the
         * current render context.
         * OpenGL requires at least 8 attachments are supported.
         * 
         * @return A signed integral type representing the number of currently
         * supported render targets.
         */
        static int getMaxNumAttachments();

        /**
         * Error code handling for framebuffer creation. The input FBO must be
         * bound/active in order to report properly.
         * 
         * @param fbo
         * A constant reference to the active framebuffer object.
         */
        static void printStatus(const framebuffer& fbo);
        
        /**
         * Get the completion status of a framebuffer based on the currently
         * used fbo_status_t.
         * 
         * An FBO must be bound/active in order to report properly.
         * 
         * @return framebuffer
         */
        static fbo_status_t getStatus(const framebuffer& fbo);
        
        /**
         * Bind the default read/write buffer
         * 
         * @param fbo_access_t
         */
        static void bindDefaultFramebuffer(fbo_access_t access = FBO_ACCESS_RW);
        
    private:
        /**
         * Access determines if a framebuffer should be rendered to, read from,
         * or both.
         */
        fbo_access_t access = FBO_ACCESS_RW;
        
        /**
         * fboID is a GPU-assigned handle to a framebuffer object within
         * OpenGL.
         */
        unsigned fboId = 0;
        
    public:
        /**
         * @brief Constructor
         * 
         * Initializes all values within *this to their default values. This
         * does not initialize a framebuffer on the GPU.
         */
        framebuffer();
        
        /**
         * @brief Copy Constructor - DELETED
         */
        framebuffer(const framebuffer&) = delete;
        
        /**
         * @brief Move Constructor
         * 
         * Moves all data from the input parameter into *this without
         * performing any copies.
         * 
         * @param fbo
         * An r-value reference to another Framebuffer Object.
         */
        framebuffer(framebuffer&& fbo);
        
        /**
         * @brief Destructor
         * 
         * Frees all memory, GPU-handles, and resources used by *this.
         */
        ~framebuffer();
        
        /**
         * @brief Copy Operator - DELETED
         */
        framebuffer& operator=(const framebuffer&) = delete;
        
        /**
         * @brief Move Operator
         * 
         * Moves all data from the input parameter into *this without
         * performing any copies.
         * 
         * @param fbo
         * An r-value reference to another Framebuffer Object.
         */
        framebuffer& operator=(framebuffer&& fbo);
        
        /**
         * Get the GPU-Assigned ID that this object references.
         * 
         * @return A non-zero unsigned integer that is used as a handle to the
         * framebuffer object represented by this object.
         * This value returns 0 if nothing is referenced by *this.
         */
        unsigned getId() const;
        
        /**
         * Get the framebuffer acccess type.
         * 
         * @return fbo_access_t
         * An enumeration that can help make perform framebuffer read/write
         * operations
         */
        fbo_access_t getAccessType() const;
        
        /**
         * Set the framebuffer access type.
         * 
         * @param a
         * An enumeration that can help make perform framebuffer read/write
         * operations.
         */
        void setAccessType(fbo_access_t a);
        
        /**
         * Bind the current framebuffer to OpenGL
         */
        void bind() const;
        
        /**
         * Unbind the current framebuffer to OpenGL
         */
        void unbind() const;
        
        /**
         * Initialize an empty framebuffer.
         * 
         * @return TRUE if a new framebuffer object was successfully generated
         * on the GPU, FALSE if otherwise.
         */
        bool init();
        
        /**
         * Release all resources used by this framebuffer object.
         */
        void terminate();
        
        /**
        
         * @brief Blit (draw) this frame buffer onto another.
         * 
         * This buffer must be accessed as LS_READ_BUFFER in order to work
         * properly.
         * 
         * @param srcOrig
         * A point that specifies the bottom-left corner of *this from which
         * to blit onto another framebuffer.
         * 
         * @param srcSize
         * A point that specifies the top-right corner of *this to blit onto
         * another framebuffer.
         * 
         * @param dstOrig
         * A point that specifies the bottom-left corner of the target
         * framebuffer at which to start blitting to.
         * 
         * @param dstSize
         * A point that specifies the top-right corner of the target
         * framebuffer at which to end the blitting.
         * 
         * @param msk
         * Used to determine if *this framebuffer's depth, stencil, and/or
         * color buffers should be copied to the target framebuffer.
         * 
         * @param fltr
         * Used to determine the type of filtering that should be performed
         * during the blitting operation.
         */
        void blit(
            const math::vec2i& srcOrig, const math::vec2i& srcSize,
            const math::vec2i& dstOrig, const math::vec2i& dstSize,
            fbo_mask_t msk, tex_filter_t fltr = TEX_FILTER_LINEAR
        ) const;
        
        /**
         * Clear the color attachment indicated by the input parameter.
         * 
         * @param A bitmask of type fbo_mask_t.
         */
        void clear(fbo_mask_t) const;
        
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
        void setDrawTargets(unsigned numTargets, const fbo_attach_t* targets);
        
        /**
         * Attach a texture to the currently bound framebuffer
         * 
         * @param atch
         * Specifies the internal OpenGL color/depth/stencil attachment to use.
         * This parameter must correlate to a fragment shader output.
         * 
         * @param trgt
         * Use this parameter in order to aid in rendering to different parts
         * of different texture types.
         * 
         * @param tex
         * A constant reference to a texture object to bind to this and use as
         * a render target.
         * 
         * @param mipmapLevel
         * The level of mipmap filtering to use for rendering.
         * 
         * @param layer
         * In the event that 'tex' is a 3D texture, this parameter specifies
         * which 3D layer to use for rendering.
         */
        void attachRenderTarget(
            fbo_attach_t atch,
            texture_target_t trgt,
            const texture& tex,
            int mipmapLevel = 0,
            int layer = 0
        );
        
        /**
         * Attach a texture to the currently bound framebuffer
         * 
         * @param atch
         * Specifies the internal OpenGL color/depth/stencil attachment to use.
         * This parameter must correlate to a fragment shader output.
         * 
         * @param tex
         * A constant reference to a texture object to bind to this and use as
         * a render target.
         */
        void attachRenderTarget(fbo_attach_t atch, const texture& tex);

        /**
         * Attach a render buffer to the currently bound framebuffer
         *
         * @param atch
         * Specifies the internal OpenGL color/depth/stencil attachment to use.
         * This parameter must correlate to a fragment shader output.
         *
         * @param rbo
         * A constant reference to a renderbuffer object to bind to this for
         * use as a render target.
         */
        void attachRenderTarget(fbo_attach_t atch, const renderbuffer& rbo);
};

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/framebuffer_impl.h"

#endif	/* __LS_DRAW_FRAMEBUFFER_H__ */
