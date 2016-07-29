
#ifndef __LS_DRAW_CAMERA_H__
#define __LS_DRAW_CAMERA_H__

#include "ls/math/vec2.h"
#include "ls/math/vec3.h"
#include "ls/math/mat4.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 Forward Declarations
-----------------------------------------------------------------------------*/
class Camera;
class BoundingBox;



/*-----------------------------------------------------------------------------
 Utility Functions
-----------------------------------------------------------------------------*/
/**
 * Determine if a point is contained within a frustum.
 * 
 * @param point
 * A 3D vector which will be tested for visibility in a view frustum.
 * 
 * @param mvpMatrix
 * A 4x4 homogenous matrix representing the input point's clip-space
 * coordinates.
 * 
 * @param fovDivisor
 * This value can be increased to reduce the calculated frustum's dimensions.
 * This is mostly used for debugging purposes.
 * 
 * @return TRUE if the input point is within the frustum extracted from
 * "mvpMatrix", FALSE if not.
 */
bool is_visible(const math::vec3& point, const math::mat4& mvpMatrix, const float fovDivisor = 1.f);

/**
 * Determine if a bounding box is contained within a frustum..
 * 
 * @param bb
 * A bounding box object which will be tested for visibility in a view frustum.
 * 
 * @param mvpMatrix
 * A 4x4 homogenous matrix representing the clip-space coordinates of the input
 * bounding box
 * 
 * @param fovDivisor
 * This value can be increased to reduce the calculated frustum's dimensions.
 * This is mostly used for debugging purposes.
 * 
 * @return TRUE if the input box is within the frustum extracted from
 * "mvpMatrix", FALSE if not.
 */
bool is_visible(const BoundingBox& bb, const math::mat4& mvpMatrix, const float fovDivisor = 1.f);



/**----------------------------------------------------------------------------
 * @brief View modes for Camera objects
-----------------------------------------------------------------------------*/
enum projection_type_t : uint32_t {
    PROJECTION_ORTHOGONAL,
    PROJECTION_PERSPECTIVE,
    PROJECTION_LOGARITHMIC_PERSPECTIVE,
    
    DEFAULT_PROJECTION = PROJECTION_PERSPECTIVE,
};



/**----------------------------------------------------------------------------
 * @brief Camera transformation class
-----------------------------------------------------------------------------*/
class Camera {
    friend class SceneFileLoader;
    
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
     * Flag to determine if *this camera needs updating.
     */
    bool dirtyFlag;
    
    /**
     * Projection type for the camera. This can help determine if the current
     * projection matrix is orthogonal, perspective, or represents a
     * logarithmic (pseudo-infinite) perspective matrix.
     */
    projection_type_t projectType;
    
    /**
     * @brief fov Determines the angle of vision for the camera.
     */
    float fov;

    /**
     * @brief Helps to determine the aspect ratio for perspective and
     * orthographic camera modes.
     */
    float aspectW;

    /**
     * @brief aspectH Helps to determine the aspect ratio for perspective
     * and orthographic camera modes.
     */
    float aspectH;

    /**
     * @brief Distance to the nearby occlusion plane.
     */
    float zNear;

    /**
     * @brief Distance to the far occlusion plane.
     */
    float zFar;

    /**
     * @brief projMatrix contains only the projection parameters of the
     * camera which make up a viewing frustum.
     */
    math::mat4 projMatrix;

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
    Camera& operator=(const Camera&);

    /**
     * @brief Move Operator
     */
    Camera& operator=(Camera&&);

    /**
     * @brief Assign a projection type for the current camera.
     * 
     * @param p
     * A value from the projection_type_t enumeration which determines what
     * type of frustum to use for the projection matrix.
     */
    void set_projection_type(const projection_type_t p);

    /**
     * @brief Retrieve the current frustum type for the projection matrix.
     * 
     * @return A value from the projection_type_t enumeration which determines
     * what type of frustum to use for the projection matrix.
     */
    projection_type_t get_projection_type() const;

    /**
     * @brief Retrieve the camera's projection matrix for external use.
     *
     * @return A 4x4 projection matrix which is used by *this for
     * projecting geometry data.
     */
    const math::mat4& get_proj_matrix() const;

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
     * @brief Set the aspect ration of the internal camera.
     *
     * @param aspect
     * A 2D vector containing both the width and height of the projection
     * matrix frustum.
     */
    void set_aspect_ratio(const math::vec2& aspect);

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
     * Determine if *this Camera object needs an update.
     * 
     * @return TRUE if a call to "update()" is needed to apply any pending
     * projection updates, FALSE if not.
     */
    bool is_dirty() const noexcept;

    /**
     * @brief Apply all pending updates to the camera's view+projection
     * matrix.
     */
    void update();
};

/*-------------------------------------
 * Get the projection matrix
-------------------------------------*/
inline const math::mat4& Camera::get_proj_matrix() const {
    return projMatrix;
}

/*-------------------------------------
 * Set the FOV
-------------------------------------*/
inline void Camera::set_fov(unsigned viewAngle) {
    dirtyFlag = true;
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
    dirtyFlag = true;
    aspectW = w;
    aspectH = h;
}

/*-------------------------------------
 * Set the aspect ratio
-------------------------------------*/
inline void Camera::set_aspect_ratio(const math::vec2& aspect) {
    dirtyFlag = true;
    aspectW = aspect[0];
    aspectH = aspect[1];
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
    dirtyFlag = true;
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
    dirtyFlag = true;
    zFar = inZFar;
}

/*-------------------------------------
 * Get the distance to the far plane.
-------------------------------------*/
inline float Camera::get_far_plane() const {
    return zFar;
}

/*-------------------------------------
 * Camera Update Inquiry
-------------------------------------*/
inline bool Camera::is_dirty() const noexcept {
    return dirtyFlag;
}

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_CAMERA_H__
