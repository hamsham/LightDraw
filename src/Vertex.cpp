/*
 * File:   draw/Vertex.cpp
 * Author: miles
 *
 * Created on June 10, 2015, 8:52 PM
 */

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/Vertex.h"

namespace ls {

/*-------------------------------------
 * Get the number of bytes occupied by an attribute
-------------------------------------*/
unsigned draw::get_num_vertex_bytes(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_BYTE:
            return sizeof (char);
        case VERTEX_DATA_UBYTE:
            return sizeof (unsigned char);
        case VERTEX_DATA_SHORT:
            return sizeof (short);
        case VERTEX_DATA_USHORT:
            return sizeof (unsigned short);
        case VERTEX_DATA_INT:
            return sizeof (int);
        case VERTEX_DATA_UINT:
            return sizeof (unsigned);
        case VERTEX_DATA_FLOAT:
            return sizeof (float);

        case VERTEX_DATA_HALF_FLOAT:
            return sizeof (float) / 2;
        case VERTEX_DATA_FIXED:
            return sizeof (int);
        case VERTEX_DATA_2_10I:
            return sizeof (int32_t);
        case VERTEX_DATA_2_10U:
            return sizeof (uint32_t);

        case VERTEX_DATA_VEC_2B:
            return sizeof (math::vec2_t<char>);
        case VERTEX_DATA_VEC_2I:
            return sizeof (math::vec2i);
        case VERTEX_DATA_VEC_2UI:
            return sizeof (math::vec2u);
        case VERTEX_DATA_VEC_2F:
            return sizeof (math::vec2f);

        case VERTEX_DATA_VEC_3B:
            return sizeof (math::vec3_t<char>);
        case VERTEX_DATA_VEC_3I:
            return sizeof (math::vec3i);
        case VERTEX_DATA_VEC_3UI:
            return sizeof (math::vec3u);
        case VERTEX_DATA_VEC_3F:
            return sizeof (math::vec3f);

        case VERTEX_DATA_VEC_4B:
            return sizeof (math::vec4_t<char>);
        case VERTEX_DATA_VEC_4I:
            return sizeof (math::vec4i);
        case VERTEX_DATA_VEC_4UI:
            return sizeof (math::vec4u);
        case VERTEX_DATA_VEC_4F:
            return sizeof (math::vec4f);

        case VERTEX_DATA_MAT_2F:
            return sizeof (math::mat2f);
        case VERTEX_DATA_MAT_2x3F:
            return sizeof (math::vec2) * 3;
        case VERTEX_DATA_MAT_2x4F:
            return sizeof (math::vec2) * 4;

        case VERTEX_DATA_MAT_3F:
            return sizeof (math::mat3f);
        case VERTEX_DATA_MAT_3x2F:
            return sizeof (math::vec3) * 2;
        case VERTEX_DATA_MAT_3x4F:
            return sizeof (math::vec3) * 4;

        case VERTEX_DATA_MAT_4F:
            return sizeof (math::mat4f);
        case VERTEX_DATA_MAT_4x2F:
            return sizeof (math::vec4) * 2;
        case VERTEX_DATA_MAT_4x3F:
            return sizeof (math::vec4) * 3;

        case VERTEX_DATA_SAMPLER_2D:
        case VERTEX_DATA_SAMPLER_2DI:
        case VERTEX_DATA_SAMPLER_2DUI:
        case VERTEX_DATA_SAMPLER_3D:
        case VERTEX_DATA_SAMPLER_3DI:
        case VERTEX_DATA_SAMPLER_3DUI:
        case VERTEX_DATA_SAMPLER_CUBE:
        case VERTEX_DATA_SAMPLER_CUBEI:
        case VERTEX_DATA_SAMPLER_CUBEUI:
        case VERTEX_DATA_SAMPLER_ARRAY:
        case VERTEX_DATA_SAMPLER_ARRAYI:
        case VERTEX_DATA_SAMPLER_ARRAYUI:
        case VERTEX_DATA_SAMPLER_SHADOW_2D:
        case VERTEX_DATA_SAMPLER_SHADOW_3D:
        case VERTEX_DATA_SAMPLER_SHADOW_CUBE:
            // samplers uniforms are set in GLSL through integer IDs
            return sizeof (int);

        default:
            LS_DEBUG_ASSERT(0 != "Unknown vertex type byte size.");
            break;
    }

