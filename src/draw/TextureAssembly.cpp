
/*
 * File:   TextureAssembly.cpp
 * Author: miles
 *
 * Created on February 29, 2016, 2:10 PM
 */

#include <utility> // std::move()

#include "ls/setup/Macros.h"

#include "ls/utils/Log.h"
#include "ls/utils/Assertions.h"

#include "ls/draw/TextureAssembly.h"
#include "ls/draw/Texture.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
TextureAssembly::~TextureAssembly() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
TextureAssembly::TextureAssembly() noexcept :
texMainType {
    tex_type_t::TEX_TYPE_2D
},
texSubType {tex_2d_type_t::TEX_SUBTYPE_2D},
mipmapLevel {0},
size {0},
attribs {}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
TextureAssembly::TextureAssembly(const TextureAssembly& a) noexcept :
texMainType {
    a.texMainType
},
texSubType {a.texSubType},
mipmapLevel {a.mipmapLevel},
size {a.size},
attribs {a.attribs}
{
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
TextureAssembly::TextureAssembly(TextureAssembly&& a) noexcept :
texMainType {
    a.texMainType
},
texSubType {a.texSubType},
mipmapLevel {a.mipmapLevel},
size {std::move(a.size)},
attribs {std::move(a.attribs)}
{
    a.clear();
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
TextureAssembly& TextureAssembly::operator =(const TextureAssembly& a) noexcept {
    texMainType = a.texMainType;
    texSubType = a.texSubType;
    mipmapLevel = a.mipmapLevel;
    size = a.size;
    attribs = a.attribs;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
TextureAssembly& TextureAssembly::operator =(TextureAssembly&& a) noexcept {
    texMainType = a.texMainType;
    texSubType = a.texSubType;
    mipmapLevel = a.mipmapLevel;
    size = std::move(a.size);
    attribs = std::move(a.attribs);

    a.clear();

    return *this;
}

/*-------------------------------------
-------------------------------------*/
void TextureAssembly::apply_attribs() const noexcept {
    glTexParameteri(texMainType, tex_param_t::TEX_PARAM_MIN_FILTER, attribs.get_min_filter());
    glTexParameteri(texMainType, tex_param_t::TEX_PARAM_MAG_FILTER, attribs.get_mag_filter());

    constexpr tex_param_t swizzles[] = {
        tex_param_t::TEX_PARAM_SWIZZLE_R,
        tex_param_t::TEX_PARAM_SWIZZLE_G,
        tex_param_t::TEX_PARAM_SWIZZLE_B,
        tex_param_t::TEX_PARAM_SWIZZLE_A
    };

    for (unsigned i = 0; i < LS_ARRAY_SIZE(swizzles); ++i) {
        glTexParameteri(texMainType, swizzles[i], attribs.get_swizzle_mode(swizzles[i]));
    }

    constexpr tex_param_t wrapModes[] = {tex_param_t::TEX_PARAM_WRAP_S, tex_param_t::TEX_PARAM_WRAP_T, tex_param_t::TEX_PARAM_WRAP_R};

    for (unsigned i = 0; i < LS_ARRAY_SIZE(wrapModes); ++i) {
        glTexParameteri(texMainType, wrapModes[i], attribs.get_wrap_mode(wrapModes[i]));
    }

    glTexParameterf(texMainType, tex_param_t::TEX_PARAM_MIN_LOD, attribs.get_min_lod());
    glTexParameterf(texMainType, tex_param_t::TEX_PARAM_MAX_LOD, attribs.get_max_lod());
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_attribs(const TextureAttrib& a) noexcept {
    attribs = a;
    return true;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_int_attrib(const tex_param_t param, const int attribVal) noexcept {
    LS_LOG_MSG("Setting texture integer parameter: ", param);
    bool ret = true;

    if (param == tex_param_t::TEX_PARAM_MIN_FILTER) {
        if (attribVal != tex_filter_t::TEX_FILTER_NEAREST
            && attribVal != tex_filter_t::TEX_FILTER_LINEAR
            && attribVal != tex_filter_t::TEX_FILTER_NEAREST_NEAREST
            && attribVal != tex_filter_t::TEX_FILTER_NEAREST_LINEAR
            && attribVal != tex_filter_t::TEX_FILTER_LINEAR_NEAREST
            && attribVal != tex_filter_t::TEX_FILTER_LINEAR_LINEAR
        ) {
            LS_LOG_ERR("\tUnknown texture minification filter: ", attribVal);
            ret = false;
        }
        else {
            attribs.set_min_filter((tex_filter_t)attribVal);
        }
    }
    else if (param == tex_param_t::TEX_PARAM_MAG_FILTER) {
        if (attribVal != tex_filter_t::TEX_FILTER_NEAREST
            && attribVal != tex_filter_t::TEX_FILTER_LINEAR
        ) {
            LS_LOG_ERR("\tUnknown texture magnification filter: ", attribVal);
            ret = false;
        }
        else {
            attribs.set_mag_filter((tex_filter_t)attribVal);
        }
    }
    else if (param == tex_param_t::TEX_PARAM_SWIZZLE_R
        || param == tex_param_t::TEX_PARAM_SWIZZLE_G
        || param == tex_param_t::TEX_PARAM_SWIZZLE_B
        || param == tex_param_t::TEX_PARAM_SWIZZLE_A
        ) {
        if (attribVal != pixel_swizzle_t::SWIZZLE_RED
            && attribVal != pixel_swizzle_t::SWIZZLE_GREEN
            && attribVal != pixel_swizzle_t::SWIZZLE_BLUE
            && attribVal != pixel_swizzle_t::SWIZZLE_ALPHA
            && attribVal != pixel_swizzle_t::SWIZZLE_ZERO
            && attribVal != pixel_swizzle_t::SWIZZLE_ONE
        ) {
            LS_LOG_ERR("\tUnknown texture swizzle filter: ", attribVal);
            ret = false;
        }
        else {
            attribs.set_swizzle_mode(param, (pixel_swizzle_t)attribVal);
        }
    }
    else if (param == tex_param_t::TEX_PARAM_WRAP_S
        || param == tex_param_t::TEX_PARAM_WRAP_T
        || param == tex_param_t::TEX_PARAM_WRAP_R
    ) {
        if (attribVal != tex_wrap_t::TEX_WRAP_CLAMP
            && attribVal != tex_wrap_t::TEX_WRAP_MIRROR_REPEAT
            && attribVal != tex_wrap_t::TEX_WRAP_REPEAT
        ) {
            LS_LOG_ERR("\tUnknown texture wrap filter: ", attribVal);
            ret = false;
        }
        else {
            attribs.set_wrap_mode(param, (tex_wrap_t)attribVal);
        }
    }
    else {
        LS_LOG_ERR("\tUnknown texture integer parameter: ", param);
        ret = false;
    }

    if (!ret) {
        LS_LOG_ERR("\tFailed.\n");
    }
    else {
        LS_LOG_MSG("\tDone.\n");
    }

    LS_DEBUG_ASSERT(ret == true);

    return ret;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_float_attrib(const tex_param_t param, const float attribVal) noexcept {
    LS_LOG_MSG("Setting texture float parameter: ", param);
    bool ret = true;

    LS_DEBUG_ASSERT((param == TEX_PARAM_MIN_LOD) || (param == TEX_PARAM_MIN_LOD));

    switch (param) {
        case TEX_PARAM_MIN_LOD:
            attribs.set_min_lod(attribVal);
            break;

        case TEX_PARAM_MAX_LOD:
            attribs.set_max_lod(attribVal);
            break;

        default:
            LS_LOG_ERR("\tUnknown texture float parameter: ", param);
            ret = false;
            break;
    }

    if (!ret) {
        LS_LOG_ERR("\tFailed.\n");
    }
    else {
        LS_LOG_MSG("\tDone.\n");
    }

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_mipmap_attrib(const unsigned level) noexcept {
    mipmapLevel = level;
    return true;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_format_attrib(const pixel_format_t internalFormat) noexcept {
    attribs.set_internal_format(internalFormat);

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_size_attrib(
    const math::vec2i& texSize,
    const tex_type_t texType,
    const tex_2d_type_t subType
    ) noexcept {
    size[0] = texSize[0];
    size[1] = texSize[1];
    size[2] = 1;

    texMainType = texType;
    texSubType = subType;

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::set_size_attrib(
    const math::vec3i& texSize,
    const tex_type_t texType,
    const tex_3d_type_t subType
    ) noexcept {
    size = texSize;

    texMainType = texType;
    texSubType = subType;

    return true;
}

/*-------------------------------------
-------------------------------------*/
void TextureAssembly::clear() noexcept {
    texMainType = tex_type_t::TEX_TYPE_2D;
    texSubType = tex_2d_type_t::TEX_SUBTYPE_2D;
    mipmapLevel = 0;
    size = math::vec3i {0};
    attribs.reset_attribs();
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::is_assembly_valid() const noexcept {
    LS_LOG_MSG("Validating a texture assembly");

    if (texMainType == tex_type_t::TEX_TYPE_2D) {
        LS_LOG_MSG("\tVerifying 2D texture parameters.");

        if (texSubType != tex_2d_type_t::TEX_SUBTYPE_2D) {
            LS_LOG_ERR("\t\tMismatched 2D Texture subtype!\n");
            return false;
        }

        LS_LOG_MSG("\t\tDone.");
    }
    else if (texMainType == tex_type_t::TEX_TYPE_CUBE) {
        LS_LOG_MSG("\tVerifying Cube texture parameters.");

        if (texSubType != tex_2d_type_t::TEX_SUBTYPE_CUBE_RIGHT
            || texSubType != tex_2d_type_t::TEX_SUBTYPE_CUBE_TOP
            || texSubType != tex_2d_type_t::TEX_SUBTYPE_CUBE_BACK
            || texSubType != tex_2d_type_t::TEX_SUBTYPE_CUBE_LEFT
            || texSubType != tex_2d_type_t::TEX_SUBTYPE_CUBE_BOTTOM
            || texSubType != tex_2d_type_t::TEX_SUBTYPE_CUBE_FRONT
        ) {
            LS_LOG_ERR("\t\tMismatched Cube Texture subtype!\n");
            return false;
        }

        LS_LOG_MSG("\t\tDone.");
    }
    else if (texMainType == tex_type_t::TEX_TYPE_3D) {
        LS_LOG_MSG("\tVerifying 3D texture parameters.");

        if (texSubType != tex_3d_type_t::TEX_SUBTYPE_3D) {
            LS_LOG_ERR("\t\tMismatched 3D Texture subtype!\n");
            return false;
        }

        LS_LOG_MSG("\t\tDone.");
    }
    else if (texMainType == tex_type_t::TEX_TYPE_2D_ARRAY) {
        LS_LOG_MSG("\tVerifying 2D Array texture parameters.");

        if (texSubType != tex_3d_type_t::TEX_SUBTYPE_2D_ARRAY) {
            LS_LOG_ERR("\t\tMismatched 2D Array Texture subtype!\n");
            return false;
        }

        LS_LOG_MSG("\t\tDone.");
    }
    else {
        LS_LOG_ERR("\tUnknown texture type being assembled.\n");
        return false;
    }

    // Validate individual attribs
    const float minLod = attribs.get_min_lod();
    const float maxLod = attribs.get_max_lod();
    LS_LOG_MSG("\tValidating texture LOD parameters.");
    if (minLod >= maxLod) {
        LS_LOG_ERR("\t\tInvalid texture LOD values: ", minLod, " >= ", maxLod, "!\n");
        return false;
    }
    LS_LOG_MSG("\t\tDone. Successfully validated a texture assembly.\n");

    return true;
}

/*-------------------------------------
-------------------------------------*/
bool TextureAssembly::assemble(Texture& tex, const void* const pData) const noexcept {
    if (!is_assembly_valid()) {
        return false;
    }

    LS_LOG_MSG("Attempting to assemble a texture object.");
    GLuint gpuId = 0;

    if (!tex.gpu_id()) {
        LS_LOG_MSG("\tGenerating a handle to a new texture object on the GPU.");
        glGenTextures(1, &gpuId);
        LS_LOG_GL_ERR();

        if (!gpuId) {
            LS_LOG_ERR("\tFailed to generate a texture object on the GPU.");
            return false;
        }

        LS_LOG_MSG("\t\tDone. Successfully generated a texture on the GPU: ", gpuId);
    }
    else {
        gpuId = tex.gpu_id();
        LS_LOG_MSG("\tAssembling data for a preexisting texture: ", gpuId);
    }

    LS_LOG_MSG("\tAllocating space for texture data using texture ", gpuId, '.');
    glBindTexture(texMainType, gpuId);

    if (texMainType == tex_type_t::TEX_TYPE_2D || texMainType == tex_type_t::TEX_TYPE_CUBE) {
        glTexImage2D(texSubType, mipmapLevel, attribs.get_internal_format(), size[0], size[1], 0, attribs.get_basic_format(), attribs.get_color_type(), pData);
    }
    else {
        glTexImage3D(texSubType, mipmapLevel, attribs.get_internal_format(), size[0], size[1], size[2], 0, attribs.get_basic_format(), attribs.get_color_type(), pData);
    }
    LS_LOG_GL_ERR();
    LS_LOG_MSG("\t\tDone. Allocated storage for texture ", gpuId, '.');

    LS_LOG_MSG("\tApplying attributes.");
    apply_attribs();

    tex.gpuId = gpuId;
    tex.texType = texMainType;
    tex.size = size;
    tex.attribs = attribs;
    LS_LOG_MSG("\t\tDone. Successfully applied attributes to texture object ", gpuId, '.');

    LS_LOG_MSG(
        "\tSuccessfully assembled a texture object:",
        "\n\t\tGPU ID:        ", tex.gpuId,
        "\n\t\tTexture Type:  ", tex.texType,
        "\n\t\tPixel Format:  ", tex.attribs.get_internal_format(),
        "\n\t\tDimensions:    ", tex.size[0], " x ", tex.size[1], " x ", tex.size[2],
        "\n"
        );

    glBindTexture(texMainType, 0);

    return true;
}

} // end draw namespace
} // end ls namepace
