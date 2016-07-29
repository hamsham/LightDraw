/*
 * File:   draw/boundingBox.cpp
 * Author: Miles Lacey
 *
 * Created on June 6, 2014, 6:57 PM
 */

#include <utility>

#include "ls/draw/BoundingBox.h"

namespace ls {
namespace draw {

/*-------------------------------------
    Constructor
-------------------------------------*/
BoundingBox::BoundingBox() noexcept {
}

/*
    Copy Constructor
-------------------------------------*/
BoundingBox::BoundingBox(const BoundingBox& bb) noexcept :
    topRearRight {bb.topRearRight},
botFrontLeft {bb.botFrontLeft}
{
}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
BoundingBox::BoundingBox(BoundingBox&& bb) noexcept :
    topRearRight {std::move(bb.topRearRight)},
botFrontLeft {std::move(bb.botFrontLeft)}
{
    bb.reset_size();
}

/*-------------------------------------
    Copy Operator
-------------------------------------*/
BoundingBox& BoundingBox::operator =(const BoundingBox& bb) noexcept {
    topRearRight = bb.topRearRight;
    botFrontLeft = bb.botFrontLeft;

    return *this;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
BoundingBox& BoundingBox::operator =(BoundingBox&& bb) noexcept {
    topRearRight = std::move(bb.topRearRight);
    botFrontLeft = std::move(bb.botFrontLeft);

    bb.reset_size();

    return *this;
}

/*-------------------------------------
    Check if a portion of another bounding box is within *this.
-------------------------------------*/
bool BoundingBox::is_in_box(const math::vec3& v) const noexcept {
    return
    v[0] <= topRearRight[0] && v[1] <= topRearRight[1] && v[2] <= topRearRight[2]
        &&
        v[0] >= botFrontLeft[0] && v[1] >= botFrontLeft[1] && v[2] >= botFrontLeft[2];
}

/*-------------------------------------
    Reset the bounds of this bounding box to their default values.
-------------------------------------*/
void BoundingBox::reset_size() noexcept {
    set_top_rear_right(math::vec3 {1.f, 1.f, 1.f});
    set_bot_front_left(math::vec3 {-1.f, -1.f, -1.f});
}

/*-------------------------------------
    Compare a point to the current set of vertices.
-------------------------------------*/
void BoundingBox::compare_and_update(const math::vec3& point) noexcept {
    math::vec3& trr = topRearRight;
    if (point[0] > trr[0]) {
        trr[0] = point[0];
    }
    if (point[1] > trr[1]) {
        trr[1] = point[1];
    }
    if (point[2] > trr[2]) {
        trr[2] = point[2];
    }

    math::vec3& bfl = botFrontLeft;
    if (point[0] < bfl[0]) {
        bfl[0] = point[0];
    }
    if (point[1] < bfl[1]) {
        bfl[1] = point[1];
    }
    if (point[2] < bfl[2]) {
        bfl[2] = point[2];
    }
}

} // end draw namespace
} // end ls namespace
