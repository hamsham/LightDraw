
#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/AnimationPlayer.h"
#include "lightsky/draw/SceneGraph.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Destructor
-------------------------------------*/
AnimationPlayer::~AnimationPlayer() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
AnimationPlayer::AnimationPlayer() noexcept :
    currentState {ANIM_STATE_STOPPED},
    numPlays {PLAY_AUTO},
    currentPercent {0.0},
    dilation {1.0}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
AnimationPlayer::AnimationPlayer(const AnimationPlayer& a) noexcept :
    currentState {a.currentState},
    numPlays {a.numPlays},
    currentPercent {a.currentPercent},
    dilation {a.dilation}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
AnimationPlayer::AnimationPlayer(AnimationPlayer&& a) noexcept :
    currentState {a.currentState},
    numPlays {a.numPlays},
    currentPercent {a.currentPercent},
    dilation {a.dilation}
{
    a.currentState = ANIM_STATE_STOPPED;
    a.numPlays = PLAY_AUTO;
    a.currentPercent = 0.0;
    a.dilation = 1.0;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
AnimationPlayer& AnimationPlayer::operator =(const AnimationPlayer& a) noexcept {
    currentState = a.currentState;
    numPlays = a.numPlays;
    currentPercent = a.currentPercent;
    dilation = a.dilation;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
AnimationPlayer& AnimationPlayer::operator =(AnimationPlayer&& a) noexcept {
    currentState = a.currentState;
    a.currentState = ANIM_STATE_STOPPED;

    numPlays = a.numPlays;
    a.numPlays = PLAY_AUTO;

    currentPercent = a.currentPercent;
    a.currentPercent = 0.0;

    dilation = a.dilation;
    a.dilation = 1.0;

    return *this;
}

/*-------------------------------------
 * Get the current number of plays
-------------------------------------*/
unsigned AnimationPlayer::get_num_plays() const noexcept {
    return numPlays;
}

/*-------------------------------------
 * Set the current number of plays
-------------------------------------*/
void AnimationPlayer::set_num_plays(const unsigned playCount) noexcept {
    numPlays = playCount;
}

/*-------------------------------------
 * Get the current tick time (ticks elapsed)
-------------------------------------*/
anim_prec_t AnimationPlayer::get_current_ticks() const noexcept {
    return currentPercent;
}

/*-------------------------------------
 * Progress an Animation
-------------------------------------*/
void AnimationPlayer::tick(SceneGraph& graph, unsigned animationIndex, uint64_t millis) noexcept {
    if (currentState != ANIM_STATE_PLAYING) {
        return;
    }

    const std::vector<Animation>& animations = graph.animations;
    const Animation& anim = animations[animationIndex];

    if (numPlays == PLAY_AUTO) {
        numPlays = (anim.get_play_mode() == animation_play_t::ANIM_PLAY_REPEAT)
            ? PLAY_REPEAT
            : PLAY_ONCE;
    }

    if (!numPlays) {
        stop_anim();
        return;
    }

    const anim_prec_t secondsDelta  = 0.001 * (anim_prec_t)millis;
    const anim_prec_t ticksDelta    = secondsDelta * anim.get_ticks_per_sec();
    const anim_prec_t percentDelta  = (ticksDelta * dilation) / anim.get_duration();
    const anim_prec_t percentDone   = currentPercent + percentDelta;
    const anim_prec_t nextPercent   = percentDone >= 0.0 ? percentDone : math::max(anim_prec_t{1}+percentDone, anim_prec_t{0});
    
    anim.animate(graph, nextPercent);

    // check for a looped Animation even when time is going backwards.
    if (percentDone >= anim_prec_t{1}
    || (currentPercent > anim_prec_t{0} && percentDone < anim_prec_t{0})
    ) {
        if (numPlays != PLAY_REPEAT) {
            --numPlays;
        }
    }
    
    currentPercent = math::fmod(nextPercent, anim_prec_t{1});

    if (!numPlays) {
        stop_anim();
    }
}

/*-------------------------------------
 * Get the current play state
-------------------------------------*/
animation_state_t AnimationPlayer::get_anim_state() const noexcept {
    return currentState;
}

/*-------------------------------------
 * Set the current play state
-------------------------------------*/
void AnimationPlayer::set_play_state(const animation_state_t playState) noexcept {
    if (currentState == ANIM_STATE_STOPPED && playState == animation_state_t::ANIM_STATE_PLAYING) {
        currentPercent = 0.0;
    }
    
    currentState = playState;
}

/*-------------------------------------
 * Determine if *this is playing
-------------------------------------*/
bool AnimationPlayer::is_playing() const noexcept {
    return currentState == ANIM_STATE_PLAYING;
}

/*-------------------------------------
 * Check if paused
-------------------------------------*/
bool AnimationPlayer::is_paused() const noexcept {
    return currentState == ANIM_STATE_PAUSED;
}

/*-------------------------------------
 * Check if stopped
-------------------------------------*/
bool AnimationPlayer::is_stopped() const noexcept {
    return currentState == ANIM_STATE_STOPPED;
}

/*-------------------------------------
 * Force stop
-------------------------------------*/
void AnimationPlayer::stop_anim() noexcept {
    currentState = ANIM_STATE_STOPPED;
    currentPercent = 0.0;
}

/*-------------------------------------
 * Get the time speed-up or slow-down value
-------------------------------------*/
anim_prec_t AnimationPlayer::get_time_dilation() const noexcept {
    return dilation;
}

/*-------------------------------------
 * Set the time speed-up or slow-down value
-------------------------------------*/
void AnimationPlayer::set_time_dilation(const anim_prec_t percentNormalTime) noexcept {
    dilation = percentNormalTime;
}

/*-------------------------------------
 * Reset all internal parameters
-------------------------------------*/
void AnimationPlayer::reset() noexcept {
    currentState = ANIM_STATE_STOPPED;
    numPlays = PLAY_AUTO;
    currentPercent = 0.0;
    dilation = 1.0;
}

} // end draw namespace
} // end ls namespace
