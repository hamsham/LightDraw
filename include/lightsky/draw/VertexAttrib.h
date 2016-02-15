/* 
 * File:   draw/VertexAttrib.h
 * Author: miles
 *
 * Created on June 10, 2015, 8:52 PM
 */

#ifndef __LS_DRAW_VERTEX_ATTRIB_H__
#define __LS_DRAW_VERTEX_ATTRIB_H__

#include <string>

#include "lightsky/utils/Assertions.h"

#include "lightsky/math/Math.h"

#include "lightsky/draw/Setup.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum vertex_data_t : GLenum;
class ShaderProgram;



/**
 * @brief Vertex Attrib Types
 * 
 * This type can be used to determine if an object is a vertex array attribute
 * or a shader uniform attribute.
*/
enum class vertex_attrib_t : int {
    UNIFORM_ATTRIB,
    VERTEX_ATTRIB
};



/**
 * @brief The Vertex Attrib structure helps to provide a layer of introspection
 * between OpenGL client code and GLSL shader code.
*/
struct VertexAttrib {
    GLuint          index;
    GLint           components;
    vertex_data_t   type;
    GLboolean       normalized;
    GLsizei         stride;
    const GLvoid*   offset;
    unsigned        instanceRate;
    std::string     name;
};



/**
 * @brief Determine the number of bytes which are used by a vertex attribute
 * type.
 * 
 * @param type
 * An enumeration which represents the type of data used by a single vertex
 * attribute.
 * 
 * @return The number of bytes used by a vertex attribute, or 0 if it is
 * undefined (such as for a texture sampler).
*/
unsigned get_num_attrib_bytes(const vertex_data_t type);

/**
 * @brief Retrieve the number of components per vertex attribute.
 * 
 * For valid vertex attribute types, this number should be 1, 2, 3, or 4.
 * 
 * For matrices, the number of components returned actually represents the
 * number of components per row.
 * 
 * @param type
 * An enumeration which represents the number of components used by a single
 * vertex attribute.
 * 
 * @return The number of components used by a vertex attribute, or 0 if it's
 * undefined (such as for a texture sampler).
*/
unsigned get_num_attrib_components(const vertex_data_t type);

/**
 * @brief Retrieve the number of shader layout slots that a single vertex
 * attribute requires.
 * 
 * For valid vertex attribute types, this number should be 1, 2, 3, or 4.
 * 
 * For matrices, the number of components returned actually represents the
 * number of rows, all other data types return 1.
 * 
 * @param type
 * An enumeration which represents the number of components used by a single
 * vertex attribute.
 * 
 * @return The number of attribute layout slots required to send a vertex
 * attribute to a shader.
*/
unsigned get_num_attrib_subcomponents(const vertex_data_t type);

/**
 * @brief Retrieve the base type of a component.
 * 
 * For example, passing VERTEX_DATA_VEC3F into this function will return
 * VERTEX_DATA_FLOAT, while passing VERTEX_DATA_VEC3I will return GL_INT.
 * 
 * This function returns VERTEX_DATA_UNKNOWN for texture/sampler types.
 * 
 * @param type
 * Contains the meta-type of a vertex attribute.
 * 
 * @return An enumeration, containing the base data type which helps to create
 * a vertex attribute.
*/
vertex_data_t get_attrib_base_type(const vertex_data_t type);

/**
 * @brief Determine if a vertex base type is one which should be normalized.
 * 
 * @param type
 * Contains the meta-type of a vertex attribute.
 * 
 * @return GL_TRUE if the input parameter is one which requires normalization,
 * GL_FALSE if not.
*/
GLboolean get_attrib_normalization(const vertex_data_t type);



/**
 * @brief Vertex Attribute types.
*/
enum vertex_data_t : GLenum {
    VERTEX_DATA_UNKNOWN = GL_ZERO,
    
    VERTEX_DATA_BYTE = GL_BYTE,
    VERTEX_DATA_UBYTE = GL_UNSIGNED_BYTE,
    VERTEX_DATA_SHORT = GL_SHORT,
    VERTEX_DATA_USHORT = GL_UNSIGNED_SHORT,
    VERTEX_DATA_INT = GL_INT,
    VERTEX_DATA_UINT = GL_UNSIGNED_INT,
    VERTEX_DATA_FLOAT = GL_FLOAT,
    VERTEX_DATA_DOUBLE = GL_DOUBLE,
    
    VERTEX_DATA_HALF_FLOAT = GL_HALF_FLOAT,
    VERTEX_DATA_FIXED = GL_FIXED,
    VERTEX_DATA_2_10U = GL_UNSIGNED_INT_2_10_10_10_REV,
    VERTEX_DATA_2_10I = GL_INT_2_10_10_10_REV,
        
