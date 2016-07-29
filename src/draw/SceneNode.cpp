
#include <utility>

#include "ls/utils/Assertions.h"

#include "ls/draw/SceneNode.h"
#include "ls/draw/SceneGraph.h"



namespace ls {
namespace draw {



/*-------------------------------------
    Reset
-------------------------------------*/
void SceneNode::reset() noexcept {
    nodeId = scene_property_t::SCENE_GRAPH_ROOT_ID;
    renderPropertyId = scene_property_t::SCENE_GRAPH_ROOT_ID;
    dataId = scene_property_t::SCENE_GRAPH_ROOT_ID;
}



} // end draw namespace
} // end ls namespace
