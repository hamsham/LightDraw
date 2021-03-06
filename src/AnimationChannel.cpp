
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
namespace
{

namespace math = ls::math;
namespace draw = ls::draw;
using draw::anim_prec_t;
} // end anonymous namespace


namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Animation Key Structure
------------------------------------------------------------------------------*/
/*-------------------------------------
 * Animation Key Destructor
-------------------------------------*/
AnimationChannel::~AnimationChannel() noexcept
{
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
{
}

/*-------------------------------------
 * Animation Key Copy Constructor
-------------------------------------*/
AnimationChannel::AnimationChannel(const AnimationChannel& ac) noexcept :
    animationMode{ac.animationMode},
    positionFrames{ac.positionFrames},
    scaleFrames{ac.scaleFrames},
    rotationFrames{ac.rotationFrames}
{
}

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
AnimationChannel& AnimationChannel::operator=(const AnimationChannel& ac) noexcept
{
    if (this == &ac)
    {
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
AnimationChannel& AnimationChannel::operator=(AnimationChannel&& ac) noexcept
{
    if (this == &ac)
    {
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
) noexcept
{
    if (!positionFrames.init(posCount)
        || !scaleFrames.init(sclCount)
        || !rotationFrames.init(rotCount)
        )
    {
        clear();
        return false;
    }

    return true;
}

/*-------------------------------------
 * Clear all Animation keys
-------------------------------------*/
void AnimationChannel::clear() noexcept
{
    animationMode = animation_flag_t::ANIM_FLAG_DEFAULT;
    positionFrames.clear();
    scaleFrames.clear();
    rotationFrames.clear();
}

/*-------------------------------------
 * Retrieve the starting time of the current animation track.
-------------------------------------*/
anim_prec_t AnimationChannel::get_start_time() const noexcept
{
    return math::min(
        positionFrames.get_start_time(),
        scaleFrames.get_start_time(),
        rotationFrames.get_start_time()
    );
}

/*-------------------------------------
 * Assign a start time for the current animation track.
-------------------------------------*/
void AnimationChannel::set_start_time(const anim_prec_t startOffset) noexcept
{
    const anim_prec_t posOffset = positionFrames.get_start_time() - get_start_time();
    positionFrames.set_start_time(startOffset + posOffset);

    const anim_prec_t sclOffset = scaleFrames.get_start_time() - get_start_time();
    scaleFrames.set_start_time(startOffset + sclOffset);

    const anim_prec_t rotOffset = rotationFrames.get_start_time() - get_start_time();
    rotationFrames.set_start_time(startOffset + rotOffset);
}

/*-------------------------------------
 * Retrieve the ending time of the current animation track.
-------------------------------------*/
anim_prec_t AnimationChannel::get_end_time() const noexcept
{
    return math::max(
        positionFrames.get_end_time(),
        scaleFrames.get_end_time(),
        rotationFrames.get_end_time()
    );
}
} // end draw namespace
} // end ls namespace
