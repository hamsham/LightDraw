/* 
 * File:   draw/color.h
 * Author: Miles Lacey
 *
 * Created on June 2, 2014, 8:12 PM
 */

#ifndef __LS_DRAW_COLOR_H__
#define	__LS_DRAW_COLOR_H__

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
    Enumerations
-----------------------------------------------------------------------------*/
/**
 * @brief Framebuffer/Texture data types.
 * 
 * These enumerations are used to specify the basic data type each pixel of a
 * texture or framebuffer.
 */
enum color_type_t : int {
    COLOR_TYPE_BYTE             = GL_BYTE,
    COLOR_TYPE_SHORT            = GL_SHORT,
    COLOR_TYPE_INT              = GL_INT,
    
    COLOR_TYPE_FLOAT            = GL_FLOAT,
    
    COLOR_TYPE_UNSIGNED_BYTE    = GL_UNSIGNED_BYTE,
    COLOR_TYPE_UNSIGNED_SHORT   = GL_UNSIGNED_SHORT,
    COLOR_TYPE_UNSIGNED_INT     = GL_UNSIGNED_INT,
    
    COLOR_TYPE_DEFAULT          = GL_UNSIGNED_BYTE,
    COLOR_TYPE_INVALID          = -1
};

/**
 * @brief GPU-side pixel formats.
 * 
 * OpenGL refers to these as the "format" parameter when using operations such
 * as "glTexImage()."
 */
enum pixel_layout_t : int {
    COLOR_LAYOUT_GRAY       = GL_DEPTH_COMPONENT,
    COLOR_LAYOUT_R          = GL_RED,
    COLOR_LAYOUT_RG         = GL_RG,
    COLOR_LAYOUT_RGB        = GL_RGB,
    COLOR_LAYOUT_RGBA       = GL_RGBA,
    
    COLOR_LAYOUT_DEFAULT    = GL_RGB
};

/**
 * @brief CPU-Side pixel formats
 * 
 * OpenGL refers to these types as the "internalFormat" parameter in functions
 * such as "glRenderbufferStorage()" and "glTexImage()"
 */
enum pixel_format_t : int {
    COLOR_FMT_GRAY_8    = GL_DEPTH_COMPONENT,
    COLOR_FMT_GRAY_16I  = GL_DEPTH_COMPONENT16,
    COLOR_FMT_GRAY_16U  = GL_DEPTH_COMPONENT16,
    COLOR_FMT_GRAY_16F  = GL_R16F,
    COLOR_FMT_GRAY_24   = GL_DEPTH_COMPONENT24,
    COLOR_FMT_GRAY_32F  = GL_DEPTH_COMPONENT32F,
    
    COLOR_FMT_R_8       = GL_R8,
    COLOR_FMT_R_16I     = GL_R16I,
    COLOR_FMT_R_16U     = GL_R16UI,
    COLOR_FMT_R_16F     = GL_R16F,
    COLOR_FMT_R_32I     = GL_R32I,
    COLOR_FMT_R_32U     = GL_R32UI,
    COLOR_FMT_R_32F     = GL_R32F,
    //COLOR_FMT_R_C       = GL_COMPRESSED_R11_EAC,
    
    COLOR_FMT_RG_8      = GL_RG8,
    COLOR_FMT_RG_16I    = GL_RG16I,
    COLOR_FMT_RG_16U    = GL_RG16UI,
    COLOR_FMT_RG_16F    = GL_RG16F,
    COLOR_FMT_RG_32I    = GL_RG32I,
    COLOR_FMT_RG_32U    = GL_RG32UI,
    COLOR_FMT_RG_32F    = GL_RG32F,
    //COLOR_FMT_RG_C      = GL_COMPRESSED_RG11_EAC,
    
    COLOR_FMT_RGB_8     = GL_RGB8,
    COLOR_FMT_RGB_16I   = GL_RGB16I,
    COLOR_FMT_RGB_16U   = GL_RGB16UI,
    COLOR_FMT_RGB_16F   = GL_RGB16F,
    COLOR_FMT_RGB_32I   = GL_RGB32I,
    COLOR_FMT_RGB_32U   = GL_RGB32UI,
    COLOR_FMT_RGB_32F   = GL_RGB32F,
    //COLOR_FMT_RGB_C     = GL_COMPRESSED_RGB8_ETC2,
    
