/*
 * File:   draw/color.h
 * Author: Miles Lacey
 *
 * Created on June 2, 2014, 8:12 PM
 */

#ifndef __LS_DRAW_COLOR_H__
#define __LS_DRAW_COLOR_H__

#include "lightsky/draw/Setup.h"



namespace ls
{
namespace draw
{



/**----------------------------------------------------------------------------
 * @brief pixel_id_t
 *
 * Identifies a 32-bit data type which is used to receive colors from a
 * texture or framebuffer instead of an array of 8-bit types.
-----------------------------------------------------------------------------*/
typedef uint32_t pixel_id_t;



/*-----------------------------------------------------------------------------
    Enumerations
-----------------------------------------------------------------------------*/
/**
 * @brief Framebuffer/Texture data types.
 *
 * These enumerations are used to specify the basic data type each pixel of a
 * texture or framebuffer.
 */
enum color_type_t : GLenum
{
    COLOR_TYPE_BYTE = GL_BYTE,
    COLOR_TYPE_UBYTE = GL_UNSIGNED_BYTE,

    COLOR_TYPE_SHORT = GL_SHORT,
    COLOR_TYPE_USHORT = GL_UNSIGNED_SHORT,
    COLOR_TYPE_USHORT_565 = GL_UNSIGNED_SHORT_5_6_5,
    COLOR_TYPE_USHORT_5551 = GL_UNSIGNED_SHORT_5_5_5_1,
    COLOR_TYPE_USHORT_4444 = GL_UNSIGNED_SHORT_4_4_4_4,

    COLOR_TYPE_INT = GL_INT,
    COLOR_TYPE_UINT = GL_UNSIGNED_INT,
    COLOR_TYPE_UINT_24_8 = GL_UNSIGNED_INT_24_8,
    COLOR_TYPE_UINT_32F_32I = GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
    COLOR_TYPE_UINT_111110F = GL_UNSIGNED_INT_10F_11F_11F_REV,
    COLOR_TYPE_UINT_999_E5 = GL_UNSIGNED_INT_5_9_9_9_REV,
    COLOR_TYPE_UINT_2101010 = GL_UNSIGNED_INT_2_10_10_10_REV,

    COLOR_TYPE_HALF_FLOAT = GL_HALF_FLOAT,
    COLOR_TYPE_FLOAT = GL_FLOAT,

    COLOR_TYPE_DEFAULT = GL_UNSIGNED_BYTE,
    COLOR_TYPE_INVALID = GL_INVALID_ENUM
};



/**
 * @brief GPU-side pixel formats.
 *
 * OpenGL refers to these as the "format" parameter when using operations such
 * as "glTexImage()."
 */
enum pixel_layout_t : GLint
{
    COLOR_LAYOUT_DEPTH = GL_DEPTH_COMPONENT,
    COLOR_LAYOUT_DEPTH_STENCIL = GL_DEPTH_STENCIL,
    COLOR_LAYOUT_STENCIL = GL_STENCIL_INDEX8,

    COLOR_LAYOUT_R = GL_RED,
    COLOR_LAYOUT_R_I = GL_RED_INTEGER,

    COLOR_LAYOUT_RG = GL_RG,
    COLOR_LAYOUT_RG_I = GL_RG_INTEGER,

    COLOR_LAYOUT_RGB = GL_RGB,
    COLOR_LAYOUT_RGB_I = GL_RGB_INTEGER,

    COLOR_LAYOUT_RGBA = GL_RGBA,
    COLOR_LAYOUT_RGBA_I = GL_RGBA_INTEGER,

#ifdef LS_DRAW_BACKEND_GL
    COLOR_LAYOUT_DEFAULT_RGB = GL_BGR,
    COLOR_LAYOUT_DEFAULT_RGBA = GL_BGRA,
#else
    COLOR_LAYOUT_DEFAULT_RGB = GL_RGB,
    COLOR_LAYOUT_DEFAULT_RGBA = GL_RGBA,
#endif

    COLOR_LAYOUT_INVALID = GL_INVALID_ENUM,
};



/**
 * @brief CPU-Side pixel formats
 *
 * OpenGL refers to these types as the "internalFormat" parameter in functions
 * such as "glRenderbufferStorage()" and "glTexImage()"
 */
enum pixel_format_t : GLenum
{
    COLOR_FMT_DEPTH_STENCIL_24_8 = GL_DEPTH24_STENCIL8,
    COLOR_FMT_DEPTH_STENCIL_32_8 = GL_DEPTH32F_STENCIL8,