    VERTEX_DATA_VEC_2B = GL_BOOL_VEC2,
    VERTEX_DATA_VEC_2I = GL_INT_VEC2,
    VERTEX_DATA_VEC_2UI = GL_UNSIGNED_INT_VEC2,
    VERTEX_DATA_VEC_2F = GL_FLOAT_VEC2,
        
    VERTEX_DATA_VEC_3B = GL_BOOL_VEC3,
    VERTEX_DATA_VEC_3I = GL_INT_VEC3,
    VERTEX_DATA_VEC_3UI = GL_UNSIGNED_INT_VEC3,
    VERTEX_DATA_VEC_3F = GL_FLOAT_VEC3,
        
    VERTEX_DATA_VEC_4B = GL_BOOL_VEC4,
    VERTEX_DATA_VEC_4I = GL_INT_VEC4,
    VERTEX_DATA_VEC_4UI = GL_UNSIGNED_INT_VEC4,
    VERTEX_DATA_VEC_4F = GL_FLOAT_VEC4,
        
    VERTEX_DATA_MAT_2F = GL_FLOAT_MAT2,
    VERTEX_DATA_MAT_2x3F = GL_FLOAT_MAT2x3,
    VERTEX_DATA_MAT_2x4F = GL_FLOAT_MAT2x4,
    
    VERTEX_DATA_MAT_3F = GL_FLOAT_MAT3,
    VERTEX_DATA_MAT_3x2F = GL_FLOAT_MAT3x2,
    VERTEX_DATA_MAT_3x4F = GL_FLOAT_MAT3x4,
    
    VERTEX_DATA_MAT_4F = GL_FLOAT_MAT4,
    VERTEX_DATA_MAT_4x2F = GL_FLOAT_MAT4x2,
    VERTEX_DATA_MAT_4x3F = GL_FLOAT_MAT4x3,
    
    VERTEX_DATA_SAMPLER_2D = GL_SAMPLER_2D,
    VERTEX_DATA_SAMPLER_2DI = GL_INT_SAMPLER_2D,
    VERTEX_DATA_SAMPLER_2DUI = GL_UNSIGNED_INT_SAMPLER_2D,
    
    VERTEX_DATA_SAMPLER_3D = GL_SAMPLER_3D,
    VERTEX_DATA_SAMPLER_3DI = GL_INT_SAMPLER_3D,
    VERTEX_DATA_SAMPLER_3DUI = GL_UNSIGNED_INT_SAMPLER_3D,
    
    VERTEX_DATA_SAMPLER_CUBE = GL_SAMPLER_CUBE,
    VERTEX_DATA_SAMPLER_CUBEI = GL_INT_SAMPLER_CUBE,
    VERTEX_DATA_SAMPLER_CUBEUI = GL_UNSIGNED_INT_SAMPLER_CUBE,
    
    VERTEX_DATA_SAMPLER_ARRAY = GL_SAMPLER_2D_ARRAY,
    VERTEX_DATA_SAMPLER_ARRAYI = GL_INT_SAMPLER_2D_ARRAY,
    VERTEX_DATA_SAMPLER_ARRAYUI = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
    
    VERTEX_DATA_SAMPLER_SHADOW_2D = GL_SAMPLER_2D_SHADOW,
    VERTEX_DATA_SAMPLER_SHADOW_3D = GL_SAMPLER_2D_ARRAY_SHADOW,
    VERTEX_DATA_SAMPLER_SHADOW_CUBE = GL_SAMPLER_CUBE_SHADOW,
    
    /*---------------------------------
     * Aliases to commonly used types.
    ---------------------------------*/
    POSITION_VERTEX_TYPE    = VERTEX_DATA_VEC_3F,
    TEXTURE_VERTEX_TYPE     = VERTEX_DATA_VEC_2F,
    COLOR_VERTEX_TYPE       = VERTEX_DATA_VEC_4F,
    
    NORMAL_VERTEX_TYPE      = VERTEX_DATA_2_10I,
    TANGENT_VERTEX_TYPE     = VERTEX_DATA_2_10I,
    BITANGENT_VERTEX_TYPE   = VERTEX_DATA_2_10I,
    
    MODEL_MAT_VERTEX_TYPE   = VERTEX_DATA_MAT_4F,
    
    BONE_ID_VERTEX_TYPE     = VERTEX_DATA_INT,
    BONE_WEIGHT_VERTEX_TYPE = VERTEX_DATA_FLOAT,
    
    AMBIENT_VERTEX_TYPE     = COLOR_VERTEX_TYPE,
    DIFFUSE_VERTEX_TYPE     = COLOR_VERTEX_TYPE,
    SPECULAR_VERTEX_TYPE    = VERTEX_DATA_FLOAT,
    ROUGHNESS_VERTEX_TYPE   = VERTEX_DATA_FLOAT,
    METALLIC_VERTEX_TYPE    = VERTEX_DATA_FLOAT,
    
