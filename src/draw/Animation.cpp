
#include <functional>
#include <utility>

#include "lightsky/math/Math.h"

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/Animation.h"
#include "lightsky/draw/SceneGraph.h"
#include "lightsky/draw/SceneNode.h"
#include "lightsky/draw/Transform.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Animation object.
------------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
Animation::~Animation() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Animation::Animation() noexcept :
    playMode {animation_play_t::ANIM_PLAY_DEFAULT},
    animationId {0},
    totalTicks {0},
    ticksPerSec {0.0},
    animName {""},
    animationIds {},
    nodeTrackIds {},
    transformIds {}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
Animation::Animation(const Animation& a) noexcept :
    playMode {a.playMode},
    animationId {a.animationId},
    totalTicks {a.totalTicks},
    ticksPerSec {a.ticksPerSec},
    animName {a.animName},
    animationIds {a.animationIds},
    nodeTrackIds {a.nodeTrackIds},
    transformIds {a.transformIds}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
Animation::Animation(Animation&& a) noexcept :
    playMode {a.playMode},
    animationId {a.animationId},
    totalTicks {a.totalTicks},
    ticksPerSec {a.ticksPerSec},
    animName {std::move(a.animName)},
    animationIds {std::move(a.animationIds)},
    nodeTrackIds {std::move(a.nodeTrackIds)},
    transformIds {std::move(a.transformIds)}
{
    a.playMode = animation_play_t::ANIM_PLAY_DEFAULT;
    a.animationId = 0;
    a.totalTicks = 0.0;
    a.ticksPerSec = 0.0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
Animation& Animation::operator =(const Animation& a) noexcept {
    playMode = a.playMode;
    animationId = a.animationId;
    totalTicks = a.totalTicks;
    ticksPerSec = a.ticksPerSec;
    animName = a.animName;
    animationIds = a.animationIds;
    nodeTrackIds = a.nodeTrackIds;
    transformIds = a.transformIds;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
Animation& Animation::operator =(Animation&& a) noexcept {
    playMode = a.playMode;
    a.playMode = animation_play_t::ANIM_PLAY_DEFAULT;

    animationId = a.animationId;
    a.animationId = 0;

    totalTicks = a.totalTicks;
    a.totalTicks = 0.0;

    ticksPerSec = a.ticksPerSec;
    a.ticksPerSec = 0.0;

    animName = std::move(a.animName);
    animationIds = std::move(a.animationIds);
    nodeTrackIds = std::move(a.nodeTrackIds);
    transformIds = std::move(a.transformIds);

    return *this;
}

/*-------------------------------------
 * Retrieve the Animation mode
-------------------------------------*/
animation_play_t Animation::get_play_mode() const noexcept {
    return playMode;
}

/*-------------------------------------
 * Set the Animation mode
-------------------------------------*/
void Animation::set_play_mode(const animation_play_t animMode) noexcept {
    playMode = animMode;
}

/*-------------------------------------
 * Retrieve the Animation's hash number
-------------------------------------*/
unsigned Animation::get_anim_id() const noexcept {
    return animationId;
}

/*-------------------------------------
 * Retrieve the Animation's name
-------------------------------------*/
const std::string& Animation::get_anim_name() const noexcept {
    return animName;
}

/*-------------------------------------
 * Set the Animation name
-------------------------------------*/
void Animation::set_anim_name(const std::string& name) noexcept {
    std::hash<std::string> hashFunc {};
    animationId = hashFunc(name);
    animName = name;
}

/*-------------------------------------
 * Set the Animation duration (ticks per second)
-------------------------------------*/
anim_prec_t Animation::get_duration() const noexcept {
    return totalTicks;
}

/*-------------------------------------
 * Retrieve the Animation duration (tps).
-------------------------------------*/
void Animation::set_duration(const anim_prec_t ticks) noexcept {
    totalTicks = ticks;
}

/*-------------------------------------
 * Retrieve the time interpolation (tps).
-------------------------------------*/
anim_prec_t Animation::get_ticks_per_sec() const noexcept {
    return ticksPerSec;
}

/*-------------------------------------
 * Retrieve the time interpolation (tps).
-------------------------------------*/
void Animation::set_ticks_per_sec(const anim_prec_t numTicks) noexcept {
    ticksPerSec = numTicks;
}

/*-------------------------------------
 * Retrieve the transformations affected by *this.
-------------------------------------*/
const std::vector<uint32_t>& Animation::get_transforms() const noexcept {
    return transformIds;
}

/*-------------------------------------
 * Retrieve the animation channels affected by *this.
-------------------------------------*/
const std::vector<uint32_t>& Animation::get_node_tracks() const noexcept {
    return nodeTrackIds;
}

/*-------------------------------------
 * Retrieve an array of animations affected by *this.
-------------------------------------*/
const std::vector<uint32_t>& Animation::get_node_animations() const noexcept {
    return animationIds;
}

/*-------------------------------------
 * Get the number of sub-animations
-------------------------------------*/
unsigned Animation::get_num_anim_channels() const noexcept {
    LS_DEBUG_ASSERT(transformIds.size() == animationIds.size());
    LS_DEBUG_ASSERT(transformIds.size() == nodeTrackIds.size());
    return transformIds.size();
}

/*-------------------------------------
 * Add a node's animation track to *this
-------------------------------------*/
void Animation::add_anim_channel(const SceneNode& node, const uint32_t nodeTrackId) noexcept {
    animationIds.push_back(node.animListId);
    nodeTrackIds.push_back(nodeTrackId);
    transformIds.push_back(node.nodeId);
}

/*-------------------------------------
 * Remove a node's animation from *this.
-------------------------------------*/
void Animation::remove_anim_channel(const unsigned channelIndex) noexcept {
    LS_DEBUG_ASSERT(transformIds.size() == animationIds.size());
    LS_DEBUG_ASSERT(transformIds.size() == nodeTrackIds.size());
    
    if (channelIndex >= transformIds.size()) {
        return;
    }
    
    animationIds.erase(animationIds.begin() + channelIndex);
    nodeTrackIds.erase(nodeTrackIds.begin() + channelIndex);
    transformIds.erase(transformIds.begin() + channelIndex);
}

/*-------------------------------------
 * Clear all sub-animations.
-------------------------------------*/
void Animation::clear_anim_channels() noexcept {
    animationIds.clear();
    nodeTrackIds.clear();
    transformIds.clear();
}

/*-------------------------------------
 * Reserve space for animation tracks.
-------------------------------------*/
void Animation::reserve_anim_channels(const unsigned reserveSize) noexcept {
    animationIds.reserve(reserveSize);
    nodeTrackIds.reserve(reserveSize);
    transformIds.reserve(reserveSize);
}

/*-------------------------------------
 * Animate a scene graph using all tracks.
-------------------------------------*/
void Animation::animate(SceneGraph& graph, const anim_prec_t percentDone) const noexcept {
    LS_DEBUG_ASSERT(percentDone >= 0.0);
    LS_DEBUG_ASSERT(transformIds.size() == animationIds.size());
    LS_DEBUG_ASSERT(transformIds.size() == nodeTrackIds.size());
    
    for (unsigned i = transformIds.size(); i --> 0;) {
        const uint32_t animChannelId    = animationIds[i];
        const uint32_t nodeTrackId      = nodeTrackIds[i];
        const uint32_t transformId      = transformIds[i];
        
        const std::vector<AnimationChannel>& nodeTracks = graph.nodeAnims[animChannelId];
        const AnimationChannel& track   = nodeTracks[nodeTrackId];
        Transform& nodeTransform        = graph.currentTransforms[transformId];
        
        LS_DEBUG_ASSERT(transformId != scene_property_t::SCENE_GRAPH_ROOT_ID);

        if (track.has_position_frame(percentDone)) {
            const math::vec3&& pos = track.get_position_frame(percentDone);
            nodeTransform.set_position(pos);
        }

        if (track.has_scale_frame(percentDone)) {
            const math::vec3&& scl = track.get_scale_frame(percentDone);
            nodeTransform.set_scale(scl);
        }

        if (track.has_rotation_frame(percentDone)) {
            math::quat&& rot = track.get_rotation_frame(percentDone);
            nodeTransform.set_orientation(rot);
        }
    }
}

/*-------------------------------------
 * Animate a scene graph using all tracks.
-------------------------------------*/
void Animation::init(SceneGraph& graph, const bool atStart) const noexcept {
    LS_DEBUG_ASSERT(transformIds.size() == animationIds.size());
    LS_DEBUG_ASSERT(transformIds.size() == nodeTrackIds.size());
    
    for (unsigned i = transformIds.size(); i --> 0;) {
        const uint32_t animChannelId    = animationIds[i];
        const uint32_t nodeTrackId      = nodeTrackIds[i];
        const uint32_t transformId      = transformIds[i];
        
        const std::vector<AnimationChannel>& nodeTracks = graph.nodeAnims[animChannelId];
        const AnimationChannel& track   = nodeTracks[nodeTrackId];
        Transform& nodeTransform        = graph.currentTransforms[transformId];

        if (track.positionFrames.is_valid()) {
            nodeTransform.set_position(atStart ? track.positionFrames.get_start_data() : track.positionFrames.get_end_data());
        }

        if (track.scaleFrames.is_valid()) {
            nodeTransform.set_scale(atStart ? track.scaleFrames.get_start_data() : track.scaleFrames.get_end_data());
        }

        if (track.rotationFrames.is_valid()) {
            nodeTransform.set_orientation(atStart ? track.rotationFrames.get_start_data() : track.rotationFrames.get_end_data());
        }
    }
}

} // end draw namespace
} // end ls namespace
