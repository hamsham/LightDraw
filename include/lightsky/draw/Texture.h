/*
 * File:   draw/texture.h
 * Author: Miles Lacey
 *
 * Created on January 27, 2014, 8:35 PM
 */

#ifndef __LS_DRAW_TEXTURE_H__
#define __LS_DRAW_TEXTURE_H__

#include "ls/utils/Assertions.h"

#include "ls/draw/Setup.h"
#include "ls/draw/Color.h"
#include "ls/draw/GLQuery.h"
#include "ls/draw/ImageResource.h"
#include "ls/draw/TextureAttrib.h"

namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class TextureAssembly;
class PixelBuffer;



/*-----------------------------------------------------------------------------
 * Texture Bindings
-----------------------------------------------------------------------------*/
enum active_texture_t {
    ACTIVE_TEXURE0 = 0x01,
    ACTIVE_TEXURE1 = 0x02,
    ACTIVE_TEXURE2 = 0x04,
    ACTIVE_TEXURE3 = 0x08,
    ACTIVE_TEXURE4 = 0x10,
    ACTIVE_TEXURE5 = 0x20,
    ACTIVE_TEXURE6 = 0x40,
    ACTIVE_TEXURE7 = 0x80,

    ACTIVE_TEXURE_NONE = 0x00,

    MAX_ACTIVE_TEXTURES = 8
};



/**----------------------------------------------------------------------------
 * Texture Objects
-----------------------------------------------------------------------------*/
class Texture {
    friend class TextureAssembly;

  private:
    /**
     * The handle to the current texture used by OpenGL.
     */
    unsigned gpuId;

    /**
     * Enumeration to determine what type of texture this is.
     */
    tex_type_t texType;

    /**
     * Member to hold the width, height, and depth of a texture stored on the
     * GPU.
     */
    math::vec3i size;

    /**
     * CPU-Side descriptors of GPU texture parameters.
     */
    TextureAttrib attribs;

  public:
    /**
     * @brief Destructor
     *
     * Releases all CPU-side memory referenced by texId. The destructor does
     * not release any GPU-side memory unless a call to "this->terminate()" has
     * been called.
     */
    ~Texture() noexcept;

    /**
     * @brief Constructor
     */
    Texture() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this. No GPU-side copies
     * are performed.
     *
     * @param tex
     * A constant reference to a Texture object who's data is to be copied
     * into *this.
     */
    Texture(const Texture&) noexcept;

    /**
     * @brief Move Constructor
     *
     * Reassigns the texture ID at *this to the one referenced by the
     * source operand. Resets the source operand to 0.
     *
     * @param t
     * An r-value reference to another texture object.
     */
    Texture(Texture&& t) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this. No GPU-side copies
     * are performed.
     *
     * @param tex
     * A constant reference to a Texture object who's data is to be copied
     * into *this.
     *
     * @return A reference to *this.
     */
    Texture& operator=(const Texture&) noexcept;

    /**
     * @brief Move Operator
     *
     * Reassigns the texture ID at *this to the one referenced by the
     * source operand. Resets the source operand to 0.
     *
     * @param t
     * An r-value reference to another texture object.
     *
     * @return A reference to *this.
     */
    Texture& operator=(Texture&& t) noexcept;

    /**
     * Get the GPU-Assigned ID used by *this.
     *
     * @return An unsigned integral type that correlates to a texture on
     * the GPU.
     */
    unsigned gpu_id() const noexcept;

    /**
     * @brief Determine if *this is able to be used for rendering.
     *
     * @return TRUE if *this contains a valid texture ID, FALSE if not.
     */
    bool is_valid() const noexcept;

    /**
     * Bind the current texture to OpenGL
     */
    void bind() const noexcept;

    /**
     * Unbind the current texture to OpenGL
     */
    void unbind() const noexcept;

    /**
     * Retrieve all current attributes to *this.
     *
     * @return A constant reference to a constant TextureAttrib object,
     * containing cached values of the current texture object's attributes.
     */
    const TextureAttrib& get_attribs() const noexcept;

