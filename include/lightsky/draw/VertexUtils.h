/*
 * File:   draw/VertexUtils.h
 * Author: Miles Lacey
 *
 * @file Vertex types and identifiers.
 * This header describes the vertex types that can be used throughout the ls
 * framework. Additional vertex types are added and updated as necessary.
 *
 * Created on April 30, 2014, 8:51 PM
 */

#ifndef __LS_DRAW_VERTEX_UTILS_H__
#define __LS_DRAW_VERTEX_UTILS_H__

#include <cstddef> // std::offsetof
#include <cstdint> // uint32_t
#include <vector>

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/VAOAttrib.h"



namespace ls
{
namespace draw
{

/*-----------------------------------------------------------------------------
 * Vertex Meta Types
-----------------------------------------------------------------------------*/
typedef std::vector<VAOAttrib> VertexDescriptor;



/*-----------------------------------------------------------------------------
 * Common Vertex Types, Natively Supported By LS_Draw
-----------------------------------------------------------------------------*/
enum common_vertex_t : uint32_t
{
    POSITION_VERTEX = 0x10000000,
    TEXTURE_VERTEX = 0x20000000,
    COLOR_VERTEX = 0x40000000,

    NORMAL_VERTEX = 0x80000000,
    TANGENT_VERTEX = 0x01000000,
    BITANGENT_VERTEX = 0x02000000,

    MODEL_MAT_VERTEX = 0x04000000,

    BONE_ID_VERTEX = 0x08000000,
    BONE_WEIGHT_VERTEX = 0x00100000,

    AMBIENT_VERTEX = 0x00200000,
    DIFFUSE_VERTEX = 0x00400000,
    SPECULAR_VERTEX = 0x00800000,
    ROUGHNESS_VERTEX = 0x00010000,
    METALLIC_VERTEX = 0x00020000,

    INDEX_VERTEX = 0x00040000,
    BBOX_TRR_VERTEX = 0x00080000,
    BBOX_BFL_VERTEX = 0x00001000,


    /**
     * @brief A standard vertex is the most commonly supported collection of
     * individual base types within the rendering framework.
     */
    STANDARD_VERTEX = (0
                       | POSITION_VERTEX
                       | TEXTURE_VERTEX
                       | NORMAL_VERTEX
                       | 0),

    /**
     * @brief A bone vertex allows for a single vertex type to contain enough
     * information to perform skeletal animation/skinning.
     */
    BONE_VERTEX = (0
                   | BONE_ID_VERTEX
                   | BONE_WEIGHT_VERTEX
                   | 0),

