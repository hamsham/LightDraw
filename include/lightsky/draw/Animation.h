
#ifndef __LS_DRAW_ANIMATION_H__
#define __LS_DRAW_ANIMATION_H__

#include <vector>
#include <string>

#include "lightsky/draw/AnimationProperty.h"
#include "lightsky/draw/AnimationChannel.h"



/*-----------------------------------------------------------------------------
 * Forward declaration of math types not instantiated in the header.
-----------------------------------------------------------------------------*/
namespace ls {
namespace math {
    template <typename num_t> struct vec3_t;
    template <typename num_t> struct quat_t;
} // end math namespace
} // end ls namespace

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward declaration of GFX types
-----------------------------------------------------------------------------*/
struct SceneNode;
class SceneGraph;



/**------------------------------------
 * Animation playback
 *
 * This enumeration allows for an Animation object to determine the number of
 * times an Animation should play. It is used by animationPlayer objects to
 * determine if an Animation plays once or multiple times.
-------------------------------------*/
enum class animation_play_t : unsigned {
    ANIM_PLAY_ONCE,
    ANIM_PLAY_REPEAT,

    ANIM_PLAY_DEFAULT = animation_play_t::ANIM_PLAY_ONCE
};



/**----------------------------------------------------------------------------
 * @brief The Animation object is used to animate nodes in a scene graph.
 *
 * This class keeps track of a single Animation, made up of 'tracks' or
 * keyframes, that are used to animate one or more meshes.
------------------------------------------------------------------------------*/
class Animation {
  private:
    /**
     * @brief playMode is by Animation players to determine if an Animation
     * loops one or more times.
     */
    animation_play_t playMode;

    /**
     * @brief animationId contains a hash value, from 'animName', which is
     * used to provide an instance of this class with a unique identifier.
     */
    unsigned animationId;

    /**
     * @brief totalTicks contains the number of ticks, or duration, of an
     * Animation.
     */
    anim_prec_t totalTicks;

    /**
     * @brief ticksPerSec determines how many ticks an Animation needs per
     * second to play.
     */
    anim_prec_t ticksPerSec;

    /**
     * @brief animName is used alongside 'animationId' to provide this
     * class with a unique, human-readable, identifier.
     */
    std::string animName;

    /**
     * @brief nodeChannels contains the sceneNode-specific list of
     * keyframes that animate one or more sceneNodes during playback.
     */
    std::vector<AnimationChannel> nodeChannels;

    /**
     * @brief playChannel is used internally to interpolate a specific
     * keyframe during playback.
     *
     * @param graph
     * A reference to a scene graph which contains a node to animate.
     *
     * @param track
     * A constant reference to the Animation keyframe that will be used to
     * interpolate the position, scaling, and orientation of a scene node.
     *
     * @param percent
     * The percentage of the total Animation which has already elapsed.
     *
     * @return The updated percent of a scene's Animation which has
     * elapsed since playback began.
     */
    anim_prec_t play_channel(
        SceneGraph& graph,
        const AnimationChannel& track,
        const anim_prec_t percent
    ) const noexcept;

  public:
    /**
     * @brief Destructor
     *
     * Cleans up all memory and resources used.
     */
    ~Animation() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all members to their default values.
     */
    Animation() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another Animation object.
     */
    Animation(const Animation& a) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves all data from the input parameter into *this without
     * performing any reallocations. The input parameter is restored to
     * its default state after this call.
     *
     * @param a
     * An r-value reference to a temporary Animation object.
     */
    Animation(Animation&& a) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another Animation object.
     *
     * @return A reference to *this.
     */
    Animation& operator =(const Animation& a) noexcept;

    /**
     * @brief Move Operator
     *
     * Moves all data from the input parameter into *this without
     * performing any reallocations. The input parameter is restored to
     * its default state after this call.
     *
     * @param a
     * An r-value reference to a temporary Animation object.
     *
     * @return A reference to *this.
     */
    Animation& operator =(Animation&& a) noexcept;

    /**
     * @brief Determine the current play mode used by *this.
     *
     * @return An enumeration which can be used to determine if *this has
     * a looping or non-looping Animation.
     */
    animation_play_t get_play_mode() const noexcept;

    /**
     * @brief Set the current play mode used by *this.
     *
     * @param animMode
     * An enumeration which can be used to determine if *this will contain
     * a looping or non-looping Animation.
     */
    void set_play_mode(const animation_play_t animMode) noexcept;

    /**
     * @brief Retrieve the unique, hashed, identifier that can be used to
     * reference *this.
     *
     * @return An unsigned integer, containing the hashed value of *this
     * Animation's name.
     */
    unsigned get_anim_id() const noexcept;

    /**
     * @brief Retrieve the name of *this Animation.
     *
     * @return A constant reference to a constant string, containing the
     * name of *this.
     */
    const std::string& get_anim_name() const noexcept;

