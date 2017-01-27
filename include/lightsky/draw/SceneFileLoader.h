
/* 
 * File:   SceneFileLoader.h
 * Author: miles
 *
 * Created on April 24, 2016, 7:10 PM
 */

#ifndef __LS_DRAW_SCENE_GRAPH_LOADER_H__
#define __LS_DRAW_SCENE_GRAPH_LOADER_H__

#include <utility> // std::pair
#include <unordered_map>

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/Camera.h"
#include "lightsky/draw/SceneGraph.h"
#include "lightsky/draw/SceneMesh.h"
#include "lightsky/draw/SceneNode.h"
#include "lightsky/draw/SceneRenderData.h"



struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiNodeAnim;
struct aiScene;

namespace Assimp {
    class Importer;
} // end Assimp namespace



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
enum common_vertex_t : unsigned; // VertexUtils.h

class ImageBuffer;
class IndexBuffer;
struct SceneMaterial;
class TextureAssembly;
class VertexBuffer;



/**----------------------------------------------------------------------------
 * Condensed meta-information about a scene file.
-----------------------------------------------------------------------------*/
struct SceneFileMetaData {
    uint32_t totalVboBytes  = 0;
    uint32_t totalVertices  = 0;
    uint32_t totalIboBytes  = 0;
    uint32_t totalIndices   = 0;
    index_element_t indexType = index_element_t::INDEX_TYPE_NONE;
};

/**----------------------------------------------------------------------------
 * A VboGroup is an intermediate structure to help determine which group of
 * vertices in an Assimp mesh belong to which section of a VBO's memory buffer.
-----------------------------------------------------------------------------*/
struct VboGroupMarker {
    common_vertex_t vertType;
    unsigned numVboBytes;
    unsigned vboOffset;
    unsigned meshOffset;
    unsigned baseVert;
    
    ~VboGroupMarker() noexcept;
    
    VboGroupMarker() noexcept;
    
    VboGroupMarker(const VboGroupMarker& v) noexcept;
    
    VboGroupMarker(VboGroupMarker&& v) noexcept;
    
    VboGroupMarker& operator=(const VboGroupMarker& v) noexcept;
    
    VboGroupMarker& operator=(VboGroupMarker&& v) noexcept;
};



/**----------------------------------------------------------------------------
 * Preloading structure which allows a file to load in a separate thread.
-----------------------------------------------------------------------------*/
class SceneFilePreLoader {

    friend class SceneFileLoader;

    private:
        std::string filepath;

        utils::Pointer<Assimp::Importer> importer;

        SceneFileMetaData sceneInfo;

        SceneGraph sceneData;

        std::string baseFileDir;

        std::vector<VboGroupMarker> vboMarkers{};

        std::unordered_map<std::string, unsigned> texturePaths;

        const aiScene* preload_mesh_data() noexcept;

        bool allocate_cpu_data(const aiScene* const pScene) noexcept;

    public:
        /**
         * @brief Destructor
         *
         * Unloads all data contain within *this.
         */
        ~SceneFilePreLoader() noexcept;

        /**
         * @brief Constructor
         *
         * Initializes all members contained within *this.
         */
        SceneFilePreLoader() noexcept;

        /**
         * @brief Copy Constructor
         *
         * Deleted to reduce excess memory allocations.
         */
        SceneFilePreLoader(const SceneFilePreLoader&) noexcept = delete;

        /**
         * @brief Move Constructor
         *
         * Moves all data from the input parameter into *this. No copies are
         * performed during the operation.
         *
         * @param r
         * An r-value reference to a temporary SceneFilePreLoader object.
         */
        SceneFilePreLoader(SceneFilePreLoader&& s) noexcept;

        /**
         * @brief Copy Operator
         *
         * Deleted to reduce excess memory allocations.
         */
        SceneFilePreLoader& operator=(const SceneFilePreLoader& s) noexcept = delete;

        /**
         * @brief Move Operator
         *
         * Moves all data from the input parameter into *this. No copies are
         * performed during the operation.
         *
         * @param s
         * An r-value reference to a temporary SceneFilePreLoader object.
         *
         * @return a reference to *this.
         */
        SceneFilePreLoader& operator=(SceneFilePreLoader&& s) noexcept;

        /**
         * @brief Unload/free all memory used by *this.
         */
        void unload() noexcept;

        /**
         * @brief Load a 3D mesh file
         *
         * @param filename
         * A string object containing the relative path name to a file that
         * should be loadable into memory.
         *
         * @return true if the file was successfully loaded into memory. False
         * if not.
         */
        bool load(const std::string& filename) noexcept;

        /**
         * @brief Verify that data loaded successfully.
         *
         * @return TRUE if *this object contains data on he CPU which can be
         * passed to a SceneFileLoader object for GPU loading, FALSE if not.
         */
        bool is_loaded() const noexcept;
};



/**----------------------------------------------------------------------------
 * The scene file loader  can be used to load a 3D scene from a file.
-----------------------------------------------------------------------------*/
class SceneFileLoader {
    
    // Private Variables
    private:
        SceneFilePreLoader preloader;
        
    // Private functions
    private:
        bool load_scene(const aiScene* const pScene) noexcept;
        
        bool allocate_gpu_data() noexcept;
        
        bool import_materials(const aiScene* const pScene) noexcept;
        
        void import_texture_path(const aiMaterial* const pMaterial, const int slotType, SceneMaterial& outMaterial, ImageBuffer& imgLoader, TextureAssembly& texAssembly) noexcept;
        
        unsigned load_texture_at_path(const std::string& path, ImageBuffer& imgLoader, TextureAssembly& texAssembly, const tex_wrap_t wrapMode) noexcept;
        
        bool import_mesh_data(const aiScene* const pScene) noexcept;
        
