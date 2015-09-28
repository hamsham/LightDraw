/* 
 * File:   depthObject.h
 * Author: Miles Lacey
 *
 * Created on August 4, 2014, 8:29 PM
 */

#ifndef __LS_DRAW_DEPTH_OBJECT_H__
#define	__LS_DRAW_DEPTH_OBJECT_H__

#include "lightsky/draw/setup.h"
#include "lightsky/draw/color.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
    @brief Depth Comparison Operations
    
    These can be sent to OpenGL in order to determine how to compare the depth
    of rasterized fragments.
-----------------------------------------------------------------------------*/
enum compare_func_t : int {
    DEPTH_FNC_ALWAYS     = GL_ALWAYS,
    DEPTH_FNC_NEVER      = GL_NEVER,
    DEPTH_FNC_LESS       = GL_LESS,
    DEPTH_FNC_EQUAL      = GL_EQUAL,
    DEPTH_FNC_LEQUAL     = GL_LEQUAL,
    DEPTH_FNC_GEQUAL     = GL_GEQUAL,
    DEPTH_FNC_NEQUAL     = GL_NOTEQUAL
};

/**----------------------------------------------------------------------------
 * @brief Render Depth Manipulation Object
 * 
 * The depth object id an artificial wrapper for depth buffer manipulation
 * within OpenGL. through this object, you can change the depth buffer's
 * state and parameters.
-----------------------------------------------------------------------------*/
class depthObject {
    private:
        /**
         * Determine if depth testing should be enabled or disabled.
         */
        bool enabled = GL_FALSE;
        
        /**
         * clearVal is used to set the value of the depth buffer when it has
         * been cleared using "glClear(GL_DEPTH_BUFFER_BIT)." This value is
         * always clamped to a value between 0 and 1, according to the OpenGL
         * documentation.
         */
        float depthClearVal = 1.f;
        
        /**
         * Used to determine the if currently rendered fragment should be kept
         * by comparing it to the one rendered before. This member is applied
         * to the function "glDepthFunc()"
         */
        compare_func_t depthFunc = DEPTH_FNC_LESS;
        
        /**
         * A GLboolean flag that, used with glDepthMask(), determines if a
         * fragment should write to the depth buffer.
         */
        bool depthMask = GL_TRUE;
        
        /**
         * The near value that should be used by OpenGL's depth buffer. This
         * value will be clamped between 0.f and 1.f. This helps determine if
         * a fragment is within the acceptable range of values that can be
         * rendered to the currently bound framebuffer.
         */
        float depthNear = 0.f;
        
        /**
         * The furthest normalized value that should be used by OpenGL's depth
         * buffer. This value will be clamped between 0.f and 1.f. This helps
         * determine if a fragment is within the acceptable range of values
         * that can be rendered to the currently bound framebuffer.
         */
        float depthFar = 1.f;
        
    public:
        /**
         * @brief Constructor
         * 
         * Creates an depthObject using the default OpenGL depth parameters.
         */
        depthObject() = default;
        
        /**
         * @brief Copy Constructor
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param d
         * A constant reference to another depthObject.
         */
        depthObject(const depthObject& d) = default;
        
        /**
         * @brief Move Constructor
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param d
         * A constant reference to another depthObject.
         */
        depthObject(depthObject&& d) = default;
        
        /**
         * @brief Destructor
         * 
         * Does nothing.
         */
        ~depthObject() = default;
        
        /**
         * @brief Copy Operator
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param d
         * An R-Value reference to another depthObject that is about
         * to go out of scope.
         */
        depthObject& operator=(const depthObject& d) = default;
        
        /**
         * @brief Move Operator
         * 
         * Copies all data from the input parameter into *this.
         * 
         * @param d
         * An R-Value reference to another depthObject that is about
         * to go out of scope.
         */
        depthObject& operator=(depthObject&& d) = default;
        
        /**
         * Bind all members to their associated depth parameters.
         */
        void bind() const;
        
