
#ifndef __LS_DRAW_ANIMATION_KEY_LIST_H__
#define __LS_DRAW_ANIMATION_KEY_LIST_H__

#include <utility> // std::move

#include "lightsky/setup/Macros.h" // LS_DECLARE_CLASS_TYPE

#include "lightsky/math/scalar_utils.h" // floor()

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Copy.h"
#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/AnimationProperty.h"



/*-----------------------------------------------------------------------------
 * Forward Declarations
------------------------------------------------------------------------------*/
namespace ls {
namespace math {
    template <typename num_t>
    struct vec3_t;

    template <typename num_t>
    struct quat_t;
} // end math namespace
} // end ls namespace

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Animation Modes
-----------------------------------------------------------------------------*/
enum animation_flag_t : unsigned {
    ANIM_FLAG_NONE = 0x00, // no interpolation, should be performed.
    ANIM_FLAG_IMMEDIATE = 0x01, // immediately jump from frame to frame.
    ANIM_FLAG_INTERPOLATE = 0x02, // linearly interpolate between the current and next frame.
    ANIM_FLAG_REPEAT = 0x04, // repeat an Animation.

    ANIM_FLAG_DEFAULT = ANIM_FLAG_INTERPOLATE
};



/*-----------------------------------------------------------------------------
 * Animation Key Frame Helper Class (for interpolating animations).
 *
 * FIXME: Animations do not play if only two keyframes are present. At least
 * 3 frames are necessary for an Animation to play.
-----------------------------------------------------------------------------*/
template <typename data_t>
class AnimationKeyList {
    private:
        /**
         * @brief numPositions contains the total number of position keys.
         */
        unsigned numFrames;

        /**
         * @brief positionTimes contains the keyframe times of a particular
         * animation's positions.
         */
        utils::Pointer<anim_prec_t[]> keyTimes;

        /**
         * @brief keyData contains a list of variables which can be
         * interpolated during an animation.
         */
        utils::Pointer<data_t[]> keyData;
        
    public:
        /*
         * Destructor
         * 
         * Frees all dynamic memory usage from *this.
         */
        ~AnimationKeyList() noexcept;
        
        /**
         * Constructor
         * 
         * Initializes all internal members to their default values. No dynamic
         * memory is allocated at this time.
         */
        AnimationKeyList() noexcept;
        
        /**
         * Copy Constructor
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param k
         * A constant reference to another AnimationKeyList type which contains
         * keyframe data.
         */
        AnimationKeyList(const AnimationKeyList& k) noexcept;
        
        /**
         * Move Constructor
         * 
         * Moves all data from the input parameter into *this. No dynamic
         * allocations are performed.
         * 
         * @param k
         * An r-value reference to another AnimationKeyList type which contains
         * keyframe data.
         */
        AnimationKeyList(AnimationKeyList&& k) noexcept;
        
        /**
         * Copy Operator
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param k
         * A constant reference to another AnimationKeyList type which contains
         * keyframe data.
         * 
         * @return A reference to *this.
         */
        AnimationKeyList& operator=(const AnimationKeyList& k) noexcept;
        
        /**
         * Move Operator
         * 
         * Moves all data from the input parameter into *this. No dynamic
         * allocations are performed.
         * 
         * @param k
         * An r-value reference to another AnimationKeyList type which contains
         * keyframe data.
         * 
         * @return A reference to *this.
         */
        AnimationKeyList& operator=(AnimationKeyList&& k) noexcept;
        
        /**
         * Free all dynamic memory from *this and return the internal
         * members to their default values.
         */
        void clear() noexcept;
        
        /**
         * Retrieve the number of keyframes in *this.
         * 
         * @return An unsigned integral type, containing the current number of
         * keyframes contained in *this.
         */
        unsigned size() const noexcept;
        
