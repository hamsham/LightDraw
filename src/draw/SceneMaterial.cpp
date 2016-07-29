
#include "ls/utils/Copy.h"

#include "ls/draw/SceneMaterial.h"


namespace ls {
namespace draw {



/*-------------------------------------
 * Material Reset
-------------------------------------*/
void SceneMaterial::reset() noexcept {
    utils::fast_fill(bindSlots, material_property_t::INVALID_MATERIAL_TEXTURE, LS_ARRAY_SIZE(bindSlots));
    utils::fast_memset(textures, 0, sizeof(textures));
    utils::fast_memset(samplers, 0, sizeof(samplers));
}



/*-------------------------------------
 * Material Validation
-------------------------------------*/
material_status_t SceneMaterial::validate() const noexcept {
    for (const int bindSlotA : bindSlots) {
        for (const int bindSlotB : bindSlots) {
            if (bindSlotA == bindSlotB
            && bindSlotA != (int)material_property_t::INVALID_MATERIAL_TEXTURE
            ) {
                return material_status_t::MATERIAL_STATUS_DUPLICATE_BIND_SLOTS;
            }
        }
    }
    
    for (const GLuint texA : textures) {
        for (const GLuint texB : textures) {
            if (texA == texB && texA != 0) { // OpenGL uses 0 to indicate 'a default texture
                return material_status_t::MATERIAL_STATUS_DUPLICATE_TEXTURES;
            }
        }
    }
    
    for (const GLuint samplerA : samplers) {
        for (const GLuint samplerB : samplers) {
            if (samplerA == samplerB && samplerA != 0) {
                return material_status_t::MATERIAL_STATUS_DUPLICATE_SAMPLERS;
            }
        }
    }
    
    return material_status_t::MATERIAL_STATUS_VALID;
}



/*-------------------------------------
 * Context Binding
-------------------------------------*/
void SceneMaterial::bind() const noexcept {
    for (unsigned i = 0; i < draw::active_texture_t::MAX_ACTIVE_TEXTURES; ++i) {
        if (bindSlots[i] == (int)draw::material_property_t::INVALID_MATERIAL_TEXTURE) {
            break;
        }

        glActiveTexture(bindSlots[i]);
        LS_LOG_GL_ERR();
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        LS_LOG_GL_ERR();
    }
}



/*-------------------------------------
 * Context Un-binding
-------------------------------------*/
void SceneMaterial::unbind() const noexcept {
    for (unsigned i = 0; i < draw::active_texture_t::MAX_ACTIVE_TEXTURES; ++i) {
        if (bindSlots[i] == (int)draw::material_property_t::INVALID_MATERIAL_TEXTURE) {
            break;
        }

        glActiveTexture(bindSlots[i]);
        LS_LOG_GL_ERR();
        glBindTexture(GL_TEXTURE_2D, 0);
        LS_LOG_GL_ERR();
    }

    glActiveTexture(draw::tex_slot_t::TEXTURE_SLOT_GPU_OFFSET);
    LS_LOG_GL_ERR();
}



} // end draw namespace
} // end ls namespace
