/* 
 * File:   draw/geometry.h
 * Author: Miles Lacey
 *
 * Created on April 6, 2014, 5:08 PM
 */

#ifndef __LS_DRAW_GEOMETRY_UTILS_H__
#define	__LS_DRAW_GEOMETRY_UTILS_H__

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * Helper function to calculate a vertex normal from 3 vertices.
 * 
 * @param v0
 * @param v1
 * @param v2
 * 
 * @return A 3-dimensional vector that represents a vertex normal.
-----------------------------------------------------------------------------*/
math::vec3 calc_vertex_normal(
    const math::vec3& v0,
    const math::vec3& v1,
    const math::vec3& v2
);

/**----------------------------------------------------------------------------
 * Calculate the tangents for a set of triangles (placed in a vertex array).
 * 
 * @param vertCount
 * The number of vertices in the input arrays.
 * 
 * @param positions
 * A pointer to an array (of size 'vertCount') of 3D vectors, representing the
 * positions of a set of triangles.
 * 
 * @param uvs
 * A pointer to an array (of size 'vertCount') of 3D vectors, representing the
 * uvs of a set of triangles.
 * 
 * @param tangents
 * A pointer to an array (of size 'vertCount') of 3D vectors, representing the
 * tangents of a set of triangles.
 * 
 * @param bitangents
 * A pointer to an array (of size 'vertCount') of 3D vectors, representing the
 * bitangents of a set of triangles.
 * 
-----------------------------------------------------------------------------*/
void calc_vertex_tangents(
    unsigned vertCount,
    const math::vec3* const positions,
    const math::vec2* const uvs,
    math::vec3* tangents,
    math::vec3* bitangents
);

/**----------------------------------------------------------------------------
 * @brief Calculate the tangents for a set of triangles (placed in a vertex
 * array).
 * 
 * @param pos0
 * The first vertex of a triangle in model-space.
 * 
 * @param pos1
 * The second vertex of a triangle in model-space.
 * 
 * @param pos2
 * The third vertex of a triangle in model-space.
 * 
 * @param uv0
 * The third UV coordinate of a triangle in texture-space.
 * 
 * @param uv1
 * The third UV coordinate of a triangle in texture-space.
 * 
 * @param uv2
 * The third UV coordinate of a triangle in texture-space.
 * 
 * @return A 3D vector, representing the tangent coordinate of a textured
 * triangle.
-----------------------------------------------------------------------------*/
math::vec3 calc_vertex_tangent(
    const math::vec3& pos0, const math::vec3& pos1, const math::vec3& pos2,
    const math::vec2& uv0,  const math::vec2& uv1,  const math::vec2& uv2
);

/**----------------------------------------------------------------------------
 * @brief Calculate the tangents for a set of triangles (placed in a vertex
 * array).
 * 
 * @param pos0
 * The first vertex of a triangle in model-space.
 * 
 * @param pos1
 * The second vertex of a triangle in model-space.
 * 
 * @param pos2
 * The third vertex of a triangle in model-space.
 * 
 * @param uv0
 * The third UV coordinate of a triangle in texture-space.
 * 
 * @param uv1
 * The third UV coordinate of a triangle in texture-space.
 * 
 * @param uv2
 * The third UV coordinate of a triangle in texture-space.
 * 
 * @return A 3D vector, representing the bi-tangent coordinate of a textured
 * triangle.
-----------------------------------------------------------------------------*/
math::vec3 calc_vertex_bitangent(
    const math::vec3& pos0, const math::vec3& pos1, const math::vec3& pos2,
    const math::vec2& uv0,  const math::vec2& uv1,  const math::vec2& uv2
);

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_GEOMETRY_UTILS_H__ */