    COLOR_FMT_DEPTH_16U = GL_DEPTH_COMPONENT16,
    COLOR_FMT_DEPTH_24U = GL_DEPTH_COMPONENT24,
    COLOR_FMT_DEPTH_32F = GL_DEPTH_COMPONENT32F,

    COLOR_FMT_R_8 = GL_R8,
    COLOR_FMT_R_8I = GL_R8I,
    COLOR_FMT_R_8U = GL_R8UI,
    COLOR_FMT_R_16I = GL_R16I,
    COLOR_FMT_R_16U = GL_R16UI,
    COLOR_FMT_R_16F = GL_R16F,
    COLOR_FMT_R_32I = GL_R32I,
    COLOR_FMT_R_32U = GL_R32UI,
    COLOR_FMT_R_32F = GL_R32F,

    COLOR_FMT_RG_8 = GL_RG8,
    COLOR_FMT_RG_8I = GL_RG8I,
    COLOR_FMT_RG_8U = GL_RG8UI,
    COLOR_FMT_RG_16I = GL_RG16I,
    COLOR_FMT_RG_16U = GL_RG16UI,
    COLOR_FMT_RG_16F = GL_RG16F,
    COLOR_FMT_RG_32I = GL_RG32I,
    COLOR_FMT_RG_32U = GL_RG32UI,
    COLOR_FMT_RG_32F = GL_RG32F,

    COLOR_FMT_RGB_8 = GL_RGB8,
    COLOR_FMT_RGB_8I = GL_RGB8I,
    COLOR_FMT_RGB_8U = GL_RGB8UI,
    COLOR_FMT_RGB_16I = GL_RGB16I,
    COLOR_FMT_RGB_16U = GL_RGB16UI,
    COLOR_FMT_RGB_16F = GL_RGB16F,
    COLOR_FMT_RGB_32I = GL_RGB32I,
    COLOR_FMT_RGB_32U = GL_RGB32UI,
    COLOR_FMT_RGB_32F = GL_RGB32F,

    COLOR_FMT_RGBA_8 = GL_RGBA8,
    COLOR_FMT_RGBA_8I = GL_RGBA8I,
    COLOR_FMT_RGBA_8U = GL_RGBA8UI,
    COLOR_FMT_RGBA_16I = GL_RGBA16I,
    COLOR_FMT_RGBA_16U = GL_RGBA16UI,
    COLOR_FMT_RGBA_16F = GL_RGBA16F,
    COLOR_FMT_RGBA_32I = GL_RGBA32I,
    COLOR_FMT_RGBA_32U = GL_RGBA32UI,
    COLOR_FMT_RGBA_32F = GL_RGBA32F,

    COLOR_FMT_SRGB_8 = GL_SRGB8,
    COLOR_FMT_SRGBA_8 = GL_SRGB8_ALPHA8,

    /*
    COLOR_FMT_R_C       = GL_COMPRESSED_R11_EAC,
    COLOR_FMT_RG_C      = GL_COMPRESSED_RG11_EAC,
    COLOR_FMT_RGB_C     = GL_COMPRESSED_RGB8_ETC2,
    COLOR_FMT_RGBA_8_C  = GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
    COLOR_FMT_RGBA_C    = GL_COMPRESSED_RGBA8_ETC2_EAC,
    COLOR_FMT_SRGB_8_C  = GL_COMPRESSED_SRGB8_ETC2,
    COLOR_FMT_SRGBA_8_C = GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
    COLOR_FMT_SRGBA_C   = GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
     */

    COLOR_FMT_RGB_565 = GL_RGB565,
    COLOR_FMT_RGB_111110 = GL_R11F_G11F_B10F,
    COLOR_FMT_RGB_9_E5 = GL_RGB9_E5,
    COLOR_FMT_RGBA_5_1 = GL_RGB5_A1,
    COLOR_FMT_RGBA_10_2 = GL_RGB10_A2,
    COLOR_FMT_RGBA_10_2U = GL_RGB10_A2UI,
    COLOR_FMT_RGBA_4 = GL_RGBA4,

    COLOR_FMT_DEFAULT_RGB = GL_RGB,
    COLOR_FMT_DEFAULT_RGBA = GL_RGBA,

    #ifdef LS_DRAW_BACKEND_GL
    COLOR_FMT_COMPRESSED_RGB = GL_COMPRESSED_RGB,
    COLOR_FMT_COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
    #endif

