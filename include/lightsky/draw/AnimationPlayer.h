
#ifndef __LS_DRAW_ANIMATION_PLAYER_H__
#define __LS_DRAW_ANIMATION_PLAYER_H__

#include <climits> // UINT_MAX
#include <cstdint> // uint64_t

#include "lightsky/draw/AnimationProperty.h"



namespace ls
{
namespace draw
{

/*-----------------------------------------------------------------------------
 * Forward declarations
-----------------------------------------------------------------------------*/
class SceneGraph;



/**----------------------------------------------------------------------------
 * @brief The animation_state_t enum detemrines the current state of playback
 * in an animationPlayer object.
-----------------------------------------------------------------------------*/
enum animation_state_t
{
    ANIM_STATE_PLAYING,
    ANIM_STATE_PAUSED,
    ANIM_STATE_STOPPED
};



/**----------------------------------------------------------------------------
 * @brief The animationPlayer class contains the ability to play an Animation
 * over a time-period.
 *
 * This class currently only plays animations in a sceneGraph object.
-----------------------------------------------------------------------------*/
class AnimationPlayer
{
  public:
    enum : unsigned
    {
        /**
         * @brief Animation flag to indicate that an Animation is repeating.
         */
        PLAY_REPEAT = UINT_MAX,

        /**
         * @brief Animation flag to determine that *this should choose the best
         * playback mode for an Animation object by checking its 'playMode'
         * setting.
         */
        PLAY_AUTO = UINT_MAX - 1,

        /**
         * @brief PLAY_ONCE will cause *this to animate a scene graph once.
         */
        PLAY_ONCE = 1,
    };

  private:
    /**
     * @brief 'currentState' sets the current playback state of *this.
     */
    animation_state_t currentState;

    /**
     * @brief 'numPlays' determines the number of plays that *this object
     * has remaining.
     */
    unsigned numPlays;

    /**
     * @brief 'currentTicks' contains the time, in ticks, that have elapsed
     * since *this object started playing animations.
     */
    anim_prec_t currentPercent;

    /**
     * @brief The time dilation, between 0.0 and DOUBLE_MAX, that will be
     * used to speed-up or slow-down Animation playback.
     *
     * This member cannot be negative until suport is added for reversed
     * interpolation of Animation keyframes.
     */
    anim_prec_t dilation;

  public:
    /**
     * @brief Destructor
     *
     * Cleans up all memory and resources used.
     */
    ~AnimationPlayer() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all members to their default values.
     */
    AnimationPlayer() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another animationPlayer object.
     */
    AnimationPlayer(const AnimationPlayer& a) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves all data from the input parameter into *this without
     * performing any reallocations. The input parameter is restored to
     * its default state after this call.
     *
     * @param a
     * An r-value reference to a temporary animationPlayer object.
     */
    AnimationPlayer(AnimationPlayer&& a) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another animationPlayer object.
     *
     * @return A reference to *this.
     */
    AnimationPlayer& operator=(const AnimationPlayer& a) noexcept;

    /**
     * @brief Move Operator
     *
     * Moves all data from the input parameter into *this without
     * performing any reallocations. The input parameter is restored to
     * its default state after this call.
     *
     * @param a
     * An r-value reference to a temporary animationPlayer object.
     *
     * @return A reference to *this.
     */
    AnimationPlayer& operator=(AnimationPlayer&& a) noexcept;

    /**
     * @brief Get the number of times that an Animation should play.
     *
     * @return An unsigned integer containing the number of plays
     * remaining.
     */
    unsigned get_num_plays() const noexcept;

    /**
     * @brief Set the number of times that an Animation should play.
     *
     * @param playCount
     * An unsigned integral number which represents the number of times
     * that an Animation should play. Set this parameter to PLAY_AUTO in
     * order to have the play-count determined by an Animation channel
     * during a call to 'tick(...)'.
     */
    void set_num_plays(const unsigned playCount = PLAY_AUTO) noexcept;

    /**
     * @brief Retrieve the total number of ticks which have elapsed since
     * playback started.
     *
     * @return A anim_prec_t-precision float which represents the number of
     * ticks that have passed since the last cal to 'tick(...)'.
     */
    anim_prec_t get_current_ticks() const noexcept;

    /**
     * @brief Animate a scene graph using the Animation object referenced
     * by a specific index.
     *
     * @param graph
     * A reference to a sceneGraph object which contains one or more
     * Animation objects.
     *
     * @param animationIndex
     * An array-offset to a specific Animation object contained within the
     * input scene graph.
     *
     * @param millis
     * The total number of milliseconds which have passed since playback
     * started.
     */
    void tick(SceneGraph& graph, unsigned animationIndex, uint64_t millis) noexcept;

    /**
     * @brief Get the current state of playback from *this.
     *
     * @return An animation_play_state_t enumeration, containing an
     * enumeration that determines if *this animates nodes in a scene
     * graoh or not.
     */
    animation_state_t get_anim_state() const noexcept;

    /**
     * @brief Allow *this Animation player to continue, pause, or halt any
     * animations contained within *this.
     *
     * No checks are performed to determine if an invalid entry is placed
     * into this function.
     *
     * @param playState
     * A constant animation_play_state_t enumeration, containing an
     * enumeration that allows this to animate nodes in a scene graoh.
     */
    void set_play_state(const animation_state_t playState) noexcept;

    /**
     * @brief Determine if any scene graph Animation updates occur
     * during subsequent calls to 'tick(...)'.
     *
     * @return TRUE if the current Animation state is set to
     * animation_state_t::ANIM_STATE_PLAYING, or FALSE if not.
     */
    bool is_playing() const noexcept;

    /**
     * @brief Determine if any scene graph Animation updates are pending
     * during subsequent calls to 'tick(...)'.
     *
     * @return TRUE if the current Animation state is set to
     * animation_state_t::ANIM_STATE_PAUSED, or FALSE if not.
     */
    bool is_paused() const noexcept;

    /**
     * @brief Determine if any scene graph Animation updates are prevented
     * during subsequent calls to 'tick(...)'.
     *
     * @return TRUE if the current Animation state is set to
     * animation_state_t::ANIM_STATE_STOPPED, or FALSE if not.
     */
    bool is_stopped() const noexcept;

    /**
     * @brief Prevent any scene graph Animation updates from occurring
     * during subsequent calls to 'tick(...)'.
     *
     * This function also sets the current playback time to 0 ticks
     * elapsed.
     */
    void stop_anim() noexcept;

    /**
     * @brief Retrieve the playback time multiplier used by *this for
     * advancing playback speed.
     *
     * @param The multiplier at which the amount of Animation ticks-per-
     * second is advanced during a call to 'tick(...)'.
     */
    anim_prec_t get_time_dilation() const noexcept;

    /**
     * @brief This function can be used to warp the time which elapses per
     * Animation update.
     *
     * Use this to speed-up or slow-down time according to a specific
     * multiplier. However, this function cannot be passed a negative value
     * until suport is added for reversed interpolation of Animation
     * keyframes.
     *
     * This function can also be used to speed-up or slow-down Animation
     * playback times by orders of magnitude (i.e. you can turn millisecond
     * updates to second-length or microsecond-length).
     *
     * @param The multiplier at which the amount of Animation ticks-per-
     * second will be advanced during a call to 'tick(...)'.
     */
    void set_time_dilation(const anim_prec_t percentNormalTime) noexcept;

    /**
     * Reset all internal members to their default values.
     */
    void reset() noexcept;
};
} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_ANIMATION_PLAYER_H__
