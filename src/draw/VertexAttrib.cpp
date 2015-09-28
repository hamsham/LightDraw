/* 
 * File:   vertexAttrib.cpp
 * Author: miles
 * 
 * Created on June 10, 2015, 8:52 PM
 */

#include "lightsky/draw/shaderProgram.h"
#include "lightsky/draw/VertexAttrib.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Get active attribute info from a shader.
-------------------------------------*/
std::string get_attrib_shader_name(
    const shaderProgram& prog,
    const GLint index,
    GLint* const outVarSize,
    GLenum* const outVarType
) {
    return prog.getAttribName(
        vertex_attrib_t::VERTEX_ATTRIB,
        index,
        outVarSize,
        outVarType
    );
}

/*-------------------------------------
 * Get the number of bytes occupied by an attribute
-------------------------------------*/
unsigned get_num_attrib_bytes(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_UNKNOWN:
            return 0;

        case VERTEX_DATA_BYTE:
            return sizeof(unsigned char);
        case VERTEX_DATA_INT:
            return sizeof(int);
        case VERTEX_DATA_UINT:
            return sizeof(unsigned);
        case VERTEX_DATA_FLOAT:
            return sizeof(float);
        case VERTEX_DATA_HALF_FLOAT:
            return 16;
            
        case VERTEX_DATA_FIXED:
        case VERTEX_DATA_2_10I:
        case VERTEX_DATA_2_10U:
            return 32;

        case VERTEX_DATA_VEC_2B:
            return sizeof(math::vec2_t<unsigned char>);
        case VERTEX_DATA_VEC_2I:
            return sizeof(math::vec2f);
        case VERTEX_DATA_VEC_2UI:
            return sizeof(math::vec2f);
        case VERTEX_DATA_VEC_2F:
            return sizeof(math::vec2f);

        case VERTEX_DATA_VEC_3B:
            return sizeof(math::vec3_t<unsigned char>);
        case VERTEX_DATA_VEC_3I:
            return sizeof(math::vec3i);
        case VERTEX_DATA_VEC_3UI:
            return sizeof(math::vec3ui);
        case VERTEX_DATA_VEC_3F:
            return sizeof(math::vec3f);

        case VERTEX_DATA_VEC_4B:
            return sizeof(math::vec4_t<unsigned char>);
        case VERTEX_DATA_VEC_4I:
            return sizeof(math::vec4i);
        case VERTEX_DATA_VEC_4UI:
            return sizeof(math::vec4ui);
        case VERTEX_DATA_VEC_4F:
            return sizeof(math::vec4f);

        case VERTEX_DATA_MAT_2F:
            return sizeof(math::mat2f);
            
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_2x4F:
            return 0;

        case VERTEX_DATA_MAT_3F:
            return sizeof(math::mat3f);
            
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_3x4F:
            return 0;

        case VERTEX_DATA_MAT_4F:
            return sizeof(math::mat4f);
            
        case VERTEX_DATA_MAT_4x2F:
        case VERTEX_DATA_MAT_4x3F:
            return 0;

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
    
    return 0;
}

/*-------------------------------------
 * Get the number of components used by an attribute
-------------------------------------*/
unsigned get_num_attrib_components(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_UNKNOWN:
            return 0;

        case VERTEX_DATA_BYTE:
        case VERTEX_DATA_INT:
        case VERTEX_DATA_UINT:
        case VERTEX_DATA_FLOAT:
        case VERTEX_DATA_HALF_FLOAT:
        case VERTEX_DATA_FIXED:
        case VERTEX_DATA_2_10I:
        case VERTEX_DATA_2_10U:
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
            return 4;

        case VERTEX_DATA_MAT_2F:
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_2x4F:
            return 2; // 2 per row

        case VERTEX_DATA_MAT_3F:
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_3x4F:
            return 3;

        case VERTEX_DATA_MAT_4F:
        case VERTEX_DATA_MAT_4x2F:
        case VERTEX_DATA_MAT_4x3F:
            return 4;

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
unsigned get_num_attrib_subcomponents(const vertex_data_t type) {
    switch (type) {
        case VERTEX_DATA_MAT_2F:
        case VERTEX_DATA_MAT_2x3F:
        case VERTEX_DATA_MAT_2x4F:
            return 2; // 2 per row

        case VERTEX_DATA_MAT_3F:
        case VERTEX_DATA_MAT_3x2F:
        case VERTEX_DATA_MAT_3x4F:
            return 3;

        case VERTEX_DATA_MAT_4F:
        case VERTEX_DATA_MAT_4x2F:
        case VERTEX_DATA_MAT_4x3F:
            return 4;
            
        default:
            break;
    }
    
    return 1;
}

/*-------------------------------------
 * Convert a component meta type into a base type.
-------------------------------------*/
vertex_data_t get_attrib_base_type(const vertex_data_t type) {
    switch(type) {
        case VERTEX_DATA_HALF_FLOAT:
        case VERTEX_DATA_FIXED:
        case VERTEX_DATA_2_10I:
        case VERTEX_DATA_2_10U:
            return type;

        case VERTEX_DATA_BYTE:
        case VERTEX_DATA_VEC_2B:
        case VERTEX_DATA_VEC_3B:
        case VERTEX_DATA_VEC_4B:
            return VERTEX_DATA_BYTE;
            
        case VERTEX_DATA_INT:
        case VERTEX_DATA_VEC_2I:
        case VERTEX_DATA_VEC_3I:
        case VERTEX_DATA_VEC_4I:
            return VERTEX_DATA_INT;
            
        case VERTEX_DATA_UINT:
        case VERTEX_DATA_VEC_2UI:
        case VERTEX_DATA_VEC_3UI:
        case VERTEX_DATA_VEC_4UI:
            return VERTEX_DATA_UINT;
            
        case VERTEX_DATA_FLOAT:
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
            
        default:
            break;
    }
    
    return VERTEX_DATA_UNKNOWN;
}

} // end draw namespace
} // end ls namespace
