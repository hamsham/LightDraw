
/*
 * gcc --std=c99 -pedantic -pedantic-errors -Wall -Werror -Wextra -O2 -I./ -g -c lsgl.c -o lsgl_d.o
 *
 * ar rcs liblsgl_d.a lsgl_d.o
 */

#include <stdint.h>
#include <stdio.h>

#include "lightsky/draw/lsgl.h"

#ifdef _WIN32
    #include <GL/wglext.h>
#elif defined (__unix__)
    #include <GL/glx.h>
#else
    #error "An unsupported OS is currently being used."
#endif

/*-----------------------------------------------------------------------------
 * Windows OpenGL Function Loading
-----------------------------------------------------------------------------*/
#ifdef _WIN32
/*-------------------------------------
 * OpenGL Library Initialization
-------------------------------------*/
HMODULE get_gl_library()
{
    return LoadLibraryA("opengl32.dll");
}

/*-------------------------------------
 * OpenGL Function Retrieval
-------------------------------------*/
uintptr_t get_gl_function(const char* const name)
{
    uintptr_t p = (uintptr_t)wglGetProcAddress(name);

    if (p == 0x0
    || (p == 0x1)
    || (p == 0x2)
    || (p == 0x3)
    || (p == (uintptr_t)-1)
    ) {
        const HMODULE ogllib = get_gl_library();
        p = (uintptr_t)GetProcAddress(ogllib, name);
    }

    return p;
}

/*-----------------------------------------------------------------------------
 * Unix-Based OpenGL Function Loading
-----------------------------------------------------------------------------*/
#elif defined (__unix__)
/*-------------------------------------
 * OpenGL Function Retrieval
-------------------------------------*/
uintptr_t get_gl_function(const char* const name)
{
    uintptr_t p = (uintptr_t)glXGetProcAddress(name);

    if (p == 0x0
    || (p == 0x1)
    || (p == 0x2)
    || (p == 0x3)
    || (p == (uintptr_t)-1)
    ) {
        p = (uintptr_t)glXGetProcAddressARB(name);
    }

    return p;
}

/*-----------------------------------------------------------------------------
 * Unknown OS
-----------------------------------------------------------------------------*/
#else
    #error "An unsupported OS is currently being used."
#endif

