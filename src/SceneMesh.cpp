
#include "lightsky/draw/SceneMesh.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * MeshMetaData
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Reset A Meshes Meta information
-------------------------------------*/
void MeshMetaData::reset() noexcept
{
    numSubmeshes = 0;
    vertTypes = (common_vertex_t)0;
    totalVerts = 0;
    indexType = index_element_t::INDEX_TYPE_NONE;
    totalIndices = 0;
}



/*-----------------------------------------------------------------------------
 * SceneMesh objects
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Reset A Mesh
-------------------------------------*/
void SceneMesh::reset() noexcept
{
    drawParams = DrawCommandParams{};
    vboId = 0;
    iboId = 0;
    metaData.reset();
}
} // end draw namespace
} // end ls namespace