    /**
     * Retrieve the current texture's size.
     *
     * @return A constant reference to a 3D integer vector containing the
     * dimensions of the current texture object.
     */
    const math::vec3i& get_size() const noexcept;

    /**
     * Modify the internal data of a texture.
     *
     * @param type
     * A value from either the tex_2d_type_t or tex_3d_type_t enumeration which
     * will tell OpenGL what type of data is held within a subset of *this
     * texture object.
     *
     * @param offset
     * A linear offset into the current texture being modified.
     *
     * @param modifySize
     * The number of bytes which are to be modified.
     *
     * @param pData
     * A constant pointer to a data store which contains data that's to be
     * copied into *this texture.
     *
     * @param level
     * Determines the mipmap level to be modified/retrieved.
     */
    void modify(
        const GLenum type,
        const int offset,
        const int modifySize,
        const void* const data,
        const int level = 0
        ) noexcept;

    /**
     * Modify the internal data of a texture using a PBO.
     *
     * @param type
     * A value from either the tex_2d_type_t or tex_3d_type_t enumeration which
     * will tell OpenGL what type of data is held within a subset of *this
     * texture object.
     *
     * @param offset
     * A linear offset into the current texture being modified.
     *
     * @param modifySize
     * The number of bytes which are to be modified.
     *
     * @param pbo
     * A constant reference to a currently bound pixel buffer object which
     * contains data that's to be copied into/out of *this texture.
     *
     * @param level
     * Determines the mipmap level to be modified/retrieved.
     */
    void modify(
        const GLenum type,
        const int offset,
        const int modifySize,
        const PixelBuffer& pbo,
        const int level = 0
        ) noexcept;

    /**
     * Modify the internal data of a texture.
     *
     * @param type
     * A value from either the tex_2d_type_t or tex_3d_type_t enumeration which
     * will tell OpenGL what type of data is held within a subset of *this
     * texture object.
     *
     * @param offset
     * A linear 2D offset into the current texture being modified.
     *
     * @param modifySize
     * The number of pixels in two dimensions which are to be modified.
     *
     * @param pData
     * A constant pointer to a data store which contains data that's to be
     * copied into *this texture.
     *
     * @param level
     * Determines the mipmap level to be modified/retrieved.
     */
    void modify(
        const tex_2d_type_t type,
        const math::vec2i& offset,
        const math::vec2i& modifySize,
        const void* const data,
        const int level = 0
        ) noexcept;

    /**
     * Modify the internal data of a texture.
     *
     * @param type
     * A value from either the tex_2d_type_t or tex_3d_type_t enumeration which
     * will tell OpenGL what type of data is held within a subset of *this
     * texture object.
     *
     * @param offset
     * A linear 2D offset into the current texture being modified.
     *
     * @param modifySize
     * The number of pixels in two dimensions which are to be modified.
     *
     * @param pbo
     * A constant reference to a currently bound pixel buffer object which
     * contains data that's to be copied into/out of *this texture.
     *
     * @param level
     * Determines the mipmap level to be modified/retrieved.
     */
    void modify(
        const tex_2d_type_t type,
        const math::vec2i& offset,
        const math::vec2i& modifySize,
        const PixelBuffer& pbo,
        const int level = 0
        ) noexcept;

    /**
     * Modify the internal data of a texture.
     *
     * @param type
     * A value from either the tex_2d_type_t or tex_3d_type_t enumeration which
     * will tell OpenGL what type of data is held within a subset of *this
     * texture object.
     *
     * @param offset
     * A linear 3D offset into the current texture being modified.
     *
     * @param modifySize
     * The number of pixels in three dimensions which are to be modified.
     *
     * @param pData
     * A constant pointer to a data store which contains data that's to be
     * copied into *this texture.
     *
     * @param level
     * Determines the mipmap level to be modified/retrieved.
     */
    void modify(
        const tex_3d_type_t type,
        const math::vec3i& offset,
        const math::vec3i& modifySize,
        const void* const data,
        const int level = 0
        ) noexcept;

