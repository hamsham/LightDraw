
/*
 * File:   TextureAttribs.h
 * Author: Miles
 *
 * Created on February 22, 2016, 5:22 PM
 */

#ifndef __LS_DRAW_TEXTURE_ATTRIBS_H__
#define __LS_DRAW_TEXTURE_ATTRIBS_H__



#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Color.h"



namespace ls
{
namespace draw
{



/**
 * Texture slots determine what binding point a shader will retrieve a sampler
 * sampler from. Multiple slots can be used at a time.
 */
enum tex_slot_t : int
{
    TEXTURE_SLOT_0 = 0,
    TEXTURE_SLOT_1 = 1,
    TEXTURE_SLOT_2 = 2,
    TEXTURE_SLOT_3 = 3,
    TEXTURE_SLOT_4 = 4,
    TEXTURE_SLOT_5 = 5,
    TEXTURE_SLOT_6 = 6,
    TEXTURE_SLOT_7 = 7,
    TEXTURE_SLOT_8 = 8,
    TEXTURE_SLOT_9 = 9,

    TEXTURE_SLOT_10 = 10,
    TEXTURE_SLOT_11 = 11,
    TEXTURE_SLOT_12 = 12,
    TEXTURE_SLOT_13 = 13,
    TEXTURE_SLOT_14 = 14,
    TEXTURE_SLOT_15 = 15,
    TEXTURE_SLOT_16 = 16,
    TEXTURE_SLOT_17 = 17,
    TEXTURE_SLOT_18 = 18,
    TEXTURE_SLOT_19 = 19,

    TEXTURE_SLOT_20 = 20,
    TEXTURE_SLOT_21 = 21,
    TEXTURE_SLOT_22 = 22,
    TEXTURE_SLOT_23 = 23,
    TEXTURE_SLOT_24 = 24,
    TEXTURE_SLOT_25 = 25,
    TEXTURE_SLOT_26 = 26,
    TEXTURE_SLOT_27 = 27,
    TEXTURE_SLOT_28 = 28,
    TEXTURE_SLOT_29 = 29,

    TEXTURE_SLOT_30 = 30,
    TEXTURE_SLOT_31 = 31,

    TEXTURE_SLOT_MAX = 32,

    TEXTURE_SLOT_DEFAULT = 0,
    TEXTURE_SLOT_DIFFUSE = TEXTURE_SLOT_DEFAULT + 0,
    TEXTURE_SLOT_NORMAL = TEXTURE_SLOT_DEFAULT + 1,
    TEXTURE_SLOT_SPECULAR = TEXTURE_SLOT_DEFAULT + 2,
    TEXTURE_SLOT_AMBIENT = TEXTURE_SLOT_DEFAULT + 3,

    TEXTURE_SLOT_INVALID = -1,
    TEXTURE_SLOT_GPU_OFFSET = GL_TEXTURE0
};



/**
 * Descriptors for different texture types
 */
enum tex_type_t : GLenum
{
    TEX_TYPE_2D = GL_TEXTURE_2D,
    TEX_TYPE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEX_TYPE_3D = GL_TEXTURE_3D,
    TEX_TYPE_CUBE = GL_TEXTURE_CUBE_MAP,
};



/**
 * Descriptors for 2D texture sub-types. Enumeration values for cube map
 * follow the OpenGL coordinate system.
 */
enum tex_2d_type_t : GLenum
{
    TEX_SUBTYPE_2D = GL_TEXTURE_2D,

    TEX_SUBTYPE_CUBE_RIGHT = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    TEX_SUBTYPE_CUBE_LEFT = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    TEX_SUBTYPE_CUBE_TOP = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    TEX_SUBTYPE_CUBE_BOTTOM = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    TEX_SUBTYPE_CUBE_BACK = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    TEX_SUBTYPE_CUBE_FRONT = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};



/**
 * Descriptors for 3D texture sub-types
 */
enum tex_3d_type_t : GLenum
{
    TEX_SUBTYPE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEX_SUBTYPE_3D = GL_TEXTURE_3D,
};



/**
 * Swizzle values for a texture.
 *
 * Texture values can have their pixel values flipped with one another in GL
 * client code rather than in GLSL. This can be done to help resolve
 * differences amongst platforms and GPU storage formats.
 */
enum pixel_swizzle_t : GLenum
{
    SWIZZLE_RED = GL_RED,
    SWIZZLE_GREEN = GL_GREEN,
    SWIZZLE_BLUE = GL_BLUE,
    SWIZZLE_ALPHA = GL_ALPHA,
    SWIZZLE_ZERO = GL_ZERO,
    SWIZZLE_ONE = GL_ONE,
};



/**
 * Texture & Framebuffer filtering specifiers
 */
enum tex_filter_t : int
{
    TEX_FILTER_NEAREST = GL_NEAREST,
    TEX_FILTER_LINEAR = GL_LINEAR,

    TEX_FILTER_NEAREST_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    TEX_FILTER_NEAREST_LINEAR = GL_NEAREST_MIPMAP_LINEAR,

