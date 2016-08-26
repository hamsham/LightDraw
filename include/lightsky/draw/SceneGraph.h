
#ifndef __LS_DRAW_SCENE_GRAPH_H__
#define __LS_DRAW_SCENE_GRAPH_H__

#include <climits> // UINT_MAX
#include <vector>

#include "lightsky/draw/Animation.h"
#include "lightsky/draw/GLContext.h"
#include "lightsky/draw/DrawParams.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward declarations
-----------------------------------------------------------------------------*/
class Camera;
struct DrawCommandParams;
struct SceneMesh;
struct SceneMaterial;
struct SceneNode;
class Transform;
class BoundingBox;



/*-----------------------------------------------------------------------------
 * Enumerations for data management
-----------------------------------------------------------------------------*/
enum scene_property_t : unsigned {
    SCENE_GRAPH_ROOT_ID = UINT_MAX
};



/**----------------------------------------------------------------------------
 * @brief the SceneGraph object contains all of the data necessary to either
 * instantiate or render SceneNodes in an OpenGL context.
-----------------------------------------------------------------------------*/
class SceneGraph {
    public: // member objects
        /**
         * Referenced by camera-type scene nodes using their
         * "SceneNode::dataId" member.
         * 
         * No two nodes should be able to reference a single camera. Doing so
         * will cause a crash when deleting nodes.
         */
        std::vector<Camera> cameras;

        /**
         * Array to contain all meshes referenced by mesh node draw commands.
         */
        std::vector<SceneMesh> meshes;

        /**
         * Bounding boxes for meshes
         */
        std::vector<BoundingBox> bounds;

        /**
         * Referenced by all mesh node types using the following relationship:
         *      "SceneGraph::nodeMeshes[SceneNode::nodeId]->materialId"
         */
        std::vector<SceneMaterial> materials;

        /**
         * Contains all empty, camera, mesh, and bode nodes in a scene graph.
         * 
         * @note Parent nodes must always have a lower array index value than
         * their children. This allows for the transformation update routines
         * to reduce the number of recursive iterations required to update
         * child nodes.
         */
        std::vector<SceneNode> nodes;

        /**
         * Referenced by all scene node types using their
         * "SceneNode::nodeId" member. Base Transformations are not expected to
         * maintain a reference to their parent transform.
         */
        std::vector<math::mat4> baseTransforms;

        /**
         * Referenced by all scene node types using their
         * "SceneNode::nodeId" member. The current transformation for a scene
         * node is expected to keep track of its parent transformation.
         */
        std::vector<Transform> currentTransforms;

        /**
         * Referenced by all scene node types using their
         * "SceneNode::nodeId" member.
         */
        std::vector<math::mat4> modelMatrices;

        /**
         * Referenced by all scene node types using their
         * "SceneNode::nodeId" member.
         */
        std::vector<std::string> nodeNames;
        
        /**
         * Contains all animations available in the current scene graph.
         */
        std::vector<Animation> animations;
        
        /**
         * Referenced by all scene node types using their
         * "SceneNode::animTrackId" member.
         */
        std::vector<std::vector<AnimationChannel>> nodeAnims;

        /**
         * Referenced by mesh-type scene nodes using their
         * "SceneNode::dataId" member.
         * 
         * No two nodes should be able to reference the same mesh count index.
         * Doing so will cause a crash when deleting nodes.
         */
        std::vector<unsigned> nodeMeshCounts;

        /**
         * Referenced by mesh-type scene nodes using their
         * "SceneNode::dataId" member.
         * 
         * No two nodes should be able to reference the same DrawCommandParam
         * array. Doing so will cause a crash when deleting nodes.
         */
        std::vector<utils::Pointer<DrawCommandParams[]>> nodeMeshes;

        /**
         * Accessor for all OpenGL data
         */
        GLContextData renderData;
    
    private: // member functions
        /**
         * Update the transformation of a single node in the transformation
         * hierarchy.
         * 
         * @param transformId
         * An array index which will determine which transform is currently being
         */
        void update_node_transform(const unsigned transformId) noexcept;
        
