
#ifndef __LS_DRAW_RENDERBUFFER_H__
#define __LS_DRAW_RENDERBUFFER_H__

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * @brief rbo_format_t Allows for better type safety when creating or resizing
 * a renderbuffer.
-----------------------------------------------------------------------------*/
enum rbo_format_t : int {
    RBO_FMT_R_8         = GL_R8,
    RBO_FMT_R_8I        = GL_R8I,
    RBO_FMT_R_8UI       = GL_R8UI,
    RBO_FMT_R_16I       = GL_R16I,
    RBO_FMT_R_16UI      = GL_R16UI,
    RBO_FMT_R_32I       = GL_R32I,
    RBO_FMT_R_32UI      = GL_R32UI,
    
    RBO_FMT_RG_8        = GL_RG8,
    RBO_FMT_RG_8I       = GL_RG8I,
    RBO_FMT_RG_8UI      = GL_RG8UI,
    RBO_FMT_RG_16I      = GL_RG16I,
    RBO_FMT_RG_16UI     = GL_RG16UI,
    RBO_FMT_RG_32I      = GL_RG32I,
    RBO_FMT_RG_32UI     = GL_RG32UI,
    
    RBO_FMT_RGB_8       = GL_RGB8,
    RBO_FMT_RGB_565     = GL_RGB565,
    
    RBO_FMT_RGBA_8      = GL_RGBA8,
    RBO_FMT_RGBA_8I     = GL_RGBA8I,
    RBO_FMT_RGBA_8UI    = GL_RGBA8UI,
    RBO_FMT_RGBA_16I    = GL_RGB16I,
    RBO_FMT_RGBA_16UI   = GL_RGB16UI,
    RBO_FMT_RGBA_32I    = GL_RGB32I,
    RBO_FMT_RGBA_32UI   = GL_RGB32UI,
    
    RBO_FMT_RGBA_4      = GL_RGBA4,
    RBO_FMT_RGBA_5_1    = GL_RGB5_A1,
    RBO_FMT_RGBA_10_2   = GL_RGB10_A2,
    RBO_FMT_RGBA_10_2UI = GL_RGB10_A2UI,
    
    RBO_FMT_SRGBA_8     = GL_SRGB8_ALPHA8,

    RBO_FMT_STENCIL_8   = GL_STENCIL_INDEX8,
    
    RBO_FMT_DEPTH_16    = GL_DEPTH_COMPONENT16,
    RBO_FMT_DEPTH_24    = GL_DEPTH_COMPONENT24,
    RBO_FMT_DEPTH_32    = GL_DEPTH_COMPONENT32F,
    
    RBO_FMT_DEPTH_STENCIL_24_8  = GL_DEPTH24_STENCIL8,
    RBO_FMT_DEPTH_STENCIL_32_8  = GL_DEPTH32F_STENCIL8,
    
    RBO_FMT_DEFAULT     = GL_RGB8
};

/**----------------------------------------------------------------------------
 * @brief The renderbuffer class is a GPU-only class which allows framebuffers
 * to render off-screen. This is useful for framebuffer operations which
 * will not be returned back to the CPU (such as for depth or stencil functions.
-----------------------------------------------------------------------------*/
class renderbuffer {
    private:
        /**
         * @brief rboId
         *
         * A handle to the GPU-side render buffer object.
         */
        unsigned rboId;

    public:
        /**
         * @brief
         */
        ~renderbuffer();

        /**
         * @brief Constructor
         *
         * Initializes all local variables.
         */
        renderbuffer();

        /**
         * @brief Copy Constructor -- DELETED
         *
         * There is no ability to copy a render buffer object through OpenGL.
         * Their content is always filled by the framebuffer which contains
         * them.
         */
        renderbuffer(const renderbuffer&) = delete;

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
        renderbuffer(renderbuffer&& rbo);

        /**
         * @brief Copy Operator -- DELETED
         *
         * There is no ability to copy a render buffer object through OpenGL.
         * Their content is always filled by the framebuffer which contains
         * them.
         */
        renderbuffer& operator=(const renderbuffer&) = delete;

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
        renderbuffer& operator=(renderbuffer&& rbo);

        /**
         * Get the GPU-Assigned ID that this object references.
         *
         * @return A non-zero unsigned integer that is used as a handle to the
         * renderbuffer object represented by this object. This value returns 0
         * if nothing is referenced by *this.
         */
        unsigned getId() const;

        /**
         * Bind the current renderbuffer to OpenGL
         */
        void bind() const;

        /**
         * Unbind the current renderbuffer to OpenGL
         */
        void unbind() const;

        /**
         * @brief Generate a handle to a renderbuffer object on the GPU.
         *
         * @return TRUE if OpenGL was successfully able to create a handle to a
         * renderbuffer object, FALSE if not.
         */
        bool init();

        /**
         * @brief Generate a handle to a renderbuffer object on the GPU and
         * allocate memory for operations on it.
         *
         * @param width
         * The pixel size, width and height, that will be used for rendering
         * operations.
         *
         * @param internalFormat
         * The color format which will be used for rendering operations.
         *
         * @return TRUE if OpenGL was successfully able to create a handle to a
         * renderbuffer object, FALSE if not.
         */
        bool init(rbo_format_t internalFormat, const math::vec2i& size);

        /**
         * @brief Terminate all GPU-side resources used by *this renderbuffer.
         */
        void terminate();
};

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/renderbuffer_impl.h"

#endif // __LS_DRAW_RENDERBUFFER_H__
