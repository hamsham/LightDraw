/*
 * File:   glsl_common.h
 * Author: Miles Lacey
 *
 * Created on September 4, 2015, 11:04 PM
 */

#ifndef __LS_DRAW_GLSL_COMMON_H__
#define __LS_DRAW_GLSL_COMMON_H__

namespace ls
{
namespace draw
{



/**----------------------------------------------------------------------------
 * These enumerations can be used to describe vertex layouts to VAO objects.
-----------------------------------------------------------------------------*/
enum class layout_loc_t : int
{
    LAYOUT_LOC_POS = 0,
    LAYOUT_LOC_TEX = 1,
    LAYOUT_LOC_NORM = 2,

    LAYOUT_LOC_MAT_ROW = 3, /* Matrices can be used for instancing */
    LAYOUT_LOC_MAT_ROW0 = 3, /* Row 1, mat4_t<>[0][] */
    LAYOUT_LOC_MAT_ROW1 = 4, /* Row 2, mat4_t<>[1][] */
    LAYOUT_LOC_MAT_ROW2 = 5, /* Row 3, mat4_t<>[2][] */
    LAYOUT_LOC_MAT_ROW3 = 6, /* Row 4, mat4_t<>[3][] */

    LAYOUT_LOC_BONE_ID = 7,
    LAYOUT_LOC_BONE_WEIGHT = 8,
};

/*-------------------------------------
 * Shader Versions
-------------------------------------*/
#if defined(LS_OS_IOS) || defined(LS_OS_IOS_SIM)
constexpr char const GLSL_DEFAULT_VERSION[] = "#version 300 es\n";
#else

constexpr char const GLSL_DEFAULT_VERSION[] = "#version 330\n";

#endif

/*-------------------------------------
 * Shader Precision
-------------------------------------*/
constexpr char const GLSL_PRECISION_LOWP_F[] = "precision lowp float;\n";

constexpr char const GLSL_PRECISION_MEDP_F[] = "precision mediump float;\n";

constexpr char const GLSL_PRECISION_HIGHP_F[] = "precision highp float;\n";

#if defined(LS_OS_IOS) || defined(LS_OS_IOS_SIM)
constexpr char const GLSL_PRECISION_DEFAULT[] = "precision mediump float;\n";
#else

constexpr char const GLSL_PRECISION_DEFAULT[] = "precision mediump float\n";

#endif

/*-------------------------------------
 * Shader Functions
-------------------------------------*/
constexpr char const GLSL_CALC_DIFFUSE_FACTOR[] = u8R"***(
float getDiffuseIntensity(in vec3 vertNorm, in vec3 lightDir) {
float brightness = dot(vertNorm, lightDir);
//return 0.5 + (0.5 * brightness);
return brightness;
}
)***";
} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_GLSL_COMMON_H__ */
