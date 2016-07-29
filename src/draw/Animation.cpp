
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
    nodeChannels {}
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
    nodeChannels {a.nodeChannels}
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
    nodeChannels {std::move(a.nodeChannels)}
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
    nodeChannels = a.nodeChannels;

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

    nodeChannels = std::move(a.nodeChannels);

    return *this;
}

/*-------------------------------------
 * Play a single Animation track.
-------------------------------------*/
anim_prec_t Animation::play_channel(
    SceneGraph& graph,
    const AnimationChannel& track,
    const anim_prec_t percent
) const noexcept {
    const unsigned nodeId = track.nodeId;
    LS_DEBUG_ASSERT(nodeId != scene_property_t::SCENE_GRAPH_ROOT_ID);
    Transform& nodeTransform = graph.currentTransforms[nodeId];

    if (track.has_position_frame(percent)) {
        const math::vec3&& pos = track.get_position_frame(percent);
        nodeTransform.set_position(pos);
    }

    if (track.has_scale_frame(percent)) {
        const math::vec3&& scl = track.get_scale_frame(percent);
        nodeTransform.set_scale(scl);
    }

    if (track.has_rotation_frame(percent)) {
        math::quat&& rot = track.get_rotation_frame(percent);
        nodeTransform.set_orientation(rot);
    }

    return percent;
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
 * Retrieve the sub-animations affected by *this (const).
-------------------------------------*/
const std::vector<AnimationChannel>& Animation::get_anim_channels() const noexcept {
    return nodeChannels;
}

/*-------------------------------------
 * Retrieve the sub-animations affected by *this.
-------------------------------------*/
std::vector<AnimationChannel>& Animation::get_anim_channels() noexcept {
    return nodeChannels;
}

/*-------------------------------------
 * Copy sub-animations into *this.
-------------------------------------*/
void Animation::set_anim_channels(const std::vector<AnimationChannel>& channels) noexcept {
    nodeChannels = channels;
}

/*-------------------------------------
 * Move sub-animations into *this.
-------------------------------------*/
void Animation::set_anim_channels(std::vector<AnimationChannel>&& channels) noexcept {
    nodeChannels = std::move(channels);
}

/*-------------------------------------
 * Get the number of sub-animations
-------------------------------------*/
unsigned Animation::get_num_anim_channels() const noexcept {
    return nodeChannels.size();
}

/*-------------------------------------
 * Add a sub-Animation to *this
-------------------------------------*/
void Animation::add_anim_channel(const AnimationChannel& channel) noexcept {
    nodeChannels.push_back(channel);
}

/*-------------------------------------
 * Move a sub-Animation into *this.
-------------------------------------*/
void Animation::add_anim_channel(AnimationChannel&& channel) noexcept {
    nodeChannels.emplace_back(std::move(channel));
}

/*-------------------------------------
 * Remove a sub-Animation from *this.
-------------------------------------*/
void Animation::remove_anim_channel(unsigned channelIndex) noexcept {
    nodeChannels.erase(nodeChannels.begin() + channelIndex);
}

/*-------------------------------------
 * Clear all sub-animations.
-------------------------------------*/
void Animation::clear_anim_channels() noexcept {
    nodeChannels.clear();
}

/*-------------------------------------
 * Animate a scene graph using all tracks.
-------------------------------------*/
void Animation::animate(SceneGraph& graph, const anim_prec_t percentDone) const noexcept {
    LS_DEBUG_ASSERT(percentDone >= 0.0);
    for (unsigned i = nodeChannels.size(); i --> 0;) {
        const AnimationChannel& track = nodeChannels[i];
        play_channel(graph, track, percentDone);
    }
}

/*-------------------------------------
 * Animate a scene graph using all tracks.
-------------------------------------*/
void Animation::init(SceneGraph& graph, const bool atStart) const noexcept {
    for (unsigned i = nodeChannels.size(); i --> 0;) {
        const AnimationChannel& track = nodeChannels[i];
        Transform& nodeTransform = graph.currentTransforms[track.nodeId];

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
