/*
 * File:   depthObject.cpp
 * Author: Miles Lacey
 *
 * Created on August 4, 2014, 8:29 PM
 */

#include "lightsky/draw/DepthObject.h"



namespace ls
{
namespace draw
{

/*-------------------------------------
    Depth object binding
-------------------------------------*/
void DepthObject::bind() const
{
    if (enabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    glClearDepthf(depthClearVal);
    glDepthFunc(depthFunc);
    glDepthMask(depthMask);
    glDepthRangef(depthNear, depthFar);
}

/*-------------------------------------
    Disable OpenGL depth parameters
-------------------------------------*/
void DepthObject::unbind() const
{
    // the default constructor already has default OpenGL params.
    const DepthObject depthObj{};
    depthObj.bind();
}

/*-------------------------------------
    Reset all members to their default values.
-------------------------------------*/
void DepthObject::terminate()
{
    *this = DepthObject{};
}
} // end draw namespace
} // end ls namespace
