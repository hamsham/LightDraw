/* 
 * File:   sceneGraph.cpp
 * Author: Miles Lacey
 * 
 * Created on January 19, 2015, 10:08 PM
 */

#include <algorithm>

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



/*-----------------------------------------------------------------------------
 * Anonymous utility functions
-----------------------------------------------------------------------------*/
namespace {
/*
void print_list(const std::vector<ls::draw::SceneNode>& l) {
    for (const ls::draw::SceneNode& n : l) {
        std::cout << n.nodeId << ' ';
    }

    std::cout << std::endl;
}



void print_list(const std::vector<ls::draw::Transform>& l) {
    for (const ls::draw::Transform& t : l) {
        std::cout << t.parentId << ' ';
    }

    std::cout << std::endl;
}
*/



/*-------------------------------------
-------------------------------------*/
template <typename list_t>
void rotate_list(
    list_t& vec,
    const unsigned start,
    const unsigned length,
    const unsigned dest
) {
    typename list_t::iterator first, middle, last;

    if (start < dest) {
        first  = vec.begin() + start;
        middle = first       + length;
        last   = vec.begin() + dest;
    }
    else {
        first  = vec.begin() + dest;
        middle = vec.begin() + start;
        last   = middle      + length;
    }

    std::rotate(first, middle, last);
}



} // end anonymous namespace



/*-----------------------------------------------------------------------------
 * SceneGraph Class
-----------------------------------------------------------------------------*/
namespace ls {
namespace draw {
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
    
    const unsigned parentId = t.parentId;
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
        
        if (ct.parentId == transformId) {
            ct.set_dirty();
        }
    }
}

