/*
 * File:   blendObject.cpp
 * Author: Miles Lacey
 *
 * Created on August 3, 2014, 6:53 PM
 */

#include "lightsky/draw/BlendObject.h"

namespace ls {
namespace draw {

/*-------------------------------------
    Enable blending and apply blend parameters to OpenGL.
-------------------------------------*/
void BlendObject::bind() const {
    if (!enabled) {
        glDisable(BLEND_STATE);
        return;
    }

    glEnable(BLEND_STATE);

    glBlendColor(blendCol[0], blendCol[1], blendCol[2], blendCol[3]);

    glBlendEquationSeparate(rgbBlendEq, alphaBlendEq);

    glBlendFuncSeparate(srcRgbBlendFunc, dstRgbBlendFunc, srcAlphaBlendFunc, dstAlphaBlendFunc);
}

/*
 * Disable OpenGL blending
 */
void BlendObject::unbind() const {
    // the default constructor already has default OpenGL params.
    BlendObject blender {};
    blender.bind();
}

/*
 * Reset all members to their default values.
 */
void BlendObject::terminate() {
    *this = BlendObject {};
}

} // end draw namespace
} // end ls namespace
