
#ifndef __LS_DRAW_TRANSFORM_H__
#define __LS_DRAW_TRANSFORM_H__

#include "lightsky/math/math.h"

namespace ls {
namespace draw {

/**
 * @brief The transform class is used to contain transformations of an object
 * in both rendering and physical simulation.
 *
 * This class is primarily intended to contain the transformations of renderable
 * objects, as well as simplify the management of their model matrices.
 */
class Transform {
    private:
        /**
         * @brief dirtyFlag
         *
         * A boolean object to determine if the internal model matrix in *this
         * needs to be readjusted.
         */
        bool dirtyFlag;
        
        /**
         * @brief position
         *
         * Represents the position of a point in 3D catersian coordinates.
         */
        math::vec3 position;

        /**
         * @brief scaling
         *
         * Represents the size of an object in 3D space.
         */
        math::vec3 scaling;

        /**
         * @brief position
         *
         * Represents the orientation of a point in both 3D and 4D space.
         */
        math::quat orientation;

        /**
         * @brief modelMatrix
         *
         * Contains the position, size, and rotation of an object in 3D space.
         */
        math::mat4 modelMatrix;

    public:
        /**
         * @brief Destructor
         */
        ~Transform();

        /**
         * @brief Constructor
         */
        Transform();

        /**
         * @brief Matrix Constructor
         *
         * @param modelMat
         * A constant reference to a matrix which will be used to pre-transform
         * *this during construction.
         */
        Transform(const math::mat4& modelMat);

        /**
         * @brief Copy Constructor
         *
         * @param t
         * A constant reference to another transform object
         */
        Transform(const Transform& t);

        /**
         * @brief Move Constructor
         *
         * @param t
         * An r-value reference to another transform object
         */
        Transform(Transform&& t);

        /**
         * @brief Copy Operator
         *
         * @param t
         * A constant reference to another transform object
         */
        Transform& operator=(const Transform& t);

        /**
         * @brief Move Operator
         *
         * @param t
         * An r-value reference to another transform object
         */
        Transform& operator=(Transform&& t);

        /**
         * @brief Check if the current transformation object needs to be
         * updated.
         *
         * Dirty transformations must have the "applyTransforms()" method
         * called in order to update the internal model matrix.
         *
         * @return TRUE if *this object had the position, scale, or orientation
         * adjusted, FALSE otherwise.
         */
        bool is_dirty() const;

        /**
         * @brief Make the current transform appear to require an update.
         *
         * Dirty transformations must have the "applyTransforms()" method
         * called in order to update the internal model matrix.
         */
        void set_dirty();

        /**
         * @brief Adjust *this transformation object's internal position.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param deltaPos
         * A 3D vector which will determine how much to move *this
         * transformation in 3D space.
         *
         * @param relative
         * If set to true, all movements will be performed relative to the
         * transformation's current orientation. If false, the movement will be
         * performed according to the global cartesian coordinate system.
         */
        void move(const math::vec3& deltaPos, bool relative = false);

        /**
         * @brief Set *this transformation object's internal position.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param newPos
         * A constant reference to a 3D vector which will adjust the internal
         * position of *this in 3D cartesian space.
         */
        void set_position(const math::vec3& newPos);

        /**
         * @brief Retrieve the current position of *this.
         *
         * @return a reference to a 3D vector, representing the position of a
         * point in 3D cartesian space.
         */
        const math::vec3& get_position() const;

        /**
         * @brief Retrieve the current position of *this by extracting the 
         * first three values (x, y, and z, respectively) of the current model
         * matrix.
         *
         * @return a 3D vector, representing the absolute position of a
         * point in 3D Cartesian space since the last internal update.
         */
        math::vec3 get_abs_position() const;

        /**
         * @brief Adjust *this transformation object's internal size.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param deltaScale
         * A 3D vector which will determine how much to resize *this
         * transformation in 3D space.
         */
        void scale(const math::vec3& deltaScale);

        /**
         * @brief Set *this transformation object's internal scaling.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param newScale
         * A constant reference to a 3D vector which will adjust the internal
         * size of *this in 3D cartesian space.
         */
        void set_scale(const math::vec3& newScale);

        /**
         * @brief Retrieve the current scaling of *this.
         *
         * @return a reference to a 3D vector, representing the scale of a
         * point in 3D cartesian space.
         */
        const math::vec3& get_scale() const;

        /**
         * @brief Adjust *this transformation object's internal orientation.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param deltaRotation
         * A quaternion which will determine how much to move *this
         * transformation in 3D space.
         */
        void rotate(const math::quat& deltaRotation);