        /**
         * Initialize and allocate an array of keyframes for *this to use.
         * 
         * This method will clear any old keyframes which previously existed in
         * *this.
         * 
         * This method is not reentrant.
         * 
         * @param keyCount
         * The desired number of keyframes to allocate.
         * 
         * @return TRUE if the internal array of keyframes was successfully
         * allocated, FALSE if not.
         */
        bool init(const unsigned keyCount) noexcept;
        
        /**
         * Determine if there are keyframes in *this to use for animation.
         * 
         * @return TRUE if *this object contains at least one keyframe to use
         * of FALSE if not.
         */
        bool is_valid() const noexcept;
        
        /**
         * Retrieve the time difference between the initial keyframe and final
         * keyframe in *this.
         * 
         * @note All animations in the library use percentage values between 
         * 0.0 and 1.0, inclusive, for time representation unless documented
         * otherwise.
         * 
         * @return A floating-point value containing the time difference
         * between the starting and ending keyframes.
         */
        anim_prec_t get_duration() const noexcept;
        
        /**
         * Retrieve the time of the starting keyframe in *this.
         * 
         * @return A floating-point value within the range (0.0, 1.0) which
         * determines when a particular keyframe should be used to start an
         * animation.
         */
        anim_prec_t get_start_time() const noexcept;
        
        /**
         * Retrieve the time of the final keyframe in *this.
         * 
         * @return A floating-point value within the range (0.0, 1.0) which
         * determines when a particular keyframe should be used to end an
         * animation.
         */
        anim_prec_t get_end_time() const noexcept;
        
        /**
         * Retrieve the time of a single keyframe from *this.
         * 
         * @return A floating-point value within the range (0.0, 1.0) which
         * determines when a particular keyframe should be used in an
         * animation.
         */
        anim_prec_t get_frame_time(const unsigned keyIndex) const noexcept;
        
        /**
         * Retrieve the data of a particular keyframe.
         * 
         * This method will raise an assertion if the index is out of range of
         * any available keys.
         * 
         * @param keyIndex
         * An array index to the desired keyframe.
         * 
         * @return A constant reference to the data within a keyframe.
         */
        const data_t& get_frame_data(const unsigned keyIndex) const noexcept;
        
        /**
         * Retrieve the data of a particular keyframe.
         * 
         * This method will raise an assertion if the index is out of range of
         * any available keys.
         * 
         * @param keyIndex
         * An array index to the desired keyframe.
         * 
         * @return A reference to the data within a keyframe.
         */
        data_t& get_frame_data(const unsigned keyIndex) noexcept;
        
        /**
         * Retrieve the data of the first keyframe in *this.
         * 
         * This method will raise an assertion if there are no available
         * frames to retrieve data from.
         * 
         * @return A reference to the initial keyframe's data.
         */
        const data_t& get_start_data() const noexcept;
        
        /**
         * Retrieve the data of the last keyframe in *this.
         * 
         * This method will raise an assertion if there are no available
         * frames to retrieve data from.
         * 
         * @return A reference to the last keyframe's data.
         */
        const data_t& get_end_data() const noexcept;
        
        /**
         * Assign data to a particular frame in *this.
         * 
         * This method will raise an assertion if the index is out of range of
         * any available keys.
         * 
         * @param keyIndex
         * An array index to the desired keyframe.
         * 
         * @param frameTime
         * A time of a single frame, represented as a percentage of an entire
         * animation, in the range (0.0, 1.0).
         * 
         * @param frameData
         * A constant reference to the data which will be used for a keyframe
         * at a perticular time.
         */
        void set_frame(const unsigned frameIndex, const anim_prec_t frameTime, const data_t& frameData) noexcept;
        
        /**
         * Retrieve the interpolation between two keyframes closest to the
         * percentage of an overall animation's length.
         * 
         * @param percent
         * A floating-point value, representing the overall time that has
         * elapsed in an animation.
         * 
         * @param animFlags
         * A set of flags which determines how the output data will be
         * interpolated.
         * 
         * @return The interpolation between two animation frames at a given
         * time of an animation.
         */
        data_t get_interpolated_data(anim_prec_t percent, const animation_flag_t animFlags) const noexcept;
        
