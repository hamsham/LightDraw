/* 
 * File:   sceneGraph.cpp
 * Author: Miles Lacey
 * 
 * Created on January 19, 2015, 10:08 PM
 */

#include <utility>

#include "ls/utils/Copy.h"

#include "ls/draw/BoundingBox.h"
#include "ls/draw/Camera.h"
#include "ls/draw/SceneGraph.h"
#include "ls/draw/SceneMesh.h"
#include "ls/draw/SceneMaterial.h"
#include "ls/draw/SceneNode.h"
#include "ls/draw/Transform.h"
#include "ls/draw/VertexBuffer.h"



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
    nodeMeshCounts.clear();
    renderData.terminate();
}

/*-------------------------------------
 * Node updating
-------------------------------------*/
void SceneGraph::update_node_transform(const unsigned transformId) noexcept {
    std::vector<draw::Transform>& transforms = currentTransforms;
    draw::Transform& t = transforms[transformId];
    
    const unsigned parentId = t.get_parent_id();
    const bool doesParentExist = parentId != draw::scene_property_t::SCENE_GRAPH_ROOT_ID;

    if (doesParentExist) {
        draw::Transform& pt = transforms[parentId];
        
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
        draw::Transform& pt = transforms[parentId];
        t.apply_pre_transform(pt.get_transform());
    }
    else {
        t.apply_transform();
    }
    
    modelMatrices[transformId] = t.get_transform();

    // TODO: implement transformation packing so the iteration can stop as
    // soon as all child transforms have been updated. There's no reason
    // to iterate past the child transforms in the transform array.
    for (unsigned i = transformId+1; i < transforms.size(); ++i) {
        draw::Transform& ct = transforms[i];
        
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



} // end draw namepsace
} // end ls namespace
