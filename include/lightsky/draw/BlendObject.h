/* 
 * File:   blendObject.h
 * Author: Miles Lacey
 *
 * Created on August 3, 2014, 6:53 PM
 */

#ifndef __LS_DRAW_BLEND_OBJECT_H__
#define	__LS_DRAW_BLEND_OBJECT_H__

#include "lightsky/draw/setup.h"
#include "lightsky/draw/color.h"

namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
    Enumerations
-----------------------------------------------------------------------------*/
/**
 * Blending equations that can be used when rendering.
 */
enum blend_state_t : int {
    BLEND_STATE = GL_BLEND
};

/**
 * Blending colors
 */
enum blend_color_t : int {
    BLEND_COLOR = GL_BLEND_COLOR
};

/**
 * Blending equations
 */
enum blend_equ_t : int {
    BLEND_EQU_ADD       = GL_FUNC_ADD,
    BLEND_EQU_SUB       = GL_FUNC_SUBTRACT,
    BLEND_EQU_REV_SUB   = GL_FUNC_REVERSE_SUBTRACT,
    BLEND_EQU_MIN       = GL_MIN,
    BLEND_EQU_MAX       = GL_MAX,
    BLEND_EQU_DEFAULT   = GL_FUNC_ADD
};

/**
 * Blending Equation Color
 */
enum blend_equ_color_t : int {
    BLEND_EQU_COL_RGB   = GL_BLEND_EQUATION_RGB,
    BLEND_EQU_COL_ALPHA = GL_BLEND_EQUATION_ALPHA
};

/**
 * Blend functions which help determine the type of blending that's applied to
 * a renderable object.
 */
enum blend_func_t : int {
    BLEND_FNC_ZERO                 = GL_ZERO,
    BLEND_FNC_ONE                  = GL_ONE,
    BLEND_FNC_SRC_COLOR            = GL_SRC_COLOR,
    BLEND_FNC_1_SUB_SRC_COLOR      = GL_ONE_MINUS_SRC_COLOR,
    BLEND_FNC_DST_COLOR            = GL_DST_COLOR,
    BLEND_FNC_1_SUB_DST_COLOR      = GL_ONE_MINUS_DST_COLOR,
    BLEND_FNC_SRC_ALPHA            = GL_SRC_ALPHA,
    BLEND_FNC_1_SUB_SRC_ALPHA      = GL_ONE_MINUS_SRC_ALPHA,
    BLEND_FNC_DST_ALPHA            = GL_DST_ALPHA,
    BLEND_FNC_1_SUB_DST_ALPHA      = GL_ONE_MINUS_DST_ALPHA,
    BLEND_FNC_CONST_COLOR          = GL_CONSTANT_COLOR,
    BLEND_FNC_1_SUB_CONST_COLOR    = GL_ONE_MINUS_CONSTANT_COLOR,
    BLEND_FNC_CONST_ALPHA          = GL_CONSTANT_ALPHA,
    BLEND_FNC_1_SUB_CONST_ALPHA    = GL_ONE_MINUS_CONSTANT_ALPHA
};

/**
 * Blending Function Color
 */
enum blend_func_color_t : int {
    BLEND_FNC_COL_SRC_RGB   = GL_BLEND_SRC_RGB,
    BLEND_FNC_COL_DST_RGB   = GL_BLEND_DST_RGB,
    BLEND_FNC_COL_SRC_ALPHA = GL_BLEND_SRC_ALPHA,
    BLEND_FNC_COL_DST_ALPHA = GL_BLEND_DST_ALPHA
};

/**----------------------------------------------------------------------------
 * The blend object manages the current blend state within OpenGL.
-----------------------------------------------------------------------------*/
class blendObject {
    private:
        /**
         * Determine if blending should be enabled or disabled. If blending is
         * disabled, no other blending parameters will be sent through the
         * rendering pipeline.
         */
        bool enabled = GL_FALSE;
        
        /**
         * The color to be used when performing blending operations in OpenGL.
         */
        color::color blendCol = color::blank;
        
        /**
         * Class Member that's used to set the RGB blending equation in OpenGL.
         */
        blend_equ_t rgbBlendEq = BLEND_EQU_ADD;
        
        /**
         * Class Member that's used to set the alpha blending equation in OpenGL
         */
        blend_equ_t alphaBlendEq = BLEND_EQU_ADD;
        
        /**
         * Class Member that's used to set the source RGB blending function in
         * OpenGL.
         */
        blend_func_t srcRgbBlendFunc = BLEND_FNC_ONE;
        
        /**
         * Class Member that's used to set the destination RGB blending function
         * in OpenGL.
         */
        blend_func_t dstRgbBlendFunc = BLEND_FNC_ZERO;
        
        /**
         * Class Member that's used to set the source alpha blending function in
         * OpenGL.
         */
        blend_func_t srcAlphaBlendFunc = BLEND_FNC_ONE;
        
        /**
         * Class Member that's used to set the destination alpha blending
         * function in OpenGL.
         */
        blend_func_t dstAlphaBlendFunc = BLEND_FNC_ZERO;

    public:
        /**
         * @brief Constructor
         * 
         * Creates an blendObject using the default OpenGL blending
         * parameters.
         */
        blendObject() = default;
        
