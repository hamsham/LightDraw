
#ifndef __LS_DRAW_TRANSFORM_H__
#define __LS_DRAW_TRANSFORM_H__

#include "lightsky/math/Math.h"

namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * Transformation meta-data
-----------------------------------------------------------------------------*/
enum transform_flags_t : uint32_t {
    TRANSFORM_FLAG_DIRTY = 0x00000001,
};



/**----------------------------------------------------------------------------
 * Transformation flags for scene data
-----------------------------------------------------------------------------*/
enum transform_type_t : uint32_t {
    TRANSFORM_TYPE_MODEL,
    
    TRANSFORM_TYPE_VIEW_FPS, // should be default for all view types
    TRANSFORM_TYPE_VIEW_ARC,
    
    TRANSFORM_TYPE_VIEW_FPS_LOCKED_Y,
    TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y,
};




/**----------------------------------------------------------------------------
 * @brief The transform class is used to contain transformations of an object
 * in both rendering and physical simulation.
 *
 * This class is primarily intended to contain the transformations of renderable
 * objects, as well as simplify the management of their model matrices.
----------------------------------------------------------------------------*/
class Transform {
//class alignas(128) Transform {
    private:
        /**
         * Index ID of a parent transformation in a Scene Graph.
         */
        uint32_t parentId;
        
        /**
         * @brief Meta-information container.
         */
        uint32_t flags;
        
        /**
         * @brief Transformation type.
         */
        transform_type_t type;

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
        
        /**
         * Convenience method to mark the internal state as clean (no
         * transforms need to be updated).
         */
        void set_clean() noexcept;

    public:
        /**
         * @brief Destructor
         */
        ~Transform() noexcept;

        /**
         * @brief Constructor
         * 
         * @param transformType
         * Allows the transformation to be initialized to a specific type.
         */
        Transform(const transform_type_t transformType = transform_type_t::TRANSFORM_TYPE_MODEL) noexcept;

        /**
         * @brief Matrix Constructor
         *
         * @param modelMat
         * A constant reference to a matrix which will be used to pre-transform
         * *this during construction.
         * 
         * @param transformType
         * Allows the transformation to be initialized to a specific type.
         */
        Transform(const math::mat4& modelMat, const transform_type_t transformType = transform_type_t::TRANSFORM_TYPE_MODEL) noexcept;

        /**
         * @brief Copy Constructor
         *
         * @param t
         * A constant reference to another transform object
         */
        Transform(const Transform& t) noexcept;

        /**
         * @brief Move Constructor
         *
         * @param t
         * An r-value reference to another transform object
         */
        Transform(Transform&& t) noexcept;

        /**
         * @brief Copy Operator
         *
         * @param t
         * A constant reference to another transform object
         */
        Transform& operator=(const Transform& t) noexcept;

        /**
         * @brief Move Operator
         *
         * @param t
         * An r-value reference to another transform object
         */
        Transform& operator=(Transform&& t) noexcept;
        
        /**
         * Set the array-index of the parent transformation which *this
         * Transform object will belong to in a Scene Graph.
         * 
         * Use of this function will not require any internal members be
         * updated (i.e. it will stay clean or dirty depending on what the
         * internal state currently is).
         * 
         * @param inIndex
         * An integer ID, containing the index of a parent transformation
         * within a SceneGraph object.
         */
        void set_parent_id(const uint32_t inIndex) noexcept;
        
        /**
         * Retrieve the index of the parent transformation which *this object
         * is modified by.
         * 
         * @return An integer ID, containing the index of a parent
         * transformation within a SceneGraph object.
         */
        uint32_t get_parent_id() const noexcept;

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
        bool is_dirty() const noexcept;

        /**
         * @brief Make the current transform appear to require an update.
         *
         * Dirty transformations must have the "applyTransforms()" method
         * called in order to update the internal model matrix.
         */
        void set_dirty() noexcept;
        
        /**
         * Determine what type of transformation *this is.
         * 
         * @return A value from the transform_type_t enumeration which can be
         * used to determine what type of transformation *this object
         * represents.
         */
        transform_type_t get_type() const noexcept;
        
        /**
         * Set the type of transformation *this object represents.
         * 
         * Setting the transformation type affects how the internal positioning
         * and orientations operate.
         * 
         * Use of this function will not require any internal members be
         * updated (i.e. it will stay clean or dirty depending on what the
         * internal state currently is).
         * 
         * @param inType
         * A value from the transform_type_t enumeration which can be used to
         * determine what type of transformation *this object represents.
         */
        void set_type(const transform_type_t inType) noexcept;

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
         * performed according to the global Cartesian coordinate system.
         */
        void move(const math::vec3& deltaPos, bool relative = false) noexcept;