        /**
         * @brief Set *this transformation object's internal orientation.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param newRotation
         * A constant reference to a quaternion which will adjust the internal
         * orientation of *this in 3D cartesian space.
         */
        void set_orientation(const math::quat& newRotation);

        /**
         * @brief Retrieve the current orientation of *this.
         *
         * @return a reference to a quaternion, representing the orientation of
         * a point in 3D cartesian space.
         */
        const math::quat& get_orientation() const;

        /**
         * @brief Apply any pending adjustments to the internal model matrix.
         * 
         * @param useSrt
         * Determine if the applied transformation needs to use an SRT
         * transformation, or a STR transformation (choose if the rotation
         * should be applied before translation, or the other way around).
         *
         * This method is implicitly called if the internal model matrix has
         * been modified manually.
         */
        void apply_transform(bool useSRT = true);

        /**
         * @brief Multiply *this by another 4x4 homogenous transformation
         * matrix.
         *
         * The model matrix contained within *this is modified as such:
         * this->modelMatrix = this->modelMatrix * deltaTransform.
         *
         * Calling this method will apply all pending transformations, making
         * firther calls to "isDirty()" to return false.
         *
         * @param deltaTransform
         * A 4x4 transformation matrix which will determine how much to adjust
         * *this transformation in 3D space.
         * 
         * @param useSrt
         * Determine if the applied transformation needs to use an SRT
         * transformation, or a STR transformation (choose if the rotation
         * should be applied before translation, or the other way around).
         */
        void apply_pre_transforms(const math::mat4& deltaTransform, bool useSRT = true);

        /**
         * @brief Multiply *this by another 4x4 homogenous transformation
         * matrix.
         *
         * The model matrix contained within *this is modified as such:
         * this->modelMatrix = deltaTransform * this->modelMatrix.
         *
         * Calling this method will apply all pending transformations, making
         * firther calls to "isDirty()" to return false.
         *
         * @param deltaTransform
         * A 4x4 transformation matrix which will determine how much to adjust
         * *this transformation in 3D space.
         * 
         * @param useSrt
         * Determine if the applied transformation needs to use an SRT
         * transformation, or a STR transformation (choose if the rotation
         * should be applied before translation, or the other way around).
         */
        void apply_post_transforms(const math::mat4& deltaTransform, bool useSRT = true);

        /**
         * @brief Set *this transformation object's internal model matrix.
         *
         * Calling this method will apply all pending transformations, making
         * further calls to "isDirty()" return FALSE.
         *
         * @param newTransform
         * A constant reference to a 4x4 matrix which, representing the
         * position, scale, and orientation of a point in 3D space.
         */
        void extract_transforms(const math::mat4& newTransform);

        /**
         * @brief Retrieve the current model matrix of *this.
         *
         * @return a reference to a 4x4 matrix, representing the position,
         * scale, and orientation of a point in 3D cartesian space.
         */
        const math::mat4& get_transform() const;
        
        /**
         * @brief Generate a 4x4 homogenous matrix which has been uniformly
         * scaled, rotated, and positioned.
         * 
         * This is the default transformation mode.
         * 
         * @return A 4x4 transformation matrix which can be used for 3D
         * positioning, rotation, and scaling.
         */
        math::mat4 get_srt_matrix() const;
        
        /**
         * @brief Generate a 4x4 homogenous matrix which has been uniformly
         * scaled, positioned, then rotated.
         * 
         * @return A 4x4 transformation matrix which can be used for 3D
         * positioning, rotation, and scaling.
         */
        math::mat4 get_str_matrix() const;
};

/*-------------------------------------
 * Check if the model matrix needs updating
-------------------------------------*/
inline bool Transform::is_dirty() const {
    return dirtyFlag;
}

/*-------------------------------------
 * Make the current transform require updating
-------------------------------------*/
inline void Transform::set_dirty() {
    dirtyFlag = true;
}

/*-------------------------------------
 * Get the current position
-------------------------------------*/
inline const math::vec3& Transform::get_position() const {
    return position;
}

/*-------------------------------------
 * Get the absolute position
-------------------------------------*/
inline math::vec3 Transform::get_abs_position() const {
    return math::vec3{modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]};
}

/*-------------------------------------
 * Get the current scale
-------------------------------------*/
inline const math::vec3& Transform::get_scale() const {
    return scaling;
}

/*-------------------------------------
 * Get the current orientation
-------------------------------------*/
inline const math::quat& Transform::get_orientation() const {
    return orientation;
}

/*-------------------------------------
 * Get the current model matrix
-------------------------------------*/
inline const math::mat4& Transform::get_transform() const {
    return modelMatrix;
}

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_TRANSFORM_H__
