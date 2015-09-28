/* 
 * File:   draw/VertexUtils.h
 * Author: Miles Lacey
 * 
 * @file Vertex types and identifiers.
 * This header describes the vertex types that can be used throughout the LS
 * framework. Additional vertex types are added and updated as necessary.
 *
 * Created on April 30, 2014, 8:51 PM
 */

#ifndef __LS_DRAW_VERTEX_H__
#define	__LS_DRAW_VERTEX_H__

#include <cstddef> // std::offsetof
#include <vector>

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/VertexAttrib.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Vertex Meta Types
-----------------------------------------------------------------------------*/
typedef std::vector<VertexAttrib> VertexDescriptor;

/*-----------------------------------------------------------------------------
 * Common Vertex Types, Natively Supported By LS_Draw
-----------------------------------------------------------------------------*/
enum common_vertex_t : unsigned {
    POSITION_VERTEX     = 0x1000,
    TEXTURE_VERTEX      = 0x2000,
    NORMAL_VERTEX       = 0x4000,
    
    TANGENT_VERTEX      = 0x8000,
    BITANGENT_VERTEX    = 0x0100,
    
    MODEL_MAT_VERTEX    = 0x0200,
    
    BONE_ID_VERTEX      = 0x0400,
    BONE_WEIGHT_VERTEX  = 0x0800,
    
//    AMBIENT_VERTEX      = 0x0010,
//    SPECULAR_VERTEX     = 0x0020,
//    DIFFUSE_VERTEX      = 0x0040,
//    ROUGHNESS_VERTEX    = 0x0080
};

/**
 * @brief A standard vertex is the most commonly supported collection of
 * individual base types within the rendering framework.
 */
constexpr unsigned STANDARD_VERTEX = 0
    | common_vertex_t::POSITION_VERTEX
    | common_vertex_t::TEXTURE_VERTEX
    | common_vertex_t::NORMAL_VERTEX
    | 0;

/**
 * @brief A bone vertex allows for a single vertex type to contain enough
 * information to perform skeletal animation/skinning.
 */
constexpr unsigned BONE_VERTEX = 0
    | common_vertex_t::BONE_ID_VERTEX
    | common_vertex_t::BONE_WEIGHT_VERTEX
    | 0;

/*-----------------------------------------------------------------------------
 * Common Vertex Names
-----------------------------------------------------------------------------*/
/**
 * @brief Common name for a vertex attribute containing positional vertices.
 */
constexpr char vertex_position[] = "pos";

/**
 * @brief Common name for a vertex attribute containing UV coordinates.
 */
constexpr char vertex_uv[] = "uv";

/**
 * @brief Common name for a vertex attribute containing vertex normals.
 */
constexpr char vertex_normal[] = "norm";

/**
 * @brief Common name for a vertex attribute containing vertex tangents.
 */
constexpr char vertex_tangent[] = "tang";

/**
 * @brief Common name for a vertex attribute containing vertex bi-tangents.
 */
constexpr char vertex_bitangent[] = "bitang";

/**
 * @brief Common name for a vertex attribute containing model matrices.
 */
constexpr char vertex_model_matrix[] = "modelMat";

/**
 * @brief Common name for a vertex attribute containing skeletal bone weights.
 */
constexpr char vertex_bone_weight[] = "boneWeight";

/**
 * @brief Common name for a vertex attribute containing skeletal bone IDs.
 */
constexpr char vertex_boneId[] = "boneId";

/**
 * @brief Common name for a vertex attribute containing index elements.
 */
constexpr char index_vertex[] = "ibo";

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
unsigned get_vertex_byte_size(const common_vertex_t vertexTypes);

/*-----------------------------------------------------------------------------
 * Vertex Index types and specifiers
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
enum index_element_t : int {
    INDEX_TYPE_UBYTE = GL_UNSIGNED_BYTE,
    INDEX_TYPE_USHORT = GL_UNSIGNED_SHORT,
    INDEX_TYPE_UINT = GL_UNSIGNED_INT,
    
    INDEX_TYPE_DEFAULT = GL_UNSIGNED_INT,
    INDEX_TYPE_NONE  = -1
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
struct IndexRange {
    /**
     * @brief Contains the index of the first vertex to be drawn in an OpenGL
     * draw call and the total number of vertices to be rendered.
     * 
     * It is up to client code to determine if this is to be used for the
     * "first" value in a call to "glDrawArrays(...)" or the "indices" value in
     * a call to "glDrawElements(...)."
     */
    unsigned first;
    
    /**
     * @brief Contains index of the final vertex to be rendered or the total
     * number of vertices to be rendered.
     * 
     * It is up to client code to determine if this is to be used for the
     * "count" value in a call to "glDrawArrays(...)" or the "count" value in a
     * call to "glDrawElements(...)."
     */
    unsigned count;
    
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
    IndexRange& operator = (const IndexRange& ir);
    
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
    IndexRange& operator = (IndexRange&& ir);
};

/*-------------------------------------
 * IndexRange Constructor
-------------------------------------*/
constexpr IndexRange::IndexRange(const unsigned f, const unsigned c) : 
    first{f},
    count{c}
{}

/*-------------------------------------
 * IndexRange Copy Constructor
-------------------------------------*/
constexpr IndexRange::IndexRange(const IndexRange& ir) : 
    first{ir.first},
    count{ir.count}
{}

/*-------------------------------------
 *  Move Constructor
-------------------------------------*/
constexpr IndexRange::IndexRange(IndexRange&& ir) : 
    first{ir.first},
    count{ir.count}
{}

/*-------------------------------------
 * IndexRange Copy Operator
-------------------------------------*/
inline IndexRange& IndexRange::operator=(const IndexRange& ir) {
    first = ir.first;
    count = ir.count;
    return *this;
}

/*-------------------------------------
 * IndexRange Move Operator
-------------------------------------*/
inline IndexRange& IndexRange::operator=(IndexRange&& ir) {
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

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_VERTEX_H__ */
