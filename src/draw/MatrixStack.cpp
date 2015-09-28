/* 
 * File:   matrixStack.cpp
 * Author: Miles Lacey
 * 
 * Created on January 25, 2014, 12:00 PM
 */

#include <utility>

#include "lightsky/draw/matrixStack.h"

namespace ls {
namespace draw {

/*-------------------------------------
 * Constructor
-------------------------------------*/
matrixStack::matrixStack() :
    stacks{
        std::stack<math::mat4>{}, // projections
        std::stack<math::mat4>{}, // view
        std::stack<math::mat4>{} // models
    },
    mvpMatrix{math::mat4(1.f)},
    vpMatrix{math::mat4(1.f)}
{
    stacks[MATRIX_USE_PROJECTION].push(math::mat4(1.f));
    stacks[MATRIX_USE_VIEW].push(math::mat4(1.f));
    stacks[MATRIX_USE_MODEL].push(math::mat4(1.f));
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
matrixStack::matrixStack(matrixStack&& ms) :
    stacks{
        std::move(ms.stacks[MATRIX_USE_PROJECTION]),
        std::move(ms.stacks[MATRIX_USE_VIEW]),
        std::move(ms.stacks[MATRIX_USE_MODEL]),
    },
    mvpMatrix{std::move(ms.mvpMatrix)},
    vpMatrix{std::move(ms.vpMatrix)}
{}

/*-------------------------------------
 * Destructor
-------------------------------------*/
matrixStack::~matrixStack() {
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
matrixStack& matrixStack::operator =(matrixStack&& ms) {
    stacks[MATRIX_USE_PROJECTION] = std::move(ms.stacks[MATRIX_USE_PROJECTION]);
    stacks[MATRIX_USE_VIEW] = std::move(ms.stacks[MATRIX_USE_VIEW]);
    stacks[MATRIX_USE_MODEL] = std::move(ms.stacks[MATRIX_USE_MODEL]);
    
    mvpMatrix = ms.mvpMatrix;
    vpMatrix = ms.vpMatrix;
    
    return *this;
}

/*-------------------------------------
 * Push a matrix onto the stack, multiplying it by the current matrix
-------------------------------------*/
void matrixStack::pushMatrix(matrix_use_t mt, const math::mat4& m) {
    // debug modes need to make sure no invalid matrix types are being used.
    #ifdef LS_DEBUG
        if (mt >= MATRIX_USE_MAX) {
            LS_LOG_ERR("Unknown matrix type pushed onto the stack: ", mt, '\n');
            return;
        }
    #endif
    
    stacks[mt].push(m * stacks[mt].top());
}

/*-------------------------------------
 * Push the identity matrix onto the stack
-------------------------------------*/
void matrixStack::pushIdentity(matrix_use_t mt) {
    // debug modes need to make sure no invalid matrix types are being used.
    #ifdef LS_DEBUG
        if (mt >= MATRIX_USE_MAX) {
            LS_LOG_ERR("Unknown matrix type pushed onto the stack: ", mt, '\n');
            return;
        }
    #endif
    
    stacks[mt].push(math::mat4{1.f});
}

/*-------------------------------------
 * Push a matrix onto the stack without multiplying it by the current matrix.
-------------------------------------*/
void matrixStack::emplaceMatrix(matrix_use_t mt, const math::mat4& m) {
    // debug modes need to make sure no invalid matrix types are being used.
    #ifdef LS_DEBUG
        if (mt >= MATRIX_USE_MAX) {
            LS_LOG_ERR("Unknown matrix type pushed onto the stack: ", mt, '\n');
            return;
        }
    #endif
    
    stacks[mt].push(m);
}

/*-------------------------------------
 * Set the matrix on top of the stack to the one passed into the function.
-------------------------------------*/
void matrixStack::loadMatrix(matrix_use_t mt, const math::mat4& m) {
    stacks[mt].top() = m;
}

/*-------------------------------------
 * Set the current matrix at the top of the stack to the identity matrix.
-------------------------------------*/
void matrixStack::loadIdentity(matrix_use_t mt) {
    stacks[mt].top() = math::mat4(1.f);
}

/*-------------------------------------
 * Pop a matrix from the stack.
-------------------------------------*/
void matrixStack::popMatrix(matrix_use_t mt) {
    // debug modes need to make sure no invalid matrix types are being used.
    #ifdef LS_DEBUG
        if (mt >= MATRIX_USE_MAX) {
            LS_LOG_ERR("Unknown matrix type popped off the stack: ", mt, '\n');
            return;
        }
    #endif
    
    stacks[mt].pop();
    
    if (stacks[mt].size() == 0) {
        stacks[mt].push(math::mat4{1.f});
    }
}

} // end draw namespace
} // end ls namespace
