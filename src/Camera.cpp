
#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Log.h"

#include "lightsky/math/vec4.h"
#include "lightsky/math/mat_utils.h" // perspective transformation functions

#include "lightsky/draw/BoundingBox.h"
#include "lightsky/draw/Camera.h"



namespace math = ls::math;



/*-----------------------------------------------------------------------------
 * Camera Functions
-----------------------------------------------------------------------------*/
namespace ls
{
/*-------------------------------------
 * Test the visibility of a point
-------------------------------------*/
bool draw::is_visible(const math::vec3& point, const math::mat4& mvpMatrix, const float fovDivisor)
{
    math::vec4&& temp = mvpMatrix * math::vec4{point[0], point[1], point[2], 1.f};

    // Debug multipliers to reduce the frustum planes
    temp[0] *= fovDivisor;
    temp[1] *= fovDivisor;

    return
        temp[0] > -temp[3] && temp[0] < temp[3] &&
        temp[1] > -temp[3] && temp[1] < temp[3] &&
        temp[2] > -temp[3] && temp[2] < temp[3];
}

/*-------------------------------------
 * Test the visibility of a Bounding Box
-------------------------------------*/
bool draw::is_visible(const BoundingBox& bb, const math::mat4& mvpMatrix, const float fovDivisor)
{
    const math::vec3& trr = bb.get_top_rear_right();
    const math::vec3& bfl = bb.get_bot_front_left();

    const math::vec3 points[] = {
        {trr[0], bfl[1], bfl[2]},
        {trr[0], trr[1], bfl[2]},
        {trr[0], trr[1], trr[2]},
        {bfl[0], trr[1], trr[2]},
        {bfl[0], bfl[1], trr[2]},
        {bfl[0], bfl[1], bfl[2]},

        {trr[0], bfl[1], trr[2]},
        {bfl[0], trr[1], bfl[2]},
    };

    for (unsigned i = 0; i < LS_ARRAY_SIZE(points); ++i)
    {
        if (draw::is_visible(points[i], mvpMatrix, fovDivisor))
        {
            return true;
        }
    }

    return false;
}


/*-----------------------------------------------------------------------------
 * Camera Class
-----------------------------------------------------------------------------*/
namespace draw
{
/*-------------------------------------
 * Default Camera Perspective
-------------------------------------*/
const math::mat4 Camera::DEFAULT_PERSPECTIVE{
    math::perspective(
        DEFAULT_VIEW_ANGLE,
        DEFAULT_ASPECT_WIDTH / DEFAULT_ASPECT_HEIGHT,
        DEFAULT_Z_NEAR,
        DEFAULT_Z_FAR
    )
};

/*-------------------------------------
 * Destructor
-------------------------------------*/
Camera::~Camera()
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Camera::Camera() :
    dirtyFlag{false},
    projectType{projection_type_t::DEFAULT_PROJECTION},
    fov{DEFAULT_VIEW_ANGLE},
    aspectW{DEFAULT_ASPECT_WIDTH},
    aspectH{DEFAULT_ASPECT_HEIGHT},
    zNear{DEFAULT_Z_NEAR},
    zFar{DEFAULT_Z_FAR},
    projMatrix{math::perspective(
        DEFAULT_VIEW_ANGLE,
        DEFAULT_ASPECT_WIDTH / DEFAULT_ASPECT_HEIGHT,
        DEFAULT_Z_NEAR,
        DEFAULT_Z_FAR
    )}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
Camera::Camera(const Camera& c)
{
    *this = c;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
Camera::Camera(Camera&& c)
{
    *this = c;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
Camera& Camera::operator=(const Camera& c)
{
    dirtyFlag = c.dirtyFlag;
    projectType = c.projectType;
    fov = c.fov;
    aspectW = c.aspectW;
    aspectH = c.aspectH;
    zNear = c.zNear;
    zFar = c.zFar;
    projMatrix = c.projMatrix;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
Camera& Camera::operator=(Camera&& c)
{
    dirtyFlag = c.dirtyFlag;
    c.dirtyFlag = false;

    projectType = c.projectType;
    c.projectType = projection_type_t::DEFAULT_PROJECTION;

    fov = c.fov;
    c.fov = DEFAULT_VIEW_ANGLE;

    aspectW = c.aspectW;
    c.aspectW = DEFAULT_ASPECT_WIDTH;

    aspectH = c.aspectH;
    c.aspectH = DEFAULT_ASPECT_HEIGHT;

    zNear = c.zNear;
    c.zNear = DEFAULT_Z_NEAR;

    zFar = c.zFar;
    c.zFar = DEFAULT_Z_FAR;

    projMatrix = c.projMatrix;
    c.projMatrix = math::perspective(
        DEFAULT_VIEW_ANGLE,
        DEFAULT_ASPECT_WIDTH / DEFAULT_ASPECT_HEIGHT,
        DEFAULT_Z_NEAR,
        DEFAULT_Z_FAR
    );

    return *this;
}

/*-------------------------------------
 * Set the current projection mode
-------------------------------------*/
void Camera::set_projection_type(const projection_type_t p)
{
    dirtyFlag = true;
    projectType = p;
}

/*-------------------------------------
 * Update Implementation
-------------------------------------*/
void Camera::update()
{
    dirtyFlag = false;

    switch (projectType)
    {
        case projection_type_t::PROJECTION_ORTHOGONAL:
            projMatrix = math::ortho(-aspectW, aspectW, -aspectH, aspectH, zNear, zFar);
            break;
        case projection_type_t::PROJECTION_PERSPECTIVE:
            projMatrix = math::perspective(fov, aspectW / aspectH, zNear, zFar);
            break;
        case projection_type_t::PROJECTION_LOGARITHMIC_PERSPECTIVE:
            projMatrix = math::infinite_perspective(fov, aspectW / aspectH, zNear);
            break;
        default:
            LS_DEBUG_ASSERT(false);
            break;
    }
}
} // end draw namespace
} // end ls namespace