    TEX_FILTER_LINEAR_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    TEX_FILTER_LINEAR_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};



/**
 * Enumeration to help distinguish between different texture wrapping modes.
 */
enum tex_wrap_t : GLenum
{
    TEX_WRAP_CLAMP = GL_CLAMP_TO_EDGE,
    TEX_WRAP_MIRROR_REPEAT = GL_MIRRORED_REPEAT,
    TEX_WRAP_REPEAT = GL_REPEAT,

    // Found in the GLES 3.0 Spec
    TEX_WRAP_DEFAULT = TEX_WRAP_REPEAT
};



/**
 * Parameters for modifying the appearance of textures.
 */
enum tex_param_t : int
{
    TEX_PARAM_INVALID = -1,

    TEX_PARAM_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    TEX_PARAM_MAG_FILTER = GL_TEXTURE_MAG_FILTER,

    TEX_PARAM_MIN_LOD = GL_TEXTURE_MIN_LOD,
    TEX_PARAM_MAX_LOD = GL_TEXTURE_MAX_LOD,

    TEX_PARAM_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
    TEX_PARAM_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
    TEX_PARAM_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
    TEX_PARAM_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,

    TEX_PARAM_WRAP_S = GL_TEXTURE_WRAP_S,
    TEX_PARAM_WRAP_T = GL_TEXTURE_WRAP_T,
    TEX_PARAM_WRAP_R = GL_TEXTURE_WRAP_R,
};



/**----------------------------------------------------------------------------
 * Texture Attribs provide meta-data about a texture object without requiring
 * client code to query OpenGL.
-----------------------------------------------------------------------------*/
class TextureAttrib
{
  private:
    color_type_t colorType;

    pixel_layout_t basicFormat;

    pixel_format_t internalFormat;

    tex_filter_t minFilter;

    tex_filter_t magFilter;

    tex_wrap_t wrapS;

    tex_wrap_t wrapT;

    tex_wrap_t wrapR;

    pixel_swizzle_t swizzleR;

    pixel_swizzle_t swizzleG;

    pixel_swizzle_t swizzleB;

    pixel_swizzle_t swizzleA;

    float minLod;

    float maxLod;


  public:
    ~TextureAttrib() noexcept;

    TextureAttrib() noexcept;

    TextureAttrib(const TextureAttrib&) noexcept;

    TextureAttrib(TextureAttrib&&) noexcept;

    TextureAttrib& operator=(const TextureAttrib&) noexcept;

    TextureAttrib& operator=(TextureAttrib&&) noexcept;

    void reset_attribs() noexcept;

    bool introspect_attribs(const tex_type_t texType) noexcept;

    void set_wrap_mode(const tex_param_t wrapParam, const tex_wrap_t wrapVal) noexcept;

    tex_wrap_t get_wrap_mode(const tex_param_t wrapParam) const noexcept;

    void set_swizzle_mode(const tex_param_t swizzleParam, const pixel_swizzle_t swizzle) noexcept;

    pixel_swizzle_t get_swizzle_mode(const tex_param_t swizzleParam) const noexcept;

    void set_lod(const float minimum = -1000.f, const float maximum = 1000.f) noexcept;

    void set_min_lod(const float minimum = -1000.f) noexcept;

    void set_max_lod(const float maximum = 1000.f) noexcept;

    float get_min_lod() const noexcept;

    float get_max_lod() const noexcept;

    void set_filtering(
        const tex_filter_t minification = tex_filter_t::TEX_FILTER_NEAREST_LINEAR,
        const tex_filter_t magnification = tex_filter_t::TEX_FILTER_LINEAR
    ) noexcept;

    void set_min_filter(const tex_filter_t minification = tex_filter_t::TEX_FILTER_NEAREST_LINEAR) noexcept;

    void set_mag_filter(const tex_filter_t magnification = tex_filter_t::TEX_FILTER_LINEAR) noexcept;

    tex_filter_t get_min_filter() const noexcept;

    tex_filter_t get_mag_filter() const noexcept;

    color_type_t get_color_type() const noexcept;

    void set_internal_format(const pixel_format_t gpuFormat) noexcept;

    pixel_format_t get_internal_format() const noexcept;

    pixel_layout_t get_basic_format() const noexcept;
};



/*-----------------------------------------------------------------------------
 * Utility functions for textures.
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Introspection of Texture Integer Attributes
-------------------------------------*/
inline GLint get_texture_int(const tex_type_t texType, const tex_param_t param) noexcept
{
    GLint retVal;
    glGetTexParameteriv((GLenum)texType, (GLenum)param, &retVal);
    return retVal;
}

/*-------------------------------------
 * Introspection of Texture Boolean Attributes
-------------------------------------*/
inline bool get_texture_bool(const tex_type_t texType, const tex_param_t param) noexcept
{
    return get_texture_int(texType, param) == GL_TRUE;
}

/*-------------------------------------
 * Introspection of Texture Float Attributes
-------------------------------------*/
inline GLfloat get_texture_float(const tex_type_t texType, const tex_param_t param) noexcept
{
    GLfloat retVal;
    glGetTexParameterfv((GLenum)texType, (GLenum)param, &retVal);
    return retVal;
}
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_TEXTURE_ATTRIBS_H__ */