        /**
         * Remove all data specific to mesh nodes.
         * 
         * @param nodeDataId
         * An unsigned integer, containing the data index of a node being
         * deleted.
         */
        void delete_mesh_node_data(const unsigned nodeDataId) noexcept;
        
        /**
         * Remove all data specific to camera nodes.
         * 
         * @param nodeDataId
         * An unsigned integer, containing the data index of a node being
         * deleted.
         */
        void delete_camera_node_data(const unsigned nodeDataId) noexcept;
        
        /**
         * Remove all animation data pertaining to the current node.
         * 
         * @param nodeId
         * The array index of a node being deleted.
         * 
         * @param animId
         * The array index of the animation being deleted.
         */
        void delete_node_animation_data(const uint32_t nodeId, const uint32_t animId) noexcept;

    public: // member functions
        /**
         * @brief Destructor
         * 
         * Calls 'terminate()' to delete all CPU and GPU-side resources.
         */
        ~SceneGraph() noexcept;

        /**
         * Constructor
         * 
         * Initializes all members in *this to their default values.
         */
        SceneGraph() noexcept;

        /**
         * Copy Constructor
         * 
         * @param s
         * A constant reference to another scene graph which will be used to
         * initialize *this.
         * 
         * This function will incur a large runtime overhead in order to copy all
         * dynamically allocated resources on both the CPU and GPU.
         */
        SceneGraph(const SceneGraph& s) noexcept;

        /**
         * Move Constructor
         * 
         * Initializes *this using data from the input parameter. No copies or data
         * reallocations will be performed.
         * 
         * @param s
         * An r-value reference to another scene graph object who's data will be
         * moved into *this.
         */
        SceneGraph(SceneGraph&& s) noexcept;

        /**
         * Copy Operator
         * 
         * @param s
         * A constant reference to another scene graph which will be used to
         * initialize *this.
         * 
         * This function will incur a large runtime overhead in order to copy all
         * dynamically allocated resources on both the CPU and GPU.
         * 
         * @return A reference to *this.
         */
        SceneGraph& operator=(const SceneGraph& s) noexcept;

        /**
         * Move Operator
         * 
         * Moves all data from the input parameter into *this. No copies or data
         * reallocations will be performed.
         * 
         * @param s
         * An r-value reference to another scene graph object who's data will be
         * moved into *this.
         * 
         * @return A reference to *this.
         */
        SceneGraph& operator=(SceneGraph&& s) noexcept;

        /**
         * @brief Terminate A scene graph by cleaning up all CPu and GPU-side
         * resources.
         */
        void terminate() noexcept;
        
        /**
         * Remove all data related to scene nodes. This includes:
         * cameras, transformations, node names, animations, node meshes.
         * 
         * All render data and bounding boxes will remain intact.
         */
        void clear_node_data() noexcept;

        /**
         * Update all scene nodes in *this scene graph.
         * 
         * All nodes in the scene graph will have their transformations updated and
         * placed into the modelMatrices array.
         */
        void update() noexcept;
        
        /**
         * Remove a node from the scene graph.
         * 
         * This function will remove all children related to the current node.
         * 
         * @param nodeIndex
         * An unsigned integral type, containing the array-index of the node to
         * remove from the graph.
         * 
         * @return The total number of nodes which were deleted.
         */
        unsigned delete_node(const unsigned nodeIndex) noexcept;
        
        /**
         * Search for a node by its name and return its index.
         * 
         * @param nameQuery
         * A constant reference to an std::string object, containing the name
         * of the node to search for.
         * 
         * @return The array-index of the node being searched for, or
         * SCENE_GRAPH_ROOT_ID if the node was not found.
         */
        unsigned find_node_id(const std::string& nameQuery) const noexcept;
};



} // end draw namepsace
} // end ls namespace

#endif	/* __LS_DRAW_SCENE_GRAPH_H__ */
