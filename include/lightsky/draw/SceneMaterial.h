
/*
 * File:   Material.h
 * Author: miles
 *
 * Created on March 9, 2016, 9:50 AM
 */

#ifndef __LS_DRAW_MATERIAL_H__
#define __LS_DRAW_MATERIAL_H__

#include "lightsky/draw/Setup.h" // GLuint
#include "lightsky/draw/BlendObject.h" // for materials with blending
#include "lightsky/draw/Color.h" // Material coloring
#include "lightsky/draw/ShaderAttrib.h" // shader_limits_t
#include "lightsky/draw/Texture.h" // active_texture_t


namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * Materials Properties
-----------------------------------------------------------------------------*/
enum material_property_t : unsigned {
    INVALID_MATERIAL = (unsigned)-1,
    INVALID_MATERIAL_TEXTURE = (unsigned)-1
};



/**----------------------------------------------------------------------------
 * Material Validation
-----------------------------------------------------------------------------*/
enum class material_status_t {
    MATERIAL_STATUS_VALID,
    MATERIAL_STATUS_DUPLICATE_BIND_SLOTS,
    MATERIAL_STATUS_DUPLICATE_TEXTURES,
    MATERIAL_STATUS_DUPLICATE_SAMPLERS
};


/**----------------------------------------------------------------------------
 * Materials which can be applied to a mesh upon rendering
-----------------------------------------------------------------------------*/
struct SceneMaterial {
    /**
     * BindSlots contains the texture binding slot of a texture. More than one
     * active binding slot means that the shader *this material is bound to can
     * use multi-texturing.
     */
    int bindSlots[active_texture_t::MAX_ACTIVE_TEXTURES];
    
    /**
     * Textures contains non-owning references to the OpenGL texture objects
     * which *this material uses.
     */
    GLuint textures[active_texture_t::MAX_ACTIVE_TEXTURES];
    
    /**
     * This member contains non-owning references to the OpenGL Sampler objects
     * which sample the textures referenced in the 'textures' member.
     */
    GLuint samplers[active_texture_t::MAX_ACTIVE_TEXTURES];
    
    /**
     * @brief The reset() function will set all bind-slots to their default
     * values.
     * 
     * Using this method is preferred over having members initialized on
     * construction to void the run-time cost of initializing multiple objects.
     * 
     * All values within 'bindSLots' are set to 'INVALID_MATERIAL_TEXTURE' and
     * all members of the 'textures' and 'samplers' arrays are set to 0.
     */
    void reset() noexcept;
    
    /**
     * Validate and retrieve any errors currently found with *this material.
     * 
     * This method will look for duplicate textures, samplers, and texture bind
     * slots.
     * 
     * @return material_status_t::MATERIAL_STATUS_VALID if *this represents a valid
     * material, otherwise an error from the material_status_t will be
     * returned.
     */
    material_status_t validate() const noexcept;
    
    /**
     * Bind all texture data in *this to the current rendering context.
     */
    void bind() const noexcept;
    
    /**
     * Remove all texture bindings in *this from the current rendering context.
     */
    void unbind() const noexcept;
};



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_MATERIAL_H__ */