/*-------------------------------------
 * Scene Updating
-------------------------------------*/
void SceneGraph::update() noexcept {
    // Transformation indices have a 1:1 relationship with node indices.
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
void SceneGraph::delete_mesh_node_data(const unsigned nodeDataId) noexcept {
    nodeMeshCounts.erase(nodeMeshCounts.begin() + nodeDataId);
    nodeMeshes.erase(nodeMeshes.begin() + nodeDataId);
}

/*-------------------------------------
 * Camera Deletion
-------------------------------------*/
void SceneGraph::delete_camera_node_data(const unsigned nodeDataId) noexcept {
    cameras.erase(cameras.begin() + nodeDataId);
}

/*-------------------------------------
 * Animation Deletion
-------------------------------------*/
void SceneGraph::delete_node_animation_data(const uint32_t nodeId, const uint32_t animId) noexcept {
    // Remove all animation channels associated with the current node.
    for (unsigned i = animations.size(); i --> 0;) {
        
        // Animations contain transformation IDs which may need to be
        // decremented. Search for transformation indices with a value greater
        // than the current node's nodeId.
        Animation& currentAnim = animations[i];
        
        // friendly class member manipulation
        std::vector<uint32_t>& currentAnimIds = currentAnim.animationIds;
        std::vector<uint32_t>& currentTransIds = currentAnim.transformIds;
        
        // Reduce the animation ID of all animations in *this.
        for (uint32_t j = currentAnimIds.size(); j --> 0;) {
            if (currentTransIds[j] == nodeId) {
                currentAnim.remove_anim_channel(j);
                continue;
            }
            
            if (currentTransIds[j] > nodeId) {
                --currentTransIds[j];
            }
            
            if (animId != scene_property_t::SCENE_GRAPH_ROOT_ID && currentAnimIds[j] > animId) {
                --currentAnimIds[j];
            }
        }
        
        // Remove any defunct animations
        if (!currentAnim.get_num_anim_channels()) {
            animations.erase(animations.begin() + i);
        }
    }
    
    // Animation Channels
    if (animId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
        nodeAnims.erase(nodeAnims.begin() + animId);
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
    
    // No mercy for client code
    LS_DEBUG_ASSERT(nodeIndex < nodes.size());
    
    // Remove all child nodes and their data
    for (unsigned i = nodes.size(); i --> nodeIndex;) {
        if (currentTransforms[i].parentId == nodeIndex) {
            numDeleted += delete_node(i);
        }
    }
    
    const SceneNode& n          = nodes[nodeIndex];
    const scene_node_t typeId   = n.type;
    const uint32_t dataId       = n.dataId;
    const uint32_t animId       = n.animListId;
    
    LS_DEBUG_ASSERT(nodeIndex == n.nodeId);
    
    // Delete any specific data associated with the node.
    switch (typeId) {
        case scene_node_t::NODE_TYPE_CAMERA:
            delete_camera_node_data(dataId);
            break;
            
        case scene_node_t::NODE_TYPE_MESH:
            delete_mesh_node_data(dataId);
            break;
            
        case scene_node_t::NODE_TYPE_EMPTY:
            break;
    }
    
    // Delete the actual node
    nodes.erase                 (nodes.begin()              + nodeIndex);
    currentTransforms.erase     (currentTransforms.begin()  + nodeIndex);
    baseTransforms.erase        (baseTransforms.begin()     + nodeIndex);
    modelMatrices.erase         (modelMatrices.begin()      + nodeIndex);
    nodeNames.erase             (nodeNames.begin()          + nodeIndex);
    
    // early exit in case there are no animations tied to the current node.
    delete_node_animation_data(nodeIndex, animId);
    
    // Decrement all node ID and data ID indices that are greater than those in
    // the current node. Also deal with the last bit of transformation data in
    // case a recursive deletion is in required.
    for (unsigned i = nodes.size(); i --> nodeIndex;) {
        SceneNode& nextNode         = nodes[i];
        const scene_node_t nextType = nextNode.type;
        unsigned& nextDataId        = nextNode.dataId;
        unsigned& nextAnimId        = nextNode.animListId;
        Transform& nextTransform    = currentTransforms[i];
        const unsigned nextParentId = nextTransform.parentId;
        
        // Placing assertion here because nodeIds must never equate to the
        // root node ID. They must always have tangible data to point at.
        LS_DEBUG_ASSERT(nextNode.nodeId != scene_property_t::SCENE_GRAPH_ROOT_ID);
        
        nodes[i].nodeId = i;
        
        if (nextParentId > nodeIndex && nextParentId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
            // decrement the next node's parent ID if necessary
            nextTransform.parentId = nextParentId - 1;
        }
        
        // the node dataId member can be equal to the root node ID. This is
        // because empty nodes may not have have data to use.
        if (nextType == typeId
        && nextDataId > dataId
        && nextDataId != scene_property_t::SCENE_GRAPH_ROOT_ID
        ) {
            --nextDataId;
        }
        
        // decrement the animation ID from all nodes with a value greater than
        // the current node's
        if (nextAnimId > animId && nextAnimId != scene_property_t::SCENE_GRAPH_ROOT_ID) {
            --nextAnimId;
        }
    }
    
    return numDeleted;
}

/*-------------------------------------
 * Node Parenting
-------------------------------------*/
bool SceneGraph::reparent_node(const unsigned nodeIndex, const unsigned newParentId) noexcept {
    if (nodeIndex == scene_property_t::SCENE_GRAPH_ROOT_ID) {
        return false;
    }
    
    if (node_is_child(newParentId, nodeIndex)) {
        LS_LOG_MSG("Cannot make a node ", nodeIndex, " a parent of its ancestor ", newParentId, '.');
        return false;
    }

    LS_DEBUG_ASSERT(nodeIndex < nodes.size());

    const unsigned numChildren      = get_num_total_children(nodeIndex);
    const unsigned displacement     = 1 + numChildren;
    const unsigned numNewSiblings   = get_num_total_children(newParentId);
    const unsigned newNodeIndex     = 1 + newParentId + numNewSiblings;

    // Keep track of the range of elements which need to be updated.
    const unsigned effectStart      = math::min(nodeIndex, newNodeIndex);
    const unsigned effectEnd        = math::max(newNodeIndex, nodeIndex+displacement);
    
    // Sentinel to help determine not to modify the node at 'nodeIndex'
    const bool movingUp             = nodeIndex < newParentId;
    
    /*
    LS_LOG_MSG(
        "Re-parenting node ", nodeIndex, " from ", currentTransforms[nodeIndex].parentId, " to ", newParentId,
        ".\nRotating ", displacement, " indices from ", nodeIndex, " to ", newNodeIndex,
        " and updating node indices between ", effectStart, '-', effectEnd
    );
    */

    rotate_list(nodes,              nodeIndex, displacement, newNodeIndex);
    rotate_list(baseTransforms,     nodeIndex, displacement, newNodeIndex);
    rotate_list(currentTransforms,  nodeIndex, displacement, newNodeIndex);
    rotate_list(modelMatrices,      nodeIndex, displacement, newNodeIndex);
    
    for (unsigned i = effectStart; i < effectEnd; ++i) {
        unsigned& rParentId = currentTransforms[i].parentId;
        const unsigned pId  = rParentId;
        const unsigned nId  = nodes[i].nodeId;
        nodes[i].nodeId     = i;

        // Update the requested node's index
        if (nId == nodeIndex) {
            rParentId = movingUp ? (newParentId-displacement) : newParentId;
            currentTransforms[i].set_dirty();
            continue;
        }

        // Determine if there's a node which even needs its parent ID updated.
        if (pId == scene_property_t::SCENE_GRAPH_ROOT_ID || pId < effectStart) {
            continue;
        }
        
        const unsigned parentDelta = nId - pId;

        //LS_LOG_MSG("\t\tMoving ", i, "'s (", nId, ") parent ID by ", parentDelta, " from ", pId, " to ", i-parentDelta);
        rParentId = i - parentDelta;
        
        currentTransforms[i].set_dirty();
    }
    
    /*
    std::cout << "To this:";
    std::cout << "\n\t"; print_list(nodes);
    std::cout << "\t"; print_list(currentTransforms);
    */
    
    // Animations need love too
    static_assert(sizeof(unsigned) == sizeof(uint32_t), "Cannot update indices of differing types.");
    
    for (Animation& anim : animations) {
        for (uint32_t& oldId : anim.transformIds) {
            const uint32_t newId = nodes[oldId].nodeId;
            const uint32_t nodeDelta = math::max(oldId, newId) - math::min(oldId, newId);
            oldId = (newId >= oldId) ? (oldId+nodeDelta) : (oldId-nodeDelta);
        }
    }
    
    //LS_LOG_MSG("\tDone.");
    
    LS_DEBUG_ASSERT(newNodeIndex <= nodes.size());
    
    return true;
}



/*-------------------------------------
 * Node Searching
-------------------------------------*/
unsigned SceneGraph::find_node_id(const std::string& nameQuery) const noexcept {
    unsigned nodeId = scene_property_t::SCENE_GRAPH_ROOT_ID;
    
    for (unsigned i = nodeNames.size(); i --> 0;) {
        if (nodeNames[i] == nameQuery) {
            return i;
        }
    }
    
    return nodeId;
}

/*-------------------------------------
 * Node Child Counting (total)
-------------------------------------*/
unsigned SceneGraph::get_num_total_children(const unsigned nodeIndex) const noexcept {
    if (nodeIndex == scene_property_t::SCENE_GRAPH_ROOT_ID) {
        return nodes.size();
    }

    unsigned numChildren = 0;
    
    for (unsigned cId = nodeIndex+1; cId < currentTransforms.size(); ++cId) {
        const unsigned pId = currentTransforms[cId].parentId;

        if (pId < nodeIndex || pId == scene_property_t::SCENE_GRAPH_ROOT_ID) {
            break;
        }

        //LS_LOG_MSG("Node ", nodeIndex, " has a child at ", cId);

        numChildren++;
    }

    return numChildren;
}

/*-------------------------------------
 * Node Child Counting (immediate)
-------------------------------------*/
unsigned SceneGraph::get_num_immediate_children(const unsigned nodeIndex) const noexcept {
    if (nodeIndex == scene_property_t::SCENE_GRAPH_ROOT_ID) {
        return nodes.size();
    }

    const unsigned startIndex   = nodeIndex+1;
    const unsigned stopIndex    = nodeIndex;
    unsigned numChildren        = 0;
    
    for (unsigned cId = startIndex; cId < currentTransforms.size(); ++cId) {
        const unsigned pId = currentTransforms[cId].parentId;

        if (pId == scene_property_t::SCENE_GRAPH_ROOT_ID || pId < stopIndex) {
            break;
        }

        //LS_LOG_MSG("Node ", nodeIndex, " has a child at ", cId);
        if (pId == nodeIndex) {
            numChildren++;
        }
    }

    return numChildren;
}

/*-------------------------------------
 * Check if a node is a child of another node
-------------------------------------*/
bool SceneGraph::node_is_child(const unsigned nodeIndex, const unsigned parentId) const noexcept {
    // parent IDs are always less than their child IDs.
    if (nodeIndex == scene_property_t::SCENE_GRAPH_ROOT_ID || parentId >= nodeIndex) {
        return false;
    }
    
    // all nodes are children of the root node
    if (parentId == scene_property_t::SCENE_GRAPH_ROOT_ID) {
        return true;
    }
    
    const unsigned pId = currentTransforms[nodeIndex].parentId;
    
    // check for ancestry
    if (pId < parentId) {
        return false;
    }
    
    // check for immediate parenting
    if (pId == parentId) {
        return true;
    }
    
    for (unsigned iter = pId; iter != scene_property_t::SCENE_GRAPH_ROOT_ID;) {
        if (iter == parentId) {
            return true;
        }
        
        iter = currentTransforms[iter].parentId;
    }
    
    return false;
}




} // end draw namepsace
} // end ls namespace
