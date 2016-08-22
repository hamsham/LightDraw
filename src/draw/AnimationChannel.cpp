
#include <utility>

#include "lightsky/math/Math.h"

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Copy.h"
#include "lightsky/utils/Log.h"

#include "lightsky/draw/AnimationChannel.h"
#include "lightsky/draw/SceneGraph.h"



/*-----------------------------------------------------------------------------
 * Anonymous helper functions
------------------------------------------------------------------------------*/
namespace {

namespace math = ls::math;
namespace draw = ls::draw;
using draw::anim_prec_t;



} // end anonymous namespace


namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Animation Key Structure
------------------------------------------------------------------------------*/
/*-------------------------------------
 * Animation Key Destructor
-------------------------------------*/
AnimationChannel::~AnimationChannel() noexcept {
    clear();
}

/*-------------------------------------
 * Animation Key Constructor
-------------------------------------*/
AnimationChannel::AnimationChannel() noexcept :
    animationMode{animation_flag_t::ANIM_FLAG_DEFAULT},
    positionFrames{},
    scaleFrames{},
    rotationFrames{}
{}

/*-------------------------------------
 * Animation Key Copy Constructor
-------------------------------------*/
AnimationChannel::AnimationChannel(const AnimationChannel& ac) noexcept :
    animationMode{ac.animationMode},
    positionFrames{ac.positionFrames},
    scaleFrames{ac.scaleFrames},
    rotationFrames{ac.rotationFrames}
{}

/*-------------------------------------
 * Animation Key Move Constructor
-------------------------------------*/
AnimationChannel::AnimationChannel(AnimationChannel&& ac) noexcept :
    animationMode{ac.animationMode},
    positionFrames{std::move(ac.positionFrames)},
    scaleFrames{std::move(ac.scaleFrames)},
    rotationFrames{std::move(ac.rotationFrames)}
{
    ac.animationMode = animation_flag_t::ANIM_FLAG_DEFAULT;
}

/*-------------------------------------
 * Animation Key Copy Operator
-------------------------------------*/
AnimationChannel& AnimationChannel::operator =(const AnimationChannel& ac) noexcept {
    if (this == &ac) {
        return *this;
    }
    
    animationMode = ac.animationMode;
    positionFrames = ac.positionFrames;
    scaleFrames = ac.scaleFrames;
    rotationFrames = ac.rotationFrames;
    
    return *this;
}

/*-------------------------------------
 * Animation Key Destructor
-------------------------------------*/
AnimationChannel& AnimationChannel::operator =(AnimationChannel&& ac) noexcept {
    if (this == &ac) {
        return *this;
    }
    
    animationMode = ac.animationMode;
    ac.animationMode = animation_flag_t::ANIM_FLAG_DEFAULT;
    
    positionFrames = std::move(ac.positionFrames);
    scaleFrames = std::move(ac.scaleFrames);
    rotationFrames = std::move(ac.rotationFrames);

    return *this;
}

/*-------------------------------------
 * Set the number of keys
-------------------------------------*/
bool AnimationChannel::set_num_frames(
    const unsigned posCount,
    const unsigned sclCount,
    const unsigned rotCount
) noexcept {
    if (!positionFrames.init(posCount)
    || !scaleFrames.init(sclCount)
    || !rotationFrames.init(rotCount)
    ) {
        clear();
        return false;
    }
    
    return true;
}

/*-------------------------------------
 * Clear all Animation keys
-------------------------------------*/
void AnimationChannel::clear() noexcept {
    animationMode = animation_flag_t::ANIM_FLAG_DEFAULT;
    positionFrames.clear();
    scaleFrames.clear();
    rotationFrames.clear();
}



} // end draw namespace
} // end ls namespace
