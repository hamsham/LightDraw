/* 
 * File:   draw/setup.h
 * Author: Miles Lacey
 *
 * Created on November 15, 2013, 8:37 PM
 */

#ifndef __LS_DRAW_SETUP_H__
#define	__LS_DRAW_SETUP_H__

#include "lightsky/setup/Setup.h"

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Hash.h"
#include "lightsky/utils/Log.h"

#include "lightsky/math/Math.h"



namespace ls {
namespace draw {



/*-------------------------------------
 * @brief LightSky OpenGL Function Initializaton
-------------------------------------*/
int init_ls_draw();



/*-------------------------------------
 * Debugging Various Messages.
-------------------------------------*/
#ifdef LS_DEBUG
    void printGlError(int line, const char* file);
#else
    inline void printGlError(int, const char*) {}
#endif



} // end draw namespace
} // end ls namespace



#ifndef LOG_GL_ERR
    #define LOG_GL_ERR() ls::draw::printGlError(__LINE__, __FILE__)
#endif



/*-------------------------------------
 * OpenGL Headers & Functions
-------------------------------------*/
#ifdef LS_OS_OSX
    #define GL_GLEXT_PROTOTYPES
    #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
    #include <OpenGL/gl.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
    #include <OpenGL/gl3ext.h>

#elif defined (LS_OS_IOS) || defined (LS_OS_IOS_SIM)
    #define GL_GLEXT_PROTOTYPES
    #include <OpenGLES/ES3/gl.h>
    #include <OpenGLES/ES3/glext.h>

#elif defined (LS_OS_LINUX)
    #define GL_GLEXT_PROTOTYPES
    #include <GLES3/gl3.h>
    /*#include <GLES3/gl2ext.h>*/
    #include <GLES3/gl3ext.h>

