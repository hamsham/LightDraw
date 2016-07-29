
#ifndef __LS_DRAW_ANIMATION_PROPERTY_H__
#define __LS_DRAW_ANIMATION_PROPERTY_H__



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



/*-----------------------------------------------------------------------------
 * Data Types used for animations
-----------------------------------------------------------------------------*/
namespace ls {
namespace draw {



/**-------------------------------------
 * @brief Data Type to be used for animation interpolation.
 * 
 * Animations themselves use single-point precision for vectors and quaternions
 * but interpolation between frames may need more precision.
-------------------------------------*/
typedef float anim_prec_t;



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_ANIMATION_PROPERTY_H__ */