        char* upload_mesh_indices(const aiMesh* const pMesh, char* pIbo, const unsigned baseIndex, const unsigned baseVertex, SceneMesh& outMesh) noexcept;

        unsigned get_mesh_group_marker(const common_vertex_t vertType, const std::vector<VboGroupMarker>& markers) const noexcept;

        /**
         * @brief Recursively reads and imports scene graph data from Assimp.
         *
         * @param pScene
         * A constant pointer to a constant aiScene object from ASSIMP.
         *
         * @param pNode
         * A pointer to a node in an Assimp scene graph.
         *
         * @param parentId
         * An index value of the parent sceneNode contained within an internal
         * array of nodes.
         *
         * @return The index of the the last child node recursively placed into
         * the scene node list.
         */
        void read_node_hierarchy(const aiScene* const pScene, const aiNode* const pNode, const unsigned parentId) noexcept;
        
        /**
         * @brief Import a sceneMeshNode object if an ASSIMP node contains
         * meshes.
         *
         * @param pNode
         * A constant pointer to an assimp mesh node.
         * 
         * @param outNode
         * A reference to the SceneNode object which will have the remainder of
         * its data initialized through this method.
         */
        void import_mesh_node(const aiNode* const pNode, SceneNode& outNode) noexcept;
        
        /**
         * @brief Import/convert a Camera node from ASSIMP.
         *
         * @param pScene
         * A constant pointer to an assimp scene from which camera and node
         * information will be loaded.
         *
         * @param camIndex
         * An index value of the camera object in ASSIMP which the imported
         * SceneNode should draw its data from.
         * 
         * @param outNode
         * A reference to the SceneNode object which will have the remainder of
         * its data initialized through this method.
         */
        void import_camera_node(const aiScene* const pScene, const unsigned camIndex, SceneNode& outNode) noexcept;

        /**
         * @brief Import all animations contained within a 3D scene file,
         * provided by assimp.
         *
         * @param pScene
         * A constant pointer to a constant ASSIMP scene structure.
         *
         * @return TRUE if all animations were successfully imported, FALSE if
         * not.
         */
        bool import_animations(const aiScene* const pScene) noexcept;

        /**
         * @brief Import a single Animation track from ASSIMP.
         *
         * @param pInAnim
         * A constant pointer to a constant Animation track.
         *
         * @param outAnim
         * A reference to the animationReel object which is to contain batched
         * Animation data for a single track.
         * 
         * @param animDuration
         * Contains the total duration of the animation, in ticks.
         *
         * @return The ID of a node who's track was successfully imported,
         * UINT_MAX if not.
         */
        unsigned import_animation_track(
            const aiNodeAnim* const pInAnim,
            AnimationChannel& outAnim,
            const anim_prec_t animDuration
        ) noexcept;
        
    public:
        /**
         * @brief Destructor
         * 
         * Unloads all data contain within *this.
         */
        ~SceneFileLoader() noexcept;
        
        /**
         * @brief Constructor
         * 
         * Initializes all members contained within *this.
         */
        SceneFileLoader() noexcept;
        
        /**
         * @brief Copy Constructor
         *
         * Deleted to reduce excess memory allocations.
         */
        SceneFileLoader(const SceneFileLoader& s) noexcept = delete;
        
        /**
         * @brief Move Constructor
         * 
         * Moves all data from the input parameter into *this. No copies are
         * performed during the operation.
         * 
         * @param r
         * An r-value reference to a temporary SceneFileLoader object.
         */
        SceneFileLoader(SceneFileLoader&& s) noexcept;
        
        /**
         * @brief Copy Operator
         *
         * Deleted to reduce excess memory allocations.
         */
        SceneFileLoader& operator=(const SceneFileLoader& s) noexcept = delete;
        
        /**
         * @brief Move Operator
         * 
         * Moves all data from the input parameter into *this. No copies are
         * performed during the operation.
         * 
         * @param s
         * An r-value reference to a temporary SceneFileLoader object.
         * 
         * @return a reference to *this.
         */
        SceneFileLoader& operator=(SceneFileLoader&& s) noexcept;
        
        /**
         * @brief Unload/free all memory used by *this.
         */
        void unload() noexcept;

        /**
         * @brief Load a 3D mesh file
         *
         * @param filename
         * A string object containing the relative path name to a file that
         * should be loadable into memory.
         *
         * @return true if the file was successfully loaded. False if not.
         */
        bool load(const std::string& filename) noexcept;

        /**
         * @brief Import in-memory mesh data, preloaded from a file.
         *
         * @param preload
         * An r-value reference to a scene preloader which is ready to be sent
         * to the GPU.
         *
         * @return true if the data was successfully loaded. False if not.
         */
        bool load(SceneFilePreLoader&& preload) noexcept;

        /**
         * @brief get_loaded_data() allows the loaded scene graph to be
         * retrieved by reference.
         *
         * @return A constant reference to the loaded scene grpah object which
         * can be used for validation, rendering, or something else.
         */
        const SceneGraph& get_loaded_data() const noexcept;

        /**
         * @brief get_loaded_data() allows the loaded scene graph to be
         * retrieved by reference.
         *
         * @return A reference to the loaded scene grpah object which can be
         * used for validation, rendering, or something else.
         */
        SceneGraph& get_loaded_data() noexcept;
};



/*-------------------------------------
 * Retrieve the loaded scene data (const)
-------------------------------------*/
inline const SceneGraph& SceneFileLoader::get_loaded_data() const noexcept {
    return preloader.sceneData;
}



/*-------------------------------------
 * Retrieve the loaded scene data
-------------------------------------*/
inline SceneGraph& SceneFileLoader::get_loaded_data() noexcept {
    return preloader.sceneData;
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SCENE_GRAPH_LOADER_H__ */