#elif defined (LS_OS_WINDOWS)
    #include <GL/gl.h>
    #include <GL/glext.h>

    #ifdef __cplusplus
    extern "C" {
    #endif

    /*-------------------------------------
     * OpenGL Function Declarations (extern)
    -------------------------------------*/
    extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
    extern PFNGLTEXIMAGE3DPROC glTexImage3D;
    extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
    extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
    extern PFNGLACTIVETEXTUREPROC glActiveTexture;
    extern PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
    extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
    extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
    extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
    extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
    extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
    extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
    extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
    extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
    extern PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d;
    extern PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv;
    extern PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f;
    extern PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv;
    extern PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i;
    extern PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv;
    extern PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s;
    extern PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv;
    extern PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d;
    extern PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
    extern PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
    extern PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;
    extern PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i;
    extern PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv;
    extern PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s;
    extern PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv;
    extern PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d;
    extern PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv;
    extern PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f;
    extern PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv;
    extern PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i;
    extern PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv;
    extern PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s;
    extern PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv;
    extern PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d;
    extern PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv;
    extern PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f;
    extern PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv;
    extern PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i;
    extern PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv;
    extern PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s;
    extern PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv;
    extern PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
    extern PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
    extern PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
    extern PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
    extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
    extern PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
    extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
    extern PFNGLPOINTPARAMETERFPROC glPointParameterf;
    extern PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
    extern PFNGLPOINTPARAMETERIPROC glPointParameteri;
    extern PFNGLPOINTPARAMETERIVPROC glPointParameteriv;
    extern PFNGLFOGCOORDFPROC glFogCoordf;
    extern PFNGLFOGCOORDFVPROC glFogCoordfv;
    extern PFNGLFOGCOORDDPROC glFogCoordd;
    extern PFNGLFOGCOORDDVPROC glFogCoorddv;
    extern PFNGLFOGCOORDPOINTERPROC glFogCoordPointer;
    extern PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
    extern PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv;
    extern PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
    extern PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv;
    extern PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
    extern PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv;
    extern PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
    extern PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv;
    extern PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
    extern PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv;
    extern PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
    extern PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
    extern PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
    extern PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
    extern PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
    extern PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
    extern PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
    extern PFNGLWINDOWPOS2DPROC glWindowPos2d;
    extern PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
    extern PFNGLWINDOWPOS2FPROC glWindowPos2f;
    extern PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
    extern PFNGLWINDOWPOS2IPROC glWindowPos2i;
    extern PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
    extern PFNGLWINDOWPOS2SPROC glWindowPos2s;
    extern PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
    extern PFNGLWINDOWPOS3DPROC glWindowPos3d;
    extern PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
    extern PFNGLWINDOWPOS3FPROC glWindowPos3f;
    extern PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
    extern PFNGLWINDOWPOS3IPROC glWindowPos3i;
    extern PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
    extern PFNGLWINDOWPOS3SPROC glWindowPos3s;
    extern PFNGLWINDOWPOS3SVPROC glWindowPos3sv;
    extern PFNGLBLENDCOLORPROC glBlendColor;
    extern PFNGLBLENDEQUATIONPROC glBlendEquation;
    extern PFNGLGENQUERIESPROC glGenQueries;
    extern PFNGLDELETEQUERIESPROC glDeleteQueries;
    extern PFNGLISQUERYPROC glIsQuery;
    extern PFNGLBEGINQUERYPROC glBeginQuery;
    extern PFNGLENDQUERYPROC glEndQuery;
    extern PFNGLGETQUERYIVPROC glGetQueryiv;
    extern PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
    extern PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
    extern PFNGLBINDBUFFERPROC glBindBuffer;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLISBUFFERPROC glIsBuffer;
    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
    extern PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
    extern PFNGLMAPBUFFERPROC glMapBuffer;
    extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
    extern PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
    extern PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;
    extern PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
    extern PFNGLDRAWBUFFERSPROC glDrawBuffers;
    extern PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
    extern PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
    extern PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
    extern PFNGLCOMPILESHADERPROC glCompileShader;
    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLCREATESHADERPROC glCreateShader;
    extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
    extern PFNGLDELETESHADERPROC glDeleteShader;
    extern PFNGLDETACHSHADERPROC glDetachShader;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
    extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
    extern PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
    extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
    extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
    extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    extern PFNGLGETSHADERIVPROC glGetShaderiv;
    extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    extern PFNGLGETSHADERSOURCEPROC glGetShaderSource;
    extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    extern PFNGLGETUNIFORMFVPROC glGetUniformfv;
    extern PFNGLGETUNIFORMIVPROC glGetUniformiv;
    extern PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
    extern PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
    extern PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
    extern PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
    extern PFNGLISPROGRAMPROC glIsProgram;
    extern PFNGLISSHADERPROC glIsShader;
    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLSHADERSOURCEPROC glShaderSource;
    extern PFNGLUSEPROGRAMPROC glUseProgram;
    extern PFNGLUNIFORM1FPROC glUniform1f;
    extern PFNGLUNIFORM2FPROC glUniform2f;
    extern PFNGLUNIFORM3FPROC glUniform3f;
    extern PFNGLUNIFORM4FPROC glUniform4f;
    extern PFNGLUNIFORM1IPROC glUniform1i;
    extern PFNGLUNIFORM2IPROC glUniform2i;
    extern PFNGLUNIFORM3IPROC glUniform3i;
    extern PFNGLUNIFORM4IPROC glUniform4i;
    extern PFNGLUNIFORM1FVPROC glUniform1fv;
    extern PFNGLUNIFORM2FVPROC glUniform2fv;
    extern PFNGLUNIFORM3FVPROC glUniform3fv;
    extern PFNGLUNIFORM4FVPROC glUniform4fv;
    extern PFNGLUNIFORM1IVPROC glUniform1iv;
    extern PFNGLUNIFORM2IVPROC glUniform2iv;
    extern PFNGLUNIFORM3IVPROC glUniform3iv;
    extern PFNGLUNIFORM4IVPROC glUniform4iv;
    extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
    extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
    extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
    extern PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
    extern PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
    extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
    extern PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
    extern PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
    extern PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
    extern PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
    extern PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
    extern PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
    extern PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
    extern PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
    extern PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
    extern PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
    extern PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
    extern PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
    extern PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
    extern PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
    extern PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
    extern PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
    extern PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
    extern PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
    extern PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
    extern PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
    extern PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
    extern PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
    extern PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
    extern PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
    extern PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
    extern PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
    extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
    extern PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
    extern PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
    extern PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
    extern PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
    extern PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
    extern PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    extern PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
    extern PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
    extern PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
    extern PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
    extern PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
    extern PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
    extern PFNGLCOLORMASKIPROC glColorMaski;
    extern PFNGLGETBOOLEANI_VPROC glGetBooleani_v;
    extern PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
    extern PFNGLENABLEIPROC glEnablei;
    extern PFNGLDISABLEIPROC glDisablei;
    extern PFNGLISENABLEDIPROC glIsEnabledi;
    extern PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
    extern PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
    extern PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
    extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
    extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
    extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
    extern PFNGLCLAMPCOLORPROC glClampColor;
    extern PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
    extern PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
    extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
    extern PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
    extern PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
    extern PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i;
    extern PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i;
    extern PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i;
    extern PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
    extern PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui;
    extern PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui;
    extern PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui;
    extern PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
    extern PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv;
    extern PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv;
    extern PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv;
    extern PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
    extern PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv;
    extern PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv;
    extern PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv;
    extern PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
    extern PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv;
    extern PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv;
    extern PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv;
    extern PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv;
    extern PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
    extern PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
    extern PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
    extern PFNGLUNIFORM1UIPROC glUniform1ui;
    extern PFNGLUNIFORM2UIPROC glUniform2ui;
    extern PFNGLUNIFORM3UIPROC glUniform3ui;
    extern PFNGLUNIFORM4UIPROC glUniform4ui;
    extern PFNGLUNIFORM1UIVPROC glUniform1uiv;
    extern PFNGLUNIFORM2UIVPROC glUniform2uiv;
    extern PFNGLUNIFORM3UIVPROC glUniform3uiv;
    extern PFNGLUNIFORM4UIVPROC glUniform4uiv;
    extern PFNGLTEXPARAMETERIIVPROC glTexParameterIiv;
    extern PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv;
    extern PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv;
    extern PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv;
    extern PFNGLCLEARBUFFERIVPROC glClearBufferiv;
    extern PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
    extern PFNGLCLEARBUFFERFVPROC glClearBufferfv;
    extern PFNGLCLEARBUFFERFIPROC glClearBufferfi;
    extern PFNGLGETSTRINGIPROC glGetStringi;
    extern PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
    extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
    extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
    extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
    extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
    extern PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
    extern PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
    extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
    extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
    extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
    extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
    extern PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
    extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
    extern PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
    extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
    extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
    extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
    extern PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
    extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
    extern PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
    extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
    extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    extern PFNGLISVERTEXARRAYPROC glIsVertexArray;
    extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
    extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
    extern PFNGLTEXBUFFERPROC glTexBuffer;
    extern PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex;
    extern PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;
    extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
    extern PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
    extern PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
    extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
    extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
    extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
    extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
    extern PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
    extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex;
    extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
    extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;
    extern PFNGLPROVOKINGVERTEXPROC glProvokingVertex;
    extern PFNGLFENCESYNCPROC glFenceSync;
    extern PFNGLISSYNCPROC glIsSync;
    extern PFNGLDELETESYNCPROC glDeleteSync;
    extern PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
    extern PFNGLWAITSYNCPROC glWaitSync;
    extern PFNGLGETINTEGER64VPROC glGetInteger64v;
    extern PFNGLGETSYNCIVPROC glGetSynciv;
    extern PFNGLGETINTEGER64I_VPROC glGetInteger64i_v;
    extern PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v;
    extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
    extern PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
    extern PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample;
    extern PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv;
    extern PFNGLSAMPLEMASKIPROC glSampleMaski;
    extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
    extern PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex;
    extern PFNGLGENSAMPLERSPROC glGenSamplers;
    extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
    extern PFNGLISSAMPLERPROC glIsSampler;
    extern PFNGLBINDSAMPLERPROC glBindSampler;
    extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
    extern PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv;
    extern PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
    extern PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv;
    extern PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv;
    extern PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv;
    extern PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv;
    extern PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv;
    extern PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv;
    extern PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv;
    extern PFNGLQUERYCOUNTERPROC glQueryCounter;
    extern PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v;
    extern PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;
    extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
    extern PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui;
    extern PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv;
    extern PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui;
    extern PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv;
    extern PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui;
    extern PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv;
    extern PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui;
    extern PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv;
    extern PFNGLVERTEXP2UIPROC glVertexP2ui;
    extern PFNGLVERTEXP2UIVPROC glVertexP2uiv;
    extern PFNGLVERTEXP3UIPROC glVertexP3ui;
    extern PFNGLVERTEXP3UIVPROC glVertexP3uiv;
    extern PFNGLVERTEXP4UIPROC glVertexP4ui;
    extern PFNGLVERTEXP4UIVPROC glVertexP4uiv;
    extern PFNGLTEXCOORDP1UIPROC glTexCoordP1ui;
    extern PFNGLTEXCOORDP1UIVPROC glTexCoordP1uiv;
    extern PFNGLTEXCOORDP2UIPROC glTexCoordP2ui;
    extern PFNGLTEXCOORDP2UIVPROC glTexCoordP2uiv;
    extern PFNGLTEXCOORDP3UIPROC glTexCoordP3ui;
    extern PFNGLTEXCOORDP3UIVPROC glTexCoordP3uiv;
    extern PFNGLTEXCOORDP4UIPROC glTexCoordP4ui;
    extern PFNGLTEXCOORDP4UIVPROC glTexCoordP4uiv;
    extern PFNGLMULTITEXCOORDP1UIPROC glMultiTexCoordP1ui;
    extern PFNGLMULTITEXCOORDP1UIVPROC glMultiTexCoordP1uiv;
    extern PFNGLMULTITEXCOORDP2UIPROC glMultiTexCoordP2ui;
    extern PFNGLMULTITEXCOORDP2UIVPROC glMultiTexCoordP2uiv;
    extern PFNGLMULTITEXCOORDP3UIPROC glMultiTexCoordP3ui;
    extern PFNGLMULTITEXCOORDP3UIVPROC glMultiTexCoordP3uiv;
    extern PFNGLMULTITEXCOORDP4UIPROC glMultiTexCoordP4ui;
    extern PFNGLMULTITEXCOORDP4UIVPROC glMultiTexCoordP4uiv;
    extern PFNGLNORMALP3UIPROC glNormalP3ui;
    extern PFNGLNORMALP3UIVPROC glNormalP3uiv;
    extern PFNGLCOLORP3UIPROC glColorP3ui;
    extern PFNGLCOLORP3UIVPROC glColorP3uiv;
    extern PFNGLCOLORP4UIPROC glColorP4ui;
    extern PFNGLCOLORP4UIVPROC glColorP4uiv;
    extern PFNGLSECONDARYCOLORP3UIPROC glSecondaryColorP3ui;
    extern PFNGLSECONDARYCOLORP3UIVPROC glSecondaryColorP3uiv;
    extern PFNGLMINSAMPLESHADINGPROC glMinSampleShading;
    extern PFNGLBLENDEQUATIONIPROC glBlendEquationi;
    extern PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei;
    extern PFNGLBLENDFUNCIPROC glBlendFunci;
    extern PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei;
    extern PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect;
    extern PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;
    extern PFNGLUNIFORM1DPROC glUniform1d;
    extern PFNGLUNIFORM2DPROC glUniform2d;
    extern PFNGLUNIFORM3DPROC glUniform3d;
    extern PFNGLUNIFORM4DPROC glUniform4d;
    extern PFNGLUNIFORM1DVPROC glUniform1dv;
    extern PFNGLUNIFORM2DVPROC glUniform2dv;
    extern PFNGLUNIFORM3DVPROC glUniform3dv;
    extern PFNGLUNIFORM4DVPROC glUniform4dv;
    extern PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
    extern PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
    extern PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
    extern PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
    extern PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
    extern PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
    extern PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
    extern PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
    extern PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
    extern PFNGLGETUNIFORMDVPROC glGetUniformdv;
    extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
    extern PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
    extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv;
    extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
    extern PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName;
    extern PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
    extern PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv;
    extern PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;
    extern PFNGLPATCHPARAMETERIPROC glPatchParameteri;
    extern PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;
    extern PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
    extern PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
    extern PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
    extern PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
    extern PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
    extern PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
    extern PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;
    extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
    extern PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed;
    extern PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed;
    extern PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv;
    extern PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
    extern PFNGLSHADERBINARYPROC glShaderBinary;
    extern PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
    extern PFNGLDEPTHRANGEFPROC glDepthRangef;
    extern PFNGLCLEARDEPTHFPROC glClearDepthf;
    extern PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
    extern PFNGLPROGRAMBINARYPROC glProgramBinary;
    extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
    extern PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
    extern PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
    extern PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
    extern PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
    extern PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
    extern PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
    extern PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
    extern PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
    extern PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
    extern PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
    extern PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
    extern PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
    extern PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
    extern PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
    extern PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
    extern PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
    extern PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
    extern PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
    extern PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
    extern PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
    extern PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
    extern PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
    extern PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
    extern PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
    extern PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
    extern PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
    extern PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
    extern PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
    extern PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
    extern PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
    extern PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
    extern PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
    extern PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
    extern PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
    extern PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
    extern PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
    extern PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
    extern PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
    extern PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
    extern PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
    extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
    extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
    extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
    extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
    extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
    extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
    extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
    extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
    extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
    extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
    extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
    extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
    extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
    extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
    extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
    extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
    extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
    extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
    extern PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
    extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
    extern PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
    extern PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
    extern PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
    extern PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
    extern PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
    extern PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
    extern PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
    extern PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
    extern PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
    extern PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;
    extern PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
    extern PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
    extern PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
    extern PFNGLSCISSORARRAYVPROC glScissorArrayv;
    extern PFNGLSCISSORINDEXEDPROC glScissorIndexed;
    extern PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
    extern PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
    extern PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
    extern PFNGLGETFLOATI_VPROC glGetFloati_v;
    extern PFNGLGETDOUBLEI_VPROC glGetDoublei_v;
    extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance;
    extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance;
    extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;
    extern PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;
    extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;
    extern PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
    extern PFNGLMEMORYBARRIERPROC glMemoryBarrier;
    extern PFNGLTEXSTORAGE1DPROC glTexStorage1D;
    extern PFNGLTEXSTORAGE2DPROC glTexStorage2D;
    extern PFNGLTEXSTORAGE3DPROC glTexStorage3D;
    extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced;
    extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;
    extern PFNGLCLEARBUFFERDATAPROC glClearBufferData;
    extern PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData;
    extern PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
    extern PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect;
    extern PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData;
    extern PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri;
    extern PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv;
    extern PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v;
    extern PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage;
    extern PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage;
    extern PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData;
    extern PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData;
    extern PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer;
    extern PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;
    extern PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect;
    extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect;
    extern PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv;
    extern PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex;
    extern PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName;
    extern PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv;
    extern PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation;
    extern PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
    extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding;
    extern PFNGLTEXBUFFERRANGEPROC glTexBufferRange;
    extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample;
    extern PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample;
    extern PFNGLTEXTUREVIEWPROC glTextureView;
    extern PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer;
    extern PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat;
    extern PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat;
    extern PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat;
    extern PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding;
    extern PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor;
    extern PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
    extern PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert;
    extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
    extern PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog;
    extern PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup;
    extern PFNGLPOPDEBUGGROUPPROC glPopDebugGroup;
    extern PFNGLOBJECTLABELPROC glObjectLabel;
    extern PFNGLGETOBJECTLABELPROC glGetObjectLabel;
    extern PFNGLOBJECTPTRLABELPROC glObjectPtrLabel;
    extern PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel;
    extern PFNGLBUFFERSTORAGEPROC glBufferStorage;
    extern PFNGLCLEARTEXIMAGEPROC glClearTexImage;
    extern PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage;
    extern PFNGLBINDBUFFERSBASEPROC glBindBuffersBase;
    extern PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange;
    extern PFNGLBINDTEXTURESPROC glBindTextures;
    extern PFNGLBINDSAMPLERSPROC glBindSamplers;
    extern PFNGLBINDIMAGETEXTURESPROC glBindImageTextures;
    extern PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers;
    extern PFNGLCLIPCONTROLPROC glClipControl;
    extern PFNGLCREATETRANSFORMFEEDBACKSPROC glCreateTransformFeedbacks;
    extern PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glTransformFeedbackBufferBase;
    extern PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glTransformFeedbackBufferRange;
    extern PFNGLGETTRANSFORMFEEDBACKIVPROC glGetTransformFeedbackiv;
    extern PFNGLGETTRANSFORMFEEDBACKI_VPROC glGetTransformFeedbacki_v;
    extern PFNGLGETTRANSFORMFEEDBACKI64_VPROC glGetTransformFeedbacki64_v;
    extern PFNGLCREATEBUFFERSPROC glCreateBuffers;
    extern PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;
    extern PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;
    extern PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData;
    extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData;
    extern PFNGLCLEARNAMEDBUFFERDATAPROC glClearNamedBufferData;
    extern PFNGLCLEARNAMEDBUFFERSUBDATAPROC glClearNamedBufferSubData;
    extern PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer;
    extern PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange;
    extern PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer;
    extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange;
    extern PFNGLGETNAMEDBUFFERPARAMETERIVPROC glGetNamedBufferParameteriv;
    extern PFNGLGETNAMEDBUFFERPARAMETERI64VPROC glGetNamedBufferParameteri64v;
    extern PFNGLGETNAMEDBUFFERPOINTERVPROC glGetNamedBufferPointerv;
    extern PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData;
    extern PFNGLCREATEFRAMEBUFFERSPROC glCreateFramebuffers;
    extern PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC glNamedFramebufferRenderbuffer;
    extern PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC glNamedFramebufferParameteri;
    extern PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glNamedFramebufferTexture;
    extern PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC glNamedFramebufferTextureLayer;
    extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC glNamedFramebufferDrawBuffer;
    extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC glNamedFramebufferDrawBuffers;
    extern PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC glNamedFramebufferReadBuffer;
    extern PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC glInvalidateNamedFramebufferData;
    extern PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC glInvalidateNamedFramebufferSubData;
    extern PFNGLCLEARNAMEDFRAMEBUFFERIVPROC glClearNamedFramebufferiv;
    extern PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC glClearNamedFramebufferuiv;
    extern PFNGLCLEARNAMEDFRAMEBUFFERFVPROC glClearNamedFramebufferfv;
    extern PFNGLCLEARNAMEDFRAMEBUFFERFIPROC glClearNamedFramebufferfi;
    extern PFNGLBLITNAMEDFRAMEBUFFERPROC glBlitNamedFramebuffer;
    extern PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC glCheckNamedFramebufferStatus;
    extern PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC glGetNamedFramebufferParameteriv;
    extern PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetNamedFramebufferAttachmentParameteriv;
    extern PFNGLCREATERENDERBUFFERSPROC glCreateRenderbuffers;
    extern PFNGLNAMEDRENDERBUFFERSTORAGEPROC glNamedRenderbufferStorage;
    extern PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC glNamedRenderbufferStorageMultisample;
    extern PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC glGetNamedRenderbufferParameteriv;
    extern PFNGLCREATETEXTURESPROC glCreateTextures;
    extern PFNGLTEXTUREBUFFERPROC glTextureBuffer;
    extern PFNGLTEXTUREBUFFERRANGEPROC glTextureBufferRange;
    extern PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D;
    extern PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
    extern PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D;
    extern PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC glTextureStorage2DMultisample;
    extern PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC glTextureStorage3DMultisample;
    extern PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D;
    extern PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
    extern PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D;
    extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC glCompressedTextureSubImage1D;
    extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC glCompressedTextureSubImage2D;
    extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC glCompressedTextureSubImage3D;
    extern PFNGLCOPYTEXTURESUBIMAGE1DPROC glCopyTextureSubImage1D;
    extern PFNGLCOPYTEXTURESUBIMAGE2DPROC glCopyTextureSubImage2D;
    extern PFNGLCOPYTEXTURESUBIMAGE3DPROC glCopyTextureSubImage3D;
    extern PFNGLTEXTUREPARAMETERFPROC glTextureParameterf;
    extern PFNGLTEXTUREPARAMETERFVPROC glTextureParameterfv;
    extern PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
    extern PFNGLTEXTUREPARAMETERIIVPROC glTextureParameterIiv;
    extern PFNGLTEXTUREPARAMETERIUIVPROC glTextureParameterIuiv;
    extern PFNGLTEXTUREPARAMETERIVPROC glTextureParameteriv;
    extern PFNGLGENERATETEXTUREMIPMAPPROC glGenerateTextureMipmap;
    extern PFNGLBINDTEXTUREUNITPROC glBindTextureUnit;
    extern PFNGLGETTEXTUREIMAGEPROC glGetTextureImage;
    extern PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC glGetCompressedTextureImage;
    extern PFNGLGETTEXTURELEVELPARAMETERFVPROC glGetTextureLevelParameterfv;
    extern PFNGLGETTEXTURELEVELPARAMETERIVPROC glGetTextureLevelParameteriv;
    extern PFNGLGETTEXTUREPARAMETERFVPROC glGetTextureParameterfv;
    extern PFNGLGETTEXTUREPARAMETERIIVPROC glGetTextureParameterIiv;
    extern PFNGLGETTEXTUREPARAMETERIUIVPROC glGetTextureParameterIuiv;
    extern PFNGLGETTEXTUREPARAMETERIVPROC glGetTextureParameteriv;
    extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
    extern PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib;
    extern PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;
    extern PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;
    extern PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;
    extern PFNGLVERTEXARRAYVERTEXBUFFERSPROC glVertexArrayVertexBuffers;
    extern PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
    extern PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat;
    extern PFNGLVERTEXARRAYATTRIBIFORMATPROC glVertexArrayAttribIFormat;
    extern PFNGLVERTEXARRAYATTRIBLFORMATPROC glVertexArrayAttribLFormat;
    extern PFNGLVERTEXARRAYBINDINGDIVISORPROC glVertexArrayBindingDivisor;
    extern PFNGLGETVERTEXARRAYIVPROC glGetVertexArrayiv;
    extern PFNGLGETVERTEXARRAYINDEXEDIVPROC glGetVertexArrayIndexediv;
    extern PFNGLGETVERTEXARRAYINDEXED64IVPROC glGetVertexArrayIndexed64iv;
    extern PFNGLCREATESAMPLERSPROC glCreateSamplers;
    extern PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines;
    extern PFNGLCREATEQUERIESPROC glCreateQueries;
    extern PFNGLGETQUERYBUFFEROBJECTI64VPROC glGetQueryBufferObjecti64v;
    extern PFNGLGETQUERYBUFFEROBJECTIVPROC glGetQueryBufferObjectiv;
    extern PFNGLGETQUERYBUFFEROBJECTUI64VPROC glGetQueryBufferObjectui64v;
    extern PFNGLGETQUERYBUFFEROBJECTUIVPROC glGetQueryBufferObjectuiv;
    extern PFNGLMEMORYBARRIERBYREGIONPROC glMemoryBarrierByRegion;
    extern PFNGLGETTEXTURESUBIMAGEPROC glGetTextureSubImage;
    extern PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC glGetCompressedTextureSubImage;
    extern PFNGLGETGRAPHICSRESETSTATUSPROC glGetGraphicsResetStatus;
    extern PFNGLGETNCOMPRESSEDTEXIMAGEPROC glGetnCompressedTexImage;
    extern PFNGLGETNTEXIMAGEPROC glGetnTexImage;
    extern PFNGLGETNUNIFORMDVPROC glGetnUniformdv;
    extern PFNGLGETNUNIFORMFVPROC glGetnUniformfv;
    extern PFNGLGETNUNIFORMIVPROC glGetnUniformiv;
    extern PFNGLGETNUNIFORMUIVPROC glGetnUniformuiv;
    extern PFNGLREADNPIXELSPROC glReadnPixels;
    extern PFNGLGETNMAPDVPROC glGetnMapdv;
    extern PFNGLGETNMAPFVPROC glGetnMapfv;
    extern PFNGLGETNMAPIVPROC glGetnMapiv;
    extern PFNGLGETNPIXELMAPFVPROC glGetnPixelMapfv;
    extern PFNGLGETNPIXELMAPUIVPROC glGetnPixelMapuiv;
    extern PFNGLGETNPIXELMAPUSVPROC glGetnPixelMapusv;
    extern PFNGLGETNPOLYGONSTIPPLEPROC glGetnPolygonStipple;
    extern PFNGLGETNCOLORTABLEPROC glGetnColorTable;
    extern PFNGLGETNCONVOLUTIONFILTERPROC glGetnConvolutionFilter;
    extern PFNGLGETNSEPARABLEFILTERPROC glGetnSeparableFilter;
    extern PFNGLGETNHISTOGRAMPROC glGetnHistogram;
    extern PFNGLGETNMINMAXPROC glGetnMinmax;
    extern PFNGLTEXTUREBARRIERPROC glTextureBarrier;
    extern PFNGLCOLORTABLEPROC glColorTable;
    extern PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
    extern PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
    extern PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
    extern PFNGLGETCOLORTABLEPROC glGetColorTable;
    extern PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
    extern PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
    extern PFNGLCOLORSUBTABLEPROC glColorSubTable;
    extern PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
    extern PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
    extern PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
    extern PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
    extern PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
    extern PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
    extern PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
    extern PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
    extern PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
    extern PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
    extern PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
    extern PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
    extern PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
    extern PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
    extern PFNGLGETHISTOGRAMPROC glGetHistogram;
    extern PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
    extern PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
    extern PFNGLGETMINMAXPROC glGetMinmax;
    extern PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
    extern PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
    extern PFNGLHISTOGRAMPROC glHistogram;
    extern PFNGLMINMAXPROC glMinmax;
    extern PFNGLRESETHISTOGRAMPROC glResetHistogram;
    extern PFNGLRESETMINMAXPROC glResetMinmax;
    extern PFNGLGETPIXELMAPXVPROC glGetPixelMapxv;
    extern PFNGLPIXELMAPXPROC glPixelMapx;
    extern PFNGLPIXELSTOREXPROC glPixelStorex;

    #ifdef __cplusplus
    }
    #endif
#else
    #error "An unsupported OS is currently being used."
#endif

#endif	/* __LS_DRAW_SETUP_H__ */