/*-------------------------------------
 * LightSky OpenGL Function Initialization
-------------------------------------*/
int lsgl_init()
{
    int ret = 0; /* Contains the number of functions initialized */

    glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)get_gl_function("glDrawRangeElements");
    if (glDrawRangeElements) {++ret;}
    glTexImage3D = (PFNGLTEXIMAGE3DPROC)get_gl_function("glTexImage3D");
    if (glTexImage3D) {++ret;}
    glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)get_gl_function("glTexSubImage3D");
    if (glTexSubImage3D) {++ret;}
    glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)get_gl_function("glCopyTexSubImage3D");
    if (glCopyTexSubImage3D) {++ret;}
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)get_gl_function("glActiveTexture");
    if (glActiveTexture) {++ret;}
    glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)get_gl_function("glSampleCoverage");
    if (glSampleCoverage) {++ret;}
    glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)get_gl_function("glCompressedTexImage3D");
    if (glCompressedTexImage3D) {++ret;}
    glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)get_gl_function("glCompressedTexImage2D");
    if (glCompressedTexImage2D) {++ret;}
    glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)get_gl_function("glCompressedTexImage1D");
    if (glCompressedTexImage1D) {++ret;}
    glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)get_gl_function("glCompressedTexSubImage3D");
    if (glCompressedTexSubImage3D) {++ret;}
    glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)get_gl_function("glCompressedTexSubImage2D");
    if (glCompressedTexSubImage2D) {++ret;}
    glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)get_gl_function("glCompressedTexSubImage1D");
    if (glCompressedTexSubImage1D) {++ret;}
    glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)get_gl_function("glGetCompressedTexImage");
    if (glGetCompressedTexImage) {++ret;}
    glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)get_gl_function("glClientActiveTexture");
    if (glClientActiveTexture) {++ret;}
    glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC)get_gl_function("glMultiTexCoord1d");
    if (glMultiTexCoord1d) {++ret;}
    glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)get_gl_function("glMultiTexCoord1dv");
    if (glMultiTexCoord1dv) {++ret;}
    glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC)get_gl_function("glMultiTexCoord1f");
    if (glMultiTexCoord1f) {++ret;}
    glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)get_gl_function("glMultiTexCoord1fv");
    if (glMultiTexCoord1fv) {++ret;}
    glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC)get_gl_function("glMultiTexCoord1i");
    if (glMultiTexCoord1i) {++ret;}
    glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)get_gl_function("glMultiTexCoord1iv");
    if (glMultiTexCoord1iv) {++ret;}
    glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC)get_gl_function("glMultiTexCoord1s");
    if (glMultiTexCoord1s) {++ret;}
    glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)get_gl_function("glMultiTexCoord1sv");
    if (glMultiTexCoord1sv) {++ret;}
    glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)get_gl_function("glMultiTexCoord2d");
    if (glMultiTexCoord2d) {++ret;}
    glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)get_gl_function("glMultiTexCoord2dv");
    if (glMultiTexCoord2dv) {++ret;}
    glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)get_gl_function("glMultiTexCoord2f");
    if (glMultiTexCoord2f) {++ret;}
    glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)get_gl_function("glMultiTexCoord2fv");
    if (glMultiTexCoord2fv) {++ret;}
    glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC)get_gl_function("glMultiTexCoord2i");
    if (glMultiTexCoord2i) {++ret;}
    glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)get_gl_function("glMultiTexCoord2iv");
    if (glMultiTexCoord2iv) {++ret;}
    glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC)get_gl_function("glMultiTexCoord2s");
    if (glMultiTexCoord2s) {++ret;}
    glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)get_gl_function("glMultiTexCoord2sv");
    if (glMultiTexCoord2sv) {++ret;}
    glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC)get_gl_function("glMultiTexCoord3d");
    if (glMultiTexCoord3d) {++ret;}
    glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)get_gl_function("glMultiTexCoord3dv");
    if (glMultiTexCoord3dv) {++ret;}
    glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC)get_gl_function("glMultiTexCoord3f");
    if (glMultiTexCoord3f) {++ret;}
    glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)get_gl_function("glMultiTexCoord3fv");
    if (glMultiTexCoord3fv) {++ret;}
    glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC)get_gl_function("glMultiTexCoord3i");
    if (glMultiTexCoord3i) {++ret;}
    glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)get_gl_function("glMultiTexCoord3iv");
    if (glMultiTexCoord3iv) {++ret;}
    glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC)get_gl_function("glMultiTexCoord3s");
    if (glMultiTexCoord3s) {++ret;}
    glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)get_gl_function("glMultiTexCoord3sv");
    if (glMultiTexCoord3sv) {++ret;}
    glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC)get_gl_function("glMultiTexCoord4d");
    if (glMultiTexCoord4d) {++ret;}
    glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)get_gl_function("glMultiTexCoord4dv");
    if (glMultiTexCoord4dv) {++ret;}
    glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC)get_gl_function("glMultiTexCoord4f");
    if (glMultiTexCoord4f) {++ret;}
    glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)get_gl_function("glMultiTexCoord4fv");
    if (glMultiTexCoord4fv) {++ret;}
    glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC)get_gl_function("glMultiTexCoord4i");
    if (glMultiTexCoord4i) {++ret;}
    glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)get_gl_function("glMultiTexCoord4iv");
    if (glMultiTexCoord4iv) {++ret;}
    glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC)get_gl_function("glMultiTexCoord4s");
    if (glMultiTexCoord4s) {++ret;}
    glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)get_gl_function("glMultiTexCoord4sv");
    if (glMultiTexCoord4sv) {++ret;}
    glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC)get_gl_function("glLoadTransposeMatrixf");
    if (glLoadTransposeMatrixf) {++ret;}
    glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC)get_gl_function("glLoadTransposeMatrixd");
    if (glLoadTransposeMatrixd) {++ret;}
    glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC)get_gl_function("glMultTransposeMatrixf");
    if (glMultTransposeMatrixf) {++ret;}
    glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC)get_gl_function("glMultTransposeMatrixd");
    if (glMultTransposeMatrixd) {++ret;}
    glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)get_gl_function("glBlendFuncSeparate");
    if (glBlendFuncSeparate) {++ret;}
    glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)get_gl_function("glMultiDrawArrays");
    if (glMultiDrawArrays) {++ret;}
    glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)get_gl_function("glMultiDrawElements");
    if (glMultiDrawElements) {++ret;}
    glPointParameterf = (PFNGLPOINTPARAMETERFPROC)get_gl_function("glPointParameterf");
    if (glPointParameterf) {++ret;}
    glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)get_gl_function("glPointParameterfv");
    if (glPointParameterfv) {++ret;}
    glPointParameteri = (PFNGLPOINTPARAMETERIPROC)get_gl_function("glPointParameteri");
    if (glPointParameteri) {++ret;}
    glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)get_gl_function("glPointParameteriv");
    if (glPointParameteriv) {++ret;}
    glFogCoordf = (PFNGLFOGCOORDFPROC)get_gl_function("glFogCoordf");
    if (glFogCoordf) {++ret;}
    glFogCoordfv = (PFNGLFOGCOORDFVPROC)get_gl_function("glFogCoordfv");
    if (glFogCoordfv) {++ret;}
    glFogCoordd = (PFNGLFOGCOORDDPROC)get_gl_function("glFogCoordd");
    if (glFogCoordd) {++ret;}
    glFogCoorddv = (PFNGLFOGCOORDDVPROC)get_gl_function("glFogCoorddv");
    if (glFogCoorddv) {++ret;}
    glFogCoordPointer = (PFNGLFOGCOORDPOINTERPROC)get_gl_function("glFogCoordPointer");
    if (glFogCoordPointer) {++ret;}
    glSecondaryColor3b = (PFNGLSECONDARYCOLOR3BPROC)get_gl_function("glSecondaryColor3b");
    if (glSecondaryColor3b) {++ret;}
    glSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BVPROC)get_gl_function("glSecondaryColor3bv");
    if (glSecondaryColor3bv) {++ret;}
    glSecondaryColor3d = (PFNGLSECONDARYCOLOR3DPROC)get_gl_function("glSecondaryColor3d");
    if (glSecondaryColor3d) {++ret;}
    glSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DVPROC)get_gl_function("glSecondaryColor3dv");
    if (glSecondaryColor3dv) {++ret;}
    glSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC)get_gl_function("glSecondaryColor3f");
    if (glSecondaryColor3f) {++ret;}
    glSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FVPROC)get_gl_function("glSecondaryColor3fv");
    if (glSecondaryColor3fv) {++ret;}
    glSecondaryColor3i = (PFNGLSECONDARYCOLOR3IPROC)get_gl_function("glSecondaryColor3i");
    if (glSecondaryColor3i) {++ret;}
    glSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IVPROC)get_gl_function("glSecondaryColor3iv");
    if (glSecondaryColor3iv) {++ret;}
    glSecondaryColor3s = (PFNGLSECONDARYCOLOR3SPROC)get_gl_function("glSecondaryColor3s");
    if (glSecondaryColor3s) {++ret;}
    glSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SVPROC)get_gl_function("glSecondaryColor3sv");
    if (glSecondaryColor3sv) {++ret;}
    glSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UBPROC)get_gl_function("glSecondaryColor3ub");
    if (glSecondaryColor3ub) {++ret;}
    glSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBVPROC)get_gl_function("glSecondaryColor3ubv");
    if (glSecondaryColor3ubv) {++ret;}
    glSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UIPROC)get_gl_function("glSecondaryColor3ui");
    if (glSecondaryColor3ui) {++ret;}
    glSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIVPROC)get_gl_function("glSecondaryColor3uiv");
    if (glSecondaryColor3uiv) {++ret;}
    glSecondaryColor3us = (PFNGLSECONDARYCOLOR3USPROC)get_gl_function("glSecondaryColor3us");
    if (glSecondaryColor3us) {++ret;}
    glSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USVPROC)get_gl_function("glSecondaryColor3usv");
    if (glSecondaryColor3usv) {++ret;}
    glSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTERPROC)get_gl_function("glSecondaryColorPointer");
    if (glSecondaryColorPointer) {++ret;}
    glWindowPos2d = (PFNGLWINDOWPOS2DPROC)get_gl_function("glWindowPos2d");
    if (glWindowPos2d) {++ret;}
    glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC)get_gl_function("glWindowPos2dv");
    if (glWindowPos2dv) {++ret;}
    glWindowPos2f = (PFNGLWINDOWPOS2FPROC)get_gl_function("glWindowPos2f");
    if (glWindowPos2f) {++ret;}
    glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC)get_gl_function("glWindowPos2fv");
    if (glWindowPos2fv) {++ret;}
    glWindowPos2i = (PFNGLWINDOWPOS2IPROC)get_gl_function("glWindowPos2i");
    if (glWindowPos2i) {++ret;}
    glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC)get_gl_function("glWindowPos2iv");
    if (glWindowPos2iv) {++ret;}
    glWindowPos2s = (PFNGLWINDOWPOS2SPROC)get_gl_function("glWindowPos2s");
    if (glWindowPos2s) {++ret;}
    glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC)get_gl_function("glWindowPos2sv");
    if (glWindowPos2sv) {++ret;}
    glWindowPos3d = (PFNGLWINDOWPOS3DPROC)get_gl_function("glWindowPos3d");
    if (glWindowPos3d) {++ret;}
    glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC)get_gl_function("glWindowPos3dv");
    if (glWindowPos3dv) {++ret;}
    glWindowPos3f = (PFNGLWINDOWPOS3FPROC)get_gl_function("glWindowPos3f");
    if (glWindowPos3f) {++ret;}
    glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC)get_gl_function("glWindowPos3fv");
    if (glWindowPos3fv) {++ret;}
    glWindowPos3i = (PFNGLWINDOWPOS3IPROC)get_gl_function("glWindowPos3i");
    if (glWindowPos3i) {++ret;}
    glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC)get_gl_function("glWindowPos3iv");
    if (glWindowPos3iv) {++ret;}
    glWindowPos3s = (PFNGLWINDOWPOS3SPROC)get_gl_function("glWindowPos3s");
    if (glWindowPos3s) {++ret;}
    glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC)get_gl_function("glWindowPos3sv");
    if (glWindowPos3sv) {++ret;}
    glBlendColor = (PFNGLBLENDCOLORPROC)get_gl_function("glBlendColor");
    if (glBlendColor) {++ret;}
    glBlendEquation = (PFNGLBLENDEQUATIONPROC)get_gl_function("glBlendEquation");
    if (glBlendEquation) {++ret;}
    glGenQueries = (PFNGLGENQUERIESPROC)get_gl_function("glGenQueries");
    if (glGenQueries) {++ret;}
    glDeleteQueries = (PFNGLDELETEQUERIESPROC)get_gl_function("glDeleteQueries");
    if (glDeleteQueries) {++ret;}
    glIsQuery = (PFNGLISQUERYPROC)get_gl_function("glIsQuery");
    if (glIsQuery) {++ret;}
    glBeginQuery = (PFNGLBEGINQUERYPROC)get_gl_function("glBeginQuery");
    if (glBeginQuery) {++ret;}
    glEndQuery = (PFNGLENDQUERYPROC)get_gl_function("glEndQuery");
    if (glEndQuery) {++ret;}
    glGetQueryiv = (PFNGLGETQUERYIVPROC)get_gl_function("glGetQueryiv");
    if (glGetQueryiv) {++ret;}
    glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)get_gl_function("glGetQueryObjectiv");
    if (glGetQueryObjectiv) {++ret;}
    glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)get_gl_function("glGetQueryObjectuiv");
    if (glGetQueryObjectuiv) {++ret;}
    glBindBuffer = (PFNGLBINDBUFFERPROC)get_gl_function("glBindBuffer");
    if (glBindBuffer) {++ret;}
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)get_gl_function("glDeleteBuffers");
    if (glDeleteBuffers) {++ret;}
    glGenBuffers = (PFNGLGENBUFFERSPROC)get_gl_function("glGenBuffers");
    if (glGenBuffers) {++ret;}
    glIsBuffer = (PFNGLISBUFFERPROC)get_gl_function("glIsBuffer");
    if (glIsBuffer) {++ret;}
    glBufferData = (PFNGLBUFFERDATAPROC)get_gl_function("glBufferData");
    if (glBufferData) {++ret;}
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)get_gl_function("glBufferSubData");
    if (glBufferSubData) {++ret;}
    glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)get_gl_function("glGetBufferSubData");
    if (glGetBufferSubData) {++ret;}
    glMapBuffer = (PFNGLMAPBUFFERPROC)get_gl_function("glMapBuffer");
    if (glMapBuffer) {++ret;}
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)get_gl_function("glUnmapBuffer");
    if (glUnmapBuffer) {++ret;}
    glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)get_gl_function("glGetBufferParameteriv");
    if (glGetBufferParameteriv) {++ret;}
    glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)get_gl_function("glGetBufferPointerv");
    if (glGetBufferPointerv) {++ret;}
    glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)get_gl_function("glBlendEquationSeparate");
    if (glBlendEquationSeparate) {++ret;}
    glDrawBuffers = (PFNGLDRAWBUFFERSPROC)get_gl_function("glDrawBuffers");
    if (glDrawBuffers) {++ret;}
    glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)get_gl_function("glStencilOpSeparate");
    if (glStencilOpSeparate) {++ret;}
    glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)get_gl_function("glStencilFuncSeparate");
    if (glStencilFuncSeparate) {++ret;}
    glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)get_gl_function("glStencilMaskSeparate");
    if (glStencilMaskSeparate) {++ret;}
    glAttachShader = (PFNGLATTACHSHADERPROC)get_gl_function("glAttachShader");
    if (glAttachShader) {++ret;}
    glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)get_gl_function("glBindAttribLocation");
    if (glBindAttribLocation) {++ret;}
    glCompileShader = (PFNGLCOMPILESHADERPROC)get_gl_function("glCompileShader");
    if (glCompileShader) {++ret;}
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)get_gl_function("glCreateProgram");
    if (glCreateProgram) {++ret;}
    glCreateShader = (PFNGLCREATESHADERPROC)get_gl_function("glCreateShader");
    if (glCreateShader) {++ret;}
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)get_gl_function("glDeleteProgram");
    if (glDeleteProgram) {++ret;}
    glDeleteShader = (PFNGLDELETESHADERPROC)get_gl_function("glDeleteShader");
    if (glDeleteShader) {++ret;}
    glDetachShader = (PFNGLDETACHSHADERPROC)get_gl_function("glDetachShader");
    if (glDetachShader) {++ret;}
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)get_gl_function("glDisableVertexAttribArray");
    if (glDisableVertexAttribArray) {++ret;}
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)get_gl_function("glEnableVertexAttribArray");
    if (glEnableVertexAttribArray) {++ret;}
    glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)get_gl_function("glGetActiveAttrib");
    if (glGetActiveAttrib) {++ret;}
    glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)get_gl_function("glGetActiveUniform");
    if (glGetActiveUniform) {++ret;}
    glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)get_gl_function("glGetAttachedShaders");
    if (glGetAttachedShaders) {++ret;}
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)get_gl_function("glGetAttribLocation");
    if (glGetAttribLocation) {++ret;}
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)get_gl_function("glGetProgramiv");
    if (glGetProgramiv) {++ret;}
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)get_gl_function("glGetProgramInfoLog");
    if (glGetProgramInfoLog) {++ret;}
    glGetShaderiv = (PFNGLGETSHADERIVPROC)get_gl_function("glGetShaderiv");
    if (glGetShaderiv) {++ret;}
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)get_gl_function("glGetShaderInfoLog");
    if (glGetShaderInfoLog) {++ret;}
    glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)get_gl_function("glGetShaderSource");
    if (glGetShaderSource) {++ret;}
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)get_gl_function("glGetUniformLocation");
    if (glGetUniformLocation) {++ret;}
    glGetUniformfv = (PFNGLGETUNIFORMFVPROC)get_gl_function("glGetUniformfv");
    if (glGetUniformfv) {++ret;}
    glGetUniformiv = (PFNGLGETUNIFORMIVPROC)get_gl_function("glGetUniformiv");
    if (glGetUniformiv) {++ret;}
    glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)get_gl_function("glGetVertexAttribdv");
    if (glGetVertexAttribdv) {++ret;}
    glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)get_gl_function("glGetVertexAttribfv");
    if (glGetVertexAttribfv) {++ret;}
    glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)get_gl_function("glGetVertexAttribiv");
    if (glGetVertexAttribiv) {++ret;}
    glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)get_gl_function("glGetVertexAttribPointerv");
    if (glGetVertexAttribPointerv) {++ret;}
    glIsProgram = (PFNGLISPROGRAMPROC)get_gl_function("glIsProgram");
    if (glIsProgram) {++ret;}
    glIsShader = (PFNGLISSHADERPROC)get_gl_function("glIsShader");
    if (glIsShader) {++ret;}
    glLinkProgram = (PFNGLLINKPROGRAMPROC)get_gl_function("glLinkProgram");
    if (glLinkProgram) {++ret;}
    glShaderSource = (PFNGLSHADERSOURCEPROC)get_gl_function("glShaderSource");
    if (glShaderSource) {++ret;}
    glUseProgram = (PFNGLUSEPROGRAMPROC)get_gl_function("glUseProgram");
    if (glUseProgram) {++ret;}
    glUniform1f = (PFNGLUNIFORM1FPROC)get_gl_function("glUniform1f");
    if (glUniform1f) {++ret;}
    glUniform2f = (PFNGLUNIFORM2FPROC)get_gl_function("glUniform2f");
    if (glUniform2f) {++ret;}
    glUniform3f = (PFNGLUNIFORM3FPROC)get_gl_function("glUniform3f");
    if (glUniform3f) {++ret;}
    glUniform4f = (PFNGLUNIFORM4FPROC)get_gl_function("glUniform4f");
    if (glUniform4f) {++ret;}
    glUniform1i = (PFNGLUNIFORM1IPROC)get_gl_function("glUniform1i");
    if (glUniform1i) {++ret;}
    glUniform2i = (PFNGLUNIFORM2IPROC)get_gl_function("glUniform2i");
    if (glUniform2i) {++ret;}
    glUniform3i = (PFNGLUNIFORM3IPROC)get_gl_function("glUniform3i");
    if (glUniform3i) {++ret;}
    glUniform4i = (PFNGLUNIFORM4IPROC)get_gl_function("glUniform4i");
    if (glUniform4i) {++ret;}
    glUniform1fv = (PFNGLUNIFORM1FVPROC)get_gl_function("glUniform1fv");
    if (glUniform1fv) {++ret;}
    glUniform2fv = (PFNGLUNIFORM2FVPROC)get_gl_function("glUniform2fv");
    if (glUniform2fv) {++ret;}
    glUniform3fv = (PFNGLUNIFORM3FVPROC)get_gl_function("glUniform3fv");
    if (glUniform3fv) {++ret;}
    glUniform4fv = (PFNGLUNIFORM4FVPROC)get_gl_function("glUniform4fv");
    if (glUniform4fv) {++ret;}
    glUniform1iv = (PFNGLUNIFORM1IVPROC)get_gl_function("glUniform1iv");
    if (glUniform1iv) {++ret;}
    glUniform2iv = (PFNGLUNIFORM2IVPROC)get_gl_function("glUniform2iv");
    if (glUniform2iv) {++ret;}
    glUniform3iv = (PFNGLUNIFORM3IVPROC)get_gl_function("glUniform3iv");
    if (glUniform3iv) {++ret;}
    glUniform4iv = (PFNGLUNIFORM4IVPROC)get_gl_function("glUniform4iv");
    if (glUniform4iv) {++ret;}
    glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)get_gl_function("glUniformMatrix2fv");
    if (glUniformMatrix2fv) {++ret;}
    glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)get_gl_function("glUniformMatrix3fv");
    if (glUniformMatrix3fv) {++ret;}
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)get_gl_function("glUniformMatrix4fv");
    if (glUniformMatrix4fv) {++ret;}
    glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)get_gl_function("glValidateProgram");
    if (glValidateProgram) {++ret;}
    glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)get_gl_function("glVertexAttrib1d");
    if (glVertexAttrib1d) {++ret;}
    glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)get_gl_function("glVertexAttrib1dv");
    if (glVertexAttrib1dv) {++ret;}
    glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)get_gl_function("glVertexAttrib1f");
    if (glVertexAttrib1f) {++ret;}
    glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)get_gl_function("glVertexAttrib1fv");
    if (glVertexAttrib1fv) {++ret;}
    glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)get_gl_function("glVertexAttrib1s");
    if (glVertexAttrib1s) {++ret;}
    glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)get_gl_function("glVertexAttrib1sv");
    if (glVertexAttrib1sv) {++ret;}
    glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)get_gl_function("glVertexAttrib2d");
    if (glVertexAttrib2d) {++ret;}
    glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)get_gl_function("glVertexAttrib2dv");
    if (glVertexAttrib2dv) {++ret;}
    glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)get_gl_function("glVertexAttrib2f");
    if (glVertexAttrib2f) {++ret;}
    glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)get_gl_function("glVertexAttrib2fv");
    if (glVertexAttrib2fv) {++ret;}
    glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)get_gl_function("glVertexAttrib2s");
    if (glVertexAttrib2s) {++ret;}
    glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)get_gl_function("glVertexAttrib2sv");
    if (glVertexAttrib2sv) {++ret;}
    glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)get_gl_function("glVertexAttrib3d");
    if (glVertexAttrib3d) {++ret;}
    glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)get_gl_function("glVertexAttrib3dv");
    if (glVertexAttrib3dv) {++ret;}
    glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)get_gl_function("glVertexAttrib3f");
    if (glVertexAttrib3f) {++ret;}
    glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)get_gl_function("glVertexAttrib3fv");
    if (glVertexAttrib3fv) {++ret;}
    glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)get_gl_function("glVertexAttrib3s");
    if (glVertexAttrib3s) {++ret;}
    glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)get_gl_function("glVertexAttrib3sv");
    if (glVertexAttrib3sv) {++ret;}
    glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)get_gl_function("glVertexAttrib4Nbv");
    if (glVertexAttrib4Nbv) {++ret;}
    glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)get_gl_function("glVertexAttrib4Niv");
    if (glVertexAttrib4Niv) {++ret;}
    glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)get_gl_function("glVertexAttrib4Nsv");
    if (glVertexAttrib4Nsv) {++ret;}
    glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)get_gl_function("glVertexAttrib4Nub");
    if (glVertexAttrib4Nub) {++ret;}
    glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)get_gl_function("glVertexAttrib4Nubv");
    if (glVertexAttrib4Nubv) {++ret;}
    glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)get_gl_function("glVertexAttrib4Nuiv");
    if (glVertexAttrib4Nuiv) {++ret;}
    glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)get_gl_function("glVertexAttrib4Nusv");
    if (glVertexAttrib4Nusv) {++ret;}
    glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)get_gl_function("glVertexAttrib4bv");
    if (glVertexAttrib4bv) {++ret;}
    glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)get_gl_function("glVertexAttrib4d");
    if (glVertexAttrib4d) {++ret;}
    glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)get_gl_function("glVertexAttrib4dv");
    if (glVertexAttrib4dv) {++ret;}
    glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)get_gl_function("glVertexAttrib4f");
    if (glVertexAttrib4f) {++ret;}
    glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)get_gl_function("glVertexAttrib4fv");
    if (glVertexAttrib4fv) {++ret;}
    glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)get_gl_function("glVertexAttrib4iv");
    if (glVertexAttrib4iv) {++ret;}
    glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)get_gl_function("glVertexAttrib4s");
    if (glVertexAttrib4s) {++ret;}
    glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)get_gl_function("glVertexAttrib4sv");
    if (glVertexAttrib4sv) {++ret;}
    glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)get_gl_function("glVertexAttrib4ubv");
    if (glVertexAttrib4ubv) {++ret;}
    glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)get_gl_function("glVertexAttrib4uiv");
    if (glVertexAttrib4uiv) {++ret;}
    glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)get_gl_function("glVertexAttrib4usv");
    if (glVertexAttrib4usv) {++ret;}
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)get_gl_function("glVertexAttribPointer");
    if (glVertexAttribPointer) {++ret;}
    glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)get_gl_function("glUniformMatrix2x3fv");
    if (glUniformMatrix2x3fv) {++ret;}
    glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)get_gl_function("glUniformMatrix3x2fv");
    if (glUniformMatrix3x2fv) {++ret;}
    glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)get_gl_function("glUniformMatrix2x4fv");
    if (glUniformMatrix2x4fv) {++ret;}
    glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)get_gl_function("glUniformMatrix4x2fv");
    if (glUniformMatrix4x2fv) {++ret;}
    glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)get_gl_function("glUniformMatrix3x4fv");
    if (glUniformMatrix3x4fv) {++ret;}
    glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)get_gl_function("glUniformMatrix4x3fv");
    if (glUniformMatrix4x3fv) {++ret;}
    glColorMaski = (PFNGLCOLORMASKIPROC)get_gl_function("glColorMaski");
    if (glColorMaski) {++ret;}
    glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)get_gl_function("glGetBooleani_v");
    if (glGetBooleani_v) {++ret;}
    glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)get_gl_function("glGetIntegeri_v");
    if (glGetIntegeri_v) {++ret;}
    glEnablei = (PFNGLENABLEIPROC)get_gl_function("glEnablei");
    if (glEnablei) {++ret;}
    glDisablei = (PFNGLDISABLEIPROC)get_gl_function("glDisablei");
    if (glDisablei) {++ret;}
    glIsEnabledi = (PFNGLISENABLEDIPROC)get_gl_function("glIsEnabledi");
    if (glIsEnabledi) {++ret;}
    glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)get_gl_function("glBeginTransformFeedback");
    if (glBeginTransformFeedback) {++ret;}
    glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)get_gl_function("glEndTransformFeedback");
    if (glEndTransformFeedback) {++ret;}
    glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)get_gl_function("glBindBufferRange");
    if (glBindBufferRange) {++ret;}
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)get_gl_function("glBindBufferBase");
    if (glBindBufferBase) {++ret;}
    glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)get_gl_function("glTransformFeedbackVaryings");
    if (glTransformFeedbackVaryings) {++ret;}
    glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)get_gl_function("glGetTransformFeedbackVarying");
    if (glGetTransformFeedbackVarying) {++ret;}
    glClampColor = (PFNGLCLAMPCOLORPROC)get_gl_function("glClampColor");
    if (glClampColor) {++ret;}
    glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)get_gl_function("glBeginConditionalRender");
    if (glBeginConditionalRender) {++ret;}
    glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)get_gl_function("glEndConditionalRender");
    if (glEndConditionalRender) {++ret;}
    glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)get_gl_function("glVertexAttribIPointer");
    if (glVertexAttribIPointer) {++ret;}
    glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)get_gl_function("glGetVertexAttribIiv");
    if (glGetVertexAttribIiv) {++ret;}
    glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)get_gl_function("glGetVertexAttribIuiv");
    if (glGetVertexAttribIuiv) {++ret;}
    glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)get_gl_function("glVertexAttribI1i");
    if (glVertexAttribI1i) {++ret;}
    glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)get_gl_function("glVertexAttribI2i");
    if (glVertexAttribI2i) {++ret;}
    glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)get_gl_function("glVertexAttribI3i");
    if (glVertexAttribI3i) {++ret;}
    glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)get_gl_function("glVertexAttribI4i");
    if (glVertexAttribI4i) {++ret;}
    glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)get_gl_function("glVertexAttribI1ui");
    if (glVertexAttribI1ui) {++ret;}
    glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)get_gl_function("glVertexAttribI2ui");
    if (glVertexAttribI2ui) {++ret;}
    glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)get_gl_function("glVertexAttribI3ui");
    if (glVertexAttribI3ui) {++ret;}
    glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)get_gl_function("glVertexAttribI4ui");
    if (glVertexAttribI4ui) {++ret;}
    glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)get_gl_function("glVertexAttribI1iv");
    if (glVertexAttribI1iv) {++ret;}
    glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)get_gl_function("glVertexAttribI2iv");
    if (glVertexAttribI2iv) {++ret;}
    glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)get_gl_function("glVertexAttribI3iv");
    if (glVertexAttribI3iv) {++ret;}
    glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)get_gl_function("glVertexAttribI4iv");
    if (glVertexAttribI4iv) {++ret;}
    glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)get_gl_function("glVertexAttribI1uiv");
    if (glVertexAttribI1uiv) {++ret;}
    glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)get_gl_function("glVertexAttribI2uiv");
    if (glVertexAttribI2uiv) {++ret;}
    glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)get_gl_function("glVertexAttribI3uiv");
    if (glVertexAttribI3uiv) {++ret;}
    glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)get_gl_function("glVertexAttribI4uiv");
    if (glVertexAttribI4uiv) {++ret;}
    glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)get_gl_function("glVertexAttribI4bv");
    if (glVertexAttribI4bv) {++ret;}
    glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)get_gl_function("glVertexAttribI4sv");
    if (glVertexAttribI4sv) {++ret;}
    glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)get_gl_function("glVertexAttribI4ubv");
    if (glVertexAttribI4ubv) {++ret;}
    glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)get_gl_function("glVertexAttribI4usv");
    if (glVertexAttribI4usv) {++ret;}
    glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)get_gl_function("glGetUniformuiv");
    if (glGetUniformuiv) {++ret;}
    glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)get_gl_function("glBindFragDataLocation");
    if (glBindFragDataLocation) {++ret;}
    glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)get_gl_function("glGetFragDataLocation");
    if (glGetFragDataLocation) {++ret;}
    glUniform1ui = (PFNGLUNIFORM1UIPROC)get_gl_function("glUniform1ui");
    if (glUniform1ui) {++ret;}
    glUniform2ui = (PFNGLUNIFORM2UIPROC)get_gl_function("glUniform2ui");
    if (glUniform2ui) {++ret;}
    glUniform3ui = (PFNGLUNIFORM3UIPROC)get_gl_function("glUniform3ui");
    if (glUniform3ui) {++ret;}
    glUniform4ui = (PFNGLUNIFORM4UIPROC)get_gl_function("glUniform4ui");
    if (glUniform4ui) {++ret;}
    glUniform1uiv = (PFNGLUNIFORM1UIVPROC)get_gl_function("glUniform1uiv");
    if (glUniform1uiv) {++ret;}
    glUniform2uiv = (PFNGLUNIFORM2UIVPROC)get_gl_function("glUniform2uiv");
    if (glUniform2uiv) {++ret;}
    glUniform3uiv = (PFNGLUNIFORM3UIVPROC)get_gl_function("glUniform3uiv");
    if (glUniform3uiv) {++ret;}
    glUniform4uiv = (PFNGLUNIFORM4UIVPROC)get_gl_function("glUniform4uiv");
    if (glUniform4uiv) {++ret;}
    glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)get_gl_function("glTexParameterIiv");
    if (glTexParameterIiv) {++ret;}
    glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)get_gl_function("glTexParameterIuiv");
    if (glTexParameterIuiv) {++ret;}
    glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)get_gl_function("glGetTexParameterIiv");
    if (glGetTexParameterIiv) {++ret;}
    glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)get_gl_function("glGetTexParameterIuiv");
    if (glGetTexParameterIuiv) {++ret;}
    glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)get_gl_function("glClearBufferiv");
    if (glClearBufferiv) {++ret;}
    glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)get_gl_function("glClearBufferuiv");
    if (glClearBufferuiv) {++ret;}
    glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)get_gl_function("glClearBufferfv");
    if (glClearBufferfv) {++ret;}
    glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)get_gl_function("glClearBufferfi");
    if (glClearBufferfi) {++ret;}
    glGetStringi = (PFNGLGETSTRINGIPROC)get_gl_function("glGetStringi");
    if (glGetStringi) {++ret;}
    glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)get_gl_function("glIsRenderbuffer");
    if (glIsRenderbuffer) {++ret;}
    glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)get_gl_function("glBindRenderbuffer");
    if (glBindRenderbuffer) {++ret;}
    glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)get_gl_function("glDeleteRenderbuffers");
    if (glDeleteRenderbuffers) {++ret;}
    glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)get_gl_function("glGenRenderbuffers");
    if (glGenRenderbuffers) {++ret;}
    glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)get_gl_function("glRenderbufferStorage");
    if (glRenderbufferStorage) {++ret;}
    glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)get_gl_function("glGetRenderbufferParameteriv");
    if (glGetRenderbufferParameteriv) {++ret;}
    glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)get_gl_function("glIsFramebuffer");
    if (glIsFramebuffer) {++ret;}
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)get_gl_function("glBindFramebuffer");
    if (glBindFramebuffer) {++ret;}
    glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)get_gl_function("glDeleteFramebuffers");
    if (glDeleteFramebuffers) {++ret;}
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)get_gl_function("glGenFramebuffers");
    if (glGenFramebuffers) {++ret;}
    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)get_gl_function("glCheckFramebufferStatus");
    if (glCheckFramebufferStatus) {++ret;}
    glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)get_gl_function("glFramebufferTexture1D");
    if (glFramebufferTexture1D) {++ret;}
    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)get_gl_function("glFramebufferTexture2D");
    if (glFramebufferTexture2D) {++ret;}
    glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)get_gl_function("glFramebufferTexture3D");
    if (glFramebufferTexture3D) {++ret;}
    glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)get_gl_function("glFramebufferRenderbuffer");
    if (glFramebufferRenderbuffer) {++ret;}
    glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)get_gl_function("glGetFramebufferAttachmentParameteriv");
    if (glGetFramebufferAttachmentParameteriv) {++ret;}
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)get_gl_function("glGenerateMipmap");
    if (glGenerateMipmap) {++ret;}
    glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)get_gl_function("glBlitFramebuffer");
    if (glBlitFramebuffer) {++ret;}
    glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)get_gl_function("glRenderbufferStorageMultisample");
    if (glRenderbufferStorageMultisample) {++ret;}
    glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)get_gl_function("glFramebufferTextureLayer");
    if (glFramebufferTextureLayer) {++ret;}
    glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)get_gl_function("glMapBufferRange");
    if (glMapBufferRange) {++ret;}
    glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)get_gl_function("glFlushMappedBufferRange");
    if (glFlushMappedBufferRange) {++ret;}
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)get_gl_function("glBindVertexArray");
    if (glBindVertexArray) {++ret;}
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)get_gl_function("glDeleteVertexArrays");
    if (glDeleteVertexArrays) {++ret;}
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)get_gl_function("glGenVertexArrays");
    if (glGenVertexArrays) {++ret;}
    glIsVertexArray = (PFNGLISVERTEXARRAYPROC)get_gl_function("glIsVertexArray");
    if (glIsVertexArray) {++ret;}
    glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)get_gl_function("glDrawArraysInstanced");
    if (glDrawArraysInstanced) {++ret;}
    glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)get_gl_function("glDrawElementsInstanced");
    if (glDrawElementsInstanced) {++ret;}
    glTexBuffer = (PFNGLTEXBUFFERPROC)get_gl_function("glTexBuffer");
    if (glTexBuffer) {++ret;}
    glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)get_gl_function("glPrimitiveRestartIndex");
    if (glPrimitiveRestartIndex) {++ret;}
    glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)get_gl_function("glCopyBufferSubData");
    if (glCopyBufferSubData) {++ret;}
    glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)get_gl_function("glGetUniformIndices");
    if (glGetUniformIndices) {++ret;}
    glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)get_gl_function("glGetActiveUniformsiv");
    if (glGetActiveUniformsiv) {++ret;}
    glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)get_gl_function("glGetActiveUniformName");
    if (glGetActiveUniformName) {++ret;}
    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)get_gl_function("glGetUniformBlockIndex");
    if (glGetUniformBlockIndex) {++ret;}
    glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)get_gl_function("glGetActiveUniformBlockiv");
    if (glGetActiveUniformBlockiv) {++ret;}
    glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)get_gl_function("glGetActiveUniformBlockName");
    if (glGetActiveUniformBlockName) {++ret;}
    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)get_gl_function("glUniformBlockBinding");
    if (glUniformBlockBinding) {++ret;}
    glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)get_gl_function("glDrawElementsBaseVertex");
    if (glDrawElementsBaseVertex) {++ret;}
    glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)get_gl_function("glDrawRangeElementsBaseVertex");
    if (glDrawRangeElementsBaseVertex) {++ret;}
    glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)get_gl_function("glDrawElementsInstancedBaseVertex");
    if (glDrawElementsInstancedBaseVertex) {++ret;}
    glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)get_gl_function("glMultiDrawElementsBaseVertex");
    if (glMultiDrawElementsBaseVertex) {++ret;}
    glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)get_gl_function("glProvokingVertex");
    if (glProvokingVertex) {++ret;}
    glFenceSync = (PFNGLFENCESYNCPROC)get_gl_function("glFenceSync");
    if (glFenceSync) {++ret;}
    glIsSync = (PFNGLISSYNCPROC)get_gl_function("glIsSync");
    if (glIsSync) {++ret;}
    glDeleteSync = (PFNGLDELETESYNCPROC)get_gl_function("glDeleteSync");
    if (glDeleteSync) {++ret;}
    glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)get_gl_function("glClientWaitSync");
    if (glClientWaitSync) {++ret;}
    glWaitSync = (PFNGLWAITSYNCPROC)get_gl_function("glWaitSync");
    if (glWaitSync) {++ret;}
    glGetInteger64v = (PFNGLGETINTEGER64VPROC)get_gl_function("glGetInteger64v");
    if (glGetInteger64v) {++ret;}
    glGetSynciv = (PFNGLGETSYNCIVPROC)get_gl_function("glGetSynciv");
    if (glGetSynciv) {++ret;}
    glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)get_gl_function("glGetInteger64i_v");
    if (glGetInteger64i_v) {++ret;}
    glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)get_gl_function("glGetBufferParameteri64v");
    if (glGetBufferParameteri64v) {++ret;}
    glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)get_gl_function("glFramebufferTexture");
    if (glFramebufferTexture) {++ret;}
    glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)get_gl_function("glTexImage2DMultisample");
    if (glTexImage2DMultisample) {++ret;}
    glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)get_gl_function("glTexImage3DMultisample");
    if (glTexImage3DMultisample) {++ret;}
    glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)get_gl_function("glGetMultisamplefv");
    if (glGetMultisamplefv) {++ret;}
    glSampleMaski = (PFNGLSAMPLEMASKIPROC)get_gl_function("glSampleMaski");
    if (glSampleMaski) {++ret;}
    glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)get_gl_function("glBindFragDataLocationIndexed");
    if (glBindFragDataLocationIndexed) {++ret;}
    glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)get_gl_function("glGetFragDataIndex");
    if (glGetFragDataIndex) {++ret;}
    glGenSamplers = (PFNGLGENSAMPLERSPROC)get_gl_function("glGenSamplers");
    if (glGenSamplers) {++ret;}
    glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)get_gl_function("glDeleteSamplers");
    if (glDeleteSamplers) {++ret;}
    glIsSampler = (PFNGLISSAMPLERPROC)get_gl_function("glIsSampler");
    if (glIsSampler) {++ret;}
    glBindSampler = (PFNGLBINDSAMPLERPROC)get_gl_function("glBindSampler");
    if (glBindSampler) {++ret;}
    glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)get_gl_function("glSamplerParameteri");
    if (glSamplerParameteri) {++ret;}
    glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)get_gl_function("glSamplerParameteriv");
    if (glSamplerParameteriv) {++ret;}
    glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)get_gl_function("glSamplerParameterf");
    if (glSamplerParameterf) {++ret;}
    glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)get_gl_function("glSamplerParameterfv");
    if (glSamplerParameterfv) {++ret;}
    glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)get_gl_function("glSamplerParameterIiv");
    if (glSamplerParameterIiv) {++ret;}
    glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)get_gl_function("glSamplerParameterIuiv");
    if (glSamplerParameterIuiv) {++ret;}
    glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)get_gl_function("glGetSamplerParameteriv");
    if (glGetSamplerParameteriv) {++ret;}
    glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)get_gl_function("glGetSamplerParameterIiv");
    if (glGetSamplerParameterIiv) {++ret;}
    glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)get_gl_function("glGetSamplerParameterfv");
    if (glGetSamplerParameterfv) {++ret;}
    glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)get_gl_function("glGetSamplerParameterIuiv");
    if (glGetSamplerParameterIuiv) {++ret;}
    glQueryCounter = (PFNGLQUERYCOUNTERPROC)get_gl_function("glQueryCounter");
    if (glQueryCounter) {++ret;}
    glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)get_gl_function("glGetQueryObjecti64v");
    if (glGetQueryObjecti64v) {++ret;}
    glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)get_gl_function("glGetQueryObjectui64v");
    if (glGetQueryObjectui64v) {++ret;}
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)get_gl_function("glVertexAttribDivisor");
    if (glVertexAttribDivisor) {++ret;}
    glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)get_gl_function("glVertexAttribP1ui");
    if (glVertexAttribP1ui) {++ret;}
    glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)get_gl_function("glVertexAttribP1uiv");
    if (glVertexAttribP1uiv) {++ret;}
    glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)get_gl_function("glVertexAttribP2ui");
    if (glVertexAttribP2ui) {++ret;}
    glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)get_gl_function("glVertexAttribP2uiv");
    if (glVertexAttribP2uiv) {++ret;}
    glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)get_gl_function("glVertexAttribP3ui");
    if (glVertexAttribP3ui) {++ret;}
    glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)get_gl_function("glVertexAttribP3uiv");
    if (glVertexAttribP3uiv) {++ret;}
    glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)get_gl_function("glVertexAttribP4ui");
    if (glVertexAttribP4ui) {++ret;}
    glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)get_gl_function("glVertexAttribP4uiv");
    if (glVertexAttribP4uiv) {++ret;}
    glVertexP2ui = (PFNGLVERTEXP2UIPROC)get_gl_function("glVertexP2ui");
    if (glVertexP2ui) {++ret;}
    glVertexP2uiv = (PFNGLVERTEXP2UIVPROC)get_gl_function("glVertexP2uiv");
    if (glVertexP2uiv) {++ret;}
    glVertexP3ui = (PFNGLVERTEXP3UIPROC)get_gl_function("glVertexP3ui");
    if (glVertexP3ui) {++ret;}
    glVertexP3uiv = (PFNGLVERTEXP3UIVPROC)get_gl_function("glVertexP3uiv");
    if (glVertexP3uiv) {++ret;}
    glVertexP4ui = (PFNGLVERTEXP4UIPROC)get_gl_function("glVertexP4ui");
    if (glVertexP4ui) {++ret;}
    glVertexP4uiv = (PFNGLVERTEXP4UIVPROC)get_gl_function("glVertexP4uiv");
    if (glVertexP4uiv) {++ret;}
    glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC)get_gl_function("glTexCoordP1ui");
    if (glTexCoordP1ui) {++ret;}
    glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC)get_gl_function("glTexCoordP1uiv");
    if (glTexCoordP1uiv) {++ret;}
    glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC)get_gl_function("glTexCoordP2ui");
    if (glTexCoordP2ui) {++ret;}
    glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC)get_gl_function("glTexCoordP2uiv");
    if (glTexCoordP2uiv) {++ret;}
    glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC)get_gl_function("glTexCoordP3ui");
    if (glTexCoordP3ui) {++ret;}
    glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC)get_gl_function("glTexCoordP3uiv");
    if (glTexCoordP3uiv) {++ret;}
    glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC)get_gl_function("glTexCoordP4ui");
    if (glTexCoordP4ui) {++ret;}
    glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC)get_gl_function("glTexCoordP4uiv");
    if (glTexCoordP4uiv) {++ret;}
    glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC)get_gl_function("glMultiTexCoordP1ui");
    if (glMultiTexCoordP1ui) {++ret;}
    glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC)get_gl_function("glMultiTexCoordP1uiv");
    if (glMultiTexCoordP1uiv) {++ret;}
    glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC)get_gl_function("glMultiTexCoordP2ui");
    if (glMultiTexCoordP2ui) {++ret;}
    glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC)get_gl_function("glMultiTexCoordP2uiv");
    if (glMultiTexCoordP2uiv) {++ret;}
    glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC)get_gl_function("glMultiTexCoordP3ui");
    if (glMultiTexCoordP3ui) {++ret;}
    glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC)get_gl_function("glMultiTexCoordP3uiv");
    if (glMultiTexCoordP3uiv) {++ret;}
    glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC)get_gl_function("glMultiTexCoordP4ui");
    if (glMultiTexCoordP4ui) {++ret;}
    glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC)get_gl_function("glMultiTexCoordP4uiv");
    if (glMultiTexCoordP4uiv) {++ret;}
    glNormalP3ui = (PFNGLNORMALP3UIPROC)get_gl_function("glNormalP3ui");
    if (glNormalP3ui) {++ret;}
    glNormalP3uiv = (PFNGLNORMALP3UIVPROC)get_gl_function("glNormalP3uiv");
    if (glNormalP3uiv) {++ret;}
    glColorP3ui = (PFNGLCOLORP3UIPROC)get_gl_function("glColorP3ui");
    if (glColorP3ui) {++ret;}
    glColorP3uiv = (PFNGLCOLORP3UIVPROC)get_gl_function("glColorP3uiv");
    if (glColorP3uiv) {++ret;}
    glColorP4ui = (PFNGLCOLORP4UIPROC)get_gl_function("glColorP4ui");
    if (glColorP4ui) {++ret;}
    glColorP4uiv = (PFNGLCOLORP4UIVPROC)get_gl_function("glColorP4uiv");
    if (glColorP4uiv) {++ret;}
    glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC)get_gl_function("glSecondaryColorP3ui");
    if (glSecondaryColorP3ui) {++ret;}
    glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC)get_gl_function("glSecondaryColorP3uiv");
    if (glSecondaryColorP3uiv) {++ret;}
    glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)get_gl_function("glMinSampleShading");
    if (glMinSampleShading) {++ret;}
    glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)get_gl_function("glBlendEquationi");
    if (glBlendEquationi) {++ret;}
    glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)get_gl_function("glBlendEquationSeparatei");
    if (glBlendEquationSeparatei) {++ret;}
    glBlendFunci = (PFNGLBLENDFUNCIPROC)get_gl_function("glBlendFunci");
    if (glBlendFunci) {++ret;}
    glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)get_gl_function("glBlendFuncSeparatei");
    if (glBlendFuncSeparatei) {++ret;}
    glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)get_gl_function("glDrawArraysIndirect");
    if (glDrawArraysIndirect) {++ret;}
    glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)get_gl_function("glDrawElementsIndirect");
    if (glDrawElementsIndirect) {++ret;}
    glUniform1d = (PFNGLUNIFORM1DPROC)get_gl_function("glUniform1d");
    if (glUniform1d) {++ret;}
    glUniform2d = (PFNGLUNIFORM2DPROC)get_gl_function("glUniform2d");
    if (glUniform2d) {++ret;}
    glUniform3d = (PFNGLUNIFORM3DPROC)get_gl_function("glUniform3d");
    if (glUniform3d) {++ret;}
    glUniform4d = (PFNGLUNIFORM4DPROC)get_gl_function("glUniform4d");
    if (glUniform4d) {++ret;}
    glUniform1dv = (PFNGLUNIFORM1DVPROC)get_gl_function("glUniform1dv");
    if (glUniform1dv) {++ret;}
    glUniform2dv = (PFNGLUNIFORM2DVPROC)get_gl_function("glUniform2dv");
    if (glUniform2dv) {++ret;}
    glUniform3dv = (PFNGLUNIFORM3DVPROC)get_gl_function("glUniform3dv");
    if (glUniform3dv) {++ret;}
    glUniform4dv = (PFNGLUNIFORM4DVPROC)get_gl_function("glUniform4dv");
    if (glUniform4dv) {++ret;}
    glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)get_gl_function("glUniformMatrix2dv");
    if (glUniformMatrix2dv) {++ret;}
    glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)get_gl_function("glUniformMatrix3dv");
    if (glUniformMatrix3dv) {++ret;}
    glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)get_gl_function("glUniformMatrix4dv");
    if (glUniformMatrix4dv) {++ret;}
    glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)get_gl_function("glUniformMatrix2x3dv");
    if (glUniformMatrix2x3dv) {++ret;}
    glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)get_gl_function("glUniformMatrix2x4dv");
    if (glUniformMatrix2x4dv) {++ret;}
    glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)get_gl_function("glUniformMatrix3x2dv");
    if (glUniformMatrix3x2dv) {++ret;}
    glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)get_gl_function("glUniformMatrix3x4dv");
    if (glUniformMatrix3x4dv) {++ret;}
    glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)get_gl_function("glUniformMatrix4x2dv");
    if (glUniformMatrix4x2dv) {++ret;}
    glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)get_gl_function("glUniformMatrix4x3dv");
    if (glUniformMatrix4x3dv) {++ret;}
    glGetUniformdv = (PFNGLGETUNIFORMDVPROC)get_gl_function("glGetUniformdv");
    if (glGetUniformdv) {++ret;}
    glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)get_gl_function("glGetSubroutineUniformLocation");
    if (glGetSubroutineUniformLocation) {++ret;}
    glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)get_gl_function("glGetSubroutineIndex");
    if (glGetSubroutineIndex) {++ret;}
    glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)get_gl_function("glGetActiveSubroutineUniformiv");
    if (glGetActiveSubroutineUniformiv) {++ret;}
    glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)get_gl_function("glGetActiveSubroutineUniformName");
    if (glGetActiveSubroutineUniformName) {++ret;}
    glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)get_gl_function("glGetActiveSubroutineName");
    if (glGetActiveSubroutineName) {++ret;}
    glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)get_gl_function("glUniformSubroutinesuiv");
    if (glUniformSubroutinesuiv) {++ret;}
    glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)get_gl_function("glGetUniformSubroutineuiv");
    if (glGetUniformSubroutineuiv) {++ret;}
    glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)get_gl_function("glGetProgramStageiv");
    if (glGetProgramStageiv) {++ret;}
    glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)get_gl_function("glPatchParameteri");
    if (glPatchParameteri) {++ret;}
    glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)get_gl_function("glPatchParameterfv");
    if (glPatchParameterfv) {++ret;}
    glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)get_gl_function("glBindTransformFeedback");
    if (glBindTransformFeedback) {++ret;}
    glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)get_gl_function("glDeleteTransformFeedbacks");
    if (glDeleteTransformFeedbacks) {++ret;}
    glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)get_gl_function("glGenTransformFeedbacks");
    if (glGenTransformFeedbacks) {++ret;}
    glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)get_gl_function("glIsTransformFeedback");
    if (glIsTransformFeedback) {++ret;}
    glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)get_gl_function("glPauseTransformFeedback");
    if (glPauseTransformFeedback) {++ret;}
    glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)get_gl_function("glResumeTransformFeedback");
    if (glResumeTransformFeedback) {++ret;}
    glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)get_gl_function("glDrawTransformFeedback");
    if (glDrawTransformFeedback) {++ret;}
    glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)get_gl_function("glDrawTransformFeedbackStream");
    if (glDrawTransformFeedbackStream) {++ret;}
    glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)get_gl_function("glBeginQueryIndexed");
    if (glBeginQueryIndexed) {++ret;}
    glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)get_gl_function("glEndQueryIndexed");
    if (glEndQueryIndexed) {++ret;}
    glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)get_gl_function("glGetQueryIndexediv");
    if (glGetQueryIndexediv) {++ret;}
    glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)get_gl_function("glReleaseShaderCompiler");
    if (glReleaseShaderCompiler) {++ret;}
    glShaderBinary = (PFNGLSHADERBINARYPROC)get_gl_function("glShaderBinary");
    if (glShaderBinary) {++ret;}
    glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)get_gl_function("glGetShaderPrecisionFormat");
    if (glGetShaderPrecisionFormat) {++ret;}
    glDepthRangef = (PFNGLDEPTHRANGEFPROC)get_gl_function("glDepthRangef");
    if (glDepthRangef) {++ret;}
    glClearDepthf = (PFNGLCLEARDEPTHFPROC)get_gl_function("glClearDepthf");
    if (glClearDepthf) {++ret;}
    glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)get_gl_function("glGetProgramBinary");
    if (glGetProgramBinary) {++ret;}
    glProgramBinary = (PFNGLPROGRAMBINARYPROC)get_gl_function("glProgramBinary");
    if (glProgramBinary) {++ret;}
    glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)get_gl_function("glProgramParameteri");
    if (glProgramParameteri) {++ret;}
    glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)get_gl_function("glUseProgramStages");
    if (glUseProgramStages) {++ret;}
    glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)get_gl_function("glActiveShaderProgram");
    if (glActiveShaderProgram) {++ret;}
    glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)get_gl_function("glCreateShaderProgramv");
    if (glCreateShaderProgramv) {++ret;}
    glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)get_gl_function("glBindProgramPipeline");
    if (glBindProgramPipeline) {++ret;}
    glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)get_gl_function("glDeleteProgramPipelines");
    if (glDeleteProgramPipelines) {++ret;}
    glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)get_gl_function("glGenProgramPipelines");
    if (glGenProgramPipelines) {++ret;}
    glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)get_gl_function("glIsProgramPipeline");
    if (glIsProgramPipeline) {++ret;}
    glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)get_gl_function("glGetProgramPipelineiv");
    if (glGetProgramPipelineiv) {++ret;}
    glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)get_gl_function("glProgramUniform1i");
    if (glProgramUniform1i) {++ret;}
    glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)get_gl_function("glProgramUniform1iv");
    if (glProgramUniform1iv) {++ret;}
    glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)get_gl_function("glProgramUniform1f");
    if (glProgramUniform1f) {++ret;}
    glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)get_gl_function("glProgramUniform1fv");
    if (glProgramUniform1fv) {++ret;}
    glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)get_gl_function("glProgramUniform1d");
    if (glProgramUniform1d) {++ret;}
    glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)get_gl_function("glProgramUniform1dv");
    if (glProgramUniform1dv) {++ret;}
    glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)get_gl_function("glProgramUniform1ui");
    if (glProgramUniform1ui) {++ret;}
    glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)get_gl_function("glProgramUniform1uiv");
    if (glProgramUniform1uiv) {++ret;}
    glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)get_gl_function("glProgramUniform2i");
    if (glProgramUniform2i) {++ret;}
    glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)get_gl_function("glProgramUniform2iv");
    if (glProgramUniform2iv) {++ret;}
    glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)get_gl_function("glProgramUniform2f");
    if (glProgramUniform2f) {++ret;}
    glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)get_gl_function("glProgramUniform2fv");
    if (glProgramUniform2fv) {++ret;}
    glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)get_gl_function("glProgramUniform2d");
    if (glProgramUniform2d) {++ret;}
    glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)get_gl_function("glProgramUniform2dv");
    if (glProgramUniform2dv) {++ret;}
    glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)get_gl_function("glProgramUniform2ui");
    if (glProgramUniform2ui) {++ret;}
    glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)get_gl_function("glProgramUniform2uiv");
    if (glProgramUniform2uiv) {++ret;}
    glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)get_gl_function("glProgramUniform3i");
    if (glProgramUniform3i) {++ret;}
    glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)get_gl_function("glProgramUniform3iv");
    if (glProgramUniform3iv) {++ret;}
    glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)get_gl_function("glProgramUniform3f");
    if (glProgramUniform3f) {++ret;}
    glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)get_gl_function("glProgramUniform3fv");
    if (glProgramUniform3fv) {++ret;}
    glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)get_gl_function("glProgramUniform3d");
    if (glProgramUniform3d) {++ret;}
    glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)get_gl_function("glProgramUniform3dv");
    if (glProgramUniform3dv) {++ret;}
    glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)get_gl_function("glProgramUniform3ui");
    if (glProgramUniform3ui) {++ret;}
    glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)get_gl_function("glProgramUniform3uiv");
    if (glProgramUniform3uiv) {++ret;}
    glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)get_gl_function("glProgramUniform4i");
    if (glProgramUniform4i) {++ret;}
    glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)get_gl_function("glProgramUniform4iv");
    if (glProgramUniform4iv) {++ret;}
    glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)get_gl_function("glProgramUniform4f");
    if (glProgramUniform4f) {++ret;}
    glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)get_gl_function("glProgramUniform4fv");
    if (glProgramUniform4fv) {++ret;}
    glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)get_gl_function("glProgramUniform4d");
    if (glProgramUniform4d) {++ret;}
    glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)get_gl_function("glProgramUniform4dv");
    if (glProgramUniform4dv) {++ret;}
    glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)get_gl_function("glProgramUniform4ui");
    if (glProgramUniform4ui) {++ret;}
    glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)get_gl_function("glProgramUniform4uiv");
    if (glProgramUniform4uiv) {++ret;}
    glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)get_gl_function("glProgramUniformMatrix2fv");
    if (glProgramUniformMatrix2fv) {++ret;}
    glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)get_gl_function("glProgramUniformMatrix3fv");
    if (glProgramUniformMatrix3fv) {++ret;}
    glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)get_gl_function("glProgramUniformMatrix4fv");
    if (glProgramUniformMatrix4fv) {++ret;}
    glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)get_gl_function("glProgramUniformMatrix2dv");
    if (glProgramUniformMatrix2dv) {++ret;}
    glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)get_gl_function("glProgramUniformMatrix3dv");
    if (glProgramUniformMatrix3dv) {++ret;}
    glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)get_gl_function("glProgramUniformMatrix4dv");
    if (glProgramUniformMatrix4dv) {++ret;}
    glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)get_gl_function("glProgramUniformMatrix2x3fv");
    if (glProgramUniformMatrix2x3fv) {++ret;}
    glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)get_gl_function("glProgramUniformMatrix3x2fv");
    if (glProgramUniformMatrix3x2fv) {++ret;}
    glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)get_gl_function("glProgramUniformMatrix2x4fv");
    if (glProgramUniformMatrix2x4fv) {++ret;}
    glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)get_gl_function("glProgramUniformMatrix4x2fv");
    if (glProgramUniformMatrix4x2fv) {++ret;}
    glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)get_gl_function("glProgramUniformMatrix3x4fv");
    if (glProgramUniformMatrix3x4fv) {++ret;}
    glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)get_gl_function("glProgramUniformMatrix4x3fv");
    if (glProgramUniformMatrix4x3fv) {++ret;}
    glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)get_gl_function("glProgramUniformMatrix2x3dv");
    if (glProgramUniformMatrix2x3dv) {++ret;}
    glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)get_gl_function("glProgramUniformMatrix3x2dv");
    if (glProgramUniformMatrix3x2dv) {++ret;}
    glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)get_gl_function("glProgramUniformMatrix2x4dv");
    if (glProgramUniformMatrix2x4dv) {++ret;}
    glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)get_gl_function("glProgramUniformMatrix4x2dv");
    if (glProgramUniformMatrix4x2dv) {++ret;}
    glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)get_gl_function("glProgramUniformMatrix3x4dv");
    if (glProgramUniformMatrix3x4dv) {++ret;}
    glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)get_gl_function("glProgramUniformMatrix4x3dv");
    if (glProgramUniformMatrix4x3dv) {++ret;}
    glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)get_gl_function("glValidateProgramPipeline");
    if (glValidateProgramPipeline) {++ret;}
    glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)get_gl_function("glGetProgramPipelineInfoLog");
    if (glGetProgramPipelineInfoLog) {++ret;}
    glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)get_gl_function("glVertexAttribL1d");
    if (glVertexAttribL1d) {++ret;}
    glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)get_gl_function("glVertexAttribL2d");
    if (glVertexAttribL2d) {++ret;}
    glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)get_gl_function("glVertexAttribL3d");
    if (glVertexAttribL3d) {++ret;}
    glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)get_gl_function("glVertexAttribL4d");
    if (glVertexAttribL4d) {++ret;}
    glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)get_gl_function("glVertexAttribL1dv");
    if (glVertexAttribL1dv) {++ret;}
    glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)get_gl_function("glVertexAttribL2dv");
    if (glVertexAttribL2dv) {++ret;}
    glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)get_gl_function("glVertexAttribL3dv");
    if (glVertexAttribL3dv) {++ret;}
    glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)get_gl_function("glVertexAttribL4dv");
    if (glVertexAttribL4dv) {++ret;}
    glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)get_gl_function("glVertexAttribLPointer");
    if (glVertexAttribLPointer) {++ret;}
    glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)get_gl_function("glGetVertexAttribLdv");
    if (glGetVertexAttribLdv) {++ret;}
    glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)get_gl_function("glViewportArrayv");
    if (glViewportArrayv) {++ret;}
    glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)get_gl_function("glViewportIndexedf");
    if (glViewportIndexedf) {++ret;}
    glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)get_gl_function("glViewportIndexedfv");
    if (glViewportIndexedfv) {++ret;}
    glScissorArrayv = (PFNGLSCISSORARRAYVPROC)get_gl_function("glScissorArrayv");
    if (glScissorArrayv) {++ret;}
    glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)get_gl_function("glScissorIndexed");
    if (glScissorIndexed) {++ret;}
    glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)get_gl_function("glScissorIndexedv");
    if (glScissorIndexedv) {++ret;}
    glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)get_gl_function("glDepthRangeArrayv");
    if (glDepthRangeArrayv) {++ret;}
    glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)get_gl_function("glDepthRangeIndexed");
    if (glDepthRangeIndexed) {++ret;}
    glGetFloati_v = (PFNGLGETFLOATI_VPROC)get_gl_function("glGetFloati_v");
    if (glGetFloati_v) {++ret;}
    glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)get_gl_function("glGetDoublei_v");
    if (glGetDoublei_v) {++ret;}
    glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)get_gl_function("glDrawArraysInstancedBaseInstance");
    if (glDrawArraysInstancedBaseInstance) {++ret;}
    glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)get_gl_function("glDrawElementsInstancedBaseInstance");
    if (glDrawElementsInstancedBaseInstance) {++ret;}
    glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)get_gl_function("glDrawElementsInstancedBaseVertexBaseInstance");
    if (glDrawElementsInstancedBaseVertexBaseInstance) {++ret;}
    glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)get_gl_function("glGetInternalformativ");
    if (glGetInternalformativ) {++ret;}
    glGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)get_gl_function("glGetActiveAtomicCounterBufferiv");
    if (glGetActiveAtomicCounterBufferiv) {++ret;}
    glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)get_gl_function("glBindImageTexture");
    if (glBindImageTexture) {++ret;}
    glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)get_gl_function("glMemoryBarrier");
    if (glMemoryBarrier) {++ret;}
    glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)get_gl_function("glTexStorage1D");
    if (glTexStorage1D) {++ret;}
    glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)get_gl_function("glTexStorage2D");
    if (glTexStorage2D) {++ret;}
    glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)get_gl_function("glTexStorage3D");
    if (glTexStorage3D) {++ret;}
    glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)get_gl_function("glDrawTransformFeedbackInstanced");
    if (glDrawTransformFeedbackInstanced) {++ret;}
    glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)get_gl_function("glDrawTransformFeedbackStreamInstanced");
    if (glDrawTransformFeedbackStreamInstanced) {++ret;}
    glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)get_gl_function("glClearBufferData");
    if (glClearBufferData) {++ret;}
    glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)get_gl_function("glClearBufferSubData");
    if (glClearBufferSubData) {++ret;}
    glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)get_gl_function("glDispatchCompute");
    if (glDispatchCompute) {++ret;}
    glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)get_gl_function("glDispatchComputeIndirect");
    if (glDispatchComputeIndirect) {++ret;}
    glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)get_gl_function("glCopyImageSubData");
    if (glCopyImageSubData) {++ret;}
    glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)get_gl_function("glFramebufferParameteri");
    if (glFramebufferParameteri) {++ret;}
    glGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)get_gl_function("glGetFramebufferParameteriv");
    if (glGetFramebufferParameteriv) {++ret;}
    glGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC)get_gl_function("glGetInternalformati64v");
    if (glGetInternalformati64v) {++ret;}
    glInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC)get_gl_function("glInvalidateTexSubImage");
    if (glInvalidateTexSubImage) {++ret;}
    glInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC)get_gl_function("glInvalidateTexImage");
    if (glInvalidateTexImage) {++ret;}
    glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)get_gl_function("glInvalidateBufferSubData");
    if (glInvalidateBufferSubData) {++ret;}
    glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)get_gl_function("glInvalidateBufferData");
    if (glInvalidateBufferData) {++ret;}
    glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)get_gl_function("glInvalidateFramebuffer");
    if (glInvalidateFramebuffer) {++ret;}
    glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)get_gl_function("glInvalidateSubFramebuffer");
    if (glInvalidateSubFramebuffer) {++ret;}
    glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)get_gl_function("glMultiDrawArraysIndirect");
    if (glMultiDrawArraysIndirect) {++ret;}
    glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)get_gl_function("glMultiDrawElementsIndirect");
    if (glMultiDrawElementsIndirect) {++ret;}
    glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)get_gl_function("glGetProgramInterfaceiv");
    if (glGetProgramInterfaceiv) {++ret;}
    glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)get_gl_function("glGetProgramResourceIndex");
    if (glGetProgramResourceIndex) {++ret;}
    glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)get_gl_function("glGetProgramResourceName");
    if (glGetProgramResourceName) {++ret;}
    glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)get_gl_function("glGetProgramResourceiv");
    if (glGetProgramResourceiv) {++ret;}
    glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)get_gl_function("glGetProgramResourceLocation");
    if (glGetProgramResourceLocation) {++ret;}
    glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)get_gl_function("glGetProgramResourceLocationIndex");
    if (glGetProgramResourceLocationIndex) {++ret;}
    glShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)get_gl_function("glShaderStorageBlockBinding");
    if (glShaderStorageBlockBinding) {++ret;}
    glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC)get_gl_function("glTexBufferRange");
    if (glTexBufferRange) {++ret;}
    glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)get_gl_function("glTexStorage2DMultisample");
    if (glTexStorage2DMultisample) {++ret;}
    glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)get_gl_function("glTexStorage3DMultisample");
    if (glTexStorage3DMultisample) {++ret;}
    glTextureView = (PFNGLTEXTUREVIEWPROC)get_gl_function("glTextureView");
    if (glTextureView) {++ret;}
    glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)get_gl_function("glBindVertexBuffer");
    if (glBindVertexBuffer) {++ret;}
    glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)get_gl_function("glVertexAttribFormat");
    if (glVertexAttribFormat) {++ret;}
    glVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)get_gl_function("glVertexAttribIFormat");
    if (glVertexAttribIFormat) {++ret;}
    glVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC)get_gl_function("glVertexAttribLFormat");
    if (glVertexAttribLFormat) {++ret;}
    glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)get_gl_function("glVertexAttribBinding");
    if (glVertexAttribBinding) {++ret;}
    glVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC)get_gl_function("glVertexBindingDivisor");
    if (glVertexBindingDivisor) {++ret;}
    glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)get_gl_function("glDebugMessageControl");
    if (glDebugMessageControl) {++ret;}
    glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)get_gl_function("glDebugMessageInsert");
    if (glDebugMessageInsert) {++ret;}
    glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)get_gl_function("glDebugMessageCallback");
    if (glDebugMessageCallback) {++ret;}
    glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)get_gl_function("glGetDebugMessageLog");
    if (glGetDebugMessageLog) {++ret;}
    glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)get_gl_function("glPushDebugGroup");
    if (glPushDebugGroup) {++ret;}
    glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)get_gl_function("glPopDebugGroup");
    if (glPopDebugGroup) {++ret;}
    glObjectLabel = (PFNGLOBJECTLABELPROC)get_gl_function("glObjectLabel");
    if (glObjectLabel) {++ret;}
    glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)get_gl_function("glGetObjectLabel");
    if (glGetObjectLabel) {++ret;}
    glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)get_gl_function("glObjectPtrLabel");
    if (glObjectPtrLabel) {++ret;}
    glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)get_gl_function("glGetObjectPtrLabel");
    if (glGetObjectPtrLabel) {++ret;}
    glBufferStorage = (PFNGLBUFFERSTORAGEPROC)get_gl_function("glBufferStorage");
    if (glBufferStorage) {++ret;}
    glClearTexImage = (PFNGLCLEARTEXIMAGEPROC)get_gl_function("glClearTexImage");
    if (glClearTexImage) {++ret;}
    glClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC)get_gl_function("glClearTexSubImage");
    if (glClearTexSubImage) {++ret;}
    glBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)get_gl_function("glBindBuffersBase");
    if (glBindBuffersBase) {++ret;}
    glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)get_gl_function("glBindBuffersRange");
    if (glBindBuffersRange) {++ret;}
    glBindTextures = (PFNGLBINDTEXTURESPROC)get_gl_function("glBindTextures");
    if (glBindTextures) {++ret;}
    glBindSamplers = (PFNGLBINDSAMPLERSPROC)get_gl_function("glBindSamplers");
    if (glBindSamplers) {++ret;}
    glBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC)get_gl_function("glBindImageTextures");
    if (glBindImageTextures) {++ret;}
    glBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC)get_gl_function("glBindVertexBuffers");
    if (glBindVertexBuffers) {++ret;}
    glClipControl = (PFNGLCLIPCONTROLPROC)get_gl_function("glClipControl");
    if (glClipControl) {++ret;}
    glCreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKSPROC)get_gl_function("glCreateTransformFeedbacks");
    if (glCreateTransformFeedbacks) {++ret;}
    glTransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC)get_gl_function("glTransformFeedbackBufferBase");
    if (glTransformFeedbackBufferBase) {++ret;}
    glTransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC)get_gl_function("glTransformFeedbackBufferRange");
    if (glTransformFeedbackBufferRange) {++ret;}
    glGetTransformFeedbackiv = (PFNGLGETTRANSFORMFEEDBACKIVPROC)get_gl_function("glGetTransformFeedbackiv");
    if (glGetTransformFeedbackiv) {++ret;}
    glGetTransformFeedbacki_v = (PFNGLGETTRANSFORMFEEDBACKI_VPROC)get_gl_function("glGetTransformFeedbacki_v");
    if (glGetTransformFeedbacki_v) {++ret;}
    glGetTransformFeedbacki64_v = (PFNGLGETTRANSFORMFEEDBACKI64_VPROC)get_gl_function("glGetTransformFeedbacki64_v");
    if (glGetTransformFeedbacki64_v) {++ret;}
    glCreateBuffers = (PFNGLCREATEBUFFERSPROC)get_gl_function("glCreateBuffers");
    if (glCreateBuffers) {++ret;}
    glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)get_gl_function("glNamedBufferStorage");
    if (glNamedBufferStorage) {++ret;}
    glNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC)get_gl_function("glNamedBufferData");
    if (glNamedBufferData) {++ret;}
    glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)get_gl_function("glNamedBufferSubData");
    if (glNamedBufferSubData) {++ret;}
    glCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)get_gl_function("glCopyNamedBufferSubData");
    if (glCopyNamedBufferSubData) {++ret;}
    glClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC)get_gl_function("glClearNamedBufferData");
    if (glClearNamedBufferData) {++ret;}
    glClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)get_gl_function("glClearNamedBufferSubData");
    if (glClearNamedBufferSubData) {++ret;}
    glMapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC)get_gl_function("glMapNamedBuffer");
    if (glMapNamedBuffer) {++ret;}
    glMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC)get_gl_function("glMapNamedBufferRange");
    if (glMapNamedBufferRange) {++ret;}
    glUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC)get_gl_function("glUnmapNamedBuffer");
    if (glUnmapNamedBuffer) {++ret;}
    glFlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC)get_gl_function("glFlushMappedNamedBufferRange");
    if (glFlushMappedNamedBufferRange) {++ret;}
    glGetNamedBufferParameteriv = (PFNGLGETNAMEDBUFFERPARAMETERIVPROC)get_gl_function("glGetNamedBufferParameteriv");
    if (glGetNamedBufferParameteriv) {++ret;}
    glGetNamedBufferParameteri64v = (PFNGLGETNAMEDBUFFERPARAMETERI64VPROC)get_gl_function("glGetNamedBufferParameteri64v");
    if (glGetNamedBufferParameteri64v) {++ret;}
    glGetNamedBufferPointerv = (PFNGLGETNAMEDBUFFERPOINTERVPROC)get_gl_function("glGetNamedBufferPointerv");
    if (glGetNamedBufferPointerv) {++ret;}
    glGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC)get_gl_function("glGetNamedBufferSubData");
    if (glGetNamedBufferSubData) {++ret;}
    glCreateFramebuffers = (PFNGLCREATEFRAMEBUFFERSPROC)get_gl_function("glCreateFramebuffers");
    if (glCreateFramebuffers) {++ret;}
    glNamedFramebufferRenderbuffer = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC)get_gl_function("glNamedFramebufferRenderbuffer");
    if (glNamedFramebufferRenderbuffer) {++ret;}
    glNamedFramebufferParameteri = (PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC)get_gl_function("glNamedFramebufferParameteri");
    if (glNamedFramebufferParameteri) {++ret;}
    glNamedFramebufferTexture = (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC)get_gl_function("glNamedFramebufferTexture");
    if (glNamedFramebufferTexture) {++ret;}
    glNamedFramebufferTextureLayer = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC)get_gl_function("glNamedFramebufferTextureLayer");
    if (glNamedFramebufferTextureLayer) {++ret;}
    glNamedFramebufferDrawBuffer = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC)get_gl_function("glNamedFramebufferDrawBuffer");
    if (glNamedFramebufferDrawBuffer) {++ret;}
    glNamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC)get_gl_function("glNamedFramebufferDrawBuffers");
    if (glNamedFramebufferDrawBuffers) {++ret;}
    glNamedFramebufferReadBuffer = (PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC)get_gl_function("glNamedFramebufferReadBuffer");
    if (glNamedFramebufferReadBuffer) {++ret;}
    glInvalidateNamedFramebufferData = (PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC)get_gl_function("glInvalidateNamedFramebufferData");
    if (glInvalidateNamedFramebufferData) {++ret;}
    glInvalidateNamedFramebufferSubData = (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC)get_gl_function("glInvalidateNamedFramebufferSubData");
    if (glInvalidateNamedFramebufferSubData) {++ret;}
    glClearNamedFramebufferiv = (PFNGLCLEARNAMEDFRAMEBUFFERIVPROC)get_gl_function("glClearNamedFramebufferiv");
    if (glClearNamedFramebufferiv) {++ret;}
    glClearNamedFramebufferuiv = (PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC)get_gl_function("glClearNamedFramebufferuiv");
    if (glClearNamedFramebufferuiv) {++ret;}
    glClearNamedFramebufferfv = (PFNGLCLEARNAMEDFRAMEBUFFERFVPROC)get_gl_function("glClearNamedFramebufferfv");
    if (glClearNamedFramebufferfv) {++ret;}
    glClearNamedFramebufferfi = (PFNGLCLEARNAMEDFRAMEBUFFERFIPROC)get_gl_function("glClearNamedFramebufferfi");
    if (glClearNamedFramebufferfi) {++ret;}
    glBlitNamedFramebuffer = (PFNGLBLITNAMEDFRAMEBUFFERPROC)get_gl_function("glBlitNamedFramebuffer");
    if (glBlitNamedFramebuffer) {++ret;}
    glCheckNamedFramebufferStatus = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC)get_gl_function("glCheckNamedFramebufferStatus");
    if (glCheckNamedFramebufferStatus) {++ret;}
    glGetNamedFramebufferParameteriv = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC)get_gl_function("glGetNamedFramebufferParameteriv");
    if (glGetNamedFramebufferParameteriv) {++ret;}
    glGetNamedFramebufferAttachmentParameteriv = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC)get_gl_function("glGetNamedFramebufferAttachmentParameteriv");
    if (glGetNamedFramebufferAttachmentParameteriv) {++ret;}
    glCreateRenderbuffers = (PFNGLCREATERENDERBUFFERSPROC)get_gl_function("glCreateRenderbuffers");
    if (glCreateRenderbuffers) {++ret;}
    glNamedRenderbufferStorage = (PFNGLNAMEDRENDERBUFFERSTORAGEPROC)get_gl_function("glNamedRenderbufferStorage");
    if (glNamedRenderbufferStorage) {++ret;}
    glNamedRenderbufferStorageMultisample = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC)get_gl_function("glNamedRenderbufferStorageMultisample");
    if (glNamedRenderbufferStorageMultisample) {++ret;}
    glGetNamedRenderbufferParameteriv = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC)get_gl_function("glGetNamedRenderbufferParameteriv");
    if (glGetNamedRenderbufferParameteriv) {++ret;}
    glCreateTextures = (PFNGLCREATETEXTURESPROC)get_gl_function("glCreateTextures");
    if (glCreateTextures) {++ret;}
    glTextureBuffer = (PFNGLTEXTUREBUFFERPROC)get_gl_function("glTextureBuffer");
    if (glTextureBuffer) {++ret;}
    glTextureBufferRange = (PFNGLTEXTUREBUFFERRANGEPROC)get_gl_function("glTextureBufferRange");
    if (glTextureBufferRange) {++ret;}
    glTextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC)get_gl_function("glTextureStorage1D");
    if (glTextureStorage1D) {++ret;}
    glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)get_gl_function("glTextureStorage2D");
    if (glTextureStorage2D) {++ret;}
    glTextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC)get_gl_function("glTextureStorage3D");
    if (glTextureStorage3D) {++ret;}
    glTextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC)get_gl_function("glTextureStorage2DMultisample");
    if (glTextureStorage2DMultisample) {++ret;}
    glTextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC)get_gl_function("glTextureStorage3DMultisample");
    if (glTextureStorage3DMultisample) {++ret;}
    glTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC)get_gl_function("glTextureSubImage1D");
    if (glTextureSubImage1D) {++ret;}
    glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)get_gl_function("glTextureSubImage2D");
    if (glTextureSubImage2D) {++ret;}
    glTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC)get_gl_function("glTextureSubImage3D");
    if (glTextureSubImage3D) {++ret;}
    glCompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC)get_gl_function("glCompressedTextureSubImage1D");
    if (glCompressedTextureSubImage1D) {++ret;}
    glCompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC)get_gl_function("glCompressedTextureSubImage2D");
    if (glCompressedTextureSubImage2D) {++ret;}
    glCompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC)get_gl_function("glCompressedTextureSubImage3D");
    if (glCompressedTextureSubImage3D) {++ret;}
    glCopyTextureSubImage1D = (PFNGLCOPYTEXTURESUBIMAGE1DPROC)get_gl_function("glCopyTextureSubImage1D");
    if (glCopyTextureSubImage1D) {++ret;}
    glCopyTextureSubImage2D = (PFNGLCOPYTEXTURESUBIMAGE2DPROC)get_gl_function("glCopyTextureSubImage2D");
    if (glCopyTextureSubImage2D) {++ret;}
    glCopyTextureSubImage3D = (PFNGLCOPYTEXTURESUBIMAGE3DPROC)get_gl_function("glCopyTextureSubImage3D");
    if (glCopyTextureSubImage3D) {++ret;}
    glTextureParameterf = (PFNGLTEXTUREPARAMETERFPROC)get_gl_function("glTextureParameterf");
    if (glTextureParameterf) {++ret;}
    glTextureParameterfv = (PFNGLTEXTUREPARAMETERFVPROC)get_gl_function("glTextureParameterfv");
    if (glTextureParameterfv) {++ret;}
    glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)get_gl_function("glTextureParameteri");
    if (glTextureParameteri) {++ret;}
    glTextureParameterIiv = (PFNGLTEXTUREPARAMETERIIVPROC)get_gl_function("glTextureParameterIiv");
    if (glTextureParameterIiv) {++ret;}
    glTextureParameterIuiv = (PFNGLTEXTUREPARAMETERIUIVPROC)get_gl_function("glTextureParameterIuiv");
    if (glTextureParameterIuiv) {++ret;}
    glTextureParameteriv = (PFNGLTEXTUREPARAMETERIVPROC)get_gl_function("glTextureParameteriv");
    if (glTextureParameteriv) {++ret;}
    glGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC)get_gl_function("glGenerateTextureMipmap");
    if (glGenerateTextureMipmap) {++ret;}
    glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC)get_gl_function("glBindTextureUnit");
    if (glBindTextureUnit) {++ret;}
    glGetTextureImage = (PFNGLGETTEXTUREIMAGEPROC)get_gl_function("glGetTextureImage");
    if (glGetTextureImage) {++ret;}
    glGetCompressedTextureImage = (PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC)get_gl_function("glGetCompressedTextureImage");
    if (glGetCompressedTextureImage) {++ret;}
    glGetTextureLevelParameterfv = (PFNGLGETTEXTURELEVELPARAMETERFVPROC)get_gl_function("glGetTextureLevelParameterfv");
    if (glGetTextureLevelParameterfv) {++ret;}
    glGetTextureLevelParameteriv = (PFNGLGETTEXTURELEVELPARAMETERIVPROC)get_gl_function("glGetTextureLevelParameteriv");
    if (glGetTextureLevelParameteriv) {++ret;}
    glGetTextureParameterfv = (PFNGLGETTEXTUREPARAMETERFVPROC)get_gl_function("glGetTextureParameterfv");
    if (glGetTextureParameterfv) {++ret;}
    glGetTextureParameterIiv = (PFNGLGETTEXTUREPARAMETERIIVPROC)get_gl_function("glGetTextureParameterIiv");
    if (glGetTextureParameterIiv) {++ret;}
    glGetTextureParameterIuiv = (PFNGLGETTEXTUREPARAMETERIUIVPROC)get_gl_function("glGetTextureParameterIuiv");
    if (glGetTextureParameterIuiv) {++ret;}
    glGetTextureParameteriv = (PFNGLGETTEXTUREPARAMETERIVPROC)get_gl_function("glGetTextureParameteriv");
    if (glGetTextureParameteriv) {++ret;}
    glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)get_gl_function("glCreateVertexArrays");
    if (glCreateVertexArrays) {++ret;}
    glDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC)get_gl_function("glDisableVertexArrayAttrib");
    if (glDisableVertexArrayAttrib) {++ret;}
    glEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC)get_gl_function("glEnableVertexArrayAttrib");
    if (glEnableVertexArrayAttrib) {++ret;}
    glVertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFERPROC)get_gl_function("glVertexArrayElementBuffer");
    if (glVertexArrayElementBuffer) {++ret;}
    glVertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)get_gl_function("glVertexArrayVertexBuffer");
    if (glVertexArrayVertexBuffer) {++ret;}
    glVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC)get_gl_function("glVertexArrayVertexBuffers");
    if (glVertexArrayVertexBuffers) {++ret;}
    glVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)get_gl_function("glVertexArrayAttribBinding");
    if (glVertexArrayAttribBinding) {++ret;}
    glVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC)get_gl_function("glVertexArrayAttribFormat");
    if (glVertexArrayAttribFormat) {++ret;}
    glVertexArrayAttribIFormat = (PFNGLVERTEXARRAYATTRIBIFORMATPROC)get_gl_function("glVertexArrayAttribIFormat");
    if (glVertexArrayAttribIFormat) {++ret;}
    glVertexArrayAttribLFormat = (PFNGLVERTEXARRAYATTRIBLFORMATPROC)get_gl_function("glVertexArrayAttribLFormat");
    if (glVertexArrayAttribLFormat) {++ret;}
    glVertexArrayBindingDivisor = (PFNGLVERTEXARRAYBINDINGDIVISORPROC)get_gl_function("glVertexArrayBindingDivisor");
    if (glVertexArrayBindingDivisor) {++ret;}
    glGetVertexArrayiv = (PFNGLGETVERTEXARRAYIVPROC)get_gl_function("glGetVertexArrayiv");
    if (glGetVertexArrayiv) {++ret;}
    glGetVertexArrayIndexediv = (PFNGLGETVERTEXARRAYINDEXEDIVPROC)get_gl_function("glGetVertexArrayIndexediv");
    if (glGetVertexArrayIndexediv) {++ret;}
    glGetVertexArrayIndexed64iv = (PFNGLGETVERTEXARRAYINDEXED64IVPROC)get_gl_function("glGetVertexArrayIndexed64iv");
    if (glGetVertexArrayIndexed64iv) {++ret;}
    glCreateSamplers = (PFNGLCREATESAMPLERSPROC)get_gl_function("glCreateSamplers");
    if (glCreateSamplers) {++ret;}
    glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)get_gl_function("glCreateProgramPipelines");
    if (glCreateProgramPipelines) {++ret;}
    glCreateQueries = (PFNGLCREATEQUERIESPROC)get_gl_function("glCreateQueries");
    if (glCreateQueries) {++ret;}
    glGetQueryBufferObjecti64v = (PFNGLGETQUERYBUFFEROBJECTI64VPROC)get_gl_function("glGetQueryBufferObjecti64v");
    if (glGetQueryBufferObjecti64v) {++ret;}
    glGetQueryBufferObjectiv = (PFNGLGETQUERYBUFFEROBJECTIVPROC)get_gl_function("glGetQueryBufferObjectiv");
    if (glGetQueryBufferObjectiv) {++ret;}
    glGetQueryBufferObjectui64v = (PFNGLGETQUERYBUFFEROBJECTUI64VPROC)get_gl_function("glGetQueryBufferObjectui64v");
    if (glGetQueryBufferObjectui64v) {++ret;}
    glGetQueryBufferObjectuiv = (PFNGLGETQUERYBUFFEROBJECTUIVPROC)get_gl_function("glGetQueryBufferObjectuiv");
    if (glGetQueryBufferObjectuiv) {++ret;}
    glMemoryBarrierByRegion = (PFNGLMEMORYBARRIERBYREGIONPROC)get_gl_function("glMemoryBarrierByRegion");
    if (glMemoryBarrierByRegion) {++ret;}
    glGetTextureSubImage = (PFNGLGETTEXTURESUBIMAGEPROC)get_gl_function("glGetTextureSubImage");
    if (glGetTextureSubImage) {++ret;}
    glGetCompressedTextureSubImage = (PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC)get_gl_function("glGetCompressedTextureSubImage");
    if (glGetCompressedTextureSubImage) {++ret;}
    glGetGraphicsResetStatus = (PFNGLGETGRAPHICSRESETSTATUSPROC)get_gl_function("glGetGraphicsResetStatus");
    if (glGetGraphicsResetStatus) {++ret;}
    glGetnCompressedTexImage = (PFNGLGETNCOMPRESSEDTEXIMAGEPROC)get_gl_function("glGetnCompressedTexImage");
    if (glGetnCompressedTexImage) {++ret;}
    glGetnTexImage = (PFNGLGETNTEXIMAGEPROC)get_gl_function("glGetnTexImage");
    if (glGetnTexImage) {++ret;}
    glGetnUniformdv = (PFNGLGETNUNIFORMDVPROC)get_gl_function("glGetnUniformdv");
    if (glGetnUniformdv) {++ret;}
    glGetnUniformfv = (PFNGLGETNUNIFORMFVPROC)get_gl_function("glGetnUniformfv");
    if (glGetnUniformfv) {++ret;}
    glGetnUniformiv = (PFNGLGETNUNIFORMIVPROC)get_gl_function("glGetnUniformiv");
    if (glGetnUniformiv) {++ret;}
    glGetnUniformuiv = (PFNGLGETNUNIFORMUIVPROC)get_gl_function("glGetnUniformuiv");
    if (glGetnUniformuiv) {++ret;}
    glReadnPixels = (PFNGLREADNPIXELSPROC)get_gl_function("glReadnPixels");
    if (glReadnPixels) {++ret;}
    glGetnMapdv = (PFNGLGETNMAPDVPROC)get_gl_function("glGetnMapdv");
    if (glGetnMapdv) {++ret;}
    glGetnMapfv = (PFNGLGETNMAPFVPROC)get_gl_function("glGetnMapfv");
    if (glGetnMapfv) {++ret;}
    glGetnMapiv = (PFNGLGETNMAPIVPROC)get_gl_function("glGetnMapiv");
    if (glGetnMapiv) {++ret;}
    glGetnPixelMapfv = (PFNGLGETNPIXELMAPFVPROC)get_gl_function("glGetnPixelMapfv");
    if (glGetnPixelMapfv) {++ret;}
    glGetnPixelMapuiv = (PFNGLGETNPIXELMAPUIVPROC)get_gl_function("glGetnPixelMapuiv");
    if (glGetnPixelMapuiv) {++ret;}
    glGetnPixelMapusv = (PFNGLGETNPIXELMAPUSVPROC)get_gl_function("glGetnPixelMapusv");
    if (glGetnPixelMapusv) {++ret;}
    glGetnPolygonStipple = (PFNGLGETNPOLYGONSTIPPLEPROC)get_gl_function("glGetnPolygonStipple");
    if (glGetnPolygonStipple) {++ret;}
    glGetnColorTable = (PFNGLGETNCOLORTABLEPROC)get_gl_function("glGetnColorTable");
    if (glGetnColorTable) {++ret;}
    glGetnConvolutionFilter = (PFNGLGETNCONVOLUTIONFILTERPROC)get_gl_function("glGetnConvolutionFilter");
    if (glGetnConvolutionFilter) {++ret;}
    glGetnSeparableFilter = (PFNGLGETNSEPARABLEFILTERPROC)get_gl_function("glGetnSeparableFilter");
    if (glGetnSeparableFilter) {++ret;}
    glGetnHistogram = (PFNGLGETNHISTOGRAMPROC)get_gl_function("glGetnHistogram");
    if (glGetnHistogram) {++ret;}
    glGetnMinmax = (PFNGLGETNMINMAXPROC)get_gl_function("glGetnMinmax");
    if (glGetnMinmax) {++ret;}
    glTextureBarrier = (PFNGLTEXTUREBARRIERPROC)get_gl_function("glTextureBarrier");
    if (glTextureBarrier) {++ret;}
    glColorTable = (PFNGLCOLORTABLEPROC)get_gl_function("glColorTable");
    if (glColorTable) {++ret;}
    glColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFVPROC)get_gl_function("glColorTableParameterfv");
    if (glColorTableParameterfv) {++ret;}
    glColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIVPROC)get_gl_function("glColorTableParameteriv");
    if (glColorTableParameteriv) {++ret;}
    glCopyColorTable = (PFNGLCOPYCOLORTABLEPROC)get_gl_function("glCopyColorTable");
    if (glCopyColorTable) {++ret;}
    glGetColorTable = (PFNGLGETCOLORTABLEPROC)get_gl_function("glGetColorTable");
    if (glGetColorTable) {++ret;}
    glGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFVPROC)get_gl_function("glGetColorTableParameterfv");
    if (glGetColorTableParameterfv) {++ret;}
    glGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIVPROC)get_gl_function("glGetColorTableParameteriv");
    if (glGetColorTableParameteriv) {++ret;}
    glColorSubTable = (PFNGLCOLORSUBTABLEPROC)get_gl_function("glColorSubTable");
    if (glColorSubTable) {++ret;}
    glCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLEPROC)get_gl_function("glCopyColorSubTable");
    if (glCopyColorSubTable) {++ret;}
    glConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1DPROC)get_gl_function("glConvolutionFilter1D");
    if (glConvolutionFilter1D) {++ret;}
    glConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2DPROC)get_gl_function("glConvolutionFilter2D");
    if (glConvolutionFilter2D) {++ret;}
    glConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERFPROC)get_gl_function("glConvolutionParameterf");
    if (glConvolutionParameterf) {++ret;}
    glConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFVPROC)get_gl_function("glConvolutionParameterfv");
    if (glConvolutionParameterfv) {++ret;}
    glConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERIPROC)get_gl_function("glConvolutionParameteri");
    if (glConvolutionParameteri) {++ret;}
    glConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIVPROC)get_gl_function("glConvolutionParameteriv");
    if (glConvolutionParameteriv) {++ret;}
    glCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1DPROC)get_gl_function("glCopyConvolutionFilter1D");
    if (glCopyConvolutionFilter1D) {++ret;}
    glCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2DPROC)get_gl_function("glCopyConvolutionFilter2D");
    if (glCopyConvolutionFilter2D) {++ret;}
    glGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTERPROC)get_gl_function("glGetConvolutionFilter");
    if (glGetConvolutionFilter) {++ret;}
    glGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFVPROC)get_gl_function("glGetConvolutionParameterfv");
    if (glGetConvolutionParameterfv) {++ret;}
    glGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIVPROC)get_gl_function("glGetConvolutionParameteriv");
    if (glGetConvolutionParameteriv) {++ret;}
    glGetSeparableFilter = (PFNGLGETSEPARABLEFILTERPROC)get_gl_function("glGetSeparableFilter");
    if (glGetSeparableFilter) {++ret;}
    glSeparableFilter2D = (PFNGLSEPARABLEFILTER2DPROC)get_gl_function("glSeparableFilter2D");
    if (glSeparableFilter2D) {++ret;}
    glGetHistogram = (PFNGLGETHISTOGRAMPROC)get_gl_function("glGetHistogram");
    if (glGetHistogram) {++ret;}
    glGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFVPROC)get_gl_function("glGetHistogramParameterfv");
    if (glGetHistogramParameterfv) {++ret;}
    glGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIVPROC)get_gl_function("glGetHistogramParameteriv");
    if (glGetHistogramParameteriv) {++ret;}
    glGetMinmax = (PFNGLGETMINMAXPROC)get_gl_function("glGetMinmax");
    if (glGetMinmax) {++ret;}
    glGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFVPROC)get_gl_function("glGetMinmaxParameterfv");
    if (glGetMinmaxParameterfv) {++ret;}
    glGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIVPROC)get_gl_function("glGetMinmaxParameteriv");
    if (glGetMinmaxParameteriv) {++ret;}
    glHistogram = (PFNGLHISTOGRAMPROC)get_gl_function("glHistogram");
    if (glHistogram) {++ret;}
    glMinmax = (PFNGLMINMAXPROC)get_gl_function("glMinmax");
    if (glMinmax) {++ret;}
    glResetHistogram = (PFNGLRESETHISTOGRAMPROC)get_gl_function("glResetHistogram");
    if (glResetHistogram) {++ret;}
    glResetMinmax = (PFNGLRESETMINMAXPROC)get_gl_function("glResetMinmax");
    if (glResetMinmax) {++ret;}
    glGetPixelMapxv = (PFNGLGETPIXELMAPXVPROC)get_gl_function("glGetPixelMapxv");
    if (glGetPixelMapxv) {++ret;}
    glPixelMapx = (PFNGLPIXELMAPXPROC)get_gl_function("glPixelMapx");
    if (glPixelMapx) {++ret;}
    glPixelStorex = (PFNGLPIXELSTOREXPROC)get_gl_function("glPixelStorex");
    if (glPixelStorex) {++ret;}
    
    return ret;
}