        /**
         * Unbind all depth parameters used by *this and restore the default
         * depth parameters to OpenGL.
         * 
         * @note Don't use this function if another depth object is going to
         * be bound immediately afterwards. Doing so may cause a redundant
         * state change within OpenGL.
         */
        void unbind() const;
        
        /**
         * Reset all currently held depth parameters to OpenGL's defaults.
         */
        void reset();
        
        /**
         * Set whether depth testing should be enabled or disabled in the
         * current rendering state.
         * 
         * @param s
         * A boolean value that will determine if OpenGL should have depth
         * testing enabled or disabled.
         */
        void setState(bool s = GL_FALSE);
        
        /**
         * Determine whether depth testing should be enabled or disabled in the
         * current rendering state.
         * 
         * @return bool
         * A boolean value that will determine if OpenGL should have depth
         * testing enabled or disabled.
         */
        bool getState() const;
        
        /**
         * Set the value to be applied to the depth buffer after a call to
         * glClear(LS_DEPTH_BUFFER_BIT).
         * 
         * @param cv
         * A single-precision float that will be clamped to the range of 0-1.
         */
        void setClearVal(float cv);
        
        /**
         * Get the value that's applied to the depth buffer when a call to
         * glClear(LS_DEPTH_BUFFER_BIT) has been made.
         * 
         * @return A single-precision float, clamped to the range of 0-1.
         */
        float getClearVal() const;
        
        /**
         * Set the function to be used when rendering fragments from the pixel
         * shader. This value helps to determine if fragments should be kept or
         * discarded.
         * 
         * @param cf
         * The comparison function to be used when rendering pixel fragments.
         */
        void setDepthFunc(compare_func_t cf = DEPTH_FNC_LESS);
        
        /**
         * Get the depth-comparison function used by this depth object.
         * 
         * @return compare_func_t
         * The comparison function to be used when rendering pixel fragments.
         */
        compare_func_t getDepthFunc() const;
        
        /**
         * Set whether to enable writes to the depth buffer.
         * 
         * @param dm
         * If TRUE then writes to the depth buffer are enabled, if FALSE then
         * they will be disabled.
         */
        void setDepthMask(bool dm = GL_TRUE);
        
        /**
         * Determine if writes to the depth buffer are enabled.
         * 
         * @return bool
         * TRUE if depth buffer writes are enabled, or FALSE if not.
         */
        bool getDepthMask() const;
        
        /**
         * Set the near value that will be used by OpenGL's depth buffer.
         * This value will be clamped between 0.f and 1.f.
         * 
         * @param n
         * Used to set the nearest renderable depth of a fragment output by the
         * pixel shader.
         */
        void setDepthNear(float n = 0.f);
        
        /**
         * Get the near value that will be used by OpenGL's depth buffer.
         * This value will be clamped between 0.f and 1.f.
         * 
         * @return A single-precision floating point value that is used to
         * determine if a value is within the screen-space depth buffer.
         */
        float getDepthNear() const;
        
        /**
         * Set the far value that will be used by OpenGL's depth buffer.
         * This value will be clamped between 0.f and 1.f.
         * 
         * @param A single-precision floating point value that will be used to
         * determine if a value is within the screen-space depth buffer.
         */
        void setDepthFar(float f = 1.f);
        
        /**
         * Get the far value that will be used by OpenGL's depth buffer.
         * This value will be clamped between 0.f and 1.f.
         * 
         * @param far
         * Used to set the farthest renderable depth of a fragment output by
         * the pixel shader.
         */
        float getDepthFar();
        
        /**
         * Set both the the near and far values that will be used by OpenGL's
         * depth buffer to determine if a fragment is of renderable depth.
         * These value will be clamped between 0.f and 1.f.
         * 
         * @param near
         * Used to set the nearest renderable depth of a fragment output by the
         * pixel shader.
         * 
         * @param far
         * Used to set the farthest renderable depth of a fragment output by
         * the pixel shader.
         */
        void setDepthRange(float n = 0.f, float f = 1.f);
};

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/depthObject_impl.h"

#endif	/* __LS_DRAW_DEPTH_OBJECT_H__ */
