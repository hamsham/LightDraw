/* 
 * File:   draw/boundingBox.h
 * Author: Miles Lacey
 *
 * Created on June 6, 2014, 6:57 PM
 */

#ifndef __LS_DRAW_BOUNDING_BOX_H__
#define	__LS_DRAW_BOUNDING_BOX_H__

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**
 * @brief Bounding Box Class
 * 
 * The orientation for a bounding box follows OpenGL coordinates, where the
 * positive XYZ coordinates point towards the top, right, front of the viewport.
 */
class boundingBox {
    private:
        math::vec3 topRearRight = {1.f, 1.f, 1.f};
        math::vec3 botFrontLeft = {-1.f, -1.f, -1.f};
        
    public:
        /**
         * @brief Constructor
         */
        boundingBox();
        
        /**
         * @brief Copy Constructor
         * 
         * Copies data from another bounding box.
         * 
         * @param bb
         * A constant reference to a fully constructed bounding box
         * object.
         */
        boundingBox(const boundingBox& bb);
        
        /**
         * @brief Move Constructor
         * 
         * Copies data from another bounding box (no moves are performed).
         * 
         * @param An r-value reference to a fully constructed bounding box
         */
        boundingBox(boundingBox&&);
        
        /**
         * @brief Destructor
         * 
         * Defaulted
         */
        ~boundingBox() = default;
        
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
        boundingBox& operator=(const boundingBox&);
        
        /**
         * @brief Move Operator
         * 
         * @param An R-Value reference to a bounding box that is about to go
         * out of scope.
         * 
         * @return A reference to *this.
         */
        boundingBox& operator=(boundingBox&&);
        
        /**
         * @brief Check if a point is within this box.
         * 
         * @param A constant reference to a vec3 object.
         * 
         * @return TRUE if the point is within *this, or FALSE if otherwise.
         */
        bool isInBox(const math::vec3&) const;
        
        /**
         * Check if a portion of another bounding box is within *this.
         * 
         * @param A constant reference to another bounding box.
         * 
         * @return TRUE if a portion of the bounding box is within *this, or
         * FALSE if it isn't.
         */
        bool isInBox(const boundingBox&) const;
        
        /**
         * Set the top-rear-right point of this bounding box.
         * 
         * @param A constant reference to a point that will be used as the top,
         * rear, right point of this bounding box.
         */
        void setTopRearRight(const math::vec3&);
        
        /**
         * Get the top-rear-right point of this bounding box.
         * 
         * @return A constant reference to the top, rear, right point of this
         * bounding box.
         */
        const math::vec3& getTopRearRight() const;
        
        /**
         * Set the bottom, front, left point of this bounding box.
         * 
         * @param A constant reference to a point that will be used as the
         * bottom, front, left point of this bounding box.
         */
        void setBotFrontLeft(const math::vec3&);
        
        /**
         * Get the bottom, front, left point of this bounding box.
         * 
         * @return A constant reference to the bottom, front, left point of this
         * bounding box.
         */
        const math::vec3& getBotFrontLeft() const;
        
        /**
         * Reset the bounds of this bounding box to their default values.
         */
        void resetSize();
        
        /**
         * Compare a point to the current set of vertices.
         * If any of the components within the parameter are larger than the
         * components of this box, the current set of points will be enlarged.
         * 
         * @param point
         * A point who's individual components should be used to update the
         * size of this bounding box.
         */
        void compareAndUpdate(const math::vec3& point);
};

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/boundingBox_impl.h"

#endif	/* __LS_DRAW_BOUNDING_BOX_H__ */
