
/* 
 * File:   OcclusionMeshLoader.h
 * Author: miles
 *
 * Created on March 11, 2016, 11:10 PM
 */

#ifndef __LS_DRAW_OCCLUSION_MESH_LOADER_H__
#define __LS_DRAW_OCCLUSION_MESH_LOADER_H__

#include "ls/draw/BufferObject.h" // buffer_map_t
#include "ls/draw/SceneMesh.h"
#include "ls/draw/SceneGraph.h"

namespace ls {
namespace draw {


enum occlusion_property_t {
    OCCLUSION_BOX_NUM_VERTS = 16
};



class OcclusionMeshLoader {
    public:
        static constexpr buffer_map_t DEFAULT_VBO_MAP_FLAGS = (buffer_map_t) (0
            | VBO_MAP_BIT_INVALIDATE_RANGE
            | VBO_MAP_BIT_UNSYNCHRONIZED
            | VBO_MAP_BIT_INVALIDATE_BUFFER
            | VBO_MAP_BIT_WRITE
            | 0);

    private:
        SceneGraph sceneData;

        /**
         * @brief Generate information about what it takes to store occlusion
         * geometry on the GPU with certain vertex data.
         *
         * @param metaData
         * A reference to a TextMetaData object which will be used to store the
         * generated textual meta-information.
         */
        static void generate_meta_data(MeshMetaData& md, const unsigned numInstances);

        bool allocate_cpu_data(const unsigned numInstances);

        bool allocate_gpu_data(const unsigned numInstances);

        bool init_cube_vbo(VertexBuffer& cubeVbo);

        bool init_bounds_vbo(VertexBuffer& boundsVbo, const unsigned numInstances);

        bool assemble_vao();

      public:
        /**
         * Destructor
         * 
         * Clears all CPU-side data from *this. A manual call to "unload()" is
         * required to free GPU-side data.
         */
        ~OcclusionMeshLoader() noexcept;

        /**
         * Constructor
         * 
         * Initializes all internal members to their default states.
         */
        OcclusionMeshLoader() noexcept;

        /**
         * Copy Constructor
         * 
         * Copies data from the input parameter into *this.
         * 
         * @param l
         * An l-value reference to another OcclusionMeshLoader object.
         * 
         * @return A reference to *this.
         */
        OcclusionMeshLoader(const OcclusionMeshLoader& l) noexcept;

        /**
         * Move Constructor
         * 
         * Moves data from the input parameter into *this.
         * 
         * @param l
         * An r-value reference to another OcclusionMeshLoader object.
         */
        OcclusionMeshLoader(OcclusionMeshLoader&& l) noexcept;

        /**
         * Copy Operator
         * 
         * Copies data from the input parameter into *this.
         * 
         * @param l
         * An l-value reference to another OcclusionMeshLoader object.
         * 
         * @return A reference to *this.
         */
        OcclusionMeshLoader& operator=(const OcclusionMeshLoader& l) noexcept;

        /**
         * Move Operator
         * 
         * Moves data from the input parameter into *this.
         * 
         * @param l
         * An r-value reference to another OcclusionMeshLoader object.
         * 
         * @return A reference to *this.
         */
        OcclusionMeshLoader& operator=(OcclusionMeshLoader&& l) noexcept;

        unsigned load(unsigned numObjects = 1) noexcept;

        void unload() noexcept;

        const SceneGraph& get_mesh() const noexcept;

        SceneGraph& get_mesh() noexcept;
};

/*-------------------------------------
 * Retrieve the currently loaded mesh (const)
-------------------------------------*/
inline const SceneGraph& OcclusionMeshLoader::get_mesh() const noexcept {
    return sceneData;
}

/*-------------------------------------
 * Retrieve the currently loaded mesh
-------------------------------------*/
inline SceneGraph& OcclusionMeshLoader::get_mesh() noexcept {
    return sceneData;
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_OCCLUSION_MESH_LOADER_H__ */
