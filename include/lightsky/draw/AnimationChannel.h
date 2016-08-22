
#ifndef __LS_DRAW_ANIMATION_REEL_H__
#define __LS_DRAW_ANIMATION_REEL_H__

#include "lightsky/utils/Pointer.h"

#include "lightsky/math/vec3.h"
#include "lightsky/math/quat.h"

#include "lightsky/draw/AnimationProperty.h"
#include "lightsky/draw/AnimationKeyList.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Animation Keys (interpolations of animations).
 *
 * FIXME: Animations do not play if only two keyframes are present. At least
 * 3 frames are necessary for an Animation to play.
-----------------------------------------------------------------------------*/
struct AnimationChannel {
    /**
     * @brief animationMode Is a bit-flag which is used during
     * "getFrame(...)". This helps to identify if different frames of an
     * Animation should be interpolated/non-interpolated, repeated/non-
     * repeated.
     */
    animation_flag_t animationMode;

    /**
     * @brief positionFrames contains the self-relative positioning of a
     * node.
     */
    AnimationKeyListVec3 positionFrames;

    /**
     * @brief scaleFrames contains the self-relative scaling of a node.
     */
    AnimationKeyListVec3 scaleFrames;
    
    /**
     * @brief rotationFrames contains the self-relative orientation of a
     * node.
     */
    AnimationKeyListQuat rotationFrames;

    /**
     * @brief Destructor
     *
     * Frees all dynamic memory from *this.
     */
    ~AnimationChannel() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all internal pointers to NULL.
     */
    AnimationChannel() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies all data from the input parameter into *this.
     *
     * @param a
     * A constant reference to another animationReel object.
     */
    AnimationChannel(const AnimationChannel& a) noexcept;

    /**
     * @brief Move Constructor
     *
     * Moves all data from the input parameter into *this without
     * performing any copies.
     *
     * @param a
     * An r-value reference to another animationReel object.
     */
    AnimationChannel(AnimationChannel&& a) noexcept;

    /**
     * @brief Copy Operator
     *
     * Copies all data from the input parameter into *this. All previously
     * stored data will be deleted.
     *
     * @param a
     * A constant reference to another animationReel object.
     *
     * @return A reference to *this.
     */
    AnimationChannel& operator =(const AnimationChannel& a) noexcept;

    /**
     * @brief Move Operator
     *
     * Moves all data from the input parameter into *this without
     * performing any copies. All previously stored data will be deleted.
     *
     * @param a
     * An r-value reference to another animationReel object.
     *
     * @return A reference to *this.
     */
    AnimationChannel& operator =(AnimationChannel&& ak) noexcept;

    /**
     * @brief Get the bitmasked Animation flags used by *this during
     * interpolation.
     *
     * @return A bitmask of the interpolation modes that are used by *this.
     */
    animation_flag_t get_anim_flags() const noexcept;

    /**
     * @brief Set the number of frames in *this to animate.
     *
     * Caling this method will delete all current data.
     *
     * @param positionKeys
     * The number of frames to allocate for position interpolation.
     *
     * @param scalingKeys
     * The number of frames to allocate for position interpolation.
     *
     * @param rotationKeys
     * The number of frames to allocate for position interpolation.
     *
     * @return TRUE if the number of requested frames were successfully
     * allocated, FALSE if not.
     */
    bool set_num_frames(
        const unsigned positionKeys,
        const unsigned scalingKeys,
        const unsigned rotationKeys
    ) noexcept;

    /**
     * @brief Clear all frames from *this and reset all internal members.
     */
    void clear() noexcept;
    
    /**
     * Run a simple check to determine if there are position frames in *this
     * which can be used for scene node animations, given a percent of the
     * animation which has already played.
     * 
     * @param animPercent
     * The percentage of the entire animation which has been played.
     * 
     * @return TRUE if there are positions in *this which can be used for
     * animations, FALSE if not.
     */
    bool has_position_frame(const anim_prec_t animPercent) const noexcept;
    
    /**
     * Run a simple check to determine if there are scaling frames in *this
     * which can be used for scene node animations, given a percent of the
     * animation which has already played.
     * 
     * @param animPercent
     * The percentage of the entire animation which has been played.
     * 
     * @return TRUE if there are scaling in *this which can be used for
     * animations, FALSE if not.
     */
    bool has_scale_frame(const anim_prec_t animPercent) const noexcept;
    