        /**
         * @brief Copy Constructor
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param A constant reference to another blendObject.
         */
        blendObject(const blendObject&) = default;
        
        /**
         * @brief Move Constructor
         * 
         * Moves all data from the input parameter into *this.
         * 
         * @param A constant reference to another blendObject.
         */
        blendObject(blendObject&&) = default;
        
        /**
         * @brief Destructor
         * 
         * Does nothing.
         */
        ~blendObject() = default;
        
        /**
         * @brief Copy Operator
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param An R-Value reference to another blendObject that is about
         * to go out of scope.
         */
        blendObject& operator=(const blendObject&) = default;
        
        /**
         * @brief Move Operator
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param An R-Value reference to another blendObject that is about
         * to go out of scope.
         */
        blendObject& operator=(blendObject&&) = default;
        
        /**
         * Enable blending in OpenGL and apply all stored parameters (blending
         * equations, functions, and color).
         */
        void bind() const;
        
        /**
         * @brief Disable Blending in OpenGL.
         * 
         * @note Don't use this function if another depth object is going to
         * be bound immediately afterwards. Doing so may cause a redundant
         * state change within OpenGL.
         */
        void unbind() const;
        
        /**
         * Reset all currently held blend parameters to OpenGL's defaults.
         */
        void reset();
        
        /**
         * Set whether blending should be enabled or disabled in the current
         * rendering state. If blending is disabled, no other blending
         * parameters will be sent through the rendering pipeline.
         * 
         * @param bool
         * A boolean value that will determine if OpenGL should have blending
         * enabled or disabled.
         */
        void setState(bool = GL_FALSE);
        
        /**
         * Determine whether this blending object should tell OpenGL to enable
         * or disable the use of blending.
         * 
         * @return bool
         * A boolean value that will determine if OpenGL should have blending
         * enabled or disabled.
         */
        bool getState() const;
        
        /**
         * Set the blending equation to be used within OpenGL.
         * 
         * @param rgbaMode
         * The blend equation that OpenGL should use internally.
         */
        void setBlendEquation(blend_equ_t rgbaMode = BLEND_EQU_ADD);
        
        /**
         * Set the OpenGL blending equation, but with different values for RGB
         * and Alpha.
         * 
         * @param rgbMode
         * The RGB blending equation
         * 
         * @param alphaMode
         * The blending equation for the alpha channel.
         */
        void setBlendEquation(
            blend_equ_t rgbMode = BLEND_EQU_ADD,
            blend_equ_t alphaMode = BLEND_EQU_ADD
        );
        
        /**
         * Get the blending equation for the RGB channels.
         * 
         * @return blend_eq_t
         */
        blend_equ_t getBlendEquationRgb() const;
        
        /**
         * Get the blending equation for the alpha channel.
         * 
         * @return blend_eq_t
         */
        blend_equ_t getBlendEquationAlpha() const;
        
        /**
         * Set the source and destination blending function.
         * 
         * @param srcFactor
         * The blending function for the source RGBA channels.
         * 
         * @param dstFactor
         * The blending equation for the destination RGBA channels.
         */
        void setBlendFunction(
            blend_func_t srcFactor = BLEND_FNC_ONE,
            blend_func_t dstFactor = BLEND_FNC_ZERO
        );
        
        /**
         * Set the source and destination blending function, with separate RGB
         * and alpha functions.
         * 
         * @param srcFactorRgb
         * The blending function for the source RGB channels.
         * 
         * @param dstFactorRgb
         * The blending equation for the destination RGB channels.
         * 
         * @param srcFactorAlpha
         * The blending function for the source alpha channel.
         * 
         * @param dstFactorAlpha
         * The blending function for the destination alpha channel.
         */
        void setBlendFunction(
            blend_func_t srcFactorRgb = BLEND_FNC_ONE,
            blend_func_t dstFactorRgb = BLEND_FNC_ZERO,
            blend_func_t srcFactorAlpha = BLEND_FNC_ONE,
            blend_func_t dstFactorAlpha = BLEND_FNC_ZERO
        );
        
        /**
         * Get the current blend function from the source RGB channel
         * 
         * @return blend_equ_color_t
         */
        blend_func_t getBlendFunctionSrcRgb() const;
        
        /**
         * Get the current blend function from the source alpha channel
         * 
         * @return blend_equ_color_t
         */
        blend_func_t getBlendFunctionSrcAlpha() const;
        
        /**
         * Get the current blend function from the destination RGB channel
         * 
         * @return blend_equ_color_t
         */
        blend_func_t getBlendFunctionDstRgb() const;
        
        /**
         * Get the current blend function from the destination alpha channel
         * 
         * @return blend_equ_color_t
         */
        blend_func_t getBlendFunctionDstAlpha() const;
        
        /**
         * Set the color that OpenGL should use for blending
         * 
         * @param rgba
         * A constant reference to an color that should be used during
         * blending operations within OpenGL.
         */
        void setBlendColor(const color::color& rgba = color::blank);
        
        /**
         * Get the current color that OpenGL is using for blending operations.
         * 
         * @return a group of four floating-point numbers, representing the
         * RGBA colors that OpenGL is using for blending.
         */
        color::color getBlendColor() const;
};

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/blendObject_impl.h"

#endif	/* __LS_DRAW_BLEND_OBJECT_H__ */

