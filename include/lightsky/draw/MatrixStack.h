/* 
 * File:   matrixStack.h
 * Author: Miles Lacey
 *
 * Created on January 25, 2014, 12:00 PM
 */

#ifndef __LS_DRAW_MATRIX_STACK_H__
#define	__LS_DRAW_MATRIX_STACK_H__

#include <stack>

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * Enumeration to facilitate matrix manipulations within the matrix stack.
-----------------------------------------------------------------------------*/
enum matrix_use_t : unsigned {
    MATRIX_USE_INVALID      = (unsigned)-1,
    
    MATRIX_USE_PROJECTION   = 0,
    MATRIX_USE_VIEW         = 1,
    MATRIX_USE_MODEL        = 2,
    
    MATRIX_USE_MAX          = 3
};

/**----------------------------------------------------------------------------
 * @brief Matrix Stack
 * 
 * This object can be used to make life MUCH easier when dealing with matrices
 * within OpenGL. Usage was designed to be fairly close to the original OpenGL
 * 1.0 spec.
-----------------------------------------------------------------------------*/
class matrixStack {
    private:
        std::stack<math::mat4> stacks[3]; // model, view, & projection
        math::mat4 mvpMatrix = {1.f};
        math::mat4 vpMatrix = {1.f};
        
    public:
        /**
         * @brief Constructor
         */
        matrixStack();
        
        /**
         * @brief Copy Constructor -- DELETED
         */
        matrixStack(const matrixStack&) = delete;
        
        /**
         * @brief Move Constructor
         * Moves all data from the input parameter into *this without performing
         * any copies.
         * 
         * @param ms
         * A reference to a temporary matrix stack object.
         */
        matrixStack(matrixStack&& ms);
        
        /**
         * @brief Destructor
         * Frees all memory/resources used by *this.
         */
        ~matrixStack();
        
        /**
         * @brief Copy Operator -- DELETED
         */
        matrixStack& operator= (const matrixStack&) = delete;
        
        /**
         * @brief Move Operator
         * Moves all data from the input parameter into *this without performing
         * any copies.
         * 
         * @param ms
         * A reference to a temporary matrix stack object.
         * 
         * @return A reference to *this
         */
        matrixStack& operator= (matrixStack&& ms);
        
        /**
         * Push a matrix onto the stack. The current matrix will be copied into
         * a new matrix and pushed onto the stack. This matrix this then
         * multiplied by the one passed into the function.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         * 
         * @param m
         * A constant reference to a 4x4 matrix.
         */
        void pushMatrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * Push an identity matrix onto the stack. The current matrix will be
         * stored and an identity matrix will be pushed onto the stack.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        void pushIdentity(matrix_use_t matrixType);
        
        /**
         * Push a matrix onto the stack. This function pushes a pre-constructed
         * matrix onto the stack. Unlike "pushMatrix(...)," this method does
         * not multiply the current matrix with the one being pushed.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         * 
         * @param m
         * A constant reference to a 4x4 matrix.
         */
        void emplaceMatrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * Remove a matrix from a specified stack.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        void popMatrix(matrix_use_t matrixType);
        
        /**
         * Set the matrix on top of the stack to the one passed into the
         * function.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         * 
         * @param 
         * The matrix to be used on top of the stack
         */
        void loadMatrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * Set the selected matrix to an identity matrix
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        void loadIdentity(matrix_use_t);
        
        /**
         * Multiply the selected matrix by the one passed into the function.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         * 
         * @param m
         * A constant reference to the matrix to multiply against the on on the
         * stack.
         */
        void multMatrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * @brief Get the current model matrix
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        const math::mat4& getMatrix(matrix_use_t matrixType) const;
        
        /**
         * Multiply the model, view, and projection stacks together
         */
        void constructMvp();
        
        /**
         * Multiply only the view and projection stacks together
         */
        void constructVp();
        
        /**
         * @brief Get the MVP Matrix
         */
        const math::mat4& getMvpMatrix() const;
        
        /**
         * @brief Get the VP Matrix
         */
        const math::mat4& getVpMatrix() const;
        
        /**
         * @brief Clear a specific Stack
         * 
         * @param matrixType
         * The stack type who's size will be queried.
         */
        unsigned size(matrix_use_t matrixType);
};

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/matrixStack_impl.h"

#endif	/* __LS_DRAW_MATRIX_STACK_H__ */
