
#include "lightsky/draw/camera.h"

namespace {

enum class view_axis_t : unsigned {
    MV_X_AXIS = 0,
    MV_Y_AXIS = 1,
    MV_Z_AXIS = 2,
    
    MV_POSITION = 3
};

/**
 * @brief Retrieve a camera's post-transformed x, y, z, or position vectors
 * from its model-view matrix.
 *  
 * @param rotationMat
 * A constant reference to the camera's model-view matrix.
 * 
 * @param axis
 * The specific axis of rotation, or position.
 * 
 * @return A 3D vector, containing the transformed model-view matrix position
 * or axis of rotation.
 */
inline ls::math::vec3 extractMVVector(const ls::math::mat4& viewMat, const view_axis_t axis) {
    const ls::math::mat3&& rotationMat = ls::math::mat3{viewMat};
    const unsigned a = LS_ENUM_VAL(axis);
    const ls::math::vec3&& mvVec = ls::math::vec3{-viewMat[a][0], -viewMat[a][1], -viewMat[a][2]};
    return mvVec * ls::math::transpose(rotationMat);
}

}

namespace ls {
namespace draw {

/*-------------------------------------
 * Default Camera Perspective
-------------------------------------*/
const math::mat4 camera::DEFAULT_PERSPECTIVE{
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
camera::~camera() {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
camera::camera() :
    viewMode{camera_mode_t::FIRST_PERSON},
    rotateFunction{&camera::rotateLockedY},
    fov{DEFAULT_VIEW_ANGLE},
    aspectW{DEFAULT_ASPECT_WIDTH},
    aspectH{DEFAULT_ASPECT_HEIGHT},
    zNear{DEFAULT_Z_NEAR},
    zFar{DEFAULT_Z_FAR},
    target{0.f, 0.f, 1.f},
    projMatrix{math::perspective(
        DEFAULT_VIEW_ANGLE,
        DEFAULT_ASPECT_WIDTH/DEFAULT_ASPECT_HEIGHT,
        DEFAULT_Z_NEAR,
        DEFAULT_Z_FAR
    )},
    viewTransform{}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
camera::camera(const camera& c) {
    *this = c;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
camera::camera(camera&& c) {
    *this = c;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
camera& camera::operator = (const camera& c) {
    viewMode = c.viewMode;
    rotateFunction = &camera::rotateLockedY;
    fov = c.fov;
    aspectW = c.aspectW;
    aspectH = c.aspectH;
    zNear = c.zNear;
    zFar = c.zFar;
    target = c.target;
    projMatrix = c.projMatrix;
    viewTransform = c.viewTransform;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
camera& camera::operator =(camera&& c) {
    this->operator=(c);
    return *this;
}

/*-------------------------------------
 * Get the absolute view position
-------------------------------------*/
math::vec3 camera::getAbsolutePosition() const {
    return extractMVVector(viewTransform.getTransform(), view_axis_t::MV_POSITION);
}

/*-------------------------------------
 * Set the camera view mode
-------------------------------------*/
void camera::setViewMode(camera_mode_t mode) {
    if (mode == viewMode) {
        return;
    }
    
    viewMode = mode;
    lookAt(getAbsolutePosition(), target, getUpDirection());
}

/*-------------------------------------
 * Get the forward direction
-------------------------------------*/
math::vec3 camera::getDirection() const {
    return extractMVVector(viewTransform.getTransform(), view_axis_t::MV_Z_AXIS);
}

/*-------------------------------------
 * Retrieve the camera's up vector
-------------------------------------*/
math::vec3 camera::getUpDirection() const {
    return extractMVVector(viewTransform.getTransform(), view_axis_t::MV_Y_AXIS);
}

/*-------------------------------------
 * Set the camera's projection parameters
-------------------------------------*/
void camera::setProjectionParams(
    float inFov,
    float aspectWidth, float aspectHeight,
    float near, float far
) {
    fov = inFov;
    aspectW = aspectWidth;
    aspectH = aspectHeight;
    zNear = near;
    zFar = far;
}

/*-------------------------------------
 * Y-Axis Locking
-------------------------------------*/
void camera::lockYAxis(bool isLocked) {
    rotateFunction = (isLocked)
        ? &camera::rotateLockedY
        : &camera::rotateUnlockedY;
}

/*-------------------------------------
 * Looking at targets
-------------------------------------*/
void camera::lookAt(const math::vec3& eye, const math::vec3& point, const math::vec3& up) {
    target = point;
    
    if (viewMode == camera_mode_t::ARCBALL) {
        viewTransform.extractTransforms(math::lookFrom(eye-target, math::vec3{0.f}, up));
    }
    else {
        viewTransform.extractTransforms(math::lookFrom(eye, target, up));
        viewTransform.setPosition(-eye);
    }
}

/*-------------------------------------
 * Basic Movement and Rotation
-------------------------------------*/
void camera::move(const math::vec3& amount) {
    if (viewMode == camera_mode_t::FIRST_PERSON) {
        viewTransform.move(amount, false);
    }
    else {
        viewTransform.move(amount, true);
    }
}

/*-------------------------------------
 * FPS Rotation (unlocked Y axis)
-------------------------------------*/
void camera::rotateUnlockedY(const math::vec3& amount) {
    const math::quat&& xAxis = math::quat{0.f, amount[0], 0.f, 1.f};
    const math::quat&& yAxis = math::quat{amount[1], 0.f, 0.f, 1.f};
    const math::quat&& zAxis = math::quat{0.f, 0.f, amount[2], 1.f};
    viewTransform.rotate(xAxis * yAxis * zAxis);
}

/*-------------------------------------
 * FPS rotation (locked Y axis)
-------------------------------------*/
void camera::rotateLockedY(const math::vec3& amount) {
    const math::quat&  orientation  = viewTransform.getOrientation();
    const math::quat&& xAxis        = math::quat{0.f, amount[0], 0.f, 1.f};
    const math::quat&& yAxis        = math::quat{amount[1], 0.f, 0.f, 1.f};
    const math::quat&& zAxis        = math::quat{0.f, 0.f, amount[2], 1.f};
    const math::quat&& camRotation  = xAxis * orientation * yAxis * zAxis;
    
    viewTransform.setOrientation(math::normalize(camRotation));
}

/*-------------------------------------
 * Update Implementation
-------------------------------------*/
void camera::update() {
    if (viewMode == camera_mode_t::ARCBALL) {
        viewTransform.applyPreTransforms(math::translate(math::mat4{1.f}, -target));
    }
    else {
        viewTransform.applyTransforms(false);
    }
}

} // end draw namespace
} // end ls namespace
