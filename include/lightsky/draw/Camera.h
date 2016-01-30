
#ifndef __LS_DRAW_CAMERA_H__
#define __LS_DRAW_CAMERA_H__

#include "lightsky/draw/Transform.h"

namespace ls {
namespace draw {
    
enum class camera_mode_t {
    ARCBALL,
    FIRST_PERSON
};

/**----------------------------------------------------------------------------
 * @brief Camera transformation class
-----------------------------------------------------------------------------*/
class Camera {
    public:
        /**
         * @brief Default Camera aspect width.
         */
        static constexpr float DEFAULT_ASPECT_WIDTH = 4.f;

        /**
         * @brief Default Camera aspect height.
         */
        static constexpr float DEFAULT_ASPECT_HEIGHT = 3.f;

        /**
         * @brief Default near-plane distance
         */
        static constexpr float DEFAULT_Z_NEAR = 0.1f;

        /**
         * @brief Default far plane distance
         */
        static constexpr float DEFAULT_Z_FAR = 100.f;

        /**
         * @brief Default angle for the field-of-view
         */
        static constexpr float DEFAULT_VIEW_ANGLE = LS_DEG2RAD(60.f);

        /**
         * @brief Default Perspective-projection matrix.
         */
        static const math::mat4 DEFAULT_PERSPECTIVE;

    private:
        /**
         * @brief viewMode controls whether or not a camera object is to be
         * used in an FPS style or arcball style.
         */
        camera_mode_t viewMode = camera_mode_t::FIRST_PERSON;
        
        /**
         * @brief rotationFunction is an array of class method pointers. This
         * will help keep camera's rotation method just between FPS/arcball
         * rotations with either a locked or unlocked Yaw axis.
         */
        void (Camera::*rotateFunction)(const math::vec3&);

        /**
         * @brief fov Determines the angle of vision for the camera.
         */
        float fov = DEFAULT_VIEW_ANGLE;

        /**
         * @brief Helps to determine the aspect ratio for perspective and
         * orthographic camera modes.
         */
        float aspectW = DEFAULT_ASPECT_WIDTH;

        /**
         * @brief aspectH Helps to determine the aspect ratio for perspective
         * and orthographic camera modes.
         */
        float aspectH = DEFAULT_ASPECT_HEIGHT;

        /**
         * @brief Distance to the nearby occlusion plane.
         */
        float zNear = DEFAULT_Z_NEAR;

        /**
         * @brief Distance to the far occlusion plane.
         */
        float zFar = DEFAULT_Z_FAR;
        
        /**
         * @brief Camera arcball center/target.
         */
        math::vec3 target;

        /**
         * @brief projMatrix contains only the projection parameters of the
         * camera which make up a viewing frustum.
         */
        math::mat4 projMatrix;
        
        /**
         * @brief The viewTransform contains all transformations used for the
         * view matrix.
         */
        Transform viewTransform;

        /**
         * @brief rotateUnlockedY
         * Function to rotate the camera with an unlocked Y axis; This method
         * allows the camera to roll.
         *
         * @param amount
         * The amount, in percent, that the camera should rotate by. These
         * angles correspond to Pitch, Yaw, and Roll, respectively.
         */
        void rotate_unlocked_y(const math::vec3& amount);

        /**
         * @brief rotateLockedY
         * Function to rotate the camera with a locked Y axis; This method
         * prevents the camera from rolling.
         *
         * @param amount
         * The amount, in percent, that the camera should rotate by. These
         * angles correspond to Pitch, Yaw, and Roll, respectively.
         */
        void rotate_locked_y(const math::vec3& amount);

    public:
        /**
         * @brief Destructor
         */
        ~Camera();
        
        /**
         * @brief Constructor
         */
        Camera();

        /**
         * @brief Copy Constructor
         */
        Camera(const Camera&);

        /**
         * @brief Move Constructor
         */
        Camera(Camera&&);

        /**
         * @brief Copy Operator
         */
        Camera& operator = (const Camera&);

        /**
         * @brief Move Operator
         */
        Camera& operator = (Camera&&);