    /**
     * @brief An Occlusion vertex only provides information about positions to
     * GLSL. Occlusion vertices are intended to have a certain number of
     * position components which are instanced and translated in GLSL by a
     * model matrix.
     */
    OCCLUSION_VERTEX = (0
                        | POSITION_VERTEX
                        | BBOX_TRR_VERTEX
                        | BBOX_BFL_VERTEX
                        | 0),
};



// The order of elements names in this array must match the order of the
// "COMMON_VERTEX_TYPES_LIST" array in "Vertex.h"
constexpr common_vertex_t COMMON_VERTEX_FLAGS_LIST[] = {
    common_vertex_t::POSITION_VERTEX,
    common_vertex_t::TEXTURE_VERTEX,
    common_vertex_t::COLOR_VERTEX,

    common_vertex_t::NORMAL_VERTEX,
    common_vertex_t::TANGENT_VERTEX,
    common_vertex_t::BITANGENT_VERTEX,

    common_vertex_t::MODEL_MAT_VERTEX,

    common_vertex_t::BONE_ID_VERTEX,
    common_vertex_t::BONE_WEIGHT_VERTEX,

    common_vertex_t::AMBIENT_VERTEX,
    common_vertex_t::DIFFUSE_VERTEX,
    common_vertex_t::SPECULAR_VERTEX,
    common_vertex_t::ROUGHNESS_VERTEX,
    common_vertex_t::METALLIC_VERTEX,

    common_vertex_t::INDEX_VERTEX,

    common_vertex_t::BBOX_TRR_VERTEX,
    common_vertex_t::BBOX_BFL_VERTEX
};

constexpr unsigned COMMON_VERTEX_FLAGS_COUNT = LS_ARRAY_SIZE(COMMON_VERTEX_FLAGS_LIST);

// Because I don't trust myself...
static_assert(
    COMMON_VERTEX_FLAGS_COUNT == COMMON_VERTEX_TYPES_COUNT,
    "Unable to match the commonly used vertex types to their attributes in Vertex.h."
);




/*-----------------------------------------------------------------------------
 * Common Vertex Names
-----------------------------------------------------------------------------*/
/**
 * @brief Common name for a vertex attribute containing positional vertices.
 */
constexpr char VERT_ATTRIB_NAME_POSITION[] = "posAttrib";

/**
 * @brief Common name for a vertex attribute containing UV coordinates.
 */
constexpr char VERT_ATTRIB_NAME_TEXTURE[] = "uvAttrib";

/**
 * @brief Common name for a vertex attribute containing floating-point color
 * information.
 */
constexpr char VERT_ATTRIB_NAME_COLOR[] = "colorAttrib";

/**
 * @brief Common name for a vertex attribute containing vertex normals.
 */
constexpr char VERT_ATTRIB_NAME_NORMAL[] = "normAttrib";

/**
 * @brief Common name for a vertex attribute containing vertex tangents.
 */
constexpr char VERT_ATTRIB_NAME_TANGENT[] = "tangAttrib";

/**
 * @brief Common name for a vertex attribute containing vertex bi-tangents.
 */
constexpr char VERT_ATTRIB_NAME_BITANGENT[] = "btngAttrib";

/**
 * @brief Common name for a vertex attribute containing model matrices.
 */
constexpr char VERT_ATTRIB_NAME_MODEL_MATRIX[] = "modelMatAttrib";

/**
 * @brief Common name for a vertex attribute containing skeletal bone IDs.
 */
constexpr char VERT_ATTRIB_NAME_BONE_ID[] = "boneWeightAttrib";

/**
 * @brief Common name for a vertex attribute containing skeletal bone weights.
 */
constexpr char VERT_ATTRIB_NAME_BONE_WEIGHT[] = "boneIdAttrib";

/**
 * @brief Common name for an ambient lighting vertex attribute.
 */
constexpr char VERT_ATTRIB_NAME_AMBIENT[] = "ambientAttrib";

/**
 * @brief Common name for a diffuse lighting vertex attribute.
 */
constexpr char VERT_ATTRIB_NAME_DIFFUSE[] = "diffuseAttrib";

/**
 * @brief Common name for a specular vertex component.
 */
constexpr char VERT_ATTRIB_NAME_SPECULAR[] = "specularAttrib";

/**
 * @brief Common name for a roughness vertex component.
 */
constexpr char VERT_ATTRIB_NAME_ROUGHNESS[] = "roughAttrib";

/**
 * @brief Common name for a metallic vertex component.
 */
constexpr char VERT_ATTRIB_NAME_METALLIC[] = "metalAttrib";

/**
 * @brief Common name for an instance index vertex component.
 */
constexpr char VERT_ATTRIB_NAME_INDEX[] = "indexAttrib";

/**
 * @brief Common name for a bounding box vertex component (Top-Rear-Right).
 */
constexpr char VERT_ATTRIB_NAME_BBOX_TRR[] = "bboxTRRAttrib";

/**
 * @brief Common name for a bounding box vertex component (Bottom-front-left).
 */
constexpr char VERT_ATTRIB_NAME_BBOX_BFL[] = "bboxBFLAttrib";



/**
 * @Brief the COMMON_VERTEX_NAMES_LIST array helps to keep track of all vertex
 * names and make iteration over them easier in client code.
 */
const char* const* get_common_vertex_names() noexcept;

constexpr unsigned COMMON_VERTEX_NAMES_COUNT = COMMON_VERTEX_FLAGS_COUNT;



/**------------------------------------
 * @brief Retrieve the offset to a particular attribute within a vertex who's
 * layout is described with the common_vertex_t enumeration. This function is
 * essentially an 'offsetof()' replacement for flexible vertex formats.
 *
 * @param vertFlags
 * A bitmask of common_vertex_t flags, representing all of the vertex elements
 * within a vertex buffer.
 *
 * @param mask
 * A single value from the common_vertex_t enumeration which identifies the
 * particular vertex attribute which should be queried for a byte-offset.
 *
 * @return A pointer representing the number of bytes that pad a vertex until
 * a particular attribute is reached.
-------------------------------------*/
unsigned get_vertex_attrib_offset(const common_vertex_t vertFlags, const common_vertex_t mask);

/**------------------------------------
 * @brief Determine the number of bytes required to store one or more vertices
 * within a flexible-vertex-format.
 *
 * @param vertexTypes
 * A bitmask, containing the types of vertices which will be used in the output
 * byte-size calculation.
 *
 * @return The size, in bytes, of a flexible-vertex-format.
-------------------------------------*/
inline unsigned get_vertex_byte_size(const common_vertex_t vertexTypes)
{
    return get_vertex_attrib_offset(vertexTypes, (common_vertex_t)0);
}

inline unsigned get_vertex_stride(const common_vertex_t vertexTypes)
{
    return get_vertex_byte_size(vertexTypes);
}



/*-----------------------------------------------------------------------------
 * Index types and specifiers
-----------------------------------------------------------------------------*/
/**------------------------------------
 * Data type for the indices used during an indexed draw command.
-------------------------------------*/
typedef unsigned int draw_index_t;



/**------------------------------------
 * @brief index_element_t helps to determine two things for draw commands:
 *      1. What the data type is for indices stored in the GPU/
 *      2. If a draw command should run use glDrawArrays() or glDrawElements().
-------------------------------------*/
enum index_element_t : int32_t
{
    INDEX_TYPE_UBYTE = GL_UNSIGNED_BYTE,
    INDEX_TYPE_USHORT = GL_UNSIGNED_SHORT,
    INDEX_TYPE_UINT = GL_UNSIGNED_INT,
    INDEX_TYPE_DEFAULT = GL_UNSIGNED_INT,
    INDEX_TYPE_NONE = -1
};



/**------------------------------------
 * @brief IndexRange
 *
 * Contains the index of the first vertex to be drawn in an OpenGL draw call
 * and the total number of vertices to be rendered. Only glDrawArrays() and
 * glDrawElements() are supported as they pretty much cover 90% of typical
 * render use cases.
 *
 * For a call to glDrawArrays(...), this pair specifies the "first" and "count"
 * parameters, respectively.
 *
 * For a call to glDrawElements(...), this pair indicates the "indices*" and
 * "count" parameters, in that order.
-------------------------------------*/
struct alignas(8) IndexRange
{
    /**
     * @brief Contains the index of the first vertex to be drawn in an OpenGL
     * draw call and the total number of vertices to be rendered.
     *
     * It is up to client code to determine if this is to be used for the
     * "first" value in a call to "glDrawArrays(...)" or the "indices" value in
     * a call to "glDrawElements(...)."
     */
    uint32_t first;

