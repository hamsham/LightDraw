
#ifndef __LS_DRAW_SCENE_MESH_H__
#define __LS_DRAW_SCENE_MESH_H__

#include <vector>

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/BoundingBox.h"
#include "lightsky/draw/DrawParams.h"
#include "lightsky/draw/SceneMaterial.h"



namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * @brief Basic structure to contain information about mesh geometry
 * residing on the GPU.
-----------------------------------------------------------------------------*/
struct MeshMetaData {
    /**
     * numSubmeshes contains the number of individually renderable blocks of
     * geometry contained within a SceneMesh.
     */
    uint32_t numSubmeshes;

    /**
     * VertTypes contains the types of common vertex types interwoven together
     * within a mesh.
     */
    common_vertex_t vertTypes;

    /**
     * TotalVerts contains the total number of vertices contained within a
     * SceneMeshes VBO.
     */
    uint32_t totalVerts;

    /**
     * IndexType contains the data type used by OpenGL to take indices from an
     * IBO and reference vertices in a VBO.
     */
    index_element_t indexType;

    /**
     * TotalIndices contains the number of index types currently held in an
     * IBO.
     */
    uint32_t totalIndices;

    /**
     * @brief Calculate the size of the currently used vertex type.
     * 
     * @return The size, in bytes, of a single vertex value currently stored
     * in a mesh object's VBO.
     */
    uint32_t calc_vertex_stride() const noexcept;

    /**
     * @brief Calculate the total number of bytes stored in a VBO for
     * rendering a mesh.
     * 
     * @return The size, in bytes, of all vertices currently stored in a mesh
     * object's VBO.
     */
    uint32_t calc_total_vertex_bytes() const noexcept;

    /**
     * @brief Calculate the size of the currently used index type.
     * 
     * @return The size, in bytes, of a single index value currently stored in
     * a mesh object's IBO.
     */
    uint32_t calc_index_stride() const noexcept;

    /**
     * @brief Calculate the total number of bytes stored in an IBO for
     * rendering a mesh using one of the 'glDrawElements*()' function
     * variations.
     * 
     * @return The size, in bytes, of all indices currently stored in a mesh
     * object's IBO.
     */
    uint32_t calc_total_index_bytes() const noexcept;

    /**
     * @brief Calculate the total number of bytes stored in a meshes VBO and
     * IBO.
     * 
     * @return The size, in bytes, of all vertices and indices currently
     * stored in a mesh object's VBO & IBO, respectively.
     */
    uint32_t calc_total_bytes() const noexcept;
    
    /**
     * @brief Reset all internal variables to their default values.
     */
    void reset() noexcept;
};

/*-------------------------------------
 * Get the current vertex stride
-------------------------------------*/
inline uint32_t MeshMetaData::calc_vertex_stride() const noexcept {
    return draw::get_vertex_byte_size(vertTypes);
}

/*-------------------------------------
 * Get the total number of bytes used by a meshes vertices.
-------------------------------------*/
inline uint32_t MeshMetaData::calc_total_vertex_bytes() const noexcept {
    return calc_vertex_stride() * totalVerts;
}

/*-------------------------------------
 * Get the current index stride
-------------------------------------*/
inline uint32_t MeshMetaData::calc_index_stride() const noexcept {
    return draw::get_index_byte_size(indexType);
}

/*-------------------------------------
 * Get the total number of bytes used by a meshes indices.
-------------------------------------*/
inline uint32_t MeshMetaData::calc_total_index_bytes() const noexcept {
    return calc_index_stride() * totalIndices;
}

/*-------------------------------------
 * Get the total number of bytes used by a mesh.
-------------------------------------*/
inline uint32_t MeshMetaData::calc_total_bytes() const noexcept {
    return calc_total_vertex_bytes() + calc_total_index_bytes();
}



/*-----------------------------------------------------------------------------
 * Meta-information and render parameters for a Mesh to be drawn with OpenGL.
-----------------------------------------------------------------------------*/
struct SceneMesh {
    /**
     * @brief Trivially copyable parameters which allow for a mesh to be drawn.
     * These parameters contain indices for a piece of geometry contained
     * within the VBO/IBO referenced by *this.
     */
    DrawCommandParams drawParams;

    /**
     * Non-owning reference to the VBO which *this object uses for rendering.
     */
    uint32_t vboId;

    /**
     * Non-owning reference to the IBO which *this object uses for rendering.
     */
    uint32_t iboId;
    
    /**
     * Meta-information about a single mesh object's VBO/IBO information.
     */
    MeshMetaData metaData;
    
    /**
     * @brief Function to reset all parameters in *this to their default
     * values.
     */
    void reset() noexcept;
};



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SCENE_MESH_H__ */