    /**
     * Run a simple check to determine if there are rotation frames in *this
     * which can be used for scene node animations, given a percent of the
     * animation which has already played.
     * 
     * @param animPercent
     * The percentage of the entire animation which has been played.
     * 
     * @return TRUE if there are rotations in *this which can be used for
     * animations, FALSE if not.
     */
    bool has_rotation_frame(const anim_prec_t animPercent) const noexcept;

    /**
     * @brief Set the position of a frame.
     *
     * @param frameIndex
     * The array-offset to an Animation frame which is to be updated. This
     * index must be less than the value returned by "getNumFrames()".
     * 
     * @param percent
     * A percentage from 0 - 1, inclusive, which determines at what point
     * within the total duration of an animation that this keyframe should be
     * set. An assertion will be raised if the percent is greater than one from
     * the previous keyframe.
     *
     * @param pos
     * A constant reference to a 3D vector, containing the node-relative
     * position which will be stored in *this at a particular frame.
     */
    void set_position_frame(
        const unsigned frameIndex,
        const anim_prec_t percent,
        const math::vec3_t<float>& pos
    ) noexcept;

    /**
     * @brief Retrieve the position that a node should be during a
     * particular frame.
     *
     * @param percent
     * The percent of the position reel, clamped between 0 and 1, which
     * has been completed.
     *
     * @return A 3D vector, containing the node-relative position which
     * will be stored in *this at a particular frame.
     */
    math::vec3_t<float> get_position_frame(const anim_prec_t percent) const noexcept;

    /**
     * @brief Set the scale of a frame.
     *
     * @param frameIndex
     * The array-offset to an Animation frame which is to be updated. This
     * index must be less than the value returned by "getNumFrames()".
     * 
     * @param percent
     * A percentage from 0 - 1, inclusive, which determines at what point
     * within the total duration of an animation that this keyframe should be
     * set. An assertion will be raised if the percent is greater than one from
     * the previous keyframe.
     *
     * @param scale
     * A constant reference to a 3D vector, containing the node-relative
     * scaling which will be stored in *this at a particular frame.
     */
    void set_scale_frame(
        const unsigned frameIndex,
        const anim_prec_t percent,
        const math::vec3_t<float>& scale
    ) noexcept;

    /**
     * @brief Retrieve the scaling that a node should contain during a
     * particular frame.
     *
     * @param percent
     * The percent of the scaling reel, clamped between 0 and 1, which
     * has been completed.
     *
     * @return A 3D vector containing the node-relative scaling which will
     * be stored in *this at a particular frame.
     */
    math::vec3_t<float> get_scale_frame(const anim_prec_t percent) const noexcept;

    /**
     * @brief Set the rotation of a frame.
     *
     * @param frameIndex
     * The array-offset to an Animation frame which is to be updated. This
     * index must be less than the value returned by "getNumFrames()".
     * 
     * @param percent
     * A percentage from 0 - 1, inclusive, which determines at what point
     * within the total duration of an animation that this keyframe should be
     * set. An assertion will be raised if the percent is greater than one from
     * the previous keyframe.
     *
     * @param rotation
     * A constant reference to a quaternion, containing the node-relative
     * rotation which will be stored in *this at a particular frame.
     */
    void set_rotation_frame(
        const unsigned frameIndex,
        const anim_prec_t percent,
        const math::quat_t<float>& rotation
    ) noexcept;

    /**
     * @brief Retrieve the rotation that should be applied to a scene node
     * during a particular frame.
     *
     * @param percent
     * The percent of the rotation reel, clamped between 0 and 1, which
     * has been completed.
     *
     * @return A quaternion containing the node-relative rotation which
     * will be stored in *this at a particular frame.
     */
    math::quat_t<float> get_rotation_frame(const anim_prec_t percent) const noexcept;

