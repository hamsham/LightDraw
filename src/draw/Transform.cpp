
#include <utility> // std::move

#include "lightsky/draw/transform.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Construction/Destruction
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Destructor
-------------------------------------*/
transform::~transform() {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
transform::transform() :
    dirtyFlag{false},
    position{0.f},
    scaling{1.f},
    orientation{0.f, 0.f, 0.f, 1.f},
    modelMatrix{1.f}
{}

/*-------------------------------------
 * Matrix Constructor
-------------------------------------*/
transform::transform(const math::mat4& modelMat) {
    extractTransforms(modelMat);
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
transform::transform(const transform& t) :
    dirtyFlag{t.dirtyFlag},
    position{t.position},
    scaling{t.scaling},
    orientation{t.orientation},
    modelMatrix{t.modelMatrix}
{}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
transform::transform(transform&& t) :
    dirtyFlag{t.dirtyFlag},
    position{std::move(t.position)},
    scaling{std::move(t.scaling)},
    orientation{std::move(t.orientation)},
    modelMatrix{std::move(t.modelMatrix)}
{}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
transform& transform::operator=(const transform& t) {
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
transform& transform::operator=(transform&& t) {
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
void transform::move(const math::vec3& deltaPos, bool relative) {
    if (relative == false) {
        const math::vec3&& translation = {
            math::dot(math::getAxisX(orientation), deltaPos),
            math::dot(math::getAxisY(orientation), deltaPos),
            math::dot(math::getAxisZ(orientation), deltaPos)
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
void transform::setPosition(const math::vec3& newPos) {
    position = newPos;
    dirtyFlag = true;
}

/*-----------------------------------------------------------------------------
 * Scaling
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the scaling
-------------------------------------*/
void transform::scale(const math::vec3& deltaScale) {
    scaling += deltaScale;
    dirtyFlag = true;
}

/*-------------------------------------
 * Set the scaling
-------------------------------------*/
void transform::setScale(const math::vec3& newScale) {
    scaling = newScale;
    dirtyFlag = true;
}

/*-----------------------------------------------------------------------------
 * Orientation
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Adjust the orientation
-------------------------------------*/
void transform::rotate(const math::quat& deltaRotation) {
    orientation = math::normalize(orientation * deltaRotation);
    dirtyFlag = true;
}

/*-------------------------------------
 * Set the orientation
-------------------------------------*/
void transform::setOrientation(const math::quat& newRotation) {
    orientation = newRotation;
    dirtyFlag = true;
}

/*-----------------------------------------------------------------------------
 * Final Transformations
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Apply all transformations to the model matrix
-------------------------------------*/
void transform::applyTransforms(bool useSRT) {
    modelMatrix = useSRT ? getSRTMatrix() : getSTRMatrix();
    dirtyFlag = false;
}

/*-------------------------------------
 * Pre-transform the matrix
-------------------------------------*/
void transform::applyPreTransforms(const math::mat4& deltaTransform, bool useSRT) {
    applyTransforms(useSRT);
    modelMatrix = modelMatrix * deltaTransform;
}

/*-------------------------------------
 * Post-transform the matrix
-------------------------------------*/
void transform::applyPostTransforms(const math::mat4& deltaTransform, bool useSRT) {
    applyTransforms(useSRT);
    modelMatrix = deltaTransform * modelMatrix;
}

/*-------------------------------------
 * Extract the transformation parameters
-------------------------------------*/
void transform::extractTransforms(const math::mat4& newTransform) {
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
    
    orientation = math::matToQuat(rotationMatrix);

    dirtyFlag = true;
}

/*-------------------------------------
 * Generate a SRT matrix for use in *this
-------------------------------------*/
math::mat4 transform::getSRTMatrix() const {
    return
        math::mat4{
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        position[0],position[1],position[2], 1.f
    }
    *
    math::quatToMat4(orientation);
}

/*-------------------------------------
 * Generate a TRS Matrix
-------------------------------------*/
math::mat4 transform::getSTRMatrix() const {
    return
    math::mat4{
        scaling[0], 0.f, 0.f, 0.f,
        0.f, scaling[1], 0.f, 0.f,
        0.f, 0.f, scaling[2], 0.f,
        0.f, 0.f, 0.f, 1.f
    }
    *
    math::quatToMat4(orientation)
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
