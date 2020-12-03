
/*
 * File:   FBOAttrib.cpp
 * Author: miles
 *
 * Created on March 4, 2016, 4:40 PM
 */

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/FBOAttrib.h"
#include "lightsky/draw/Texture.h"
#include "lightsky/draw/RenderBuffer.h"



namespace ls
{
namespace draw
{

/*-------------------------------------
 * Destructor
-------------------------------------*/
FBOAttrib::~FBOAttrib() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
FBOAttrib::FBOAttrib() noexcept
{
    reset_attribs();
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
FBOAttrib::FBOAttrib(const FBOAttrib& a) noexcept
{
    *this = a;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
FBOAttrib::FBOAttrib(FBOAttrib&& a) noexcept
{
    *this = std::move(a);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
FBOAttrib& FBOAttrib::operator=(const FBOAttrib& a) noexcept
{
    targetType = a.targetType;
    attachType = a.attachType;
    texType = a.texType;
    texLevel = a.texLevel;
    texLayer = a.texLayer;
    pTargetData = a.pTargetData;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
FBOAttrib& FBOAttrib::operator=(FBOAttrib&& a) noexcept
{
    *this = a;
    a.reset_attribs();

    return *this;
}

/*-------------------------------------
 * Attrib Reset
-------------------------------------*/
void FBOAttrib::reset_attribs() noexcept
{
    targetType = fbo_target_t::FBO_TARGET_INVALID;
    attachType = fbo_attach_t::FBO_ATTACHMENT_INVALID;
    texType = fbo_texture_t::FBO_2D_TEXTURE;
    texLevel = 0;
    texLayer = 0;
    pTargetData = nullptr;
}

/*-------------------------------------
 * Set the current target type
-------------------------------------*/
void FBOAttrib::set_target_type(
    Texture& t,
    const fbo_texture_t type,
    const unsigned mipmapLevel,
    const unsigned arrayLayer
) noexcept
{
    if (t.get_texture_type() == tex_type_t::TEX_TYPE_3D || t.get_texture_type() == tex_type_t::TEX_TYPE_2D_ARRAY)
    {
        if (type != fbo_texture_t::FBO_3D_TEXTURE || (arrayLayer && (int)arrayLayer < t.get_size()[2]))
        {
            LS_DEBUG_ASSERT(false);
            reset_attribs();
            return;
        }
    }
    else if (t.get_texture_type() == tex_type_t::TEX_TYPE_CUBE)
    {
        switch (type)
        {
            case fbo_texture_t::FBO_POS_X_TEXTURE:
            case fbo_texture_t::FBO_NEG_X_TEXTURE:
            case fbo_texture_t::FBO_POS_Y_TEXTURE:
            case fbo_texture_t::FBO_NEG_Y_TEXTURE:
            case fbo_texture_t::FBO_POS_Z_TEXTURE:
            case fbo_texture_t::FBO_NEG_Z_TEXTURE:
                break;

            default:
                LS_DEBUG_ASSERT(false);
                reset_attribs();
                return;
        }
    }

    targetType = fbo_target_t::FBO_TARGET_TEXTURE;
    texType = type;
    texLevel = mipmapLevel;
    texLayer = arrayLayer;
    pTargetData = &t;
}

/*-------------------------------------
 * Set the current target type
-------------------------------------*/
void FBOAttrib::set_target_type(RenderBuffer& r) noexcept
{
    targetType = fbo_target_t::FBO_TARGET_RENDERBUFFER;
    texType = fbo_texture_t::FBO_2D_TEXTURE;
    texLevel = 0;
    texLayer = 0;
    pTargetData = &r;
}

/*-------------------------------------
 * Retrieve a texture object if one exists.
-------------------------------------*/
Texture* FBOAttrib::get_texture() const noexcept
{
    if (targetType != fbo_target_t::FBO_TARGET_TEXTURE)
    {
        return nullptr;
    }

    return reinterpret_cast<Texture*>(pTargetData);
}

/*-------------------------------------
 * Retrieve a render buffer object if one exists.
-------------------------------------*/
RenderBuffer* FBOAttrib::get_renderbuffer() const noexcept
{
    if (targetType != fbo_target_t::FBO_TARGET_RENDERBUFFER)
    {
        return nullptr;
    }

    return reinterpret_cast<RenderBuffer*>(pTargetData);
}
} // end draw namespace
} // end ls namespace
