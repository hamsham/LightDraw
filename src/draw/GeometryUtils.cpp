/* 
 * File:   draw/geometry_utils.cpp
 * Author: Miles Lacey
 * 
 * Created on April 6, 2014, 5:08 PM
 */

#include "lightsky/draw/geometry_utils.h"

namespace ls {

/*-------------------------------------
    Calculate the normal vector of a triangle
-------------------------------------*/
math::vec3 draw::calc_vertex_normal(
    const math::vec3& v0,
    const math::vec3& v1,
    const math::vec3& v2
) {
    const math::vec3&& outVec1 = v1-v0;
    const math::vec3&& outVec2 = v2-v0;
    
    return math::normalize(math::vec3{
        (outVec1[1]*outVec2[2]) - (outVec1[2] - outVec2[1]),
        -((outVec2[2]*outVec1[0]) - (outVec2[0] - outVec1[2])),
        (outVec1[0] - outVec2[1]) - (outVec1[1] - outVec2[0])
    });
}

/*-------------------------------------
    Calculate the tangents for a set of triangles (placed in a vertex array).
-------------------------------------*/
void draw::calc_vertex_tangents(
    unsigned vertCount,
    const math::vec3* const positions,
    const math::vec2* const uvs,
    math::vec3* tangents,
    math::vec3* bitangents
) {
    for (unsigned i = 0; i < vertCount; ++i) {
        const math::vec3&& deltaPos1 = positions[i+1] - positions[i];
        const math::vec3&& deltaPos2 = positions[i+2] - positions[i];
        
        const math::vec2&& deltaUv1 = uvs[i+1] - uvs[i];
        const math::vec2&& deltaUv2 = uvs[i+2] - uvs[i];
        
        const float r = 1.f / ((deltaUv1[0]*deltaUv2[1]) - (deltaUv1[1]*deltaUv2[0]));

        tangents[i] = tangents[i+1] = tangents[i+2] =
            math::vec3{(deltaPos1*deltaUv2[1]) - (deltaPos2*deltaUv1[1])} * r;

        bitangents[i] = bitangents[i+1] = bitangents[i+2] =
            math::vec3{(deltaPos2*deltaUv1[0]) - (deltaPos1*deltaUv2[0])} * r;
    }
}

/*-------------------------------------
 * Calculate the tangent of a textured triangle in model-space.
-------------------------------------*/
math::vec3 draw::calc_vertex_tangent(
    const math::vec3& pos0,
    const math::vec3& pos1,
    const math::vec3& pos2,
    const math::vec2& uv0,
    const math::vec2& uv1,
    const math::vec2& uv2
) {
    const math::vec3&& deltaPos1 = pos1 - pos0;
    const math::vec3&& deltaPos2 = pos2 - pos0;

    const math::vec2&& deltaUv1 = uv1 - uv0;
    const math::vec2&& deltaUv2 = uv2 - uv0;

    const float r = 1.f / ((deltaUv1[0]*deltaUv2[1]) - (deltaUv1[1]*deltaUv2[0]));

    return math::vec3{(deltaPos1*deltaUv2[1]) - (deltaPos2*deltaUv1[1])} * r;
}

/*-------------------------------------
 * Calculate the bi-tangent of a textured triangle in model-space.
-------------------------------------*/
math::vec3 draw::calc_vertex_bitangent(
    const math::vec3& pos0,
    const math::vec3& pos1,
    const math::vec3& pos2,
    const math::vec2& uv0,
    const math::vec2& uv1,
    const math::vec2& uv2
) {
    const math::vec3&& deltaPos1 = pos1 - pos0;
    const math::vec3&& deltaPos2 = pos2 - pos0;

    const math::vec2&& deltaUv1 = uv1 - uv0;
    const math::vec2&& deltaUv2 = uv2 - uv0;

    const float r = 1.f / ((deltaUv1[0]*deltaUv2[1]) - (deltaUv1[1]*deltaUv2[0]));

    return math::vec3{(deltaPos2*deltaUv1[0]) - (deltaPos1*deltaUv2[0])} * r;
}

} // end ls namespace