    /**
     * @brief Retrieve the position, scale, and rotation of a node at a
     * percentage of its total frame index.
     *
     * This method is intended for use by the "Animation" object, which
     * contains the total time that an Animation reel operates at.
     *
     * This method returns immediately if the current Animation flags
     * are equal to "ANIM_NONE."
     *
     * @param outPosition
     * A reference to a 3D vector, containing the position of a node at a
     * given percentage of its Animation reel.
     *
     * @param outscale
     * A reference to a 3D vector, containing the scaling of a node at a
     * given percentage of its Animation reel.
     *
     * @param outRotation
     * A reference to a quaternion, containing the rotation of a node at a
     * given percentage of its Animation reel.
     *
     * @param percentFinished
     * The percent of the Animation reel, clamped between 0 and 1, which
     * has been completed.
     *
     * @return TRUE if an interpolation was performed, FALSE if not.
     */
    bool get_frame(
        math::vec3_t<float>& outPosition,
        math::vec3_t<float>& outscale,
        math::quat_t<float>& outRotation,
        const anim_prec_t percentFinished // currentTime / timePerSecond
    ) const noexcept;
};

/*-------------------------------------
 * Rotation availability check
-------------------------------------*/
inline bool AnimationChannel::has_position_frame(const anim_prec_t animPercent) const noexcept {
    //return positionFrames.is_valid();
    return animPercent >= positionFrames.get_start_time() && animPercent <= positionFrames.get_end_time();
}

/*-------------------------------------
 * Rotation availability check
-------------------------------------*/
inline bool AnimationChannel::has_scale_frame(const anim_prec_t animPercent) const noexcept {
    //return scaleFrames.is_valid();
    return animPercent >= scaleFrames.get_start_time() && animPercent <= scaleFrames.get_end_time();
}

/*-------------------------------------
 * Rotation availability check
-------------------------------------*/
inline bool AnimationChannel::has_rotation_frame(const anim_prec_t animPercent) const noexcept {
    //return rotationFrames.is_valid();
    return animPercent >= rotationFrames.get_start_time() && animPercent <= rotationFrames.get_end_time();
}

/*-------------------------------------
 * Set a single position key
-------------------------------------*/
inline void AnimationChannel::set_position_frame(
    const unsigned frameIndex,
    const anim_prec_t percent,
    const math::vec3_t<float>& pos
) noexcept {
    LS_DEBUG_ASSERT(percent >= -1.0 && percent <= 1.0);
    positionFrames.set_frame(frameIndex, percent, pos);
}

/*-------------------------------------
 * Set a single scale key
-------------------------------------*/
inline void AnimationChannel::set_scale_frame(
    const unsigned frameIndex,
    const anim_prec_t percent,
    const math::vec3_t<float>& scale
) noexcept {
    LS_DEBUG_ASSERT(percent >= -1.0 && percent <= 1.0);
    scaleFrames.set_frame(frameIndex, percent, scale);
}

/*-------------------------------------
 * Set a single rotation key
-------------------------------------*/
inline void AnimationChannel::set_rotation_frame(
    const unsigned frameIndex,
    const anim_prec_t percent,
    const math::quat_t<float>& rot
) noexcept {
    LS_DEBUG_ASSERT(percent >= -1.0 && percent <= 1.0);
    rotationFrames.set_frame(frameIndex, percent, rot);
}

/*-------------------------------------
 * Get a single position key
-------------------------------------*/
inline math::vec3 AnimationChannel::get_position_frame(const anim_prec_t percent) const noexcept {
    return positionFrames.get_interpolated_data(percent, animationMode);
}

/*-------------------------------------
 * Get a single scale key
-------------------------------------*/
inline math::vec3 AnimationChannel::get_scale_frame(const anim_prec_t percent) const noexcept {
    return scaleFrames.get_interpolated_data(percent, animationMode);
}

/*-------------------------------------
 * Get a single rotaion key
-------------------------------------*/
inline math::quat AnimationChannel::get_rotation_frame(const anim_prec_t percent) const noexcept {
    return rotationFrames.get_interpolated_data(percent, animationMode);
}

/*-------------------------------------
 * Animation Key Interpolator
-------------------------------------*/
inline bool AnimationChannel::get_frame(
    math::vec3& outPosition,
    math::vec3& outScale,
    math::quat& outRotation,
    anim_prec_t percentFinished
) const noexcept {
    outPosition = get_position_frame(percentFinished);
    outScale    = get_scale_frame(percentFinished);
    outRotation = get_rotation_frame(percentFinished);
    return true;
}

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_ANIMATION_REEL_H__
