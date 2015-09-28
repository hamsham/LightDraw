/* 
 * File:   draw/VertexUtils.cpp
 * Author: Miles Lacey
 *
 * Created on April 30, 2014, 8:51 PM
 */

#include <limits>

#include "lightsky/draw/VertexUtils.h"

namespace ls {

/*
constexpr char draw::vertex_position[];

constexpr char draw::vertex_uv[];

constexpr char draw::vertex_normal[];

constexpr char draw::vertex_tangent[];

constexpr char draw::vertex_bitangent[];

constexpr char draw::vertex_model_matrix[];

constexpr char draw::vertex_bone_weight[];

constexpr char draw::vertex_boneId[];
*/

/*-------------------------------------
 * Get the number of bytes required to store a flexible vertex format.
-------------------------------------*/
unsigned draw::get_vertex_byte_size(const draw::common_vertex_t vertexTypes) {
    unsigned numBytes = 0;
    
    if (vertexTypes & common_vertex_t::POSITION_VERTEX)     {numBytes += sizeof(math::vec3);}
    if (vertexTypes & common_vertex_t::TEXTURE_VERTEX)      {numBytes += sizeof(math::vec2);}
    if (vertexTypes & common_vertex_t::NORMAL_VERTEX)       {numBytes += sizeof(math::vec3);}
    if (vertexTypes & common_vertex_t::TANGENT_VERTEX)      {numBytes += sizeof(math::vec3);}
    if (vertexTypes & common_vertex_t::BITANGENT_VERTEX)    {numBytes += sizeof(math::vec3);}
    if (vertexTypes & common_vertex_t::MODEL_MAT_VERTEX)    {numBytes += sizeof(math::mat4);}
    if (vertexTypes & common_vertex_t::BONE_ID_VERTEX)      {numBytes += sizeof(unsigned);}
    if (vertexTypes & common_vertex_t::BONE_WEIGHT_VERTEX)  {numBytes += sizeof(float);}
    
    return numBytes;
}

/*-------------------------------------
 * Get the minimum required index format required to perform indexed rendering.
-------------------------------------*/
draw::index_element_t draw::get_required_index_type(const unsigned numVertices) {
    return numVertices <= std::numeric_limits<unsigned short>::max()
        ? draw::index_element_t::INDEX_TYPE_USHORT
        : draw::index_element_t::INDEX_TYPE_UINT;
}

} // end ls namespace