        /**
         * Calculate the percent of interpolation which is required to mix the
         * data between two animation frames.
         * 
         * @param totalAnimPercent
         * The overall percent of time elapsed in an animation.
         * 
         * @param outCurrFrame
         * A reference to an unsigned integer, which will contain the array
         * index of the current frame in *this which should be used for
         * interpolation.
         * 
         * @param outNextFrame
         * A reference to an unsigned integer, which will contain the array
         * index of the next frame in *this which should be used for
         * interpolation.
         * 
         * @return A percentage, which should be used to determine the amount
         * of interpolation between the frames at 'outCurrFrane' and
         * 'outNextFrame.' This return value may be greater than the standard
         * range (0.0, 1.0), In such a case, it's up to the function caller to
         * determine what to do.
         */
        anim_prec_t calc_frame_interpolation(
            const anim_prec_t totalAnimPercent,
            unsigned& outCurrFrame,
            unsigned& outNextFrame
        ) const noexcept;
};

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
AnimationKeyList<data_t>::~AnimationKeyList() noexcept {
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
AnimationKeyList<data_t>::AnimationKeyList() noexcept :
    numFrames{0},
    keyTimes{nullptr},
    keyData{nullptr}
{}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
AnimationKeyList<data_t>::AnimationKeyList(const AnimationKeyList& a) noexcept :
    numFrames{0},
    keyTimes{nullptr},
    keyData{nullptr}
{
    *this = a;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
AnimationKeyList<data_t>::AnimationKeyList(AnimationKeyList&& a) noexcept :
    numFrames{a.numFrames},
    keyTimes{std::move(a.keyTimes)},
    keyData{std::move(a.keyData)}
{
    a.numFrames = 0;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
AnimationKeyList<data_t>& AnimationKeyList<data_t>::operator=(const AnimationKeyList& k) noexcept {
    if (this == &k) {
        return *this;
    }
    
    if (!k.numFrames) {
        if (numFrames) {
            clear();
        }
        return *this;
    }
    
    if (k.numFrames != numFrames) {
        keyTimes.reset(new anim_prec_t[k.numFrames]);
        keyData.reset(new data_t[k.numFrames]);
    }
    
    if (!keyTimes || !keyData) {
        clear();
        return *this;
    }
    
    numFrames = k.numFrames;
    
    utils::fast_memcpy(keyTimes.get(), keyTimes.get(), sizeof(anim_prec_t)*numFrames);
    utils::fast_memcpy(keyTimes.get(), keyData.get(), sizeof(data_t)*numFrames);
    
    return *this;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
AnimationKeyList<data_t>& AnimationKeyList<data_t>::operator=(AnimationKeyList&& k) noexcept {
    if (this == &k) {
        return *this;
    }
    
    numFrames = k.numFrames;
    k.numFrames = 0;
    
    keyTimes = std::move(k.keyTimes);
    keyData = std::move(k.keyData);
    
    return *this;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
void AnimationKeyList<data_t>::clear() noexcept {
    numFrames = 0;
    keyTimes.reset();
    keyData.reset();
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline unsigned AnimationKeyList<data_t>::size() const noexcept {
    return numFrames;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
bool AnimationKeyList<data_t>::init(const unsigned keyCount) noexcept {
    if (!keyCount) {
        if (numFrames) {
            clear();
        }
        return true;
    }
    
    if (keyCount != numFrames) {
        keyTimes.reset(new anim_prec_t[keyCount]);
        keyData.reset(new data_t[keyCount]);
    }
    
    if (!keyTimes || !keyData) {
        clear();
        return false;
    }
    
    numFrames = keyCount;
    utils::fast_memset(keyTimes.get(), 0, sizeof(anim_prec_t)*numFrames);
    utils::fast_memset(keyData.get(), 0, sizeof(data_t)*numFrames);
    
    return true;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline bool AnimationKeyList<data_t>::is_valid() const noexcept {
    return numFrames > 0;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline anim_prec_t AnimationKeyList<data_t>::get_duration() const noexcept {
    return get_end_time() - get_start_time();
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline anim_prec_t AnimationKeyList<data_t>::get_start_time() const noexcept {
    return numFrames ? keyTimes[0] : anim_prec_t{0};
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline anim_prec_t AnimationKeyList<data_t>::get_end_time() const noexcept {
    return numFrames ? keyTimes[numFrames-1] : anim_prec_t{0};
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline anim_prec_t AnimationKeyList<data_t>::get_frame_time(const unsigned keyIndex) const noexcept {
    LS_DEBUG_ASSERT(keyIndex < numFrames);
    return keyTimes[keyIndex];
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline const data_t& AnimationKeyList<data_t>::get_frame_data(const unsigned keyIndex) const noexcept {
    LS_DEBUG_ASSERT(keyIndex < numFrames);
    return keyData[keyIndex];
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline data_t& AnimationKeyList<data_t>::get_frame_data(const unsigned keyIndex) noexcept {
    LS_DEBUG_ASSERT(keyIndex < numFrames);
    return keyData[keyIndex];
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline const data_t& AnimationKeyList<data_t>::get_start_data() const noexcept {
    LS_DEBUG_ASSERT(numFrames > 0);
    return keyData[0];
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline const data_t& AnimationKeyList<data_t>::get_end_data() const noexcept {
    LS_DEBUG_ASSERT(numFrames > 0);
    return keyData[numFrames-1];
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
inline void AnimationKeyList<data_t>::set_frame(
    const unsigned frameIndex,
    const anim_prec_t frameTime,
    const data_t& frameData
) noexcept {
    LS_DEBUG_ASSERT(numFrames > 0);
    keyTimes[frameIndex] = frameTime;
    keyData[frameIndex] = frameData;
}

/*-------------------------------------
 * Frame difference interpolator
-------------------------------------*/
template <typename data_t>
inline anim_prec_t AnimationKeyList<data_t>::calc_frame_interpolation(
    const anim_prec_t totalAnimPercent,
    unsigned& outCurrFrame,
    unsigned& outNextFrame
) const noexcept {
    LS_DEBUG_ASSERT(numFrames > 0);
    
    outCurrFrame = 0;
    outNextFrame = 1;
    
    // If there's one thing I hate more in hot code paths than branches, it's
    // loops. One day I'll find out how to get rid of this. The key frames in
    // an animation channel, the longer this will take to run.
    while (keyTimes[outNextFrame] <= totalAnimPercent && outNextFrame < numFrames-1) {
        outCurrFrame++;
        outNextFrame++;
    }
    
    const anim_prec_t currTime = keyTimes[outCurrFrame];
    const anim_prec_t nextTime = keyTimes[outNextFrame];
    const anim_prec_t frameDelta = nextTime - currTime;
    
    const anim_prec_t ret = anim_prec_t{1} - ((nextTime-totalAnimPercent) / frameDelta);
    
    return ret;
}

/*-------------------------------------
-------------------------------------*/
template <typename data_t>
data_t AnimationKeyList<data_t>::get_interpolated_data(anim_prec_t, const animation_flag_t) const noexcept {
    LS_ASSERT(false);
    return data_t{};
}

template <>
math::vec3_t<float> AnimationKeyList<math::vec3_t<float>>::get_interpolated_data(anim_prec_t percent, const animation_flag_t animFlags) const noexcept;

template <>
math::quat_t<float> AnimationKeyList<math::quat_t<float>>::get_interpolated_data(anim_prec_t percent, const animation_flag_t animFlags) const noexcept;




/*-----------------------------------------------------------------------------
 * Pre-Compiled Template Specializations
-----------------------------------------------------------------------------*/
LS_DECLARE_CLASS_TYPE(AnimationKeyListVec3, AnimationKeyList, math::vec3_t<float>);
LS_DECLARE_CLASS_TYPE(AnimationKeyListQuat, AnimationKeyList, math::quat_t<float>);




} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_ANIMATION_KEY_LIST_H__