    COLOR_FMT_INVALID = GL_INVALID_ENUM,
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
pixel_layout_t get_color_layout(const pixel_format_t internalFormat) noexcept;



/**
 * Retrieve the appropriate byte layout of a pixel format.
 *
 * @param internalFormat
 * A value from the pixel_format_t enumeration which should be used to deduce
 * an appropriate color type for a pixel.
 *
 * @return A value from the color_type_t which determines what the appropriate
 * byte layout of a single pixel value should be.
 */
color_type_t get_color_type(const pixel_format_t internalFormat) noexcept;



/**
 * Retrieve the number of components contained within a pixel format
 *
 * @param internalFormat
 * A value from the pixel_format_t enumeration which should be used to deduce
 * the number of components contained within a pixel.
 *
 * @return An unsigned integral type which contains the number of components
 * contained within a single pixel.
 */
unsigned get_num_pixel_components(const pixel_format_t internalFormat) noexcept;



/**
 * Retrieve the number of bytes occupied by a single pixel
 *
 * @param colorType
 * A value from the color_type_t enumeration which should be used to deduce
 * an appropriate byte size for a pixel component or components.
 *
 * @return An unsigned integral type which contains the number of bytes
 * required for a single pixel format.
 */
unsigned get_num_color_bytes(const color_type_t colorType) noexcept;



/*-----------------------------------------------------------------------------
    Useful Typedefs
-----------------------------------------------------------------------------*/
namespace color
{
typedef math::vec4_t<unsigned char> colorub_t;

typedef math::vec4_t<unsigned short> colorus_t;

typedef math::vec4_t<unsigned int> colorui_t;

typedef math::vec4_t<float> colorf_t;

typedef colorf_t color;



/*-----------------------------------------------------------------------------
    Color Types
-----------------------------------------------------------------------------*/
static const color blank = color{0.f, 0.f, 0.f, 0.f};

// Primary Colors
static const color red = color{1.f, 0.f, 0.f, 1.f};

static const color green = color{0.f, 1.f, 0.f, 1.f};

static const color blue = color{0.f, 0.f, 1.f, 1.f};

//Secondary Colors
static const color magenta = color{1.f, 0.f, 1.f, 1.f};

static const color yellow = color{1.f, 1.f, 0.f, 1.f};

static const color cyan = color{0.f, 1.f, 1.f, 1.f};

// Monochrome
static const color black = color{0.f, 0.f, 0.f, 1.f};

static const color white = color{1.f, 1.f, 1.f, 1.f};

static const color gray = color{0.5f, 0.5f, 0.5f, 1.f};



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
template <typename int_t, typename float_t = float>
math::vec4_t <float_t> color_int_to_float(const math::vec4_t <int_t>& c)
{
    const float_t range = float_t{1.0} / ((float_t)std::numeric_limits<int_t>::max());
    return math::vec4_t<float_t>{
        range * (float_t)c[0],
        range * (float_t)c[1],
        range * (float_t)c[2],
        range * (float_t)c[3]
    };
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
template <typename int_t, typename float_t = float, int_t range = std::numeric_limits<int_t>::max()>
constexpr math::vec4_t <int_t> color_float_to_int(const math::vec4_t <float_t>& c)
{
    return math::vec4_t < int_t > {
        ls::math::ranged_floor<float_t, int_t, range>((c[0] * range) + float_t{0.5}),
        ls::math::ranged_floor<float_t, int_t, range>((c[1] * range) + float_t{0.5}),
        ls::math::ranged_floor<float_t, int_t, range>((c[2] * range) + float_t{0.5}),
        ls::math::ranged_floor<float_t, int_t, range>((c[3] * range) + float_t{0.5})
    };
}



/*-------------------------------------
 * Color Conversion to ID
-------------------------------------*/
constexpr pixel_id_t color_to_id(const colorub_t& col) noexcept
{
    return 0
           | (col[0] << 0)
           | (col[1] << 8)
           | (col[2] << 16);
}



/*-------------------------------------
 * ID Conversion to Color
-------------------------------------*/
inline colorf_t id_to_color(const pixel_id_t id, const float normFactor = 1.f / 255.f) noexcept
{
    return colorf_t{
        (float)((id & 0x000000FF) >> 0) * normFactor,
        (float)((id & 0x0000FF00) >> 8) * normFactor,
        (float)((id & 0x00FF0000) >> 16) * normFactor,
        1.f
    };
}
} // end color namespace
} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_COLOR_H__ */
