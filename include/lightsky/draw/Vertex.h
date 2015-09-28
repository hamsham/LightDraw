/* 
 * File:   draw/vertex.h
 * Author: Miles Lacey
 * 
 * @file Vertex types and identifiers.
 * This header describes the vertex types that can be used throughout the LS
 * framework. Additional vertex types are added and updated as necessary.
 *
 * Created on April 30, 2014, 8:51 PM
 */

#ifndef __LS_DRAW_VERTEX_H__
#define	__LS_DRAW_VERTEX_H__

#include <cstddef> // std::offsetof
#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * In some instances it's better to use 4D vectors for vertex attributes,
 * rather than 3D vectors (such as for x86/x64 alignment). Use a typedef to
 * specify which one is to be used for the currently targeted platform.
-----------------------------------------------------------------------------*/
typedef math::vec3 vert_pos_t;
typedef math::vec2 vert_uv_t;
typedef math::vec3 vert_norm_t;

typedef unsigned bone_vert_id_t;
typedef float bone_vert_weight_t;

/**----------------------------------------------------------------------------
 * @brief Basic vertex building block in LS.
 * 
 * This vertex type contains enough information to light a textured mesh.
-----------------------------------------------------------------------------*/
struct vertex {
    vert_pos_t pos;
    vert_uv_t uv;
    vert_norm_t norm;
};

/**----------------------------------------------------------------------------
 * @brief The bone vertex contains the amount of influence that a bone,
 * at a particular ID, will have on a vertex.
-----------------------------------------------------------------------------*/
struct boneVertex {
    bone_vert_id_t vertIndex;
    bone_vert_weight_t weight;
};

/**----------------------------------------------------------------------------
 * These enumerations can be used to describe vertex layouts to VAO objects.
-----------------------------------------------------------------------------*/
enum class vertex_attrib_t : int {
    VERTEX_ATTRIB_POS           = 0,
    VERTEX_ATTRIB_TEX           = 1,
    VERTEX_ATTRIB_NORM          = 2,
    
    VERTEX_ATTRIB_MAT_ROW       = 3, /* Matrices can be used for instancing */
    VERTEX_ATTRIB_MAT_ROW0      = 3, /* Row 1, mat4_t<>[0][] */
    VERTEX_ATTRIB_MAT_ROW1      = 4, /* Row 2, mat4_t<>[1][] */
    VERTEX_ATTRIB_MAT_ROW2      = 5, /* Row 3, mat4_t<>[2][] */
    VERTEX_ATTRIB_MAT_ROW3      = 6, /* Row 4, mat4_t<>[3][] */
            
    VERTEX_ATTRIB_BONE_ID       = 7,
    VERTEX_ATTRIB_BONE_WEIGHT   = 8,
};

/**----------------------------------------------------------------------------
 * These enumerations can be used to describe vertex layouts to VAO objects.
-----------------------------------------------------------------------------*/
enum class vertex_desc_t : int {
    ELEMENT_COUNT_POS           = offsetof(vertex, pos),
    ELEMENT_COUNT_TEX           = offsetof(vertex, uv),
    ELEMENT_COUNT_NORM          = offsetof(vertex, norm),
    
    // Matrices should be included in separate VBOs from regular vertices
    ELEMENT_COUNT_MAT_ROW       = 0,
    ELEMENT_COUNT_MAT_ROW0      = sizeof(ls::math::vec4) * 0,
    ELEMENT_COUNT_MAT_ROW1      = sizeof(ls::math::vec4) * 1,
    ELEMENT_COUNT_MAT_ROW2      = sizeof(ls::math::vec4) * 2,
    ELEMENT_COUNT_MAT_ROW3      = sizeof(ls::math::vec4) * 3,
    
    ELEMENT_COUNT_BONE_ID       = offsetof(boneVertex, vertIndex),
    ELEMENT_COUNT_BONE_WEIGHT   = offsetof(boneVertex, weight),
};

/**----------------------------------------------------------------------------
 * Offsets to different attributes in a vertex.
-----------------------------------------------------------------------------*/
enum vertex_offset_t : unsigned {
    VERTEX_OFFSET_POS   = (offsetof(vertex, pos)  + offsetof(vert_pos_t, v)),
    VERTEX_OFFSET_TEX   = (offsetof(vertex, uv)   + offsetof(vert_uv_t, v)),
    VERTEX_OFFSET_NORM  = (offsetof(vertex, norm) + offsetof(vert_norm_t, v))
};

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_VERTEX_H__ */