    /**
     * @brief Set *this Animation's name.
     *
     * Caling this function will reset *this Animation's unique integer ID
     * to the hash of the input name.
     *
     * @param name
     * A constant reference to a string, containing the new name of *this.
     */
    void set_anim_name(const std::string& name) noexcept;

    /**
     * @brief Get the duration, in ticks, of *this Animation.
     *
     * Ticks have no units and are merely used to transition from one pont
     * in time to another. They are similar to 'seconds' in the same way
     * that 'radians' are similar to 'degrees'.
     *
     * @return A anim_prec_t-precision float, containing the number of ticks
     * that an Animation plays.
     */
    anim_prec_t get_duration() const noexcept;

    /**
     * @brief Set the duration, in ticks, of *this Animation.
     *
     * @param ticks
     * A anim_prec_t-precition float, containing the number of ticks that an
     * Animation will play.
     */
    void set_duration(const anim_prec_t ticks) noexcept;

    /**
     * @brief Get playback speed, in ticks per second, that *this object
     * will run at.
     *
     * @return A anim_prec_t-precision float, containing the speed of *this
     * Animation in ticks/sec.
     */
    anim_prec_t get_ticks_per_sec() const noexcept;

    /**
     * @brief Set playback speed, in ticks per second, that *this object
     * will run at.
     *
     * @param numTicks
     * A anim_prec_t-precision float, containing the speed of *this Animation in
     * ticks/sec.
     */
    void set_ticks_per_sec(const anim_prec_t numTicks) noexcept;

    /**
     * @brief Retrieve the list of Animation keyframes used by a single
     * Animation (const).
     *
     * Each sub-list of keyframes contained within the return value can
     * reference its own sceneNode. This means that only one animationReel
     * can reference a single sceneNode, but *this can animate multiple
     * sceneNodes by using multiple animationReels.
     *
     * @return A constant reference to a constant vector of animationReels.
     */
    const std::vector<AnimationChannel>& get_anim_channels() const noexcept;

    /**
     * @brief Retrieve the list of Animation keyframes used by a single
     * Animation.
    void setTi
     *
     * Each sub-list of keyframes contained within the return value can
     * reference its own sceneNode. This means that only one animationReel
     * can reference a single sceneNode, but *this can animate multiple
     * sceneNodes by using multiple animationReels.
     *
     * @return A reference to a constant vector of animationReels.
     */
    std::vector<AnimationChannel>& get_anim_channels() noexcept;

    /**
     * @brief Set the Animation channels that will be used by *this.
     *
     * @param channels
     * A constant reference to a vector of animationReels that *this will
     * use when animating nodes in a sceneGraph.
     */
    void set_anim_channels(const std::vector<AnimationChannel>& channels) noexcept;

    /**
     * @brief Set the Animation channels that will be used by *this.
     *
     * No memory realocations will occur when using this function overload.
     *
     * @param channels
     * An r-valuereference to a vector of animationReels that *this will
     * use when animating nodes in a sceneGraph.
     */
    void set_anim_channels(std::vector<AnimationChannel>&& channels) noexcept;

    /**
     * @brief Get the number of Animation channels that will be animated by
     * *this.
     */
    unsigned get_num_anim_channels() const noexcept;

    /**
     * @brief Add an Animation channel to *this.
     *
     * @param channel
     * A constant reference to an animationReel which contains keyframes to
     * interpolate when animating a sceneGraph.
     */
    void add_anim_channel(const AnimationChannel& channel) noexcept;

    /**
     * @brief Add an Animation channel to *this.
     *
     * No memory realocations will occur when using this function overload.
     *
     * @param channel
     * An r-value reference to an animationReel which contains keyframes to
     * interpolate when animating a sceneGraph.
     */
    void add_anim_channel(AnimationChannel&& channel) noexcept;

    /**
     * Remove a single Animation channel from *this.
     *
     * @param channelIndex
     * The index of the Animation channel to remove.
     */
    void remove_anim_channel(unsigned channelIndex) noexcept;

    /**
     * Remove all Animation keyframes and channels inside of *this.
     */
    void clear_anim_channels() noexcept;

    /**
     * @brief Animate nodes in a sceneGraph.
     *
     * This function will permanently update the model matrix contained
     * within the animated sceneNodes until otherwise specified.
     *
     * @param graph
     * A reference to a sceneGraph object who's internal nodes will be
     * transformed according to the keyframes in *this.
     *
     * @param percentDone
     * The percent of the animation which has been played in total. An
     * assertion will be raised if this value is less than 0.0.
     */
    void animate(SceneGraph& graph, const anim_prec_t percentDone) const noexcept;
    
    /**
     * Initialize the animation transformations for all nodes in a scene graph.
     * 
     * @param graph
     * A reference to a sceneGraph object who's internal nodes will be
     * transformed according to the initial keyframes in *this.
     * 
     * @param atStart
     * Determines if the animation should be initialized with data from the
     * first set of keyframes or the last.
     */
    void init(SceneGraph& graph, const bool atStart = true) const noexcept;
};

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_ANIMATION_H__
