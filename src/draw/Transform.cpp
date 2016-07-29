
#include <utility> // std::move

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/Transform.h"



/*-----------------------------------------------------------------------------
 * Anonymous helper stuff for the Camera class.
-----------------------------------------------------------------------------*/
namespace {



enum class view_axis_t : unsigned {
    MV_X_AXIS = 0,
    MV_Y_AXIS = 1,
    MV_Z_AXIS = 2
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
    const unsigned a = LS_ENUM_VAL(axis);
    const ls::math::mat3&& rotationMat = ls::math::mat3 {viewMat};
    const ls::math::vec3&& mvVec = -rotationMat[a];

    return ls::math::transpose(rotationMat) * mvVec;
}



} // end anonymous namespace

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Construction/Destruction
-----------------------------------------------------------------------------*/

/*-------------------------------------
 * Destructor
-------------------------------------*/
Transform::~Transform() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Transform::Transform(const transform_type_t transformType) noexcept :
    parentId{0},
    flags{0},
    type{transformType},
    position{0.f},
    scaling{1.f, 1.f, 1.f},
    orientation{0.f, 0.f, 0.f, 1.f},
    modelMatrix{1.f}
{}

/*-------------------------------------
 * Matrix Constructor
-------------------------------------*/
Transform::Transform(const math::mat4& modelMat, const transform_type_t transformType) noexcept :
    Transform{transformType}
{
    extract_transforms(modelMat);
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
Transform::Transform(const Transform& t) noexcept :
    parentId{t.parentId},
    flags{t.flags},
    type{t.type},
    position{t.position},
    scaling{t.scaling},
    orientation{t.orientation},
    modelMatrix{t.modelMatrix}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
Transform::Transform(Transform&& t) noexcept :
    parentId{t.parentId},
    flags{t.flags},
    type{t.type},
    position{std::move(t.position)},
    scaling{std::move(t.scaling)},
    orientation{std::move(t.orientation)},
    modelMatrix{std::move(t.modelMatrix)}
{
    t.parentId = 0;
    t.flags = 0;
    t.type = transform_type_t::TRANSFORM_TYPE_MODEL;
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
Transform& Transform::operator =(const Transform& t) noexcept {
    parentId = t.parentId;
    flags = t.flags;
    type = t.type;
    position = t.position;
    scaling = t.scaling;
    orientation = t.orientation;
    modelMatrix = t.modelMatrix;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
Transform& Transform::operator =(Transform&& t) noexcept {
    parentId = t.parentId;
    t.parentId = 0;
    
    flags = t.flags;
    t.flags = 0;
    
    type = t.type;
    t.type = transform_type_t::TRANSFORM_TYPE_MODEL;
    
    position = std::move(t.position);
    scaling = std::move(t.scaling);
    orientation = std::move(t.orientation);
    modelMatrix = std::move(t.modelMatrix);

    return *this;
}

/*-----------------------------------------------------------------------------
 * Positioning
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the position
-------------------------------------*/
void Transform::move(const math::vec3& deltaPos, bool relative) noexcept {
    if (type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC || type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y) {
        relative = !relative;
    }
    
    if (relative == false) {
        const math::vec3&& translation = {
            math::dot(math::get_x_axis(orientation), deltaPos),
            math::dot(math::get_y_axis(orientation), deltaPos),
            math::dot(math::get_z_axis(orientation), deltaPos)
        };
        position += translation;
    }
    else {
        position += deltaPos;
    }

    set_dirty();
}

/*-------------------------------------
 * Set the position
-------------------------------------*/
void Transform::set_position(const math::vec3& newPos) noexcept {
    position = newPos;
    set_dirty();
}

/*-------------------------------------
 * Get the absolute position
-------------------------------------*/
math::vec3 Transform::get_abs_position() const noexcept {
    if (type == transform_type_t::TRANSFORM_TYPE_MODEL) {
        math::vec3{modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]};
    }
    
    const math::mat3&& rotationMat = ls::math::mat3{modelMatrix};
    const math::vec3&& mvVec = math::vec3{-modelMatrix[3][0], -modelMatrix[3][1], -modelMatrix[3][2]};
    return ls::math::transpose(rotationMat) * mvVec;
}

/*-----------------------------------------------------------------------------
 * Scaling
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the scaling
-------------------------------------*/
void Transform::scale(const math::vec3& deltaScale) noexcept {
    scaling += deltaScale;
    set_dirty();
}

/*-------------------------------------
 * Set the scaling
-------------------------------------*/
void Transform::set_scale(const math::vec3& newScale) noexcept {
    scaling = newScale;
    set_dirty();
}

/*-----------------------------------------------------------------------------
 * Orientation
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the orientation
-------------------------------------*/
void Transform::rotate(const math::quat& deltaRotation) noexcept {
    orientation = math::normalize(orientation * deltaRotation);
    set_dirty();
}

/*-------------------------------------
 * Adjust the orientation in percentages
-------------------------------------*/
void Transform::rotate(const math::vec3& amount) noexcept {
    const math::quat&& xAxis = math::quat {amount[1], 0.f, 0.f, 1.f};
    const math::quat&& yAxis = math::quat {0.f, amount[0], 0.f, 1.f};
    const math::quat&& zAxis = math::quat {0.f, 0.f, amount[2], 1.f};
    
    if (type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y
    || type == transform_type_t::TRANSFORM_TYPE_VIEW_FPS_LOCKED_Y
    ) {
        const math::quat&& newOrientation = xAxis * orientation * yAxis * zAxis;
        set_orientation(math::normalize(newOrientation));
    }
    else {
        rotate(xAxis * yAxis * zAxis);
    }
    set_dirty();
}

/*-------------------------------------
 * Set the orientation
-------------------------------------*/
void Transform::set_orientation(const math::quat& newRotation) noexcept {
    orientation = newRotation;
    set_dirty();
}

/*-----------------------------------------------------------------------------
 * Final Transformations
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Apply all transformations to the model matrix
-------------------------------------*/
void Transform::apply_transform(bool useSRT) noexcept {
    if (type == transform_type_t::TRANSFORM_TYPE_VIEW_FPS || type == transform_type_t::TRANSFORM_TYPE_VIEW_FPS_LOCKED_Y) {
        useSRT = !useSRT;
    }
    
    modelMatrix = useSRT ? get_srt_matrix() : get_str_matrix();
    set_clean();
}

/*-------------------------------------
 * Pre-transform the matrix
-------------------------------------*/
void Transform::apply_post_transform(const math::mat4& deltaTransform, bool useSRT) noexcept {
    apply_transform(useSRT);
    modelMatrix = modelMatrix * deltaTransform;
}

/*-------------------------------------
 * Post-transform the matrix
-------------------------------------*/
void Transform::apply_pre_transform(const math::mat4& deltaTransform, bool useSRT) noexcept {
    apply_transform(useSRT);
    modelMatrix = deltaTransform * modelMatrix;
}

/*-------------------------------------
 * Extract the transformation parameters (math::mat3)
-------------------------------------*/
void Transform::extract_transforms(math::mat3 rotationMatrix) noexcept {
    // Scaling must be done uniformly, otherwise this will produce strange
    // results.
    scaling[0] = math::length(rotationMatrix[0]);
    scaling[1] = math::length(rotationMatrix[1]);
    scaling[2] = math::length(rotationMatrix[2]);

    if (math::determinant(rotationMatrix) < 0.f) {
        scaling = -scaling;
    }

    if (scaling[0]) {
        rotationMatrix[0] /= scaling[0];
    }
    if (scaling[1]) {
        rotationMatrix[1] /= scaling[1];
    }
    if (scaling[2]) {
        rotationMatrix[2] /= scaling[2];
    }

    orientation = math::mat_to_quat(rotationMatrix);

    set_dirty();
}

/*-------------------------------------
 * Extract the transformation parameters (math::mat4)
-------------------------------------*/
void Transform::extract_transforms(const math::mat4& newTransform) noexcept {
    position[0] = newTransform[3][0];
    position[1] = newTransform[3][1];
    position[2] = newTransform[3][2];

    // delegate for maintenance purposes
    extract_transforms(math::mat3{newTransform});
}

/*-------------------------------------
 * Generate a SRT matrix for use in *this
-------------------------------------*/
math::mat4 Transform::get_srt_matrix() const noexcept {
    return math::mat4{
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        position[0], position[1], position[2], 1.f
    } * math::quat_to_mat4(orientation);
    /*
    return math::mat4 {
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        0.f, 0.f, 0.f, 1.f
    } * math::mat4{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        position[0], position[1], position[2], 1.f
    } * math::quat_to_mat4(orientation);
    */
}

/*-------------------------------------
 * Generate a TRS Matrix
-------------------------------------*/
math::mat4 Transform::get_str_matrix() const noexcept {
    return math::mat4 {
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        0.f, 0.f, 0.f, 1.f
    } * math::quat_to_mat4(orientation) * math::mat4{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        position[0], position[1], position[2], 1.f
    };
}

/*-----------------------------------------------------------------------------
 * Axis Orientations
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Get the forward direction
-------------------------------------*/
math::vec3 Transform::get_forwards_direction() const noexcept {
    return type == transform_type_t::TRANSFORM_TYPE_MODEL
        ? math::get_z_axis(orientation)
        : extractMVVector(modelMatrix, view_axis_t::MV_Z_AXIS);
}

/*-------------------------------------
 * Retrieve the camera's up vector
-------------------------------------*/
math::vec3 Transform::get_up_direction() const noexcept {
    return type == transform_type_t::TRANSFORM_TYPE_MODEL
        ? math::get_y_axis(orientation)
        : extractMVVector(modelMatrix, view_axis_t::MV_Y_AXIS);
}

/*-------------------------------------
 * Retrieve the camera's right vector
 * 
 * TODO: Test this, make sure it's not returning left-handed coordinates
-------------------------------------*/
math::vec3 Transform::get_right_direction() const noexcept {
    return type == transform_type_t::TRANSFORM_TYPE_MODEL
        ? math::get_x_axis(orientation)
        : extractMVVector(modelMatrix, view_axis_t::MV_X_AXIS);
}

/*-------------------------------------
 * Y-Axis Locking
-------------------------------------*/
void Transform::lock_y_axis(const bool isLocked) noexcept {
    LS_DEBUG_ASSERT(type != transform_type_t::TRANSFORM_TYPE_MODEL);
    
    if (isLocked) {
        if (type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC) {
            type = transform_type_t::TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y;
        }
        else if (type == transform_type_t::TRANSFORM_TYPE_VIEW_FPS) {
            type = transform_type_t::TRANSFORM_TYPE_VIEW_FPS_LOCKED_Y;
        }
    }
    else {
        if (type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y) {
            type = transform_type_t::TRANSFORM_TYPE_VIEW_ARC;
        }
        else if (type == transform_type_t::TRANSFORM_TYPE_VIEW_FPS_LOCKED_Y) {
            type = transform_type_t::TRANSFORM_TYPE_VIEW_FPS;
        }
    }
}

/*-------------------------------------
 * Looking at targets
-------------------------------------*/
void Transform::look_at(const math::vec3& eye, const math::vec3& target, const math::vec3& up) {
    LS_DEBUG_ASSERT(type != transform_type_t::TRANSFORM_TYPE_MODEL);

    if (type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC
    || type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y
    ) {
        extract_transforms(math::look_from(eye, target, up));
    }
    else {
        extract_transforms(math::pure_look_at(eye, target, up));
        set_position(-eye);
    }
}



} // end draw namespace
} // end ls namespace
