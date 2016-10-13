#include "Functions.hpp"


PFNWGLCREATECONTEXTATTRIBSARBPROC			GreatVEngine::OpenGL::wglCreateContextAttribsARB					= nullptr;

PFNGLENABLEIPROC							GreatVEngine::OpenGL::glEnablei = nullptr;
PFNGLDISABLEIPROC							GreatVEngine::OpenGL::glDisablei = nullptr;
// Draw Func
PFNGLCOLORMASKIPROC							GreatVEngine::OpenGL::glColorMaski = nullptr;
PFNGLBLENDCOLORPROC							GreatVEngine::OpenGL::glBlendColor = nullptr;
PFNGLBLENDFUNCIPROC							GreatVEngine::OpenGL::glBlendFunci = nullptr;
PFNGLBLENDFUNCSEPARATEPROC					GreatVEngine::OpenGL::glBlendFuncSeparate = nullptr;
PFNGLBLENDFUNCSEPARATEIPROC					GreatVEngine::OpenGL::glBlendFuncSeparatei = nullptr;
PFNGLBLENDEQUATIONPROC						GreatVEngine::OpenGL::glBlendEquation = nullptr;
PFNGLBLENDEQUATIONIPROC						GreatVEngine::OpenGL::glBlendEquationi = nullptr;
PFNGLDRAWARRAYSINSTANCEDPROC				GreatVEngine::OpenGL::glDrawArraysInstanced = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC				GreatVEngine::OpenGL::glDrawElementsInstanced = nullptr;
// TextureFunc
PFNGLACTIVETEXTUREPROC						GreatVEngine::OpenGL::glActiveTexture = nullptr;
PFNGLGETTEXPARAMETERIIVPROC					GreatVEngine::OpenGL::glGetTexParameterIiv = nullptr;
PFNGLGENERATEMIPMAPPROC						GreatVEngine::OpenGL::glGenerateMipmap = nullptr;
PFNGLTEXIMAGE3DPROC							GreatVEngine::OpenGL::glTexImage3D = nullptr;
// Samplers
PFNGLGENSAMPLERSPROC						GreatVEngine::OpenGL::glGenSamplers = nullptr;
PFNGLDELETESAMPLERSPROC						GreatVEngine::OpenGL::glDeleteSamplers = nullptr;
PFNGLISSAMPLERPROC							GreatVEngine::OpenGL::glIsSampler = nullptr;
PFNGLBINDSAMPLERPROC						GreatVEngine::OpenGL::glBindSampler = nullptr;
PFNGLSAMPLERPARAMETERIPROC					GreatVEngine::OpenGL::glSamplerParameteri = nullptr;
PFNGLSAMPLERPARAMETERIVPROC					GreatVEngine::OpenGL::glSamplerParameteriv = nullptr;
PFNGLSAMPLERPARAMETERFPROC					GreatVEngine::OpenGL::glSamplerParameterf = nullptr;
PFNGLSAMPLERPARAMETERFVPROC					GreatVEngine::OpenGL::glSamplerParameterfv = nullptr;
PFNGLSAMPLERPARAMETERIIVPROC				GreatVEngine::OpenGL::glSamplerParameterIiv = nullptr;
PFNGLSAMPLERPARAMETERIUIVPROC				GreatVEngine::OpenGL::glSamplerParameterIuiv = nullptr;
PFNGLGETSAMPLERPARAMETERIVPROC				GreatVEngine::OpenGL::glGetSamplerParameteriv = nullptr;
PFNGLGETSAMPLERPARAMETERIIVPROC				GreatVEngine::OpenGL::glGetSamplerParameterIiv = nullptr;
PFNGLGETSAMPLERPARAMETERFVPROC				GreatVEngine::OpenGL::glGetSamplerParameterfv = nullptr;
PFNGLGETSAMPLERPARAMETERIUIVPROC			GreatVEngine::OpenGL::glGetSamplerParameterIuiv = nullptr;
// Shader func
PFNGLCREATEPROGRAMPROC						GreatVEngine::OpenGL::glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC						GreatVEngine::OpenGL::glDeleteProgram = nullptr;
PFNGLLINKPROGRAMPROC						GreatVEngine::OpenGL::glLinkProgram = nullptr;
PFNGLVALIDATEPROGRAMPROC					GreatVEngine::OpenGL::glValidateProgram = nullptr;
PFNGLUSEPROGRAMPROC							GreatVEngine::OpenGL::glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC						GreatVEngine::OpenGL::glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC					GreatVEngine::OpenGL::glGetProgramInfoLog = nullptr;
PFNGLCREATESHADERPROC						GreatVEngine::OpenGL::glCreateShader = nullptr;
PFNGLDELETESHADERPROC						GreatVEngine::OpenGL::glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC						GreatVEngine::OpenGL::glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC						GreatVEngine::OpenGL::glCompileShader = nullptr;
PFNGLATTACHSHADERPROC						GreatVEngine::OpenGL::glAttachShader = nullptr;
PFNGLDETACHSHADERPROC						GreatVEngine::OpenGL::glDetachShader = nullptr;
PFNGLGETSHADERIVPROC						GreatVEngine::OpenGL::glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC					GreatVEngine::OpenGL::glGetShaderInfoLog = nullptr;
PFNGLGETUNIFORMLOCATIONPROC					GreatVEngine::OpenGL::glGetUniformLocation = nullptr;
PFNGLUNIFORM1IPROC							GreatVEngine::OpenGL::glUniform1i = nullptr;
PFNGLUNIFORM1FPROC							GreatVEngine::OpenGL::glUniform1f = nullptr;
PFNGLUNIFORM2FPROC							GreatVEngine::OpenGL::glUniform2f = nullptr;
PFNGLUNIFORM3FPROC							GreatVEngine::OpenGL::glUniform3f = nullptr;
PFNGLUNIFORM4FPROC							GreatVEngine::OpenGL::glUniform4f = nullptr;
PFNGLUNIFORMMATRIX3FVPROC					GreatVEngine::OpenGL::glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC					GreatVEngine::OpenGL::glUniformMatrix4fv = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC				GreatVEngine::OpenGL::glGetUniformBlockIndex = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC			GreatVEngine::OpenGL::glGetActiveUniformBlockiv = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC			GreatVEngine::OpenGL::glGetActiveUniformBlockName = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC				GreatVEngine::OpenGL::glUniformBlockBinding = nullptr;
PFNGLBINDATTRIBLOCATIONPROC					GreatVEngine::OpenGL::glBindAttribLocation = nullptr;
PFNGLGETACTIVEATTRIBPROC					GreatVEngine::OpenGL::glGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC					GreatVEngine::OpenGL::glGetActiveUniform = nullptr;
PFNGLGETATTRIBLOCATIONPROC					GreatVEngine::OpenGL::glGetAttribLocation = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC				GreatVEngine::OpenGL::glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC			GreatVEngine::OpenGL::glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC			GreatVEngine::OpenGL::glDisableVertexAttribArray = nullptr;
// Buffer Func
PFNGLGENVERTEXARRAYSPROC					GreatVEngine::OpenGL::glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC					GreatVEngine::OpenGL::glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC					GreatVEngine::OpenGL::glBindVertexArray = nullptr;
PFNGLBINDBUFFERPROC							GreatVEngine::OpenGL::glBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC						GreatVEngine::OpenGL::glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC							GreatVEngine::OpenGL::glGenBuffers = nullptr;
PFNGLISBUFFERPROC							GreatVEngine::OpenGL::glIsBuffer = nullptr;
PFNGLBUFFERDATAPROC							GreatVEngine::OpenGL::glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC						GreatVEngine::OpenGL::glBufferSubData = nullptr;
PFNGLGETBUFFERSUBDATAPROC					GreatVEngine::OpenGL::glGetBufferSubData = nullptr;
PFNGLMAPBUFFERPROC							GreatVEngine::OpenGL::glMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC						GreatVEngine::OpenGL::glUnmapBuffer = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC				GreatVEngine::OpenGL::glGetBufferParameteriv = nullptr;
PFNGLGETBUFFERPOINTERVPROC					GreatVEngine::OpenGL::glGetBufferPointerv = nullptr;
// Framebuffer Func
PFNGLCLEARBUFFERIVPROC						GreatVEngine::OpenGL::glClearBufferiv = nullptr;
PFNGLCLEARBUFFERUIVPROC						GreatVEngine::OpenGL::glClearBufferuiv = nullptr;
PFNGLCLEARBUFFERFVPROC						GreatVEngine::OpenGL::glClearBufferfv = nullptr;
PFNGLCLEARBUFFERFIPROC						GreatVEngine::OpenGL::glClearBufferfi = nullptr;
PFNGLBINDFRAMEBUFFERPROC					GreatVEngine::OpenGL::glBindFramebuffer = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC					GreatVEngine::OpenGL::glDeleteFramebuffers = nullptr;
PFNGLGENFRAMEBUFFERSPROC					GreatVEngine::OpenGL::glGenFramebuffers = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC				GreatVEngine::OpenGL::glCheckFramebufferStatus = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC					GreatVEngine::OpenGL::glFramebufferTexture = nullptr;
PFNGLFRAMEBUFFERTEXTURE1DPROC				GreatVEngine::OpenGL::glFramebufferTexture1D = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC				GreatVEngine::OpenGL::glFramebufferTexture2D = nullptr;
PFNGLFRAMEBUFFERTEXTURE3DPROC				GreatVEngine::OpenGL::glFramebufferTexture3D = nullptr;
PFNGLDRAWBUFFERSPROC						GreatVEngine::OpenGL::glDrawBuffers = nullptr;
// Transform Feedback
PFNGLBEGINTRANSFORMFEEDBACKPROC				GreatVEngine::OpenGL::glBeginTransformFeedback = nullptr;
PFNGLENDTRANSFORMFEEDBACKPROC				GreatVEngine::OpenGL::glEndTransformFeedback = nullptr;
PFNGLBINDBUFFERRANGEPROC					GreatVEngine::OpenGL::glBindBufferRange = nullptr;
PFNGLBINDBUFFERBASEPROC						GreatVEngine::OpenGL::glBindBufferBase = nullptr;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC			GreatVEngine::OpenGL::glTransformFeedbackVaryings = nullptr;
// GL_ARB_occlusion_query / GL_ARB_occlusion_query2(GL_ANY_SAMPLES_PASSED) GL 3.3
PFNGLGENQUERIESPROC							GreatVEngine::OpenGL::glGenQueries = nullptr;
PFNGLDELETEQUERIESPROC						GreatVEngine::OpenGL::glDeleteQueries = nullptr;
PFNGLISQUERYPROC							GreatVEngine::OpenGL::glIsQuery = nullptr;
PFNGLBEGINQUERYPROC							GreatVEngine::OpenGL::glBeginQuery = nullptr;
PFNGLENDQUERYPROC							GreatVEngine::OpenGL::glEndQuery = nullptr;
PFNGLGETQUERYIVPROC							GreatVEngine::OpenGL::glGetQueryiv = nullptr;
PFNGLGETQUERYOBJECTIVPROC					GreatVEngine::OpenGL::glGetQueryObjectiv = nullptr;
PFNGLGETQUERYOBJECTUIVPROC					GreatVEngine::OpenGL::glGetQueryObjectuiv = nullptr;
// GL_ARB_copy_buffer
PFNGLCOPYBUFFERSUBDATAPROC					GreatVEngine::OpenGL::glCopyBufferSubData = nullptr;

// GL 4.0
#if GVE_OPENGL_VERSION >= 400
PFNGLPATCHPARAMETERIPROC					GreatVEngine::OpenGL::glPatchParameteri = nullptr;
#endif
// GL 4.3
#if GVE_OPENGL_VERSION >= 430
PFNGLBINDIMAGETEXTUREPROC					GreatVEngine::OpenGL::glBindImageTexture = nullptr;
PFNGLDISPATCHCOMPUTEPROC					GreatVEngine::OpenGL::glDispatchCompute = nullptr;
#endif