    return 0;
}

/*-------------------------------------
 * Get the number of components used by an attribute
-------------------------------------*/
unsigned draw::get_vertex_components(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_UNKNOWN:
            return 0;

        case VERTEX_DATA_BYTE:
        case VERTEX_DATA_UBYTE:
        case VERTEX_DATA_SHORT:
        case VERTEX_DATA_USHORT:
        case VERTEX_DATA_INT:
        case VERTEX_DATA_UINT:
        case VERTEX_DATA_HALF_FLOAT:
        case VERTEX_DATA_FLOAT:
        case VERTEX_DATA_FIXED:
            return 1;

        case VERTEX_DATA_VEC_2B:
        case VERTEX_DATA_VEC_2I:
        case VERTEX_DATA_VEC_2UI:
        case VERTEX_DATA_VEC_2F:
            return 2;

        case VERTEX_DATA_VEC_3B:
        case VERTEX_DATA_VEC_3I:
        case VERTEX_DATA_VEC_3UI:
        case VERTEX_DATA_VEC_3F:
            return 3;

        case VERTEX_DATA_VEC_4B:
        case VERTEX_DATA_VEC_4I:
        case VERTEX_DATA_VEC_4UI:
        case VERTEX_DATA_VEC_4F:
        case VERTEX_DATA_2_10I:
        case VERTEX_DATA_2_10U:
            return 4;

        case VERTEX_DATA_MAT_2F:
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_2x4F:
            return 2; // 2 per column

        case VERTEX_DATA_MAT_3F:
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_3x4F:
            return 3; // 3 per column

        case VERTEX_DATA_MAT_4F:
        case VERTEX_DATA_MAT_4x2F:
        case VERTEX_DATA_MAT_4x3F:
            return 4; // 4 per column

        case VERTEX_DATA_SAMPLER_2D:
        case VERTEX_DATA_SAMPLER_2DI:
        case VERTEX_DATA_SAMPLER_2DUI:
        case VERTEX_DATA_SAMPLER_3D:
        case VERTEX_DATA_SAMPLER_3DI:
        case VERTEX_DATA_SAMPLER_3DUI:
        case VERTEX_DATA_SAMPLER_CUBE:
        case VERTEX_DATA_SAMPLER_CUBEI:
        case VERTEX_DATA_SAMPLER_CUBEUI:
        case VERTEX_DATA_SAMPLER_ARRAY:
        case VERTEX_DATA_SAMPLER_ARRAYI:
        case VERTEX_DATA_SAMPLER_ARRAYUI:
        case VERTEX_DATA_SAMPLER_SHADOW_2D:
        case VERTEX_DATA_SAMPLER_SHADOW_3D:
        case VERTEX_DATA_SAMPLER_SHADOW_CUBE:
        default:
            break;
    }

    return 1;
}

/*-------------------------------------
 * Get the number of sub-components used by an attribute
-------------------------------------*/
unsigned draw::get_vertex_subcomponents(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_MAT_2F:
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_4x2F:
            return 2; // 2 per row
            
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_3F:
        case VERTEX_DATA_MAT_4x3F:
            return 3; // 3 per row
            
        case VERTEX_DATA_MAT_2x4F:
        case VERTEX_DATA_MAT_3x4F:
        case VERTEX_DATA_MAT_4F:
            return 4; // 4 per row
            

        default:
            break;
    }

    return 1;
}

