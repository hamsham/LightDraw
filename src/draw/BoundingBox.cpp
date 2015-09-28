/* 
 * File:   draw/boundingBox.cpp
 * Author: Miles Lacey
 * 
 * Created on June 6, 2014, 6:57 PM
 */

#include <utility>

#include "lightsky/draw/boundingBox.h"

namespace ls {
namespace draw {

/*-------------------------------------
    Constructor
-------------------------------------*/
boundingBox::boundingBox() {
}

/*
    Copy Constructor
-------------------------------------*/
boundingBox::boundingBox(const boundingBox& bb) :
    topRearRight{bb.topRearRight},
    botFrontLeft{bb.botFrontLeft}
{}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
boundingBox::boundingBox(boundingBox&& bb) :
    topRearRight{std::move(bb.topRearRight)},
    botFrontLeft{std::move(bb.botFrontLeft)}
{
    bb.resetSize();
}

/*-------------------------------------
    Copy Operator
-------------------------------------*/
boundingBox& boundingBox::operator=(const boundingBox& bb) {
    topRearRight = bb.topRearRight;
    botFrontLeft = bb.botFrontLeft;
    
    return *this;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
boundingBox& boundingBox::operator =(boundingBox&& bb) {
    topRearRight = std::move(bb.topRearRight);
    botFrontLeft = std::move(bb.botFrontLeft);
    
    bb.resetSize();
    
    return *this;
}

/*-------------------------------------
    Check if a portion of another bounding box is within *this.
-------------------------------------*/
bool boundingBox::isInBox(const math::vec3& v) const {
    return
    v[0] <= topRearRight[0] && v[1] <= topRearRight[1] && v[2] <= topRearRight[2]
    &&
    v[0] >= botFrontLeft[0] && v[1] >= botFrontLeft[1] && v[2] >= botFrontLeft[2];
}

/*-------------------------------------
    Reset the bounds of this bounding box to their default values.
-------------------------------------*/
void boundingBox::resetSize() {
    setTopRearRight(math::vec3{1.f, 1.f, 1.f});
    setBotFrontLeft(math::vec3{-1.f, -1.f, -1.f});
}

/*-------------------------------------
    Compare a point to the current set of vertices.
-------------------------------------*/
void boundingBox::compareAndUpdate(const math::vec3& point) {
    math::vec3& trr = topRearRight;
    if (point[0] > trr[0]) {trr[0] = point[0];}
    if (point[1] > trr[1]) {trr[1] = point[1];}
    if (point[2] > trr[2]) {trr[2] = point[2];}
    
    math::vec3& bfl = botFrontLeft;
    if (point[0] < bfl[0]) {bfl[0] = point[0];}
    if (point[1] < bfl[1]) {bfl[1] = point[1];}
    if (point[2] < bfl[2]) {bfl[2] = point[2];}
}

} // end draw namespace
} // end ls namespace
