
#ifndef __LS_DRAW_SCENE_GRAPH_H__
#define __LS_DRAW_SCENE_GRAPH_H__

#include <climits> // UINT_MAX
#include <vector>

#include "ls/draw/Animation.h"
#include "ls/draw/GLContext.h"
#include "ls/draw/DrawParams.h"



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
        std::vector<Camera> cameras;

        std::vector<SceneMesh> meshes;

        std::vector<BoundingBox> bounds;

        std::vector<SceneMaterial> materials;

        std::vector<SceneNode> nodes;

        std::vector<math::mat4> baseTransforms;

        std::vector<Transform> currentTransforms;

        std::vector<math::mat4> modelMatrices;

        std::vector<std::string> nodeNames;
        
        std::vector<Animation> animations;

        std::vector<unsigned> nodeMeshCounts;

        std::vector<utils::Pointer<DrawCommandParams[]>> nodeMeshes;

        GLContextData renderData;
    
    private:
      /**
       * Update the transformation of a single node in the transformation
       * hierarchy.
       * 
       * @param transformId
       * An array index which will determine which transform is currently being
       */
      void update_node_transform(const unsigned transformId) noexcept;

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
         * Update all scene nodes in *this scene graph.
         * 
         * All nodes in the scene graph will have their transformations updated and
         * placed into the modelMatrices array.
         */
        void update() noexcept;
};



} // end draw namepsace
} // end ls namespace

#endif	/* __LS_DRAW_SCENE_GRAPH_H__ */