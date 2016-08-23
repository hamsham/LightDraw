
#include <type_traits>

#include "lightsky/math/Math.h"

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/SceneFileUtility.h"

namespace math = ls::math;
namespace draw = ls::draw;
using ls::draw::common_vertex_t;
using ls::draw::pack_vertex_normal;



/*-------------------------------------
 * Convert Assimp draw types to internal ones
-------------------------------------*/
static constexpr draw::buffer_map_t DEFAULT_VBO_MAP_FLAGS = (draw::buffer_map_t) (0
  | draw::VBO_MAP_BIT_INVALIDATE_RANGE
  | draw::VBO_MAP_BIT_UNSYNCHRONIZED
  | draw::VBO_MAP_BIT_INVALIDATE_BUFFER
  | draw::VBO_MAP_BIT_WRITE
  | 0);



/*-------------------------------------
 * Convert Assimp draw types to internal ones
-------------------------------------*/
draw::draw_mode_t convert_assimp_draw_mode(const aiMesh* const pMesh) noexcept {
    namespace draw = draw;

    if (0 != (pMesh->mPrimitiveTypes & aiPrimitiveType::aiPrimitiveType_POINT)) {
        return draw::draw_mode_t::DRAW_MODE_POINTS;
    }

    if (0 != (pMesh->mPrimitiveTypes & aiPrimitiveType::aiPrimitiveType_LINE)) {
        return draw::draw_mode_t::DRAW_MODE_LINES;
    }

    return draw::draw_mode_t::DRAW_MODE_TRIS;
}



/*-------------------------------------
 * Convert Assimp vertex attributes into internal enumerations
-------------------------------------*/
common_vertex_t convert_assimp_verts(const aiMesh* const pMesh) noexcept {
    std::underlying_type<common_vertex_t>::type vertTypes = 0;

    if (pMesh->HasFaces()) {
        vertTypes |= common_vertex_t::POSITION_VERTEX;
    }

    if (pMesh->HasTextureCoords(0)) {
        vertTypes |= common_vertex_t::TEXTURE_VERTEX;
    }

    if (pMesh->HasNormals()) {
        vertTypes |= common_vertex_t::NORMAL_VERTEX;
    }

    if (pMesh->HasTangentsAndBitangents()) {
        vertTypes |= common_vertex_t::TANGENT_VERTEX | common_vertex_t::BITANGENT_VERTEX;
    }

    if (pMesh->HasVertexColors(0)) {
        vertTypes |= common_vertex_t::COLOR_VERTEX;
    }

    if (!vertTypes) {
        LS_LOG_ERR("Warning: No vertex data found for the imported submesh \"", pMesh->mName.C_Str(), ".\"");
    }

    return (common_vertex_t)vertTypes;
}



/*-------------------------------------
 * Convert Assimp's texture mapping to internally recognized ones
------------------------------------*/
draw::tex_wrap_t convert_assimp_tex_wrapping(const aiTextureMapMode inWrapMode) noexcept {
    draw::tex_wrap_t outWrapping = draw::tex_wrap_t::TEX_WRAP_DEFAULT;

    switch(inWrapMode) {
        case aiTextureMapMode::aiTextureMapMode_Clamp:
        case aiTextureMapMode::aiTextureMapMode_Decal:
            outWrapping = draw::tex_wrap_t::TEX_WRAP_CLAMP;
            break;

        case aiTextureMapMode::aiTextureMapMode_Mirror:
            outWrapping = draw::tex_wrap_t::TEX_WRAP_MIRROR_REPEAT;
            break;

        case aiTextureMapMode::aiTextureMapMode_Wrap:
            outWrapping = draw::tex_wrap_t::TEX_WRAP_REPEAT;
            break;

        default:
            break;
    }

    return outWrapping;
}



