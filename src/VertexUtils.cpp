/*
 * File:   draw/VertexUtils.cpp
 * Author: Miles Lacey
 *
 * Created on April 30, 2014, 8:51 PM
 */

#include <limits>

#include "lightsky/math/scalar_utils.h"

#include "lightsky/utils/Endian.h"

#include "lightsky/draw/VAOAttrib.h"
#include "lightsky/draw/VertexUtils.h"

namespace ls {

const char* const* draw::get_common_vertex_names() noexcept {
    static const char* const names[] = {
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

        draw::VERT_ATTRIB_NAME_INDEX,
        
        draw::VERT_ATTRIB_NAME_BBOX_TRR,
        draw::VERT_ATTRIB_NAME_BBOX_BFL
    };

    // Because I still don't trust myself...
    static_assert(
        LS_ARRAY_SIZE(names) == draw::COMMON_VERTEX_FLAGS_COUNT,
        "Unable to match the commonly used vertex names to their attributes in Vertex.h."
        );

    return names;
}

/*-------------------------------------
 * Get the number of bytes which pad a vertex until a particular attribute can be found.
-------------------------------------*/
unsigned draw::get_vertex_attrib_offset(const common_vertex_t vertexTypes, const common_vertex_t mask) {
    unsigned numBytes = 0;

    constexpr unsigned numFlags = LS_ARRAY_SIZE(COMMON_VERTEX_FLAGS_LIST);
    static_assert(numFlags == LS_ARRAY_SIZE(COMMON_VERTEX_TYPES_LIST), "Non-matching vertex type array lengths!");

    for (unsigned i = 0; i < numFlags; ++i) {
        const common_vertex_t currentType = COMMON_VERTEX_FLAGS_LIST[i];
        if (vertexTypes & currentType) {
            if (currentType & mask) {
                break;
            }
            
            numBytes += get_num_vertex_bytes(COMMON_VERTEX_TYPES_LIST[i]);
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
    switch (indexType) {
        case INDEX_TYPE_UBYTE:
            return sizeof (unsigned char);
        case INDEX_TYPE_USHORT:
            return sizeof (unsigned short);
        case INDEX_TYPE_UINT:
            return sizeof (unsigned int);
        default:
            break;
    }

    return 0;
}

} // end ls namespace
