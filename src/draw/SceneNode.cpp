
#include <utility>

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/SceneNode.h"
#include "lightsky/draw/SceneGraph.h"



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
