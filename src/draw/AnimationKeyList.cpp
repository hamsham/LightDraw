
#include "ls/math/Math.h"

#include "ls/utils/Log.h"

#include "ls/draw/AnimationKeyList.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Template Specializations
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * 3D vector interpolation
-------------------------------------*/
template <>
math::vec3 AnimationKeyList<math::vec3>::get_interpolated_data(anim_prec_t percent, const animation_flag_t animFlags) const noexcept {
    if (percent <= get_start_time()) {
        return get_start_data();
    }
    
    if (percent >= get_end_time() && (animFlags & animation_flag_t::ANIM_FLAG_REPEAT) == 0) {
        return get_end_data();
    }
    
    unsigned currFrame, nextFrame;
    anim_prec_t interpAmount = calc_frame_interpolation(percent, currFrame, nextFrame);
    
    if ((animFlags & animation_flag_t::ANIM_FLAG_IMMEDIATE) != 0) {
        interpAmount = anim_prec_t{0.0};
    }
    
    const math::vec3& c = keyData[currFrame];
    const math::vec3& n = keyData[nextFrame];
    
    return math::mix<float>(c, n, interpAmount);
}

/*-------------------------------------
 * Quaternion interpolation
-------------------------------------*/
template <>
math::quat AnimationKeyList<math::quat>::get_interpolated_data(anim_prec_t percent, const animation_flag_t animFlags) const noexcept {
    if (percent <= get_start_time()) {
        return get_start_data();
    }
    
    if (percent >= get_end_time() && (animFlags & animation_flag_t::ANIM_FLAG_REPEAT) == 0) {
        return get_end_data();
    }
    
    unsigned currFrame, nextFrame;
    anim_prec_t interpAmount = calc_frame_interpolation(percent, currFrame, nextFrame);
    
    if ((animFlags & animation_flag_t::ANIM_FLAG_IMMEDIATE) != 0) {
        interpAmount = anim_prec_t{0.0};
    }
    
    const math::quat& c = keyData[currFrame];
    const math::quat& n = keyData[nextFrame];
    
    // Regular linear interpolations don't work correctly for rotations over
    // 180 degrees.
    return math::slerp<float>(c, n, interpAmount);
}


/*-----------------------------------------------------------------------------
 * Pre-Compiled Template Specializations
-----------------------------------------------------------------------------*/
LS_DEFINE_CLASS_TYPE(AnimationKeyList, math::vec3);
LS_DEFINE_CLASS_TYPE(AnimationKeyList, math::quat);




} // end draw namespace
} // end ls namespace
