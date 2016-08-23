/* 
 * File:   sceneGraph.cpp
 * Author: Miles Lacey
 * 
 * Created on January 19, 2015, 10:08 PM
 */

#include <utility>

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Copy.h"

#include "lightsky/draw/BoundingBox.h"
#include "lightsky/draw/Camera.h"
#include "lightsky/draw/SceneGraph.h"
#include "lightsky/draw/SceneMesh.h"
#include "lightsky/draw/SceneMaterial.h"
#include "lightsky/draw/SceneNode.h"
#include "lightsky/draw/Transform.h"
#include "lightsky/draw/VertexBuffer.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * SceneGraph Class
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
SceneGraph::~SceneGraph() noexcept {
    terminate();
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
SceneGraph::SceneGraph() noexcept :
    cameras(),
    meshes(),
    bounds(),
    materials(),
    nodes(),
    baseTransforms(),
    currentTransforms(),
    modelMatrices(),
    nodeNames(),
    animations(),
    nodeAnims(),
    nodeMeshCounts(),
    nodeMeshes(),
    renderData()
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
SceneGraph::SceneGraph(const SceneGraph& s) noexcept {
    *this = s;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
SceneGraph::SceneGraph(SceneGraph&& s) noexcept {
    *this = std::move(s);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
SceneGraph& SceneGraph::operator=(const SceneGraph& s) noexcept {
    cameras = s.cameras;
    meshes = s.meshes;
    bounds = s.bounds;
    materials = s.materials;
    nodes = s.nodes;
    baseTransforms = s.baseTransforms;
    currentTransforms = s.currentTransforms;
    modelMatrices = s.modelMatrices;
    nodeNames = s.nodeNames;
    animations = s.animations;
    nodeAnims = s.nodeAnims;
    
    LS_DEBUG_ASSERT(s.nodeMeshCounts.size() == s.nodeMeshes.size());
    nodeMeshCounts = s.nodeMeshCounts;
    
    nodeMeshes.reserve(s.nodeMeshes.size());
    
    for (unsigned i = 0; i < s.nodeMeshes.size(); ++i) {
        const utils::Pointer<DrawCommandParams[]>& inMeshes = s.nodeMeshes[i];
        const unsigned inMeshCount = s.nodeMeshCounts[i];
        
        LS_DEBUG_ASSERT(inMeshCount > 0);
        
        utils::Pointer<DrawCommandParams[]> outMeshes{new DrawCommandParams[inMeshCount]};
        
        utils::fast_copy(outMeshes.get(), inMeshes.get(), inMeshCount);
        
        nodeMeshCounts.push_back(inMeshCount);
        nodeMeshes.emplace_back(std::move(outMeshes));
    }
    
    renderData = s.renderData;
    
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
SceneGraph& SceneGraph::operator=(SceneGraph&& s) noexcept {
    cameras = std::move(s.cameras);
    meshes = std::move(s.meshes);
    bounds = std::move(s.bounds);
    materials = std::move(s.materials);
    nodes = std::move(s.nodes);
    baseTransforms = std::move(s.baseTransforms);
    currentTransforms = std::move(s.currentTransforms);
    modelMatrices = std::move(s.modelMatrices);
    nodeNames = std::move(s.nodeNames);
    animations = std::move(s.animations);
    nodeAnims = std::move(s.nodeAnims);
    nodeMeshCounts = std::move(s.nodeMeshCounts);
    nodeMeshes = std::move(s.nodeMeshes);
    renderData = std::move(s.renderData);
    
    return *this;
}

/*-------------------------------------
 * Terminate
-------------------------------------*/
void SceneGraph::terminate() noexcept {
    cameras.clear();
    meshes.clear();
    bounds.clear();
    materials.clear();
    nodes.clear();
    baseTransforms.clear();
    currentTransforms.clear();
    modelMatrices.clear();
    nodeNames.clear();
    animations.clear();
    nodeAnims.clear();
    nodeMeshCounts.clear();
    renderData.terminate();
}

/*-------------------------------------
 * Node updating
-------------------------------------*/
void SceneGraph::update_node_transform(const unsigned transformId) noexcept {
    draw::Transform& t = currentTransforms[transformId];
    
    const unsigned parentId = t.get_parent_id();
    const bool doesParentExist = parentId != draw::scene_property_t::SCENE_GRAPH_ROOT_ID;

    if (doesParentExist) {
        draw::Transform& pt = currentTransforms[parentId];
        
        // update all parent node data before attempting to update *this.
        if (pt.is_dirty()) {
            update_node_transform(parentId);
            t.set_dirty();
        }
    }

    // Early exit
    if (!t.is_dirty()) {
        return;
    }
    
    if (doesParentExist) {
        draw::Transform& pt = currentTransforms[parentId];
        t.apply_pre_transform(pt.get_transform());
    }
    else {
        t.apply_transform();
    }
    
    modelMatrices[transformId] = t.get_transform();

    // TODO: implement transformation packing so the iteration can stop as
    // soon as all child transforms have been updated. There's no reason
    // to iterate past the child transforms in the transform array.
    for (unsigned i = transformId+1; i < currentTransforms.size(); ++i) {
        draw::Transform& ct = currentTransforms[i];
        
        if (ct.get_parent_id() == transformId) {
            ct.set_dirty();
        }
    }
}

/*-------------------------------------
 * Scene Updating
-------------------------------------*/
void SceneGraph::update() noexcept {
    for (unsigned i = 0; i < currentTransforms.size(); ++i) {
        if (currentTransforms[i].is_dirty()) {
            update_node_transform(i);
        }
    }
    
    for (Camera& cam : cameras) {
        if (cam.is_dirty()) {
            cam.update();
        }
    }
}

/*-------------------------------------
 * Mesh Node Deletion
-------------------------------------*/
void SceneGraph::delete_mesh_node_data(const SceneNode& n) noexcept {
    nodeMeshCounts.erase(nodeMeshCounts.begin() + n.dataId);
    nodeMeshes.erase(nodeMeshes.begin() + n.dataId);
}

/*-------------------------------------
 * Camera Deletion
-------------------------------------*/
void SceneGraph::delete_camera_node_data(const SceneNode& n) noexcept {
    cameras.erase(cameras.begin() + n.dataId);
}

/*-------------------------------------
 * Animation Deletion
-------------------------------------*/
void SceneGraph::delete_node_animation_data(const SceneNode& n) noexcept {
    const uint32_t animId = n.animListId;
    
    // early exit
    if (animId == scene_property_t::SCENE_GRAPH_ROOT_ID) {
        return;
    }
    
    // Deal with all animation objects
    for (unsigned i = animations.size(); i --> 0;) {
        // Animations will automatically decrement transformation indices with
        // a value greater than the current node's dataId.
        animations[i].remove_anim_channel(n);
        
        // Remove any defunct animations
        if (!animations[i].get_num_anim_channels()) {
            animations.erase(animations.begin() + i);
        }
    }
    
    // Animation Channels
    if (n.animListId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
        nodeAnims.erase(nodeAnims.begin() + n.animListId);
    }
    
    // decrement the animation ID from all nodes with a value greater than the
    // current node's ID
    for (unsigned i = nodes.size(); i --> 0;) {
        if (nodes[i].animListId > animId && nodes[i].animListId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
            nodes[i].animListId -= 1;
        }
    }
}

/*-------------------------------------
 * Delete all nodes
-------------------------------------*/
void SceneGraph::clear_node_data() noexcept {
    cameras.clear();
    nodes.clear();
    baseTransforms.clear();
    currentTransforms.clear();
    modelMatrices.clear();
    nodeNames.clear();
    animations.clear();
    nodeAnims.clear();
    nodeMeshCounts.clear();
    nodeMeshes.clear();
}

/*-------------------------------------
 * Node Deletion
-------------------------------------*/
unsigned SceneGraph::delete_node(const unsigned nodeIndex) noexcept {
    unsigned numDeleted = 1;
    
    if (nodeIndex == scene_property_t::SCENE_GRAPH_ROOT_ID) {
        numDeleted = nodes.size();
        clear_node_data();
        return numDeleted;
    }
    
    if (nodeIndex >= nodes.size()) {
        return 0;
    }
    
    const SceneNode& n = nodes[nodeIndex];
    const unsigned nodeId = n.nodeId;
    
    for (unsigned i = nodes.size(); i --> nodeIndex;) {
        SceneNode& nextNode             = nodes[i];
        const unsigned nextTransform    = nextNode.nodeId;
        const unsigned nextParentId     = currentTransforms[nextTransform].get_parent_id();
        
        // Delete child nodes
        if (nextParentId == nodeId) {
            numDeleted += delete_node(i);
        }
    }
    
    // Delete any specific data associated with the node.
    switch (n.type) {
        case scene_node_t::NODE_TYPE_CAMERA:
            delete_camera_node_data(n);
            break;
            
        case scene_node_t::NODE_TYPE_MESH:
            delete_mesh_node_data(n);
            break;
            
        case scene_node_t::NODE_TYPE_EMPTY:
            break;
    }
    
    delete_node_animation_data(n);

    // Node Name
    nodeNames.erase(nodeNames.begin() + nodeId);
    
    // Node Base Transformation
    baseTransforms.erase(baseTransforms.begin() + nodeId);

    // Node Model Matrix
    modelMatrices.erase(modelMatrices.begin() + nodeId);
    
    // Decrement all node ID and data ID indices that are greater than those in
    // the current node. Also deal with the last bit of transformation data in
    // case a recursive deletion is in required.
    for (unsigned i = nodes.size(); i --> nodeIndex;) {
        SceneNode& nextNode             = nodes[i];
        const unsigned nextTransform    = nextNode.nodeId;
        const unsigned nextParentId     = currentTransforms[nextTransform].get_parent_id();
        
        // Delete child nodes
        if (nextParentId > nodeId && nextParentId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
            // decrement the next node's parent ID if necessary
            currentTransforms[nextTransform].set_parent_id(nextParentId - 1);
        }
        
        // Placing assertion here because nodeIds must never equate to the
        // root node ID. They must always have tangible data to point at.
        LS_DEBUG_ASSERT(nextNode.nodeId != scene_property_t::SCENE_GRAPH_ROOT_ID);
        
        if (nextNode.nodeId > nodeId) {
            nextNode.nodeId -= 1;
        }
        
        // the node dataId member can be equal to the root node ID. This is
        // because empty nodes may not have have data to use.
        if (nextNode.dataId > n.dataId && nextNode.dataId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
            nextNode.dataId -= 1;
        }
    }
    
    // Fin.
    currentTransforms.erase(currentTransforms.begin() + nodeId);
    
    nodes.erase(nodes.begin() + nodeIndex);
    
    return numDeleted;
}



} // end draw namepsace
} // end ls namespace
