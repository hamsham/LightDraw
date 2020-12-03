
#include <utility>

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/SceneNode.h"
#include "lightsky/draw/SceneGraph.h"



namespace ls
{
namespace draw
{



/*-------------------------------------
    Reset
-------------------------------------*/
void SceneNode::reset() noexcept
{
    type = scene_node_t::NODE_TYPE_EMPTY;
    nodeId = scene_property_t::SCENE_GRAPH_ROOT_ID;
    dataId = scene_property_t::SCENE_GRAPH_ROOT_ID;
    animListId = scene_property_t::SCENE_GRAPH_ROOT_ID;
}
} // end draw namespace
} // end ls namespace