    COLOR_FMT_RGBA_8    = GL_RGBA8,
    //COLOR_FMT_RGBA_8_C  = GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
    COLOR_FMT_RGBA_16I  = GL_RGBA16I,
    COLOR_FMT_RGBA_16U  = GL_RGBA16UI,
    COLOR_FMT_RGBA_16F  = GL_RGBA16F,
    COLOR_FMT_RGBA_32I  = GL_RGBA32I,
    COLOR_FMT_RGBA_32U  = GL_RGBA32UI,
    COLOR_FMT_RGBA_32F  = GL_RGBA32F,
    //COLOR_FMT_RGBA_C    = GL_COMPRESSED_RGBA8_ETC2_EAC,
    
    COLOR_FMT_SRGB      = GL_SRGB,
    COLOR_FMT_SRGB_8    = GL_SRGB8,
    //COLOR_FMT_SRGB_8_C  = GL_COMPRESSED_SRGB8_ETC2,
    COLOR_FMT_SRGBA_8   = GL_SRGB8_ALPHA8,
    //COLOR_FMT_SRGBA_8_C = GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
    //COLOR_FMT_SRGBA_C   = GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
    
    COLOR_FMT_DEFAULT   = GL_RGB
};

/**
 * Convert a pixel format to a standard color layout.
 * 
 * @param internalFormat
 * The internal format of a texture used by OpenGL.
 * 
 * @return A pixel_layout_t which can be used for the "format" parameter of
 * OpenGL texture objects.
 */
pixel_layout_t getColorLayout(pixel_format_t internalFormat);

/*-----------------------------------------------------------------------------
    Useful Typedefs
-----------------------------------------------------------------------------*/
namespace color {
typedef math::vec4_t<unsigned char>     colorub_t;
typedef math::vec4_t<unsigned short>    colorus_t;
typedef math::vec4_t<unsigned int>      colorui_t;
typedef math::vec4_t<float>             colorf_t;

typedef colorf_t color;

/*-----------------------------------------------------------------------------
    Color Types
-----------------------------------------------------------------------------*/
static constexpr color blank    = color{0.f, 0.f, 0.f, 0.f};

// Primary Colors
static constexpr color red      = color{1.f, 0.f, 0.f, 1.f};
static constexpr color green    = color{0.f, 1.f, 0.f, 1.f};
static constexpr color blue     = color{0.f, 0.f, 1.f, 1.f};

//Secondary Colors
static constexpr color magenta  = color{1.f, 0.f, 1.f, 1.f};
static constexpr color yellow   = color{1.f, 1.f, 0.f, 1.f};
static constexpr color cyan     = color{0.f, 1.f, 1.f, 1.f};

// Monochrome
static constexpr color black    = color{0.f, 0.f, 0.f, 1.f};
static constexpr color white    = color{1.f, 1.f, 1.f, 1.f};
static constexpr color gray     = color{0.5f, 0.5f, 0.5f, 1.f};

/*-----------------------------------------------------------------------------
 * Color Functions
-----------------------------------------------------------------------------*/
/**
 * @brief Convert a color using an integral type into a float color.
 * 
 * The returned color's precision will be clamped to [-1, 1]. The maximum
 * possible value of the input parameter's base type must be greater than 0.
 * 
 * @param c
 * An RGBA color which uses a signed or unsigned integral precision as its base
 * type.
 * 
 * @return A color object which uses floating-point precision for its base type.
 */
template <typename int_t, typename float_t=float>
math::vec4_t<float_t> color_int_to_float(const math::vec4_t<int_t>& c) {
    const float_t range = float_t{1.0} / ((float_t)std::numeric_limits<int_t>::max());
    math::vec4_t<float_t> ret;
    
    ret[0] = range * (float_t)c[0];
    ret[1] = range * (float_t)c[1];
    ret[2] = range * (float_t)c[2];
    ret[3] = range * (float_t)c[3];
    
    return ret;
}

/**
 * @brief Convert a color using a float type into an integral color.
 * 
 * The returned color's internal values will be clamped between the base type's
 * std::numeric_limits::max() and std::numeric_limits::min() values. The
 * maximum and minimum possible values of the input parameter's values must be
 * within the range of [-1, 1].
 * 
 * @param c
 * An RGBA color which uses floating-point precision as its base type.
 * 
 * @return A color object which uses integral precision for its base type.
 */
template <typename int_t, typename float_t=float>
math::vec4_t<int_t> color_float_to_int(const math::vec4_t<float_t>& c) {
    const int_t range = std::numeric_limits<int_t>::max();
    math::vec4_t<int_t> ret;
    
    ret[0] = (int_t)std::floor((c[0] * range) + float_t{0.5});
    ret[1] = (int_t)std::floor((c[1] * range) + float_t{0.5});
    ret[2] = (int_t)std::floor((c[2] * range) + float_t{0.5});
    ret[3] = (int_t)std::floor((c[3] * range) + float_t{0.5});
    
    return ret;
}

} // end color namespace
} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_COLOR_H__ */
