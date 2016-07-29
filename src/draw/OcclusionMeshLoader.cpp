
/* 
 * File:   OcclusionMeshLoader.cpp
 * Author: miles
 * 
 * Created on March 11, 2016, 11:10 PM
 */

#include <algorithm>
#include <utility>

#include "ls/draw/OcclusionMeshLoader.h"
#include "ls/draw/VAOAssembly.h"
#include "ls/draw/VertexArray.h"
#include "ls/draw/VertexUtils.h"

namespace ls {
namespace draw {



/*-------------------------------------
-------------------------------------*/
OcclusionMeshLoader::~OcclusionMeshLoader() noexcept {
}

/*-------------------------------------
-------------------------------------*/
OcclusionMeshLoader::OcclusionMeshLoader() noexcept :
    sceneData{}
{}

/*-------------------------------------
-------------------------------------*/
OcclusionMeshLoader::OcclusionMeshLoader(const OcclusionMeshLoader& l) noexcept :
    sceneData{l.sceneData}
{}

/*-------------------------------------
-------------------------------------*/
OcclusionMeshLoader::OcclusionMeshLoader(OcclusionMeshLoader&& l) noexcept :
    sceneData{std::move(l.sceneData)}
{}

/*-------------------------------------
-------------------------------------*/
OcclusionMeshLoader& OcclusionMeshLoader::operator =(const OcclusionMeshLoader& l) noexcept {
    sceneData = l.sceneData;
    return *this;
}

/*-------------------------------------
-------------------------------------*/
OcclusionMeshLoader& OcclusionMeshLoader::operator =(OcclusionMeshLoader&& l) noexcept {
    sceneData = std::move(l.sceneData);
    return *this;
}

/*-------------------------------------
 * Meta information about the occluder being generated.
-------------------------------------*/
void OcclusionMeshLoader::generate_meta_data(MeshMetaData& metaData, const unsigned numInstances) {
    metaData.numSubmeshes = numInstances;

    metaData.vertTypes = common_vertex_t::OCCLUSION_VERTEX;
    metaData.totalVerts = occlusion_property_t::OCCLUSION_BOX_NUM_VERTS;

    metaData.indexType = index_element_t::INDEX_TYPE_NONE;
    metaData.totalIndices = 0;

    LS_LOG_MSG(
        "Occlusion Geometry Meta Data:",
        "\n\tBytes Per Vertex:  ", metaData.calc_vertex_stride(),
        "\n\tVertex Count:      ", metaData.totalVerts,
        "\n\tVertex Bytes:      ", metaData.calc_total_vertex_bytes(),
        "\n\tBytes Per Index:   ", metaData.calc_index_stride(),
        "\n\tIndex Count:       ", metaData.totalIndices,
        "\n\tIndex Bytes:       ", metaData.calc_total_index_bytes(),
        "\n\tTotal Bytes:       ", metaData.calc_total_bytes()
    );
}

/*-------------------------------------
 * Cube VBO Initialization
-------------------------------------*/
bool OcclusionMeshLoader::init_cube_vbo(VertexBuffer& cubeVbo) {
    static constexpr math::vec3 points[OCCLUSION_BOX_NUM_VERTS] = {
        math::vec3{1.f, 1.f, -1.f},
        math::vec3{-1.f, 1.f, -1.f},
        math::vec3{1.f, 1.f, 1.f},
        math::vec3{-1.f, 1.f, 1.f},
        
        math::vec3{-1.f, -1.f, 1.f},
        math::vec3{-1.f, 1.f, -1.f},
        math::vec3{-1.f, -1.f, -1.f},
        math::vec3{1.f, 1.f, -1.f},
        
        math::vec3{1.f, -1.f, -1.f},
        math::vec3{1.f, 1.f, 1.f},
        math::vec3{1.f, -1.f, 1.f},
        math::vec3{-1.f, 1.f, 1.f},
        
        math::vec3{-1.f, -1.f, 1.f},
        math::vec3{-1.f, -1.f, -1.f},
        math::vec3{1.f, -1.f, 1.f},
        math::vec3{1.f, -1.f, -1.f}
    };
    constexpr unsigned numBytes = sizeof(points);
    
    if (!cubeVbo.init() || !cubeVbo.setup_attribs(common_vertex_t::POSITION_VERTEX)) {
        LS_LOG_ERR("\tAn error occurred while initializing a cube VBO for occluder geometry.");
        return false;
    }

    cubeVbo.bind();
    LS_LOG_GL_ERR();
    
    cubeVbo.set_data(numBytes, points, buffer_access_t::VBO_STATIC_DRAW);
    LS_LOG_GL_ERR();
    
    cubeVbo.unbind();
    LS_LOG_GL_ERR();
    
    
    return true;
}

/*-------------------------------------
 * Bounding Box VBO Initialization
-------------------------------------*/
bool OcclusionMeshLoader::init_bounds_vbo(VertexBuffer& boundsVbo, const unsigned numInstances) {
    static const BoundingBox bb{};
    // VERY important that this order follows the enumerations in VertexUtils.h
    static const math::vec3 bounds[]    = {bb.get_top_rear_right(), bb.get_bot_front_left()};
    constexpr unsigned pointsPerBox     = LS_ARRAY_SIZE(bounds);
    constexpr common_vertex_t attribs   = (common_vertex_t)(common_vertex_t::BBOX_TRR_VERTEX | common_vertex_t::BBOX_BFL_VERTEX);
    const unsigned byteStride           = draw::get_vertex_byte_size(attribs);
    const unsigned numBytes             = byteStride*numInstances;
    
    if (!boundsVbo.init() || !boundsVbo.setup_attribs(attribs)) {
        LS_LOG_ERR("\tAn error occurred while initializing a bounding-box VBO for occluder geometry.");
        return false;
    }

    boundsVbo.bind();
    boundsVbo.set_data(numBytes, nullptr, buffer_access_t::VBO_STREAM_DRAW);
    LS_LOG_GL_ERR();
    
    // VBO Mapping
    math::vec3* pVerts = (math::vec3*)boundsVbo.map_data(0, numBytes, DEFAULT_VBO_MAP_FLAGS);
    LS_LOG_GL_ERR();
    
    LS_LOG_MSG("MAPPING ", numBytes, " BYTES OF DATA FOR AN OCCLUSION VBO");
    
    if (!pVerts) {
        boundsVbo.unbind();
        LS_LOG_ERR("\tAn error occurred while attempting to map a VBO for occluder geometry.");
        return false;
    }
    
    for (unsigned i = 0; i < numInstances*pointsPerBox; ++i) {
        *(pVerts++) = bounds[i % pointsPerBox];
    }
    
    boundsVbo.unmap_data();
    boundsVbo.unbind();
    
    for (unsigned i = 0; i < boundsVbo.get_num_attribs(); ++i) {
        VBOAttrib& vboAttrib = boundsVbo.get_attrib(i);

        vboAttrib.set_num_elements(1);
        vboAttrib.set_byte_stride(byteStride);
        
        // one bounding box extent for every vertex instance instance rendered
        vboAttrib.set_instance_rate(1);
    }
    
    return true;
}

/*-------------------------------------
 * CPU Memory Initialization
-------------------------------------*/
bool OcclusionMeshLoader::allocate_cpu_data(const unsigned numInstances) {
    GLContextData& renderData           = sceneData.renderData;
    VAODataList& vaos                   = renderData.vaos;
    VBODataList& vbos                   = renderData.vbos;
    std::vector<SceneMesh>& meshData    = sceneData.meshes;
    
    vaos.reserve(1);
    vbos.reserve(2);
    
    vaos.add(VertexArray{});
    vbos.add(VertexBuffer{});
    vbos.add(VertexBuffer{});
    
    meshData.resize(1);
    meshData.shrink_to_fit();
    
    SceneMesh& mesh = sceneData.meshes.front();

    DrawCommandParams& occluder = mesh.drawParams;
    occluder.drawFunc   = (draw_func_t)(draw_func_t::DRAW_ARRAYS | draw_func_t::DRAW_INSTANCED);
    occluder.drawMode   = draw_mode_t::DRAW_MODE_TRI_STRIP;
    occluder.indexType  = index_element_t::INDEX_TYPE_NONE;
    occluder.first      = 0;
    occluder.count      = occlusion_property_t::OCCLUSION_BOX_NUM_VERTS;

    sceneData.bounds.resize(numInstances);

    return true;
}

/*-------------------------------------
 * GPU Memory Initialization
-------------------------------------*/
bool OcclusionMeshLoader::allocate_gpu_data(const unsigned numInstances) {
    GLContextData& renderData   = sceneData.renderData;
    VBODataList& vbos           = renderData.vbos;
    VertexBuffer& cubeVbo       = vbos.front();
    VertexBuffer& boundsVbo     = vbos.back();

    return init_cube_vbo(cubeVbo) && init_bounds_vbo(boundsVbo, numInstances);
}

/*-------------------------------------
 * VAO Creation
-------------------------------------*/
bool OcclusionMeshLoader::assemble_vao() {
    SceneMesh& meshData         = sceneData.meshes.front();
    GLContextData& renderData   = sceneData.renderData;
    const VBODataList& vbos     = renderData.vbos;
    
    ls::utils::Pointer<draw::VAOAssembly> pAssembly {new(std::nothrow) draw::VAOAssembly {}};

    // using metaData.vertTypes vs passing in the user-requested vertex flags.
    // some of the preprocessing functions may have added/removed flags.
    const unsigned setBitCount = math::count_set_bits(meshData.metaData.vertTypes);
    
    LS_LOG_MSG("\t\tAssigning ", vbos.size(), " VBOs to an occlusion mesh VAO.");
    for (unsigned i = 0, vaoIndex = 0; i < vbos.size(); ++i) {
        const VertexBuffer& vbo = vbos[i];
        for (unsigned j = 0; j < vbo.get_num_attribs(); ++j) {
            pAssembly->set_vbo_attrib(vaoIndex++, vbo, j);
        }
    }

    LS_LOG_MSG("\t\tAssigning ", setBitCount, " attributes to an occlusion mesh VAO.");
    for (unsigned i = 0, vaoIndex = 0; i < COMMON_VERTEX_NAMES_COUNT; ++i) {
        const common_vertex_t vertFlag = COMMON_VERTEX_FLAGS_LIST[i];

        if (0 == (meshData.metaData.vertTypes & vertFlag)) {
            continue;
        }
        else {
            LS_ASSERT(pAssembly->set_attrib_name(vaoIndex++, draw::get_common_vertex_names()[i]));
        }
    }

    VAODataList& vaos = renderData.vaos;
    VertexArray& vao = vaos.front();
    
    LS_ASSERT(pAssembly->assemble(vao));

    LS_LOG_MSG("\t\tValidating an occlusion mesh VAO was successfully created.");
    return vao.is_valid();
}

/*-------------------------------------
-------------------------------------*/
unsigned OcclusionMeshLoader::load(unsigned numObjects) noexcept {
    LS_LOG_MSG("Attempting to load occlusion geometry.");
    numObjects = math::max(numObjects, 1u);

    LS_LOG_MSG("\tAllocating memory for occlusion mesh data.");
    if (!allocate_cpu_data(numObjects)) {
        LS_LOG_ERR("\tFailed to allocate memory for occlusion mesh data.\n");
        unload();
        return 0;
    }
    LS_LOG_MSG("\t\tDone.");
    
    SceneMesh& meshData = sceneData.meshes.front();
    generate_meta_data(meshData.metaData, numObjects);

    if (!allocate_gpu_data(numObjects)) {
        unload();
        LS_LOG_ERR("\tUnable to initialize occlusion mesh data.\n");
        return 0;
    }

    LS_LOG_MSG("\tGenerating a VAO to store occlusion mesh data.");
    if (!assemble_vao()) {
        LS_LOG_ERR("\t\tUnable create a mesh VAO from occlusion data.\n");
        unload();
        return 0;
    }
    LS_LOG_MSG("\t\tDone.");
    
    const GLContextData& renderData = sceneData.renderData;
    meshData.drawParams.vaoId = renderData.vaos.front().gpu_id();
    meshData.vboId = renderData.vbos.back().gpu_id();
    
    MeshMetaData& metaData = meshData.metaData;

    LS_LOG_MSG(
        "\tSuccessfully sent an occlusion mesh to the GPU.",
        "\n\t\tInstances:   ", metaData.numSubmeshes,
        "\n\t\tVertices:    ", metaData.totalVerts,
        "\n\t\tVert Size:   ", metaData.calc_total_vertex_bytes(), " bytes"
        "\n\t\tIndices:     ", metaData.totalIndices,
        "\n\t\tIndex Size:  ", metaData.calc_total_index_bytes(), " bytes"
        "\n\t\tTotal Size:  ", metaData.calc_total_bytes(), " bytes",
        '\n'
    );

    return metaData.numSubmeshes;
}

/*-------------------------------------
-------------------------------------*/
void OcclusionMeshLoader::unload() noexcept {
    sceneData.terminate();
}



} // end draw namespace
} // end ls namespace