/*-------------------------------------
 * Retreive the next VBO Group marker in a list of markers.
-------------------------------------*/
draw::VboGroupMarker* get_matching_marker(const common_vertex_t inVertType, std::vector<draw::VboGroupMarker>& markers) noexcept {
    for (draw::VboGroupMarker& m : markers) {
        if (inVertType == m.vertType) {
            return &m;
        }
    }

    return nullptr;
}



/*-------------------------------------
 * Helper function to map a VBO/IBO
-------------------------------------*/
char* map_scene_file_buffer(draw::BufferObject& b, const unsigned numBytes) noexcept {
    b.bind();
    void* const pB = b.map_data(0, numBytes, DEFAULT_VBO_MAP_FLAGS);

    if (!pB) {
        LS_LOG_ERR("\t\tFailed to map a buffer from GPU to CPU memory while importing a mesh file.");
        return nullptr;
    }
    return (char*)pB;
}



/*-------------------------------------
 * Calculate a portion of vertex data that a glyph should contain.
-------------------------------------*/
template <typename data_t>
inline char* set_mesh_vertex_data(char* const pVert, const data_t& data, const unsigned vertStride) noexcept {
    *reinterpret_cast<data_t * const> (pVert) = data;
    return pVert + vertStride;
}



/*-------------------------------------
 * Calculate the vertex positions for a mesh.
-------------------------------------*/
unsigned calc_mesh_geometry_pos(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept {
    const unsigned numVertices = pMesh->mNumVertices;
    const aiVector3D* const pInVerts = pMesh->mVertices;

    for (unsigned i = 0; i < numVertices; ++i) {
        const aiVector3D& inVert = pInVerts[i];
        pVbo = set_mesh_vertex_data(pVbo, convert_assimp_vector(inVert), vertStride);
    }

    return numVertices * get_vertex_byte_size(common_vertex_t::POSITION_VERTEX);
}



/*-------------------------------------
 * Convert Assimp UVs to Internal Uvs.
-------------------------------------*/
unsigned calc_mesh_geometry_uvs(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept {
    const unsigned numVertices = pMesh->mNumVertices;
    const aiVector3D* const inUvs = pMesh->mTextureCoords[aiTextureType_NONE];

    for (unsigned i = 0; i < numVertices; ++i) {
        const aiVector3D& inUv = inUvs[i];
        pVbo = set_mesh_vertex_data(pVbo, math::vec2{inUv.x, inUv.y}, vertStride);
    }

    return numVertices * get_vertex_byte_size(common_vertex_t::TEXTURE_VERTEX);
}



/*-------------------------------------
 * Convert Assimp Normals to Internal Normals.
-------------------------------------*/
unsigned calc_mesh_geometry_norms(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept {
    const unsigned numVertices = pMesh->mNumVertices;
    const aiVector3D* const pInNorms = pMesh->mNormals;

    for (unsigned i = 0; i < numVertices; ++i) {
        const aiVector3D& inNorm = pInNorms[i];
        pVbo = set_mesh_vertex_data(pVbo, convert_assimp_normal(inNorm), vertStride);
    }

    return numVertices * get_vertex_byte_size(common_vertex_t::NORMAL_VERTEX);
}



/*-------------------------------------
 * Convert Assimp Tangents & BiTangents to Internal ones.
 * Add an index for each submesh to the VBO.
-------------------------------------*/
unsigned calc_mesh_geometry_tangent(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride,
    const common_vertex_t tangentType
) noexcept {
    const unsigned numVertices = pMesh->mNumVertices;

    for (unsigned i = 0; i < numVertices; ++i) {
        const aiVector3D& inTng = (tangentType == common_vertex_t::TANGENT_VERTEX) ? pMesh->mTangents[i] : pMesh->mBitangents[i];
        pVbo = set_mesh_vertex_data(pVbo, convert_assimp_normal(inTng), vertStride);
    }

    return numVertices * get_vertex_byte_size(tangentType);
}



/*-------------------------------------
 * Convert Assimp Colors to Internal Colors.
-------------------------------------*/
unsigned calc_mesh_geometry_colors(
    const aiMesh* const pMesh,
    char* pVbo,
    const unsigned vertStride
) noexcept {
    const unsigned numVertices = pMesh->mNumVertices;
    const aiColor4D* const pInColors = pMesh->mColors[aiTextureType_NONE];

    for (unsigned i = 0; i < numVertices; ++i) {
        const aiColor4D& inColor = pInColors[i];
        pVbo = set_mesh_vertex_data(pVbo, convert_assimp_color(inColor), vertStride);
    }

    return numVertices * get_vertex_byte_size(common_vertex_t::COLOR_VERTEX);
}



/*-------------------------------------
 * Function to dispatch all text-loading responsibilities to their respective loaders.
-------------------------------------*/
unsigned upload_mesh_vertices(
    const aiMesh* const pMesh,
    char* pVbo,
    const common_vertex_t vertTypes
) noexcept {
    const unsigned vertStride = get_vertex_stride(vertTypes);
    unsigned offset = 0;
    unsigned bytesWritten = 0;

    if (vertTypes & common_vertex_t::POSITION_VERTEX) {
        bytesWritten += calc_mesh_geometry_pos(pMesh, pVbo+offset, vertStride);
        offset += get_vertex_byte_size(common_vertex_t::POSITION_VERTEX);
    }

    if (vertTypes & common_vertex_t::TEXTURE_VERTEX) {
        bytesWritten += calc_mesh_geometry_uvs(pMesh, pVbo+offset, vertStride);
        offset += get_vertex_byte_size(common_vertex_t::TEXTURE_VERTEX);
    }

    if (vertTypes & common_vertex_t::NORMAL_VERTEX) {
        bytesWritten += calc_mesh_geometry_norms(pMesh, pVbo+offset, vertStride);
        offset += get_vertex_byte_size(common_vertex_t::NORMAL_VERTEX);
    }

    if (vertTypes & common_vertex_t::TANGENT_VERTEX) {
        bytesWritten += calc_mesh_geometry_tangent(pMesh, pVbo+offset, vertStride, common_vertex_t::TANGENT_VERTEX);
        offset += get_vertex_byte_size(common_vertex_t::TANGENT_VERTEX);
    }

    if (vertTypes & common_vertex_t::BITANGENT_VERTEX) {
        bytesWritten += calc_mesh_geometry_tangent(pMesh, pVbo+offset, vertStride, common_vertex_t::BITANGENT_VERTEX);
        offset += get_vertex_byte_size(common_vertex_t::BITANGENT_VERTEX);
    }

    if (vertTypes & common_vertex_t::COLOR_VERTEX) {
        bytesWritten += calc_mesh_geometry_colors(pMesh, pVbo+offset, vertStride);
        offset += get_vertex_byte_size(common_vertex_t::COLOR_VERTEX);
    }

    return bytesWritten;
}



/*-------------------------------------
 * Count all scene nodes in an aiScene
-------------------------------------*/
unsigned count_assimp_nodes(const aiNode* const pNode) noexcept {
    if (!pNode) {
        return 0;
    }

    const unsigned numChildren = pNode->mNumChildren;
    unsigned numNodes = 1 + numChildren;

    for (unsigned i = 0; i < numChildren; ++i) {
        numNodes += count_assimp_nodes(pNode->mChildren[i]);
    }

    return numNodes;
}



/*-------------------------------------
 * Setup an animation for importing
-------------------------------------*/
draw::Animation setup_imported_animation(
    const char* const name,
    const draw::anim_prec_t duration,
    const draw::anim_prec_t ticksPerSec,
    const unsigned numChannels
) noexcept {
    draw::Animation anim{};
    
    anim.set_duration(duration);
    anim.set_anim_name(std::string{name});
    anim.set_ticks_per_sec(ticksPerSec > 0.0 ? ticksPerSec : 23.976);
    anim.reserve_anim_channels(numChannels);
    
    return anim;
}