        /**
         * @brief Reconstruct the projection matrix in *this so it uses
         * orthographic projection.
         */
        void make_ortho();

        /**
         * @brief Reconstruct the projection matrix in *this so it uses
         * perspective projection.
         */
        void make_perspective();

        /**
         * @brief Store a set of perspective-projection parameters in *this so
         * the next call to *makeOrtho()" or "makePerspective()" will adjust
         * according to the input parameters.
         * 
         * @param inFov
         * Determines the horizontal viewing angle, in radians, of the
         * projection matrix.
         * 
         * @param aspectWidth
         * Determines the width, in user-defined units, of the camera's
         * projection matrix.
         * 
         * @param aspectHeight
         * Determines the height, in user-defined units, of the camera's
         * projection matrix.
         * 
         * @param near
         * Determines the internal projection's clipping distance for nearby
         * objects.
         * 
         * @param far
         * Determines the internal projection's clipping distance for distant
         * objects.
         */
        void set_projection_params(
            float inFov         = DEFAULT_VIEW_ANGLE,
            float aspectWidth   = DEFAULT_ASPECT_WIDTH,
            float aspectHeight  = DEFAULT_ASPECT_HEIGHT,
            float near          = DEFAULT_Z_NEAR,
            float far           = DEFAULT_Z_FAR
        );
        
        /**
         * @brief Get the view/look mode for *this.
         * 
         * @return An enumeration of the type camera_mode_t.
         */
        camera_mode_t get_view_mode() const;
        
        /**
         * @brief Set the view/look mode for *this.
         * 
         * @param mode
         * An enumeration of the type camera_mode_t.
         */
        void set_view_mode(camera_mode_t mode);

        /**
         * @brief Get the current position of the camera in 3D cartesian space.
         * 
         * @return A 3D vector, containing the X, Y, and Z position of the
         * camera's view matrix.
         */
        const math::vec3& get_position() const;

        /**
         * @brief Get the current position of the view matrix.
         * 
         * @return A 3D vector, containing the first three values of the final
         * row of the view matrix, representing the absolute position before
         * a perspective divide.
         */
        math::vec3 get_abs_position() const;

        /**
         * @brief Set the position of the camera in 3D cartesian space.
         * 
         * @param p
         * A 3D vector, containing the desired X, Y, and Z position of the
         * camera.
         */
        void set_position(const math::vec3& p);

        /**
         * @brief Get the center target of the arcball rotation camera in 3D
         * cartesian space.
         * 
         * @return A 3D vector, containing the X, Y, and Z position of the
         * camera's orbital movement.
         */
        const math::vec3& get_target() const;

        /**
         * @brief Set the center target of the arcball rotation camera in 3D
         * cartesian space.
         * 
         * @param t
         * A 3D vector, containing the desired X, Y, and Z position of the
         * camera's orbital movement.
         */
        void set_target(const math::vec3& p);

        /**
         * @brief Retrieve the current direction that the camera is facing.
         * 
         * @return A 3D unit vector containing the view matrix Z-axis.
         */
        math::vec3 get_direction() const;

        /**
         * @brief Get the camera's normal vector.
         * 
         * @return a 3D vector, used to determine which direction is upwards.
         */
        math::vec3 get_up_direction() const;

        /**
         * @brief Retrieve the camera's view matrix for external use.
         * 
         * @return A 4x4 rotation matrix which is used for the camera's
         * internal orientation.
         */
        const math::mat4& get_view_matrix() const;

        /**
         * @brief Retrieve the camera's projection matrix for external use.
         * 
         * @return A 4x4 projection matrix which is used by *this for
         * projecting geometry data.
         */
        const math::mat4& get_proj_matrix() const;

        /**
         * @brief Calculate the combined view-projection matrix of the camera.
         * 
         * @return The view and projection matrices, multiplied together to
         * create a VP matrix.
         */
        math::mat4 get_vp_matrix() const ;