/*-------------------------------------
 * OpenGL Function Declarations (static)
-------------------------------------*/
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
PFNGLTEXIMAGE3DPROC glTexImage3D;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d;
PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv;
PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f;
PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv;
PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i;
PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv;
PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s;
PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv;
PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;
PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i;
PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv;
PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s;
PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv;
PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d;
PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv;
PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f;
PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv;
PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i;
PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv;
PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s;
PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv;
PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d;
PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv;
PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f;
PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv;
PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i;
PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv;
PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s;
PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv;
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
PFNGLPOINTPARAMETERFPROC glPointParameterf;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
PFNGLPOINTPARAMETERIPROC glPointParameteri;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv;
PFNGLFOGCOORDFPROC glFogCoordf;
PFNGLFOGCOORDFVPROC glFogCoordfv;
PFNGLFOGCOORDDPROC glFogCoordd;
PFNGLFOGCOORDDVPROC glFogCoorddv;
PFNGLFOGCOORDPOINTERPROC glFogCoordPointer;
PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv;
PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv;
PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv;
PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv;
PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv;
PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
PFNGLWINDOWPOS2DPROC glWindowPos2d;
PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
PFNGLWINDOWPOS2FPROC glWindowPos2f;
PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
PFNGLWINDOWPOS2IPROC glWindowPos2i;
PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
PFNGLWINDOWPOS2SPROC glWindowPos2s;
PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
PFNGLWINDOWPOS3DPROC glWindowPos3d;
PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
PFNGLWINDOWPOS3FPROC glWindowPos3f;
PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
PFNGLWINDOWPOS3IPROC glWindowPos3i;
PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
PFNGLWINDOWPOS3SPROC glWindowPos3s;
PFNGLWINDOWPOS3SVPROC glWindowPos3sv;
PFNGLBLENDCOLORPROC glBlendColor;
PFNGLBLENDEQUATIONPROC glBlendEquation;
PFNGLGENQUERIESPROC glGenQueries;
PFNGLDELETEQUERIESPROC glDeleteQueries;
PFNGLISQUERYPROC glIsQuery;
PFNGLBEGINQUERYPROC glBeginQuery;
PFNGLENDQUERYPROC glEndQuery;
PFNGLGETQUERYIVPROC glGetQueryiv;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLISBUFFERPROC glIsBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
PFNGLMAPBUFFERPROC glMapBuffer;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERSOURCEPROC glGetShaderSource;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETUNIFORMFVPROC glGetUniformfv;
PFNGLGETUNIFORMIVPROC glGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLISSHADERPROC glIsShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLUNIFORM1FVPROC glUniform1fv;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORM1IVPROC glUniform1iv;
PFNGLUNIFORM2IVPROC glUniform2iv;
PFNGLUNIFORM3IVPROC glUniform3iv;
PFNGLUNIFORM4IVPROC glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
PFNGLCOLORMASKIPROC glColorMaski;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
PFNGLENABLEIPROC glEnablei;
PFNGLDISABLEIPROC glDisablei;
PFNGLISENABLEDIPROC glIsEnabledi;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
PFNGLCLAMPCOLORPROC glClampColor;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
PFNGLUNIFORM1UIPROC glUniform1ui;
PFNGLUNIFORM2UIPROC glUniform2ui;
PFNGLUNIFORM3UIPROC glUniform3ui;
PFNGLUNIFORM4UIPROC glUniform4ui;
PFNGLUNIFORM1UIVPROC glUniform1uiv;
PFNGLUNIFORM2UIVPROC glUniform2uiv;
PFNGLUNIFORM3UIVPROC glUniform3uiv;
PFNGLUNIFORM4UIVPROC glUniform4uiv;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv;
PFNGLCLEARBUFFERIVPROC glClearBufferiv;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
PFNGLCLEARBUFFERFVPROC glClearBufferfv;
PFNGLCLEARBUFFERFIPROC glClearBufferfi;
PFNGLGETSTRINGIPROC glGetStringi;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLISVERTEXARRAYPROC glIsVertexArray;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLTEXBUFFERPROC glTexBuffer;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;
PFNGLPROVOKINGVERTEXPROC glProvokingVertex;
PFNGLFENCESYNCPROC glFenceSync;
PFNGLISSYNCPROC glIsSync;
PFNGLDELETESYNCPROC glDeleteSync;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
PFNGLWAITSYNCPROC glWaitSync;
PFNGLGETINTEGER64VPROC glGetInteger64v;
PFNGLGETSYNCIVPROC glGetSynciv;
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample;
PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv;
PFNGLSAMPLEMASKIPROC glSampleMaski;
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex;
PFNGLGENSAMPLERSPROC glGenSamplers;
PFNGLDELETESAMPLERSPROC glDeleteSamplers;
PFNGLISSAMPLERPROC glIsSampler;
PFNGLBINDSAMPLERPROC glBindSampler;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv;
PFNGLQUERYCOUNTERPROC glQueryCounter;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv;
PFNGLVERTEXP2UIPROC glVertexP2ui;
PFNGLVERTEXP2UIVPROC glVertexP2uiv;
PFNGLVERTEXP3UIPROC glVertexP3ui;
PFNGLVERTEXP3UIVPROC glVertexP3uiv;
PFNGLVERTEXP4UIPROC glVertexP4ui;
PFNGLVERTEXP4UIVPROC glVertexP4uiv;
PFNGLTEXCOORDP1UIPROC glTexCoordP1ui;
PFNGLTEXCOORDP1UIVPROC glTexCoordP1uiv;
PFNGLTEXCOORDP2UIPROC glTexCoordP2ui;
PFNGLTEXCOORDP2UIVPROC glTexCoordP2uiv;
PFNGLTEXCOORDP3UIPROC glTexCoordP3ui;
PFNGLTEXCOORDP3UIVPROC glTexCoordP3uiv;
PFNGLTEXCOORDP4UIPROC glTexCoordP4ui;
PFNGLTEXCOORDP4UIVPROC glTexCoordP4uiv;
PFNGLMULTITEXCOORDP1UIPROC glMultiTexCoordP1ui;
PFNGLMULTITEXCOORDP1UIVPROC glMultiTexCoordP1uiv;
PFNGLMULTITEXCOORDP2UIPROC glMultiTexCoordP2ui;
PFNGLMULTITEXCOORDP2UIVPROC glMultiTexCoordP2uiv;
PFNGLMULTITEXCOORDP3UIPROC glMultiTexCoordP3ui;
PFNGLMULTITEXCOORDP3UIVPROC glMultiTexCoordP3uiv;
PFNGLMULTITEXCOORDP4UIPROC glMultiTexCoordP4ui;
PFNGLMULTITEXCOORDP4UIVPROC glMultiTexCoordP4uiv;
PFNGLNORMALP3UIPROC glNormalP3ui;
PFNGLNORMALP3UIVPROC glNormalP3uiv;
PFNGLCOLORP3UIPROC glColorP3ui;
PFNGLCOLORP3UIVPROC glColorP3uiv;
PFNGLCOLORP4UIPROC glColorP4ui;
PFNGLCOLORP4UIVPROC glColorP4uiv;
PFNGLSECONDARYCOLORP3UIPROC glSecondaryColorP3ui;
PFNGLSECONDARYCOLORP3UIVPROC glSecondaryColorP3uiv;
PFNGLMINSAMPLESHADINGPROC glMinSampleShading;
PFNGLBLENDEQUATIONIPROC glBlendEquationi;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei;
PFNGLBLENDFUNCIPROC glBlendFunci;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei;
PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;
PFNGLUNIFORM1DPROC glUniform1d;
PFNGLUNIFORM2DPROC glUniform2d;
PFNGLUNIFORM3DPROC glUniform3d;
PFNGLUNIFORM4DPROC glUniform4d;
PFNGLUNIFORM1DVPROC glUniform1dv;
PFNGLUNIFORM2DVPROC glUniform2dv;
PFNGLUNIFORM3DVPROC glUniform3dv;
PFNGLUNIFORM4DVPROC glUniform4dv;
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
PFNGLGETUNIFORMDVPROC glGetUniformdv;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName;
PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv;
PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;
PFNGLPATCHPARAMETERIPROC glPatchParameteri;
PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed;
PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed;
PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv;
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
PFNGLSHADERBINARYPROC glShaderBinary;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
PFNGLDEPTHRANGEFPROC glDepthRangef;
PFNGLCLEARDEPTHFPROC glClearDepthf;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
PFNGLPROGRAMBINARYPROC glProgramBinary;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;
PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
PFNGLSCISSORARRAYVPROC glScissorArrayv;
PFNGLSCISSORINDEXEDPROC glScissorIndexed;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
PFNGLGETFLOATI_VPROC glGetFloati_v;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
PFNGLMEMORYBARRIERPROC glMemoryBarrier;
PFNGLTEXSTORAGE1DPROC glTexStorage1D;
PFNGLTEXSTORAGE2DPROC glTexStorage2D;
PFNGLTEXSTORAGE3DPROC glTexStorage3D;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;
PFNGLCLEARBUFFERDATAPROC glClearBufferData;
PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData;
PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect;
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData;
PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv;
PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v;
PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage;
PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage;
PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData;
PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;
PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv;
PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv;
PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding;
PFNGLTEXBUFFERRANGEPROC glTexBufferRange;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample;
PFNGLTEXTUREVIEWPROC glTextureView;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat;
PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat;
PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding;
PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup;
PFNGLOBJECTLABELPROC glObjectLabel;
PFNGLGETOBJECTLABELPROC glGetObjectLabel;
PFNGLOBJECTPTRLABELPROC glObjectPtrLabel;
PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel;
PFNGLBUFFERSTORAGEPROC glBufferStorage;
PFNGLCLEARTEXIMAGEPROC glClearTexImage;
PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage;
PFNGLBINDBUFFERSBASEPROC glBindBuffersBase;
PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange;
PFNGLBINDTEXTURESPROC glBindTextures;
PFNGLBINDSAMPLERSPROC glBindSamplers;
PFNGLBINDIMAGETEXTURESPROC glBindImageTextures;
PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers;
PFNGLCLIPCONTROLPROC glClipControl;
PFNGLCREATETRANSFORMFEEDBACKSPROC glCreateTransformFeedbacks;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glTransformFeedbackBufferBase;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glTransformFeedbackBufferRange;
PFNGLGETTRANSFORMFEEDBACKIVPROC glGetTransformFeedbackiv;
PFNGLGETTRANSFORMFEEDBACKI_VPROC glGetTransformFeedbacki_v;
PFNGLGETTRANSFORMFEEDBACKI64_VPROC glGetTransformFeedbacki64_v;
PFNGLCREATEBUFFERSPROC glCreateBuffers;
PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;
PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;
PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData;
PFNGLCLEARNAMEDBUFFERDATAPROC glClearNamedBufferData;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC glClearNamedBufferSubData;
PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer;
PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange;
PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange;
PFNGLGETNAMEDBUFFERPARAMETERIVPROC glGetNamedBufferParameteriv;
PFNGLGETNAMEDBUFFERPARAMETERI64VPROC glGetNamedBufferParameteri64v;
PFNGLGETNAMEDBUFFERPOINTERVPROC glGetNamedBufferPointerv;
PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData;
PFNGLCREATEFRAMEBUFFERSPROC glCreateFramebuffers;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC glNamedFramebufferRenderbuffer;
PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC glNamedFramebufferParameteri;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glNamedFramebufferTexture;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC glNamedFramebufferTextureLayer;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC glNamedFramebufferDrawBuffer;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC glNamedFramebufferDrawBuffers;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC glNamedFramebufferReadBuffer;
PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC glInvalidateNamedFramebufferData;
PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC glInvalidateNamedFramebufferSubData;
PFNGLCLEARNAMEDFRAMEBUFFERIVPROC glClearNamedFramebufferiv;
PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC glClearNamedFramebufferuiv;
PFNGLCLEARNAMEDFRAMEBUFFERFVPROC glClearNamedFramebufferfv;
PFNGLCLEARNAMEDFRAMEBUFFERFIPROC glClearNamedFramebufferfi;
PFNGLBLITNAMEDFRAMEBUFFERPROC glBlitNamedFramebuffer;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC glCheckNamedFramebufferStatus;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC glGetNamedFramebufferParameteriv;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetNamedFramebufferAttachmentParameteriv;
PFNGLCREATERENDERBUFFERSPROC glCreateRenderbuffers;
PFNGLNAMEDRENDERBUFFERSTORAGEPROC glNamedRenderbufferStorage;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC glNamedRenderbufferStorageMultisample;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC glGetNamedRenderbufferParameteriv;
PFNGLCREATETEXTURESPROC glCreateTextures;
PFNGLTEXTUREBUFFERPROC glTextureBuffer;
PFNGLTEXTUREBUFFERRANGEPROC glTextureBufferRange;
PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC glTextureStorage2DMultisample;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC glTextureStorage3DMultisample;
PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC glCompressedTextureSubImage1D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC glCompressedTextureSubImage2D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC glCompressedTextureSubImage3D;
PFNGLCOPYTEXTURESUBIMAGE1DPROC glCopyTextureSubImage1D;
PFNGLCOPYTEXTURESUBIMAGE2DPROC glCopyTextureSubImage2D;
PFNGLCOPYTEXTURESUBIMAGE3DPROC glCopyTextureSubImage3D;
PFNGLTEXTUREPARAMETERFPROC glTextureParameterf;
PFNGLTEXTUREPARAMETERFVPROC glTextureParameterfv;
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
PFNGLTEXTUREPARAMETERIIVPROC glTextureParameterIiv;
PFNGLTEXTUREPARAMETERIUIVPROC glTextureParameterIuiv;
PFNGLTEXTUREPARAMETERIVPROC glTextureParameteriv;
PFNGLGENERATETEXTUREMIPMAPPROC glGenerateTextureMipmap;
PFNGLBINDTEXTUREUNITPROC glBindTextureUnit;
PFNGLGETTEXTUREIMAGEPROC glGetTextureImage;
PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC glGetCompressedTextureImage;
PFNGLGETTEXTURELEVELPARAMETERFVPROC glGetTextureLevelParameterfv;
PFNGLGETTEXTURELEVELPARAMETERIVPROC glGetTextureLevelParameteriv;
PFNGLGETTEXTUREPARAMETERFVPROC glGetTextureParameterfv;
PFNGLGETTEXTUREPARAMETERIIVPROC glGetTextureParameterIiv;
PFNGLGETTEXTUREPARAMETERIUIVPROC glGetTextureParameterIuiv;
PFNGLGETTEXTUREPARAMETERIVPROC glGetTextureParameteriv;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib;
PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;
PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC glVertexArrayVertexBuffers;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat;
PFNGLVERTEXARRAYATTRIBIFORMATPROC glVertexArrayAttribIFormat;
PFNGLVERTEXARRAYATTRIBLFORMATPROC glVertexArrayAttribLFormat;
PFNGLVERTEXARRAYBINDINGDIVISORPROC glVertexArrayBindingDivisor;
PFNGLGETVERTEXARRAYIVPROC glGetVertexArrayiv;
PFNGLGETVERTEXARRAYINDEXEDIVPROC glGetVertexArrayIndexediv;
PFNGLGETVERTEXARRAYINDEXED64IVPROC glGetVertexArrayIndexed64iv;
PFNGLCREATESAMPLERSPROC glCreateSamplers;
PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines;
PFNGLCREATEQUERIESPROC glCreateQueries;
PFNGLGETQUERYBUFFEROBJECTI64VPROC glGetQueryBufferObjecti64v;
PFNGLGETQUERYBUFFEROBJECTIVPROC glGetQueryBufferObjectiv;
PFNGLGETQUERYBUFFEROBJECTUI64VPROC glGetQueryBufferObjectui64v;
PFNGLGETQUERYBUFFEROBJECTUIVPROC glGetQueryBufferObjectuiv;
PFNGLMEMORYBARRIERBYREGIONPROC glMemoryBarrierByRegion;
PFNGLGETTEXTURESUBIMAGEPROC glGetTextureSubImage;
PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC glGetCompressedTextureSubImage;
PFNGLGETGRAPHICSRESETSTATUSPROC glGetGraphicsResetStatus;
PFNGLGETNCOMPRESSEDTEXIMAGEPROC glGetnCompressedTexImage;
PFNGLGETNTEXIMAGEPROC glGetnTexImage;
PFNGLGETNUNIFORMDVPROC glGetnUniformdv;
PFNGLGETNUNIFORMFVPROC glGetnUniformfv;
PFNGLGETNUNIFORMIVPROC glGetnUniformiv;
PFNGLGETNUNIFORMUIVPROC glGetnUniformuiv;
PFNGLREADNPIXELSPROC glReadnPixels;
PFNGLGETNMAPDVPROC glGetnMapdv;
PFNGLGETNMAPFVPROC glGetnMapfv;
PFNGLGETNMAPIVPROC glGetnMapiv;
PFNGLGETNPIXELMAPFVPROC glGetnPixelMapfv;
PFNGLGETNPIXELMAPUIVPROC glGetnPixelMapuiv;
PFNGLGETNPIXELMAPUSVPROC glGetnPixelMapusv;
PFNGLGETNPOLYGONSTIPPLEPROC glGetnPolygonStipple;
PFNGLGETNCOLORTABLEPROC glGetnColorTable;
PFNGLGETNCONVOLUTIONFILTERPROC glGetnConvolutionFilter;
PFNGLGETNSEPARABLEFILTERPROC glGetnSeparableFilter;
PFNGLGETNHISTOGRAMPROC glGetnHistogram;
PFNGLGETNMINMAXPROC glGetnMinmax;
PFNGLTEXTUREBARRIERPROC glTextureBarrier;
PFNGLCOLORTABLEPROC glColorTable;
PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
PFNGLGETCOLORTABLEPROC glGetColorTable;
PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
PFNGLCOLORSUBTABLEPROC glColorSubTable;
PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
PFNGLGETHISTOGRAMPROC glGetHistogram;
PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
PFNGLGETMINMAXPROC glGetMinmax;
PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
PFNGLHISTOGRAMPROC glHistogram;
PFNGLMINMAXPROC glMinmax;
PFNGLRESETHISTOGRAMPROC glResetHistogram;
PFNGLRESETMINMAXPROC glResetMinmax;
PFNGLGETPIXELMAPXVPROC glGetPixelMapxv;
PFNGLPIXELMAPXPROC glPixelMapx;
PFNGLPIXELSTOREXPROC glPixelStorex;

