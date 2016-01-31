/* 
 * File:   draw/VertexUtils.cpp
 * Author: Miles Lacey
 *
 * Created on April 30, 2014, 8:51 PM
 */

#include <limits>

#include "lightsky/draw/VertexAttrib.h"
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
    
    if (vertexTypes & common_vertex_t::POSITION_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::POSITION_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::TEXTURE_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::TEXTURE_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::COLOR_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::COLOR_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::NORMAL_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::NORMAL_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::TANGENT_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::TANGENT_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::BITANGENT_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::BITANGENT_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::MODEL_MAT_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::MODEL_MAT_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::BONE_ID_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::BONE_ID_VERTEX_TYPE);
    }
    
    if (vertexTypes & common_vertex_t::BONE_WEIGHT_VERTEX) {
        numBytes += get_num_attrib_bytes(vertex_data_t::BONE_WEIGHT_VERTEX_TYPE);
    }
    
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

/*-------------------------------------
 * Get the number of bytes required to store a set of indices.
-------------------------------------*/
unsigned draw::get_index_byte_size(const draw::index_element_t indexType) {
    switch(indexType) {
        case INDEX_TYPE_UBYTE:
            return sizeof(unsigned char);
        case INDEX_TYPE_USHORT:
            return sizeof(unsigned short);
        case INDEX_TYPE_UINT:
            return sizeof(unsigned int);
        default:
            break;
    }
    
    return (unsigned)-1;
}

} // end ls namespace
