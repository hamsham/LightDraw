/* 
 * File:   draw/VertexUtils.cpp
 * Author: Miles Lacey
 *
 * Created on April 30, 2014, 8:51 PM
 */

#include <limits>

#include "lightsky/math/scalar_utils.h"

#include "lightsky/draw/VertexAttrib.h"
#include "lightsky/draw/VertexUtils.h"

namespace ls {

const std::string draw::VERT_ATTRIB_NAME_POSITION = "posAttrib";

const std::string draw::VERT_ATTRIB_NAME_TEXTURE = "uvAttrib";

const std::string draw::VERT_ATTRIB_NAME_COLOR = "colorAttrib";

const std::string draw::VERT_ATTRIB_NAME_NORMAL = "normAttrib";

const std::string draw::VERT_ATTRIB_NAME_TANGENT = "tangAttrib";

const std::string draw::VERT_ATTRIB_NAME_BITANGENT = "bitangAttrib";

const std::string draw::VERT_ATTRIB_NAME_MODEL_MATRIX = "modelMatAttrib";

const std::string draw::VERT_ATTRIB_NAME_BONE_WEIGHT = "boneWeightAttrib";

const std::string draw::VERT_ATTRIB_NAME_BONE_ID = "boneIdAttrib";

const std::string draw::VERT_ATTRIB_NAME_AMBIENT = "ambientAttrib";

const std::string draw::VERT_ATTRIB_NAME_DIFFUSE = "diffuseAttrib";

const std::string draw::VERT_ATTRIB_NAME_SPECULAR = "specularAttrib";

const std::string draw::VERT_ATTRIB_NAME_ROUGHNESS = "roughAttrib";

const std::string draw::VERT_ATTRIB_NAME_METALLIC = "metalAttrib";

const std::string draw::VERT_ATTRIB_NAME_INDEX = "indexAttrib";



/*-------------------------------------
 * Get the number of bytes which pad a vertex until a particular attribute can be found.
-------------------------------------*/
unsigned draw::get_vertex_attrib_offset(const common_vertex_t vertexTypes, const common_vertex_t mask) {
    unsigned numBytes = 0;
    
    constexpr common_vertex_t flags[] = {
        common_vertex_t::POSITION_VERTEX,
        common_vertex_t::TEXTURE_VERTEX,
        common_vertex_t::NORMAL_VERTEX,
        common_vertex_t::COLOR_VERTEX,
        common_vertex_t::TANGENT_VERTEX,
        common_vertex_t::BITANGENT_VERTEX,
        common_vertex_t::MODEL_MAT_VERTEX,
        common_vertex_t::BONE_ID_VERTEX,
        common_vertex_t::BONE_WEIGHT_VERTEX,
        common_vertex_t::AMBIENT_VERTEX,
        common_vertex_t::DIFFUSE_VERTEX,
        common_vertex_t::SPECULAR_VERTEX,
        common_vertex_t::ROUGHNESS_VERTEX,
        common_vertex_t::METALLIC_VERTEX
    };
    constexpr vertex_data_t types[] = {
        vertex_data_t::POSITION_VERTEX_TYPE,
        vertex_data_t::TEXTURE_VERTEX_TYPE,
        vertex_data_t::NORMAL_VERTEX_TYPE,
        vertex_data_t::COLOR_VERTEX_TYPE,
        vertex_data_t::TANGENT_VERTEX_TYPE,
        vertex_data_t::BITANGENT_VERTEX_TYPE,
        vertex_data_t::MODEL_MAT_VERTEX_TYPE,
        vertex_data_t::BONE_ID_VERTEX_TYPE,
        vertex_data_t::BONE_WEIGHT_VERTEX_TYPE,
        vertex_data_t::AMBIENT_VERTEX_TYPE,
        vertex_data_t::DIFFUSE_VERTEX_TYPE,
        vertex_data_t::SPECULAR_VERTEX_TYPE,
        vertex_data_t::ROUGHNESS_VERTEX_TYPE,
        vertex_data_t::METALLIC_VERTEX_TYPE
    };
    
    constexpr unsigned numFlags = LS_ARRAY_SIZE(flags);
    static_assert(numFlags == LS_ARRAY_SIZE(types), "Non-matching vertex type array lengths!");
    
    for (unsigned i = 0; i < numFlags; ++i) {
        if (vertexTypes & flags[i]) {
            if (vertexTypes & mask) {
                break;
            }
            else {
                numBytes += get_num_attrib_bytes(types[i]);
            }
        }
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
