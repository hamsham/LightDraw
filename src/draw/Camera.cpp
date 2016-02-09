
#include "lightsky/draw/Camera.h"

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
    const unsigned          a           = LS_ENUM_VAL(axis);
    const ls::math::mat3&&  rotationMat = ls::math::mat3{viewMat};
    const ls::math::vec3&&  mvVec       = -rotationMat[a];
    
    return mvVec * ls::math::transpose(rotationMat);
}

}

namespace ls {
namespace draw {

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
Camera::~Camera() {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Camera::Camera() :
    viewMode{camera_mode_t::FIRST_PERSON},
    rotateFunction{&Camera::rotate_locked_y},
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
Camera::Camera(const Camera& c) {
    *this = c;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
Camera::Camera(Camera&& c) {
    *this = c;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
Camera& Camera::operator = (const Camera& c) {
    viewMode = c.viewMode;
    rotateFunction = &Camera::rotate_locked_y;
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
Camera& Camera::operator =(Camera&& c) {
    this->operator=(c);
    return *this;
}

/*-------------------------------------
 * Get the absolute view position
-------------------------------------*/
math::vec3 Camera::get_abs_position() const {
    //return extractMVVector(viewTransform.get_transform(), view_axis_t::MV_POSITION);
    return viewTransform.get_abs_position();
}

/*-------------------------------------
 * Set the camera view mode
-------------------------------------*/
void Camera::set_view_mode(camera_mode_t mode) {
    if (mode == viewMode) {
        return;
    }
    
    viewMode = mode;
    look_at(get_abs_position(), target, get_up_direction());
}

/*-------------------------------------
 * Get the forward direction
-------------------------------------*/
math::vec3 Camera::get_eye_direction() const {
    return extractMVVector(viewTransform.get_transform(), view_axis_t::MV_Z_AXIS);
}

/*-------------------------------------
 * Retrieve the camera's up vector
-------------------------------------*/
math::vec3 Camera::get_up_direction() const {
    return extractMVVector(viewTransform.get_transform(), view_axis_t::MV_Y_AXIS);
}

/*-------------------------------------
 * Retrieve the camera's right vector
-------------------------------------*/
math::vec3 Camera::get_right_direction() const {
    return extractMVVector(viewTransform.get_transform(), view_axis_t::MV_X_AXIS);
}

/*-------------------------------------
 * Set the camera's projection parameters
-------------------------------------*/
void Camera::set_projection_params(
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
void Camera::lock_y_axis(bool isLocked) {
    rotateFunction = (isLocked)
        ? &Camera::rotate_locked_y
        : &Camera::rotate_unlocked_y;
}

/*-------------------------------------
 * Looking at targets
-------------------------------------*/
void Camera::look_at(const math::vec3& eye, const math::vec3& point, const math::vec3& up) {
    target = point;
    
    if (viewMode == camera_mode_t::ARCBALL) {
        viewTransform.extract_transforms(math::look_from(eye-target, math::vec3{0.f}, up));
    }
    else {
        viewTransform.extract_transforms(math::look_from(eye, target, up));
        viewTransform.set_position(-eye);
    }
}

/*-------------------------------------
 * Basic Movement and Rotation
-------------------------------------*/
void Camera::move(const math::vec3& amount) {
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
void Camera::rotate_unlocked_y(const math::vec3& amount) {
    const math::quat&& xAxis = math::quat{0.f, amount[0], 0.f, 1.f};
    const math::quat&& yAxis = math::quat{amount[1], 0.f, 0.f, 1.f};
    const math::quat&& zAxis = math::quat{0.f, 0.f, amount[2], 1.f};
    viewTransform.rotate(xAxis * yAxis * zAxis);
}

/*-------------------------------------
 * FPS rotation (locked Y axis)
-------------------------------------*/
void Camera::rotate_locked_y(const math::vec3& amount) {
    const math::quat&  orientation  = viewTransform.get_orientation();
    const math::quat&& xAxis        = math::quat{0.f, amount[0], 0.f, 1.f};
    const math::quat&& yAxis        = math::quat{amount[1], 0.f, 0.f, 1.f};
    const math::quat&& zAxis        = math::quat{0.f, 0.f, amount[2], 1.f};
    const math::quat&& camRotation  = xAxis * orientation * yAxis * zAxis;
    
    viewTransform.set_orientation(math::normalize(camRotation));
}

/*-------------------------------------
 * Update Implementation
-------------------------------------*/
void Camera::update() {
    if (viewMode == camera_mode_t::ARCBALL) {
        viewTransform.apply_pre_transforms(math::translate(math::mat4{1.f}, target));
    }
    else {
        viewTransform.apply_transform(false);
    }
}

} // end draw namespace
} // end ls namespace