        /**
         * @brief Set *this transformation object's internal position.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param newPos
         * A constant reference to a 3D vector which will adjust the internal
         * position of *this in 3D Cartesian space.
         */
        void set_position(const math::vec3& newPos) noexcept;

        /**
         * @brief Retrieve the current position of *this.
         *
         * @return a reference to a 3D vector, representing the position of a
         * point in 3D Cartesian space.
         */
        const math::vec3& get_position() const noexcept;

        /**
         * @brief Retrieve the current position of *this by extracting the
         * first three values (x, y, and z, respectively) of the current model
         * matrix.
         *
         * @return a 3D vector, representing the absolute position of a
         * point in 3D Cartesian space since the last internal update.
         */
        math::vec3 get_abs_position() const noexcept;

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
        void scale(const math::vec3& deltaScale) noexcept;

        /**
         * @brief Set *this transformation object's internal scaling.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param newScale
         * A constant reference to a 3D vector which will adjust the internal
         * size of *this in 3D Cartesian space.
         */
        void set_scale(const math::vec3& newScale) noexcept;

        /**
         * @brief Retrieve the current scaling of *this.
         *
         * @return a reference to a 3D vector, representing the scale of a
         * point in 3D Cartesian space.
         */
        const math::vec3& get_scale() const noexcept;

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
        void rotate(const math::quat& deltaRotation) noexcept;
        
        /**
         * Rotate a transformation by a certain amount in the X, Y, and Z
         * directions.
         *
         * @param amount
         * An amount, as a set of percentages between -1.0 - 1.0, that the
         * transformation should rotate by. These angles correspond to Pitch,
         * Yaw, and Roll, respectively.
         */
        void rotate(const math::vec3& amount) noexcept;

        /**
         * @brief Set *this transformation object's internal orientation.
         *
         * Calling this method will cause the "isDirty()" function to return
         * TRUE until "applyTransforms()" is called.
         *
         * @param newRotation
         * A constant reference to a quaternion which will adjust the internal
         * orientation of *this in 3D Cartesian space.
         */
        void set_orientation(const math::quat& newRotation) noexcept;

        /**
         * @brief Retrieve the current orientation of *this.
         *
         * @return a reference to a quaternion, representing the orientation of
         * a point in 3D Cartesian space.
         */
        const math::quat& get_orientation() const noexcept;

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
        void apply_transform(bool useSRT = true) noexcept;

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
        void apply_post_transform(const math::mat4& deltaTransform, bool useSRT = true) noexcept;

        /**
         * @brief Multiply *this by another 4x4 homogenous transformation
         * matrix.
         *
         * The model matrix contained within *this is modified as such:
         * this->modelMatrix = deltaTransform * this->modelMatrix.
         *
         * Calling this method will apply all pending transformations, making
         * further calls to "isDirty()" to return false.
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
        void apply_pre_transform(const math::mat4& deltaTransform, bool useSRT = true) noexcept;

        /**
         * @brief Set *this transformation object's internal model matrix.
         *
         * This override takes a math::mat3 object as a parameter. Since a
         * math::mat3 cannot contain a 3D position due to a lack of elements,
         * it will leave the internal position unchanged.
         *
         * Calling this method will discard all pending transformations and
         * further calls to "isDirty()" return TRUE.
         *
         * @param rotationMatrix
         * A constant reference to a 3x3 matrix which, representing the scale
         * and orientation of a point in 3D space.
         */
        void extract_transforms(math::mat3 rotationMatrix) noexcept;

        /**
         * @brief Set *this transformation object's internal model matrix.
         *
         * Calling this method will discard all pending transformations and
         * further calls to "isDirty()" return TRUE.
         *
         * @param newTransform
         * A constant reference to a 4x4 matrix which, representing the
         * position, scale, and orientation of a point in 3D space.
         */
        void extract_transforms(const math::mat4& newTransform) noexcept;

        /**
         * @brief Retrieve the current model matrix of *this.
         *
         * @return a reference to a 4x4 matrix, representing the position,
         * scale, and orientation of a point in 3D Cartesian space.
         */
        const math::mat4& get_transform() const noexcept;

        /**
         * @brief Generate a 4x4 homogenous matrix which has been uniformly
         * scaled, rotated, and positioned.
         *
         * This is the default transformation mode.
         *
         * @return A 4x4 transformation matrix which can be used for 3D
         * positioning, rotation, and scaling.
         */
        math::mat4 get_srt_matrix() const noexcept;

