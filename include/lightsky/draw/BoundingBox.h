/*
 * File:   draw/boundingBox.h
 * Author: Miles Lacey
 *
 * Created on June 6, 2014, 6:57 PM
 */

#ifndef __LS_DRAW_BOUNDING_BOX_H__
#define __LS_DRAW_BOUNDING_BOX_H__

#include "lightsky/draw/Setup.h"

namespace ls {
namespace draw {



/**
 * @brief Bounding Box Class
 *
 * The orientation for a bounding box follows OpenGL coordinates, where the
 * positive XYZ coordinates point towards the top, right, front of the viewport.
 */
class BoundingBox {
  private:
    math::vec3 topRearRight = {1.f, 1.f, 1.f};
    math::vec3 botFrontLeft = {-1.f, -1.f, -1.f};

  public:
    /**
     * @brief Constructor
     */
    BoundingBox() noexcept;

    /**
     * @brief Copy Constructor
     *
     * Copies data from another bounding box.
     *
     * @param bb
     * A constant reference to a fully constructed bounding box
     * object.
     */
    BoundingBox(const BoundingBox& bb) noexcept;

    /**
     * @brief Move Constructor
     *
     * Copies data from another bounding box (no moves are performed).
     *
     * @param An r-value reference to a fully constructed bounding box
     */
    BoundingBox(BoundingBox&&) noexcept;

    /**
     * @brief Destructor
     *
     * Defaulted
     */
    ~BoundingBox() noexcept = default;

    /**
     * @brief Copy Operator
     *
     * Copies data from another bounding box.
     *
     * @param A constant reference to a fully constructed bounding box
     * object.
     *
     * @return A reference to *this.
     */
    BoundingBox& operator=(const BoundingBox&) noexcept;

    /**
     * @brief Move Operator
     *
     * @param An R-Value reference to a bounding box that is about to go
     * out of scope.
     *
     * @return A reference to *this.
     */
    BoundingBox& operator=(BoundingBox&&) noexcept;

    /**
     * @brief Check if a point is within this box.
     *
     * @param A constant reference to a vec3 object.
     *
     * @return TRUE if the point is within *this, or FALSE if otherwise.
     */
    bool is_in_box(const math::vec3&) const noexcept;

    /**
     * Check if a portion of another bounding box is within *this.
     *
     * @param A constant reference to another bounding box.
     *
     * @return TRUE if a portion of the bounding box is within *this, or
     * FALSE if it isn't.
     */
    bool is_in_box(const BoundingBox&) const noexcept;

    /**
     * Set the top-rear-right point of this bounding box.
     *
     * @param A constant reference to a point that will be used as the top,
     * rear, right point of this bounding box.
     */
    void set_top_rear_right(const math::vec3&) noexcept;

    /**
     * Get the top-rear-right point of this bounding box.
     *
     * @return A constant reference to the top, rear, right point of this
     * bounding box.
     */
    const math::vec3& get_top_rear_right() const noexcept;

    /**
     * Set the bottom, front, left point of this bounding box.
     *
     * @param A constant reference to a point that will be used as the
     * bottom, front, left point of this bounding box.
     */
    void set_bot_front_left(const math::vec3&) noexcept;

    /**
     * Get the bottom, front, left point of this bounding box.
     *
     * @return A constant reference to the bottom, front, left point of this
     * bounding box.
     */
    const math::vec3& get_bot_front_left() const noexcept;

    /**
     * Reset the bounds of this bounding box to their default values.
     */
    void reset_size() noexcept;

    /**
     * Compare a point to the current set of vertices.
     * If any of the components within the parameter are larger than the
     * components of this box, the current set of points will be enlarged.
     *
     * @param point
     * A point who's individual components should be used to update the
     * size of this bounding box.
     */
    void compare_and_update(const math::vec3& point) noexcept;
};

/*-------------------------------------
    Check if a point is within this box.
-------------------------------------*/
inline bool BoundingBox::is_in_box(const BoundingBox& bb) const noexcept {
    return is_in_box(bb.topRearRight) || is_in_box(bb.botFrontLeft);
}

/*-------------------------------------
    Set the top-rear-right point of this bounding box.
-------------------------------------*/
inline void BoundingBox::set_top_rear_right(const math::vec3& v) noexcept {
    topRearRight = v;
}

/*-------------------------------------
    Get the top-rear-right point of this bounding box.
-------------------------------------*/
inline const math::vec3& BoundingBox::get_top_rear_right() const noexcept {
    return topRearRight;
}

/*-------------------------------------
    Set the bottom, front, left point of this bounding box.
-------------------------------------*/
inline void BoundingBox::set_bot_front_left(const math::vec3& v) noexcept {
    botFrontLeft = v;
}

/*-------------------------------------
    Get the bottom, front, left point of this bounding box.
-------------------------------------*/
inline const math::vec3& BoundingBox::get_bot_front_left() const noexcept {
    return botFrontLeft;
}

} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_BOUNDING_BOX_H__ */