/*-------------------------------------
 * Get the type of sub-components used by an attribute
-------------------------------------*/
draw::vertex_data_t draw::get_vertex_subcomponent_type(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_MAT_2F:
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_2x4F:
            return VERTEX_DATA_VEC_2F;

        case VERTEX_DATA_MAT_3F:
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_3x4F:
            return VERTEX_DATA_VEC_3F;

        case VERTEX_DATA_MAT_4F:
        case VERTEX_DATA_MAT_4x2F:
        case VERTEX_DATA_MAT_4x3F:
            return VERTEX_DATA_VEC_4F;

        default:
            break;
    }

    return type;
}

/*-------------------------------------
 * Convert a component meta type into a base type.
-------------------------------------*/
draw::vertex_data_t draw::get_vertex_base_type(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_BYTE:
        case VERTEX_DATA_UBYTE:
        case VERTEX_DATA_SHORT:
        case VERTEX_DATA_USHORT:
        case VERTEX_DATA_INT:
        case VERTEX_DATA_UINT:
        case VERTEX_DATA_HALF_FLOAT:
        case VERTEX_DATA_FLOAT:
        case VERTEX_DATA_2_10I:
        case VERTEX_DATA_2_10U:
            return type;

        case VERTEX_DATA_VEC_2B:
        case VERTEX_DATA_VEC_3B:
        case VERTEX_DATA_VEC_4B:
            return VERTEX_DATA_BYTE;

        case VERTEX_DATA_VEC_2I:
        case VERTEX_DATA_VEC_3I:
        case VERTEX_DATA_VEC_4I:
            return VERTEX_DATA_INT;

        case VERTEX_DATA_VEC_2UI:
        case VERTEX_DATA_VEC_3UI:
        case VERTEX_DATA_VEC_4UI:
            return VERTEX_DATA_UINT;

        case VERTEX_DATA_VEC_2F:
        case VERTEX_DATA_VEC_3F:
        case VERTEX_DATA_VEC_4F:
        case VERTEX_DATA_MAT_2F:
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_2x4F:
        case VERTEX_DATA_MAT_3F:
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_3x4F:
        case VERTEX_DATA_MAT_4F:
        case VERTEX_DATA_MAT_4x2F:
        case VERTEX_DATA_MAT_4x3F:
            return VERTEX_DATA_FLOAT;

        case VERTEX_DATA_SAMPLER_2D:
        case VERTEX_DATA_SAMPLER_2DI:
        case VERTEX_DATA_SAMPLER_2DUI:
        case VERTEX_DATA_SAMPLER_3D:
        case VERTEX_DATA_SAMPLER_3DI:
        case VERTEX_DATA_SAMPLER_3DUI:
        case VERTEX_DATA_SAMPLER_CUBE:
        case VERTEX_DATA_SAMPLER_CUBEI:
        case VERTEX_DATA_SAMPLER_CUBEUI:
        case VERTEX_DATA_SAMPLER_ARRAY:
        case VERTEX_DATA_SAMPLER_ARRAYI:
        case VERTEX_DATA_SAMPLER_ARRAYUI:
        case VERTEX_DATA_SAMPLER_SHADOW_2D:
        case VERTEX_DATA_SAMPLER_SHADOW_3D:
        case VERTEX_DATA_SAMPLER_SHADOW_CUBE:
            // samplers uniforms are set in GLSL through integer IDs
            return VERTEX_DATA_INT;

        default:
            break;
    }

    // GL_FIXED has no apparent base type
    return VERTEX_DATA_UNKNOWN;
}

/*-------------------------------------
 * Determine if a vertex base type is one which should be normalized.
-------------------------------------*/
GLboolean draw::get_vertex_normalization(const vertex_data_t type) {
    return (type == vertex_data_t::VERTEX_DATA_FIXED
        || type == vertex_data_t::VERTEX_DATA_2_10U
        || type == vertex_data_t::VERTEX_DATA_2_10I)
        ? GL_TRUE
        : GL_FALSE;
}

} // end ls namespace
