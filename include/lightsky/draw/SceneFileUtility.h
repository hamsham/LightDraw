
#ifndef __LS_DRAW_SCENE_FILE_UTILITY_H__
#define __LS_DRAW_SCENE_FILE_UTILITY_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "lightsky/draw/PackedVertex.h"
#include "lightsky/draw/SceneFileLoader.h"




/*-----------------------------------------------------------------------------
 * Private local functions, structures, and enumerations
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Assimp Import Flags & Enumerations
 *
 * These meshes were hand-selected through much trial & error. Do not touch
 * unless you want a swarm of bees hidden in your breakfast cereal box.
-------------------------------------*/
enum : unsigned int {
    SCENE_FILE_IMPORT_FLAGS = 0
        //| aiProcess_OptimizeGraph
        | aiProcess_OptimizeMeshes
        | aiProcess_FindInstances
        | aiProcess_SortByPType
        | aiProcess_RemoveComponent
        | aiProcess_JoinIdenticalVertices
        | aiProcess_FindDegenerates
        | aiProcess_FixInfacingNormals
        | aiProcess_FindInvalidData
        | aiProcess_ValidateDataStructure
        | aiProcess_ImproveCacheLocality
        | aiProcess_TransformUVCoords
        | aiProcess_RemoveRedundantMaterials
        | 0
};



/*-------------------------------------
 * Convert an Assimp vector to an internal one
-------------------------------------*/
template <typename num_t>
ls::math::vec2_t<num_t> convert_assimp_vector(const aiVector2t<num_t>& inVec) noexcept {
    return ls::math::vec2_t<num_t>{inVec.x, inVec.y};
}



/*-------------------------------------
 * Convert an Assimp vector to an internal one
-------------------------------------*/
template <typename num_t>
ls::math::vec3_t<num_t> convert_assimp_vector(const aiVector3t<num_t>& inVec) noexcept {
    return ls::math::vec3_t<num_t>{inVec.x, inVec.y, inVec.z};
}

template <typename num_t>
int32_t convert_assimp_normal(const aiVector3t<num_t>& inVec) noexcept {
    return ls::draw::pack_vertex_normal(ls::math::vec3_t<num_t>{inVec.x, inVec.y, inVec.z});
}



/*-------------------------------------
 * Convert an Assimp vector to an internal one
-------------------------------------*/
template <typename num_t>
ls::math::quat_t<num_t> convert_assimp_quaternion(const aiQuaterniont<num_t>& inQuat) noexcept {
    return ls::math::quat_t<num_t>{inQuat.x, inQuat.y, inQuat.z, inQuat.w};
}



/*-------------------------------------
 * Convert an Assimp color to an internal one
-------------------------------------*/
template <typename num_t>
ls::draw::color::colorf_t convert_assimp_color(const aiColor4t<num_t>& inColor) noexcept {
    return ls::draw::color::colorf_t{inColor.r, inColor.g, inColor.b, inColor.a};
}



/*-------------------------------------
 * Convert an Assimp matrix to an internal one
-------------------------------------*/
template <typename num_t>
ls::math::mat3_t<num_t> convert_assimp_matrix(const aiMatrix3x3t<num_t>& inMat) noexcept {
    return ls::math::mat3_t<num_t>{
        inMat.a1, inMat.b1, inMat.c1,
        inMat.a2, inMat.b2, inMat.c2,
        inMat.a3, inMat.b3, inMat.c3,
    };
}



/*-------------------------------------
 * Convert an Assimp matrix to an internal one
-------------------------------------*/
template <typename num_t>
ls::math::mat4_t<num_t> convert_assimp_matrix(const aiMatrix4x4t<num_t>& inMat) noexcept {
    return ls::math::mat4_t<num_t>{
        inMat.a1, inMat.b1, inMat.c1, inMat.d1,
        inMat.a2, inMat.b2, inMat.c2, inMat.d2,
        inMat.a3, inMat.b3, inMat.c3, inMat.d3,
        inMat.a4, inMat.b4, inMat.c4, inMat.d4
    };
}



/*-------------------------------------
 * Convert Assimp draw types to internal ones
-------------------------------------*/
ls::draw::draw_mode_t convert_assimp_draw_mode(const aiMesh* const pMesh) noexcept;



/*-------------------------------------
 * Convert Assimp vertex attributes into internal enumerations
-------------------------------------*/
ls::draw::common_vertex_t convert_assimp_verts(const aiMesh* const pMesh) noexcept;



/*-------------------------------------
 * Convert Assimp's texture mapping to internally recognized ones
------------------------------------*/
ls::draw::tex_wrap_t convert_assimp_tex_wrapping(const aiTextureMapMode inWrapMode) noexcept;



/*-------------------------------------
 * Retreive the next VBO Group marker in a list of markers.
-------------------------------------*/
ls::draw::VboGroupMarker* get_matching_marker(const ls::draw::common_vertex_t inVertType, std::vector<ls::draw::VboGroupMarker>& markers) noexcept;



/*-------------------------------------
 * Helper function to map a VBO/IBO
-------------------------------------*/
char* map_scene_file_buffer(ls::draw::BufferObject& b, const unsigned numBytes) noexcept;



/*-------------------------------------
 * Calculate the vertex positions for a mesh.
-------------------------------------*/
unsigned calc_mesh_geometry_pos(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept;



/*-------------------------------------
 * Convert Assimp UVs to Internal Uvs.
-------------------------------------*/
unsigned calc_mesh_geometry_uvs(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept;



/*-------------------------------------
 * Convert Assimp Normals to Internal Normals.
-------------------------------------*/
unsigned calc_mesh_geometry_norms(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept;



/*-------------------------------------
 * Convert Assimp Tangents & BiTangents to Internal ones.
 * Add an index for each submesh to the VBO.
-------------------------------------*/
unsigned calc_mesh_geometry_tangent(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride,
    const ls::draw::common_vertex_t tangentType
) noexcept;



/*-------------------------------------
 * Convert Assimp Colors to Internal Colors.
-------------------------------------*/
unsigned calc_mesh_geometry_colors(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept;



/*-------------------------------------
 * Function to dispatch all text-loading responsibilities to their respective loaders.
-------------------------------------*/
unsigned upload_mesh_vertices(
    const aiMesh* const pMesh,
    char* const pVbo,
    const ls::draw::common_vertex_t vertTypes
) noexcept;


/*-------------------------------------
 * Check to see if a node is a mesh/camera/bone/light node
-------------------------------------*/
template <typename assimp_data_t>
int is_node_type(
    const aiNode* const pNode,
    const assimp_data_t* const* const pItems,
    const unsigned itemCount
) noexcept {
    const aiString& nodeName = pNode->mName;

    for (int i = 0; i < (int)itemCount; ++i) {
        const assimp_data_t* const pItem = pItems[i];
        const aiString& itemName = pItem->mName;

        if (strcmp(nodeName.C_Str(), itemName.C_Str()) == 0) {
            return i;
        }
    }

    return -1;
}



template <>
inline int is_node_type<aiMesh>(const aiNode* const pNode, const aiMesh* const* const, const unsigned) noexcept {
    return pNode->mNumMeshes > 0;
}



/*-------------------------------------
 * Count all scene nodes in an aiScene
-------------------------------------*/
unsigned count_assimp_nodes(const aiNode* const pNode) noexcept;



#endif /* __LS_DRAW_SCENE_FILE_UTILITY_H__ */