    /**
     * @brief Contains index of the final vertex to be rendered or the total
     * number of vertices to be rendered.
     *
     * It is up to client code to determine if this is to be used for the
     * "count" value in a call to "glDrawArrays(...)" or the "count" value in a
     * call to "glDrawElements(...)."
     */
    uint32_t count;

    /**
     * @brief Constructor
     *
     * Initializes all internal members to their default values.
     *
     * @param f
     * A value to provide for the "first" member in *this. It is up to client
     * code to determine if this is to be used for the "first" value in a call
     * to "glDrawArrays(...)" or the "indices" value in a call to
     * "glDrawElements(...)."
     *
     * @param c
     * A value to provide for the "count" member in *this. It is up to client
     * code to determine if this is to be used for the "count" value in a call
     * to "glDrawArrays(...)" or the "count" value in a call to
     * "glDrawElements(...)."
     */
    constexpr IndexRange(const unsigned f = 0, const unsigned c = 0);

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param ir
     * A constant reference to another IndexRange object who's data is to be
     * copied into *this.
     */
    constexpr IndexRange(const IndexRange& ir);

    /**
     * @brief Move Constructor
     *
     * Copies all data from the input parameter into *this then resets all
     * members within the input parameter to their default values.
     *
     * @param ir
     * An r-value reference to another IndexRange object who's data is to be
     * copied into *this.
     */
    constexpr IndexRange(IndexRange&& ir);

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param ir
     * A constant reference to another IndexRange object who's data is to be
     * copied into *this.
     *
     * @return A reference to *this.
     */
    IndexRange& operator=(const IndexRange& ir);

    /**
     * @brief Move Operator
     *
     * Copies all data from the input parameter into *this then resets all
     * members within the input parameter to their default values.
     *
     * @param ir
     * An r-value reference to another IndexRange object who's data is to be
     * copied into *this.
     *
     * @return A reference to *this.
     */
    IndexRange& operator=(IndexRange&& ir);
};

/*-------------------------------------
 * IndexRange Constructor
-------------------------------------*/
constexpr IndexRange::IndexRange(const unsigned f, const unsigned c) :
    first{f},
    count{c}
{
}

/*-------------------------------------
 * IndexRange Copy Constructor
-------------------------------------*/
constexpr IndexRange::IndexRange(const IndexRange& ir) :
    first{ir.first},
    count{ir.count}
{
}

/*-------------------------------------
 *  Move Constructor
-------------------------------------*/
constexpr IndexRange::IndexRange(IndexRange&& ir) :
    first{ir.first},
    count{ir.count}
{
}

/*-------------------------------------
 * IndexRange Copy Operator
-------------------------------------*/
inline IndexRange& IndexRange::operator=(const IndexRange& ir)
{
    first = ir.first;
    count = ir.count;
    return *this;
}

/*-------------------------------------
 * IndexRange Move Operator
-------------------------------------*/
inline IndexRange& IndexRange::operator=(IndexRange&& ir)
{
    first = ir.first;
    count = ir.count;
    return *this;
}

/**------------------------------------
 * @brief Determine the minimum required storage format needed to render a
 * number of vertices within OpenGL.
 *
 * @param numVertices
 * An unsigned integral type, containing the number of vertices which are to be
 * rendered using indexed draw calls.
 *
 * @return An index_element_t, containing either INDEX_TYPE_USHORT or
 * INDEX_TYPE_UINT, based on the number of vertices passed into the function.
-------------------------------------*/
index_element_t get_required_index_type(const unsigned numVertices);

/**------------------------------------
 * @brief Determine the number of bytes required to store one or more indices
 * within a flexible-index-format.
 *
 * @param indexType
 * An enumeration containing the types of indices which will be used in the
 * output byte-size calculation.
 *
 * @return The size, in bytes, of a flexible-index-format.
-------------------------------------*/
unsigned get_index_byte_size(const index_element_t indexType);
} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_VERTEX_UTILS_H__ */