    /**
     * Modify the internal data of a texture.
     *
     * @param type
     * A value from either the tex_2d_type_t or tex_3d_type_t enumeration which
     * will tell OpenGL what type of data is held within a subset of *this
     * texture object.
     *
     * @param offset
     * A linear 3D offset into the current texture being modified.
     *
     * @param modifySize
     * The number of pixels in three dimensions which are to be modified.
     *
     * @param pbo
     * A constant reference to a currently bound pixel buffer object which
     * contains data that's to be copied into/out of *this texture.
     *
     * @param level
     * Determines the mipmap level to be modified/retrieved.
     */
    void modify(
        const tex_3d_type_t type,
        const math::vec3i& offset,
        const math::vec3i& modifySize,
        const PixelBuffer& pbo,
        const int level = 0
        ) noexcept;

    /**
     * Release all memory referenced by *this.
     */
    void terminate() noexcept;

    /**
     * Get the texture type of that this texture uses in OpenGL.
     *
     * @return A value from the tex_type_t enumeration which determines the
     * main 2D or 3D texture type describing *this.
     */
    tex_type_t get_texture_type() const noexcept;
};

/*-----------------------------------------------------------------------------
 * Texture Object member functions
-----------------------------------------------------------------------------*/

/*-------------------------------------
    Get the GPU-Assigned ID used by *this.
-------------------------------------*/
inline unsigned Texture::gpu_id() const noexcept {
    return gpuId;
}

/*-------------------------------------
 Determine if *this can be used during rendering operations.
-------------------------------------*/
inline bool Texture::is_valid() const noexcept {
    return gpu_id() != 0;
}

/*-------------------------------------
    Bind the current texture to OpenGL
-------------------------------------*/
inline void Texture::bind() const noexcept {
    glBindTexture(texType, gpu_id());
}

/*-------------------------------------
    Unbind the current texture to OpenGL
-------------------------------------*/
inline void Texture::unbind() const noexcept {
    glBindTexture(texType, 0);
}

/*-------------------------------------
 Get the current texture's attribtues
-------------------------------------*/
inline const TextureAttrib& Texture::get_attribs() const noexcept {
    return attribs;
}

/*-------------------------------------
 Get the current texture's size
-------------------------------------*/
inline const math::vec3i& Texture::get_size() const noexcept {
    return size;
}

/*-------------------------------------
    Get the texture type of that this texture uses in OpenGL
-------------------------------------*/
inline tex_type_t Texture::get_texture_type() const noexcept {
    return texType;
}



/*-----------------------------------------------------------------------------
 * Texture Object Utility functions
-----------------------------------------------------------------------------*/

/**------------------------------------
 * Get the maximum texture size supported by OpenGL.
 *
 * @return an integral type which can be used to determine the maximum
 * byte size of a texture supported by the GPU.
-------------------------------------*/
inline int get_max_texture_size() noexcept {
    return get_gl_int(GL_MAX_TEXTURE_SIZE);
}

/**
 * Retrieve the GPU-side ID of the texture currently bound as a specific
 * texture type.
 *
 * @param texType A value from the texture_type_t enumeration.
 *
 * @return The GPU handle to the texture type currently bound to as a 2D or
 * 3D texture type.
 */
inline GLuint get_active_texture_id(const tex_type_t texType) noexcept {
    return get_gl_uint((GLenum) texType);
}

/**
 * Retrieve the currently bound texture unit.
 *
 * This can help provide information to client code when using multitexturing.
 *
 * @return The currently bound multi-texture target. The initial value is
 * GL_TEXTURE0.
 */
inline tex_slot_t get_active_texture_unit() noexcept {
    return (tex_slot_t) get_gl_int(GL_ACTIVE_TEXTURE);
}

} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_TEXTURE_H__ */
