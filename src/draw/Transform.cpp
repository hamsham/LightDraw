
#include <utility> // std::move

#include "lightsky/draw/Transform.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Construction/Destruction
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
Transform::~Transform() {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
Transform::Transform() :
    dirtyFlag{false},
    position{0.f},
    scaling{1.f},
    orientation{0.f, 0.f, 0.f, 1.f},
    modelMatrix{1.f}
{}

/*-------------------------------------
 * Matrix Constructor
-------------------------------------*/
Transform::Transform(const math::mat4& modelMat) {
    extract_transforms(modelMat);
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
Transform::Transform(const Transform& t) :
    dirtyFlag{t.dirtyFlag},
    position{t.position},
    scaling{t.scaling},
    orientation{t.orientation},
    modelMatrix{t.modelMatrix}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
Transform::Transform(Transform&& t) :
    dirtyFlag{t.dirtyFlag},
    position{std::move(t.position)},
    scaling{std::move(t.scaling)},
    orientation{std::move(t.orientation)},
    modelMatrix{std::move(t.modelMatrix)}
{}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
Transform& Transform::operator=(const Transform& t) {
    dirtyFlag = t.dirtyFlag;
    position = t.position;
    scaling = t.scaling;
    orientation = t.orientation;
    modelMatrix = t.modelMatrix;

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
Transform& Transform::operator=(Transform&& t) {
    dirtyFlag = t.dirtyFlag;
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
void Transform::move(const math::vec3& deltaPos, bool relative) {
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

    dirtyFlag = true;
}

/*-------------------------------------
 * Set the position
-------------------------------------*/
void Transform::set_position(const math::vec3& newPos) {
    position = newPos;
    dirtyFlag = true;
}

/*-----------------------------------------------------------------------------
 * Scaling
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the scaling
-------------------------------------*/
void Transform::scale(const math::vec3& deltaScale) {
    scaling += deltaScale;
    dirtyFlag = true;
}

/*-------------------------------------
 * Set the scaling
-------------------------------------*/
void Transform::set_scale(const math::vec3& newScale) {
    scaling = newScale;
    dirtyFlag = true;
}

/*-----------------------------------------------------------------------------
 * Orientation
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the orientation
-------------------------------------*/
void Transform::rotate(const math::quat& deltaRotation) {
    orientation = math::normalize(orientation * deltaRotation);
    dirtyFlag = true;
}

/*-------------------------------------
 * Set the orientation
-------------------------------------*/
void Transform::set_orientation(const math::quat& newRotation) {
    orientation = newRotation;
    dirtyFlag = true;
}

/*-----------------------------------------------------------------------------
 * Final Transformations
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Apply all transformations to the model matrix
-------------------------------------*/
void Transform::apply_transform(bool useSRT) {
    modelMatrix = useSRT ? get_srt_matrix() : get_str_matrix();
    dirtyFlag = false;
}

/*-------------------------------------
 * Pre-transform the matrix
-------------------------------------*/
void Transform::apply_pre_transforms(const math::mat4& deltaTransform, bool useSRT) {
    apply_transform(useSRT);
    modelMatrix = modelMatrix * deltaTransform;
}

/*-------------------------------------
 * Post-transform the matrix
-------------------------------------*/
void Transform::apply_post_transforms(const math::mat4& deltaTransform, bool useSRT) {
    apply_transform(useSRT);
    modelMatrix = deltaTransform * modelMatrix;
}

/*-------------------------------------
 * Extract the transformation parameters
-------------------------------------*/
void Transform::extract_transforms(const math::mat4& newTransform) {
    position[0] = newTransform[3][0];
    position[1] = newTransform[3][1];
    position[2] = newTransform[3][2];
    
    math::mat3&& rotationMatrix = {newTransform};
    
    // Scaling must be done uniformly, otherwise this will produce strange
    // results.
    scaling[0] = math::length(rotationMatrix[0]);
    scaling[1] = math::length(rotationMatrix[1]);
    scaling[2] = math::length(rotationMatrix[2]);
    
    if (math::determinant(rotationMatrix) < 0.f) {
        scaling = -scaling;
    }
    
    if (scaling[0]) {rotationMatrix[0] /= scaling[0];}
    if (scaling[1]) {rotationMatrix[1] /= scaling[1];}
    if (scaling[2]) {rotationMatrix[2] /= scaling[2];}
    
    orientation = math::mat_to_quat(rotationMatrix);

    dirtyFlag = true;
}

/*-------------------------------------
 * Generate a SRT matrix for use in *this
-------------------------------------*/
math::mat4 Transform::get_srt_matrix() const {
    return
        math::mat4{
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        position[0],position[1],position[2], 1.f
    }
    *
    math::quat_to_mat4(orientation);
}

/*-------------------------------------
 * Generate a TRS Matrix
-------------------------------------*/
math::mat4 Transform::get_str_matrix() const {
    return
    math::mat4{
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        0.f, 0.f, 0.f, 1.f
    }
    *
    math::quat_to_mat4(orientation)
    *
    math::mat4{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        position[0], position[1], position[2], 1.f
    };
}



} // end draw namespace
} // end ls namespace