    INDEX_VERTEX_TYPE    = VERTEX_DATA_UINT
};



// The order of elements names in this array must match the order of the
// "COMMON_VERTEX_FLAGS_LIST" array in "VertexUtils.h"
constexpr vertex_data_t COMMON_VERTEX_TYPES_LIST[] = {
    vertex_data_t::POSITION_VERTEX_TYPE,
    vertex_data_t::TEXTURE_VERTEX_TYPE,
    vertex_data_t::COLOR_VERTEX_TYPE,
    
    vertex_data_t::NORMAL_VERTEX_TYPE,
    vertex_data_t::TANGENT_VERTEX_TYPE,
    vertex_data_t::BITANGENT_VERTEX_TYPE,
    
    vertex_data_t::MODEL_MAT_VERTEX_TYPE,
    
    vertex_data_t::BONE_ID_VERTEX_TYPE,
    vertex_data_t::BONE_WEIGHT_VERTEX_TYPE,
    
    vertex_data_t::AMBIENT_VERTEX_TYPE,
    vertex_data_t::DIFFUSE_VERTEX_TYPE,
    vertex_data_t::SPECULAR_VERTEX_TYPE,
    vertex_data_t::ROUGHNESS_VERTEX_TYPE,
    
    vertex_data_t::METALLIC_VERTEX_TYPE,
    
    vertex_data_t::INDEX_VERTEX_TYPE
};

constexpr unsigned COMMON_VERTEX_TYPES_COUNT LS_ARRAY_SIZE(COMMON_VERTEX_TYPES_LIST);



/*-----------------------------------------------------------------------------
 * Vertex Attrib Layout Functions
-----------------------------------------------------------------------------*/
inline VertexAttrib create_vertex_attrib(const vertex_data_t type) {
    return VertexAttrib{
        0,
        (int)get_num_attrib_components(type),
        get_attrib_base_type(type),
        get_attrib_normalization(type),
        (GLsizei)get_num_attrib_components(type),
        nullptr,
        0,
        ""
    };
}

template <vertex_data_t data_t>
inline VertexAttrib create_vertex_attrib() {
    ls::utils::runtime_assert(
        false,
        ls::utils::error_t::LS_ERROR,
        "Custom vertex attributes have not been implemented."
    );
    
    return VertexAttrib{0, 0, VERTEX_DATA_UNKNOWN, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_FIXED>() {
    return VertexAttrib{0, 1, VERTEX_DATA_FIXED, GL_TRUE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_2_10I>() {
    return VertexAttrib{0, 4, VERTEX_DATA_2_10I, GL_TRUE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_2_10U>() {
    return VertexAttrib{0, 4, VERTEX_DATA_2_10U, GL_TRUE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_BYTE>() {
    return VertexAttrib{0, 1, VERTEX_DATA_BYTE, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_UBYTE>() {
    return VertexAttrib{0, 1, VERTEX_DATA_UBYTE, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_SHORT>() {
    return VertexAttrib{0, 1, VERTEX_DATA_SHORT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_USHORT>() {
    return VertexAttrib{0, 1, VERTEX_DATA_USHORT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_INT>() {
    return VertexAttrib{0, 1, VERTEX_DATA_INT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_UINT>() {
    return VertexAttrib{0, 1, VERTEX_DATA_UINT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_FLOAT>() {
    return VertexAttrib{0, 1, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_2B>() {
    return VertexAttrib{0, 2, VERTEX_DATA_BYTE, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_2I>() {
    return VertexAttrib{0, 2, VERTEX_DATA_INT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_2UI>() {
    return VertexAttrib{0, 2, VERTEX_DATA_UINT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_2F>() {
    return VertexAttrib{0, 2, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_3B>() {
    return VertexAttrib{0, 3, VERTEX_DATA_BYTE, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_3I>() {
    return VertexAttrib{0, 3, VERTEX_DATA_INT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_3UI>() {
    return VertexAttrib{0, 3, VERTEX_DATA_UINT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_3F>() {
    return VertexAttrib{0, 3, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_4B>() {
    return VertexAttrib{0, 4, VERTEX_DATA_BYTE, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_4I>() {
    return VertexAttrib{0, 4, VERTEX_DATA_INT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_4UI>() {
    return VertexAttrib{0, 4, VERTEX_DATA_UINT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_VEC_4F>() {
    return VertexAttrib{0, 4, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_MAT_2F>() {
    return VertexAttrib{0, 2, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_MAT_3F>() {
    return VertexAttrib{0, 3, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

template <>
inline VertexAttrib create_vertex_attrib<VERTEX_DATA_MAT_4F>() {
    return VertexAttrib{0, 4, VERTEX_DATA_FLOAT, GL_FALSE, 0, nullptr, 0, ""};
}

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_VERTEX_ATTRIB_H__ */