        /**
         * @brief Generate a 4x4 homogenous matrix which has been uniformly
         * scaled, positioned, then rotated.
         *
         * @return A 4x4 transformation matrix which can be used for 3D
         * positioning, rotation, and scaling.
         */
        math::mat4 get_str_matrix() const noexcept;

        /**
         * @brief Retrieve the current direction that the internal model
         * matrix is facing.
         *
         * @return A 3D unit vector containing the model matrix Z-axis.
         */
        math::vec3 get_forwards_direction() const noexcept;

        /**
         * @brief Retrieve the current upwards direction of the internal model
         * matrix.
         *
         * @return A 3D unit vector containing the model matrix Y-axis.
         */
        math::vec3 get_up_direction() const noexcept;

        /**
         * @brief Retrieve the current rightwards direction of the internal
         * model matrix is facing.
         *
         * @return A 3D unit vector containing the model matrix X-axis.
         */
        math::vec3 get_right_direction() const noexcept;

        /**
         * @brief Set whether or not the Y axis of the camera should be locked.
         *
         * @param shouldLock
         * A boolean value, determining if the camera should be permitted to
         * roll.
         */
        void lock_y_axis(const bool shouldLock) noexcept;

        /**
         * @brief Determine if the Y axis of a view transform is currently locked.
         *
         * @return A boolean value, determining if the camera is currently locked.
         */
        bool is_y_axis_locked() const noexcept;

        /**
         * @brief Set an orientation for view transformations.
         * 
         * This method has no effect for model transformations.
         *
         * @param eye
         * The desired position of the view matrix.
         *
         * @param target
         * The point at which the view/projection frustum should face.
         *
         * @param up
         * The direction which determines which way is upwards.
         */
        void look_at(const math::vec3& eye, const math::vec3& target, const math::vec3& up = {0.f, 1.f, 0.f});

        /**
         * @brief Set an orientation for view transformations.
         * 
         * This method has no effect for model transformations.
         *
         * @param point
         * A point that the camera should look at.
         */
        void look_at(const math::vec3& target);
};

/*-------------------------------------
 * Set the array-index of the parent transformation.
-------------------------------------*/
inline void Transform::set_parent_id(const uint32_t inIndex) noexcept {
    parentId = inIndex;
}

/*-------------------------------------
 * Retrieve the index of the parent transformation.
-------------------------------------*/
inline uint32_t Transform::get_parent_id() const noexcept {
    return parentId;
}

/*-------------------------------------
-------------------------------------*/
inline void Transform::set_clean() noexcept {
    flags &= ~transform_flags_t::TRANSFORM_FLAG_DIRTY;
}

/*-------------------------------------
 * Check if the model matrix needs updating
-------------------------------------*/
inline bool Transform::is_dirty() const noexcept {
    return (flags & transform_flags_t::TRANSFORM_FLAG_DIRTY) != 0;
}

/*-------------------------------------
 * Make the current transform require updating
-------------------------------------*/
inline void Transform::set_dirty() noexcept {
    flags |= transform_flags_t::TRANSFORM_FLAG_DIRTY;
}

/*-------------------------------------
 * Get the type of transformation *this object represents.
-------------------------------------*/
inline transform_type_t Transform::get_type() const noexcept {
    return type;
}

/*-------------------------------------
 * Set the type of transformation *this object represents.
-------------------------------------*/
inline void Transform::set_type(const transform_type_t inType) noexcept {
    type = inType;
}

/*-------------------------------------
 * Get the current position
-------------------------------------*/
inline const math::vec3& Transform::get_position() const noexcept {
    return position;
}

/*-------------------------------------
 * Get the current scale
-------------------------------------*/
inline const math::vec3& Transform::get_scale() const noexcept {
    return scaling;
}

/*-------------------------------------
 * Get the current orientation
-------------------------------------*/
inline const math::quat& Transform::get_orientation() const noexcept {
    return orientation;
}

/*-------------------------------------
 * Get the current model matrix
-------------------------------------*/
inline const math::mat4& Transform::get_transform() const noexcept {
    return modelMatrix;
}

/*-------------------------------------
 * Y-Axis Lock Check
-------------------------------------*/
inline bool Transform::is_y_axis_locked() const noexcept {
    return type == transform_type_t::TRANSFORM_TYPE_VIEW_ARC_LOCKED_Y
    || type == transform_type_t::TRANSFORM_TYPE_VIEW_FPS_LOCKED_Y;
}

/*-------------------------------------
 * Look At function from the current cam position
-------------------------------------*/
inline void Transform::look_at(const math::vec3& target) {
    look_at(position, target, math::vec3{0.f, 1.f, 0.f});
}

} // end draw namespace
} // end ls namespace

#endif // __LS_DRAW_TRANSFORM_H__