        /**
         * @brief Get the internal orientation of the camera.
         * 
         * @return A quaternion used internally to represent the camera's
         * 3D orientation.
         */
        const math::quat& get_orientation() const;

        /**
         * @brief Set the internal orientation of the camera.
         * 
         * @param o
         * A quaternion that will be used to represent the camera's 3D
         * orientation.
         */
        void set_orientation(const math::quat& o);
        
        /**
         * @brief Set the field of view for the camera;
         * 
         * Remember to call either "makeOrtho()" or "makePerspective()" after
         * calling this method.
         * 
         * @param viewAngle
         * The desired horizontal angle, in radians, which the field of view
         * should be set to.
         */
        void set_fov(unsigned viewAngle);
        
        /**
         * @brief Retrieve the horizontal field of view of the camera.
         * 
         * @return A floating point number, representing the number of radians
         * of the field of view of the camera.
         */
        float get_fov() const;

        /**
         * @brief Set the aspect ration of the internal camera.
         * 
         * @param w
         * The width of the projection matrix frustum.
         * 
         * @param h
         * The height of the projection matrix frustum.
         */
        void set_aspect_ratio(float w, float h);

        /**
         * @brief Get the aspect ratio of the camera's projection matrix.
         * 
         * @return A floating-point value, representing the width of the
         * projection matrix frustum, divided by its height.
         */
        float get_aspect_ratio() const;

        /**
         * @brief Get the width of the projection matrix frustum.
         * 
         * @return A floating-point value, representing the width of the
         * projection matrix frustum.
         */
        float get_aspect_width() const;

        /**
         * @brief Get the height of the projection matrix frustum.
         * 
         * @return A floating-point value, representing the height of the
         * projection matrix frustum.
         */
        float get_aspect_height() const;

        /**
         * @brief Set the distance to the camera's near-clipping plane.
         * 
         * @param inZnear
         * A floating point value, representing the distance to the camera's
         * near-clipping plane.
         */
        void set_near_plane(float inZNear);

        /**
         * @brief Get the distance to the camera's near-clipping plane.
         * 
         * @return A floating point value, representing the distance to the
         * camera's near-clipping plane.
         */
        float get_near_plane() const;

        /**
         * @brief Set the distance to the camera's far-clipping plane.
         * 
         * @param inZfar
         * A floating point value, representing the distance to the camera's
         * far-clipping plane.
         */
        void set_far_plane(float inZFar);

        /**
         * @brief Get the distance to the camera's far-clipping plane.
         * 
         * @return A floating point value, representing the distance to the
         * camera's far-clipping plane.
         */
        float get_far_plane() const;

        /**
         * @brief Set whether or not the Y axis of the camera should be locked.
         * 
         * @param shouldLock
         * A boolean value, determining if the camera should be permitted to
         * roll.
         */
        void lock_y_axis(bool shouldLock);

        /**
         * @brief Make the camera frustum face in the direction of a target.
         * 
         * @param eye
         * The desired position of the camera.
         * 
         * @param point
         * The point at which the view/projection frustum should face.
         * 
         * @param up
         * The direction which determines which way is upwards.
         */
        void look_at(const math::vec3& eye, const math::vec3& point, const math::vec3& up = {0.f, 1.f, 0.f});

        /**
         * @brief Look at a point using the camera's current position and
         * normal vector.
         * 
         * @param point
         * A point that the camera should look at.
         */
        void look_at(const math::vec3& point);

        /**
         * @brief Modify the camera's current position by a given amount.
         * 
         * @param amount
         * The distance that the camera's current position should be modified
         * by.
         */
        void move(const math::vec3& amount);

        /**
         * @brief Modify the camera's current orientation by a given amount.
         * 
         * @param amount
         * The rotation delta which should be applied to the camera's current
         * rotation.
         */
        void rotate(const math::vec3& amount);

