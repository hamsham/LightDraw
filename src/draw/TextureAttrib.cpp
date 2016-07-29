
/*
 * File:   TextureAttribs.cpp
 * Author: Miles
 *
 * Created on February 22, 2016, 5:22 PM
 */

#include <algorithm>

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/TextureAttrib.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
TextureAttrib::~TextureAttrib() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
TextureAttrib::TextureAttrib() noexcept {
    reset_attribs();
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
TextureAttrib::TextureAttrib(const TextureAttrib& a) noexcept {
    *this = a;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
TextureAttrib::TextureAttrib(TextureAttrib&& a) noexcept {
    *this = std::move(a);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
TextureAttrib& TextureAttrib::operator =(const TextureAttrib& a) noexcept {
    colorType = a.colorType;
    basicFormat = a.basicFormat;
    internalFormat = a.internalFormat;

    minFilter = a.minFilter;
    magFilter = a.magFilter;

    wrapS = a.wrapS;
    wrapT = a.wrapT;
    wrapR = a.wrapR;

    swizzleR = a.swizzleR;
    swizzleG = a.swizzleG;
    swizzleB = a.swizzleB;
    swizzleA = a.swizzleA;

    minLod = a.minLod;
    maxLod = a.maxLod;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
TextureAttrib& TextureAttrib::operator =(TextureAttrib&& a) noexcept {
    *this = a;
    a.reset_attribs();

    return *this;
}

/*-------------------------------------
 * Reset all attributes to their OpenGL defaults
-------------------------------------*/
void TextureAttrib::reset_attribs() noexcept {
    colorType = color_type_t::COLOR_TYPE_DEFAULT;
    basicFormat = pixel_layout_t::COLOR_LAYOUT_DEFAULT_RGB;
    internalFormat = pixel_format_t::COLOR_FMT_DEFAULT_RGB;

    minFilter = tex_filter_t::TEX_FILTER_NEAREST_LINEAR;
    magFilter = tex_filter_t::TEX_FILTER_LINEAR;

    wrapS = tex_wrap_t::TEX_WRAP_REPEAT;
    wrapT = tex_wrap_t::TEX_WRAP_REPEAT;
    wrapR = tex_wrap_t::TEX_WRAP_REPEAT;

    swizzleR = pixel_swizzle_t::SWIZZLE_RED;
    swizzleG = pixel_swizzle_t::SWIZZLE_GREEN;
    swizzleB = pixel_swizzle_t::SWIZZLE_BLUE;
    swizzleA = pixel_swizzle_t::SWIZZLE_ALPHA;

    minLod = -1000;
    maxLod = 1000;
}

/*-------------------------------------
 * Introspection (most) of a Texture Attributes
-------------------------------------*/
bool TextureAttrib::introspect_attribs(const tex_type_t texType) noexcept {
    // TODO: Figure out how to retrieve the color type, pixel layout, and
    // pixel format of a texture.

    minFilter = (tex_filter_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_MIN_FILTER);
    minFilter = (tex_filter_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_MAG_FILTER);

    wrapS = (tex_wrap_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_WRAP_S);
    wrapR = (tex_wrap_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_WRAP_R);
    wrapT = (tex_wrap_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_WRAP_T);

    swizzleR = (pixel_swizzle_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_SWIZZLE_R);
    swizzleG = (pixel_swizzle_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_SWIZZLE_G);
    swizzleB = (pixel_swizzle_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_SWIZZLE_B);
    swizzleA = (pixel_swizzle_t)draw::get_texture_int(texType, tex_param_t::TEX_PARAM_SWIZZLE_A);

    minLod = draw::get_texture_float(texType, tex_param_t::TEX_PARAM_MIN_LOD);
    maxLod = draw::get_texture_float(texType, tex_param_t::TEX_PARAM_MAX_LOD);

    return true;
}

/*-------------------------------------
 * Set the texture's clamping/wrapping mode
-------------------------------------*/
void TextureAttrib::set_wrap_mode(const tex_param_t wrapParam, const tex_wrap_t wrapVal) noexcept {
    LS_DEBUG_ASSERT(
        (wrapParam == tex_param_t::TEX_PARAM_WRAP_S) ||
        (wrapParam == tex_param_t::TEX_PARAM_WRAP_T) ||
        (wrapParam == tex_param_t::TEX_PARAM_WRAP_R)
        );

    if (wrapParam == tex_param_t::TEX_PARAM_WRAP_S) {
        wrapS = wrapVal;
    }
    else if (wrapParam == tex_param_t::TEX_PARAM_WRAP_T) {
        wrapT = wrapVal;
    }
    else {
        wrapR = wrapVal;
    }
}

/*-------------------------------------
 * Retrieve the texture's clamping/wrapping mode
-------------------------------------*/
tex_wrap_t TextureAttrib::get_wrap_mode(const tex_param_t wrapParam) const noexcept {
    LS_DEBUG_ASSERT(
        (wrapParam == tex_param_t::TEX_PARAM_WRAP_S) ||
        (wrapParam == tex_param_t::TEX_PARAM_WRAP_T) ||
        (wrapParam == tex_param_t::TEX_PARAM_WRAP_R)
        );

    if (wrapParam == tex_param_t::TEX_PARAM_WRAP_S) {
        return wrapS;
    }
    else if (wrapParam == tex_param_t::TEX_PARAM_WRAP_T) {
        return wrapT;
    }

    return wrapR;
}

/*-------------------------------------
 * Set a particular swizzle for a texture
-------------------------------------*/
void TextureAttrib::set_swizzle_mode(const tex_param_t swizzleParam, const pixel_swizzle_t swizzle) noexcept {
    LS_DEBUG_ASSERT(
        (swizzleParam == tex_param_t::TEX_PARAM_SWIZZLE_R) ||
        (swizzleParam == tex_param_t::TEX_PARAM_SWIZZLE_G) ||
        (swizzleParam == tex_param_t::TEX_PARAM_SWIZZLE_B) ||
        (swizzleParam == tex_param_t::TEX_PARAM_SWIZZLE_A)
        );

    switch (swizzleParam) {
        case tex_param_t::TEX_PARAM_SWIZZLE_R:
            swizzleR = swizzle;
            break;

        case tex_param_t::TEX_PARAM_SWIZZLE_G:
            swizzleG = swizzle;
            break;

        case tex_param_t::TEX_PARAM_SWIZZLE_B:
            swizzleB = swizzle;
            break;

        case tex_param_t::TEX_PARAM_SWIZZLE_A:
            swizzleA = swizzle;
            break;

        default:
            break;
    }
}

/*-------------------------------------
 * Retrieve a particular swizzle for a texture
-------------------------------------*/
pixel_swizzle_t TextureAttrib::get_swizzle_mode(const tex_param_t swizzle) const noexcept {
    switch (swizzle) {
        case tex_param_t::TEX_PARAM_SWIZZLE_R:
            return swizzleR;

        case tex_param_t::TEX_PARAM_SWIZZLE_G:
            return swizzleG;

        case tex_param_t::TEX_PARAM_SWIZZLE_B:
            return swizzleB;

        case tex_param_t::TEX_PARAM_SWIZZLE_A:
            return swizzleA;

        default:
            break;
    }

    LS_DEBUG_ASSERT(
        (swizzle == tex_param_t::TEX_PARAM_SWIZZLE_R) ||
        (swizzle == tex_param_t::TEX_PARAM_SWIZZLE_G) ||
        (swizzle == tex_param_t::TEX_PARAM_SWIZZLE_B) ||
        (swizzle == tex_param_t::TEX_PARAM_SWIZZLE_A)
        );

    return (pixel_swizzle_t)tex_param_t::TEX_PARAM_INVALID;
}

/*-------------------------------------
 * Set the maximum & minimum mipmapping LOD bias
-------------------------------------*/
void TextureAttrib::set_lod(const float minimum, const float maximum) noexcept {
    LS_DEBUG_ASSERT(minimum < maximum);
    set_min_lod(minimum);
    set_max_lod(maximum);
}

/*-------------------------------------
 * Set the maximum mipmapping LOD bias
-------------------------------------*/
void TextureAttrib::set_min_lod(const float minimum) noexcept {
    minLod = minimum;
}

/*-------------------------------------
 * Set the maximum mipmapping LOD bias
-------------------------------------*/
void TextureAttrib::set_max_lod(const float maximum) noexcept {
    maxLod = maximum;
}

/*-------------------------------------
 * Get the minimum mipmapping LOD bias
-------------------------------------*/
float TextureAttrib::get_min_lod() const noexcept {
    return minLod;
}

/*-------------------------------------
 * Get the maximum mipmapping LOD bias
-------------------------------------*/
float TextureAttrib::get_max_lod() const noexcept {
    return maxLod;
}

/*-------------------------------------
 * Set the texture's filtering modes
-------------------------------------*/
void TextureAttrib::set_filtering(const tex_filter_t minification, const tex_filter_t magnification) noexcept {
    set_min_filter(minification);
    set_mag_filter(magnification);
}

/*-------------------------------------
 * Set a texture's minification mode
-------------------------------------*/
void TextureAttrib::set_min_filter(const tex_filter_t minification) noexcept {
    minFilter = minification;
}

/*-------------------------------------
 * Set a texture's magnification mode
-------------------------------------*/
void TextureAttrib::set_mag_filter(const tex_filter_t magnification) noexcept {
    magFilter = magnification;
}

/*-------------------------------------
 * Get the minification type
-------------------------------------*/
tex_filter_t TextureAttrib::get_min_filter() const noexcept {
    return minFilter;
}

/*-------------------------------------
 * Get the magnification type
-------------------------------------*/
tex_filter_t TextureAttrib::get_mag_filter() const noexcept {
    return magFilter;
}

/*-------------------------------------
 * Retrieve the coloring information of a texture's pixels
-------------------------------------*/
color_type_t TextureAttrib::get_color_type() const noexcept {
    return colorType;
}

/*-------------------------------------
 * Set the data format of a texture's pixels
-------------------------------------*/
void TextureAttrib::set_internal_format(const pixel_format_t gpuFormat) noexcept {
    internalFormat = gpuFormat;
    basicFormat = draw::get_color_layout(internalFormat);
    colorType = draw::get_color_type(internalFormat);
}

/*-------------------------------------
 * Retrieve the data format of a texture's pixels
-------------------------------------*/
pixel_format_t TextureAttrib::get_internal_format() const noexcept {
    return internalFormat;
}

/*-------------------------------------
 * Retrieve the layout of a texture's pixels.
-------------------------------------*/
pixel_layout_t TextureAttrib::get_basic_format() const noexcept {
    return basicFormat;
}



} // end draw namespace
} // end ls namespace
