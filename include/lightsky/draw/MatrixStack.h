/* 
 * File:   matrixStack.h
 * Author: Miles Lacey
 *
 * Created on January 25, 2014, 12:00 PM
 */

#ifndef __LS_DRAW_MATRIX_STACK_H__
#define	__LS_DRAW_MATRIX_STACK_H__

#include <stack>

#include "lightsky/draw/Setup.h"

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
class MatrixStack {
    private:
        std::stack<math::mat4> stacks[3]; // model, view, & projection
        math::mat4 mvpMatrix = {1.f};
        math::mat4 vpMatrix = {1.f};
        
    public:
        /**
         * @brief Constructor
         */
        MatrixStack();
        
        /**
         * @brief Copy Constructor -- DELETED
         */
        MatrixStack(const MatrixStack&) = delete;
        
        /**
         * @brief Move Constructor
         * Moves all data from the input parameter into *this without performing
         * any copies.
         * 
         * @param ms
         * A reference to a temporary matrix stack object.
         */
        MatrixStack(MatrixStack&& ms);
        
        /**
         * @brief Destructor
         * Frees all memory/resources used by *this.
         */
        ~MatrixStack();
        
        /**
         * @brief Copy Operator -- DELETED
         */
        MatrixStack& operator= (const MatrixStack&) = delete;
        
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
        MatrixStack& operator= (MatrixStack&& ms);
        
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
        void push_matrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * Push an identity matrix onto the stack. The current matrix will be
         * stored and an identity matrix will be pushed onto the stack.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        void push_identity(matrix_use_t matrixType);
        
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
        void emplace_matrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * Remove a matrix from a specified stack.
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        void pop_matrix(matrix_use_t matrixType);
        
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
        void load_matrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * Set the selected matrix to an identity matrix
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        void load_identity(matrix_use_t);
        
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
        void mult_matrix(matrix_use_t matrixType, const math::mat4& m);
        
        /**
         * @brief Get the current model matrix
         * 
         * @param matrixType
         * The type of matrix to be used on the stack.
         */
        const math::mat4& get_matrix(matrix_use_t matrixType) const;
        
        /**
         * Multiply the model, view, and projection stacks together
         */
        void construct_mvp();
        
        /**
         * Multiply only the view and projection stacks together
         */
        void construct_vp();
        
        /**
         * @brief Get the MVP Matrix
         */
        const math::mat4& get_mvp_matrix() const;
        
        /**
         * @brief Get the VP Matrix
         */
        const math::mat4& get_vp_matrix() const;
        
        /**
         * @brief Clear a specific Stack
         * 
         * @param matrixType
         * The stack type who's size will be queried.
         */
        unsigned size(matrix_use_t matrixType);
};

/*-------------------------------------
 * Multiply the selected matrix with the one passed into the function.
-------------------------------------*/
inline void MatrixStack::mult_matrix(matrix_use_t mt, const math::mat4& m) {
    stacks[mt].top() = m * stacks[mt].top();
}

/*-------------------------------------
 * Multiply the matrix stack
-------------------------------------*/
inline void MatrixStack::construct_mvp() {
    this->construct_vp();
    mvpMatrix = vpMatrix * stacks[MATRIX_USE_MODEL].top();
}

/*-------------------------------------
 * Multiply the view and projection stacks
-------------------------------------*/
inline void MatrixStack::construct_vp() {
    vpMatrix = stacks[MATRIX_USE_PROJECTION].top() * stacks[MATRIX_USE_VIEW].top();
}

/*-------------------------------------
 * Get the current model matrix
-------------------------------------*/
inline const math::mat4& MatrixStack::get_matrix(matrix_use_t mt) const {
    return stacks[mt].top();
}

/*-------------------------------------
 * Get the MVP Matrix
-------------------------------------*/
inline const math::mat4& MatrixStack::get_mvp_matrix() const {
    return mvpMatrix;
}

/*-------------------------------------
 * Get the VP Matrix
-------------------------------------*/
inline const math::mat4& MatrixStack::get_vp_matrix() const {
    return vpMatrix;
}

/*-------------------------------------
 * Clear a specific Stack
-------------------------------------*/
inline unsigned MatrixStack::size(matrix_use_t mt) {
    return stacks[mt].size();
}

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_MATRIX_STACK_H__ */
