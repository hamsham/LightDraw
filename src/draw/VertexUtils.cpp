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

const std::string draw::COMMON_VERTEX_NAMES_LIST[] = {
    draw::VERT_ATTRIB_NAME_POSITION,
    draw::VERT_ATTRIB_NAME_TEXTURE,
    draw::VERT_ATTRIB_NAME_COLOR,
    
    draw::VERT_ATTRIB_NAME_NORMAL,
    draw::VERT_ATTRIB_NAME_TANGENT,
    draw::VERT_ATTRIB_NAME_BITANGENT,
    
    draw::VERT_ATTRIB_NAME_MODEL_MATRIX,
    
    draw::VERT_ATTRIB_NAME_BONE_ID,
    draw::VERT_ATTRIB_NAME_BONE_WEIGHT,
    
    draw::VERT_ATTRIB_NAME_AMBIENT,
    draw::VERT_ATTRIB_NAME_DIFFUSE,
    draw::VERT_ATTRIB_NAME_SPECULAR,
    draw::VERT_ATTRIB_NAME_ROUGHNESS,
    draw::VERT_ATTRIB_NAME_METALLIC,
    
    draw::VERT_ATTRIB_NAME_INDEX
};



/*-------------------------------------
 * Get the number of bytes which pad a vertex until a particular attribute can be found.
-------------------------------------*/
unsigned draw::get_vertex_attrib_offset(const common_vertex_t vertexTypes, const common_vertex_t mask) {
    unsigned numBytes = 0;
    
    constexpr unsigned numFlags = LS_ARRAY_SIZE(COMMON_VERTEX_FLAGS_LIST);
    static_assert(numFlags == LS_ARRAY_SIZE(COMMON_VERTEX_TYPES_LIST), "Non-matching vertex type array lengths!");
    
    for (unsigned i = 0; i < numFlags; ++i) {
        if (vertexTypes & COMMON_VERTEX_FLAGS_LIST[i]) {
            if (vertexTypes & mask) {
                break;
            }
            else {
                numBytes += get_num_attrib_bytes(COMMON_VERTEX_TYPES_LIST[i]);
            }
        }
    }
    
    return numBytes;
}



/**------------------------------------
 * @brief Convert a 3D vertex normal into a VERTEX_DATA_2_10I format.
-------------------------------------*/
int32_t draw::pack_vertex_normal(const math::vec3& norm) {
    const int32_t x = math::scale_num_to_range<float, int32_t>(norm[0], -1.f, 1.f, -1024, 1024);
    const int32_t y = math::scale_num_to_range<float, int32_t>(norm[1], -1.f, 1.f, -1024, 1024);
    const int32_t z = math::scale_num_to_range<float, int32_t>(norm[2], -1.f, 1.f, -1024, 1024);
    
    return 0
        | (x << 20)
        | (y << 10)
        | (z << 0);
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