        /**
         * @brief Apply all pending updates to the camera's view+projection
         * matrix.
         */
        void update();
};

/*-------------------------------------
 * Create an orthographic camera
-------------------------------------*/
inline void Camera::make_ortho() {
    projMatrix = math::ortho(0.f, aspectW, 0.f, aspectH, zNear, zFar);
}

/*-------------------------------------
 * Create an perspective-projection camera
-------------------------------------*/
inline void Camera::make_perspective() {
    projMatrix = math::perspective(fov, aspectW/aspectH, zNear, zFar);
}

/*-------------------------------------
 * Get the camera view mode
-------------------------------------*/
inline camera_mode_t Camera::get_view_mode() const {
    return viewMode;
}

/*-------------------------------------
 * Get the camera position
-------------------------------------*/
inline const math::vec3& Camera::get_position() const {
    return viewTransform.get_position();
}

/*-------------------------------------
 * Set the camera position
-------------------------------------*/
inline void Camera::set_position(const math::vec3& p) {
    viewTransform.set_position(p);
}

/*-------------------------------------
 * Get the camera target
-------------------------------------*/
inline const math::vec3& Camera::get_target() const {
    return target;
}

/*-------------------------------------
 * Set the camera target
-------------------------------------*/
inline void Camera::set_target(const math::vec3& t) {
    target = t;
    viewTransform.set_dirty();
}

/*-------------------------------------
 * Get the camera view matrix
-------------------------------------*/
inline const math::mat4& Camera::get_view_matrix() const {
    return viewTransform.get_transform();
}

/*-------------------------------------
 * Get the projection matrix
-------------------------------------*/
inline const math::mat4& Camera::get_proj_matrix() const {
    return projMatrix;
}

/*-------------------------------------
 * Generate and return the View/Projection matrix
-------------------------------------*/
inline math::mat4 Camera::get_vp_matrix() const {
    return projMatrix * viewTransform.get_transform();
}

/*-------------------------------------
 * Get the camera's orientation
-------------------------------------*/
inline const math::quat& Camera::get_orientation() const {
    return viewTransform.get_orientation();
}

/*-------------------------------------
 * Set the camera's rotation orientation.
-------------------------------------*/
inline void Camera::set_orientation(const math::quat& o) {
    viewTransform.set_orientation(o);
}

/*-------------------------------------
 * Set the FOV
-------------------------------------*/
inline void Camera::set_fov(unsigned viewAngle) {
    fov = viewAngle;
}

/*-------------------------------------
 * Get the FOV
-------------------------------------*/
inline float Camera::get_fov() const {
    return fov;
}

/*-------------------------------------
 * Set the aspect ratio
-------------------------------------*/
inline void Camera::set_aspect_ratio(float w, float h) {
    aspectW = w;
    aspectH = h;
}

/*-------------------------------------
 * Get the current aspect ratio.
-------------------------------------*/
inline float Camera::get_aspect_ratio() const {
    return aspectW / aspectH;
}

/*-------------------------------------
 * Get the current aspect width.
-------------------------------------*/
inline float Camera::get_aspect_width() const {
    return aspectW;
}

/*-------------------------------------
 * Get the current aspect height.
-------------------------------------*/
inline float Camera::get_aspect_height() const {
    return aspectH;
}

/*-------------------------------------
 * Set the near plane distance
-------------------------------------*/
inline void Camera::set_near_plane(float inZNear) {
    zNear = inZNear;
}

/*-------------------------------------
 * Get the near plane distance
-------------------------------------*/
inline float Camera::get_near_plane() const {
    return zNear;
}

/*-------------------------------------
 * Set the far plane distance
-------------------------------------*/
inline void Camera::set_far_plane(float inZFar) {
    zFar = inZFar;
}

/*-------------------------------------
 * Get the distance to the far plane.
-------------------------------------*/
inline float Camera::get_far_plane() const {
    return zFar;
}

/*-------------------------------------
 * Look At function from the current cam position
-------------------------------------*/
inline void Camera::look_at(const math::vec3& point) {
    look_at(viewTransform.get_position(), point, get_up_direction());
}

/*-------------------------------------
 * Camera Rotation function
-------------------------------------*/
inline void Camera::rotate(const math::vec3& amount) {
    (this->*rotateFunction)(amount);
}

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_CAMERA_H__
