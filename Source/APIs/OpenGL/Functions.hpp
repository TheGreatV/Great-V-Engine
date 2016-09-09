#pragma region Include
#pragma once


#include "Header.hpp"
#pragma endregion


#define GVE_OPENGL_330 1


namespace GreatVEngine
{
	namespace OpenGL
	{
		extern PFNWGLCREATECONTEXTATTRIBSARBPROC			wglCreateContextAttribsARB;

		extern PFNGLENABLEIPROC									glEnablei;
		extern PFNGLDISABLEIPROC								glDisablei;
		// Draw Func
		extern PFNGLCOLORMASKIPROC								glColorMaski;
		extern PFNGLBLENDCOLORPROC								glBlendColor;
		extern PFNGLBLENDFUNCIPROC								glBlendFunci;
		extern PFNGLBLENDFUNCSEPARATEPROC						glBlendFuncSeparate;
		extern PFNGLBLENDFUNCSEPARATEIPROC						glBlendFuncSeparatei;
		extern PFNGLBLENDEQUATIONPROC							glBlendEquation;
		extern PFNGLBLENDEQUATIONIPROC							glBlendEquationi;
		extern PFNGLDRAWARRAYSINSTANCEDPROC						glDrawArraysInstanced;
		extern PFNGLDRAWELEMENTSINSTANCEDPROC					glDrawElementsInstanced;
		// TextureFunc
		extern PFNGLACTIVETEXTUREPROC							glActiveTexture;
		extern PFNGLGETTEXPARAMETERIIVPROC						glGetTexParameterIiv;
		extern PFNGLGENERATEMIPMAPPROC							glGenerateMipmap;
		extern PFNGLTEXIMAGE3DPROC								glTexImage3D;
		// Shader func
		extern PFNGLCREATEPROGRAMPROC							glCreateProgram;
		extern PFNGLDELETEPROGRAMPROC							glDeleteProgram;
		extern PFNGLLINKPROGRAMPROC								glLinkProgram;
		extern PFNGLVALIDATEPROGRAMPROC							glValidateProgram;
		extern PFNGLUSEPROGRAMPROC								glUseProgram;
		extern PFNGLGETPROGRAMIVPROC							glGetProgramiv;
		extern PFNGLGETPROGRAMINFOLOGPROC						glGetProgramInfoLog;
		extern PFNGLCREATESHADERPROC							glCreateShader;
		extern PFNGLDELETESHADERPROC							glDeleteShader;
		extern PFNGLSHADERSOURCEPROC							glShaderSource;
		extern PFNGLCOMPILESHADERPROC							glCompileShader;
		extern PFNGLATTACHSHADERPROC							glAttachShader;
		extern PFNGLDETACHSHADERPROC							glDetachShader;
		extern PFNGLGETSHADERIVPROC								glGetShaderiv;
		extern PFNGLGETSHADERINFOLOGPROC						glGetShaderInfoLog;
		extern PFNGLGETUNIFORMLOCATIONPROC						glGetUniformLocation;
		extern PFNGLUNIFORM1IPROC								glUniform1i;
		extern PFNGLUNIFORM1FPROC								glUniform1f;
		extern PFNGLUNIFORM2FPROC								glUniform2f;
		extern PFNGLUNIFORM3FPROC								glUniform3f;
		extern PFNGLUNIFORM4FPROC								glUniform4f;
		extern PFNGLUNIFORMMATRIX3FVPROC						glUniformMatrix3fv;
		extern PFNGLUNIFORMMATRIX4FVPROC						glUniformMatrix4fv;
		extern PFNGLGETUNIFORMBLOCKINDEXPROC					glGetUniformBlockIndex;
		extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC					glGetActiveUniformBlockiv;
		extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC				glGetActiveUniformBlockName;
		extern PFNGLUNIFORMBLOCKBINDINGPROC						glUniformBlockBinding;
		extern PFNGLBINDATTRIBLOCATIONPROC						glBindAttribLocation;
		extern PFNGLGETACTIVEATTRIBPROC							glGetActiveAttrib;
		extern PFNGLGETACTIVEUNIFORMPROC						glGetActiveUniform;
		extern PFNGLGETATTRIBLOCATIONPROC						glGetAttribLocation;
		extern PFNGLVERTEXATTRIBPOINTERPROC						glVertexAttribPointer;
		extern PFNGLENABLEVERTEXATTRIBARRAYPROC					glEnableVertexAttribArray;
		extern PFNGLDISABLEVERTEXATTRIBARRAYPROC				glDisableVertexAttribArray;
		// Buffer Func
		extern PFNGLGENVERTEXARRAYSPROC							glGenVertexArrays;
		extern PFNGLDELETEVERTEXARRAYSPROC						glDeleteVertexArrays;
		extern PFNGLBINDVERTEXARRAYPROC							glBindVertexArray;
		extern PFNGLBINDBUFFERPROC								glBindBuffer;
		extern PFNGLDELETEBUFFERSPROC							glDeleteBuffers;
		extern PFNGLGENBUFFERSPROC								glGenBuffers;
		extern PFNGLISBUFFERPROC								glIsBuffer;
		extern PFNGLBUFFERDATAPROC								glBufferData;
		extern PFNGLBUFFERSUBDATAPROC							glBufferSubData;
		extern PFNGLGETBUFFERSUBDATAPROC						glGetBufferSubData;
		extern PFNGLMAPBUFFERPROC								glMapBuffer;
		extern PFNGLUNMAPBUFFERPROC								glUnmapBuffer;
		extern PFNGLGETBUFFERPARAMETERIVPROC					glGetBufferParameteriv;
		extern PFNGLGETBUFFERPOINTERVPROC						glGetBufferPointerv;
		// Framebuffer Func
		extern PFNGLCLEARBUFFERIVPROC							glClearBufferiv;
		extern PFNGLCLEARBUFFERUIVPROC							glClearBufferuiv;
		extern PFNGLCLEARBUFFERFVPROC							glClearBufferfv;
		extern PFNGLCLEARBUFFERFIPROC							glClearBufferfi;
		extern PFNGLBINDFRAMEBUFFERPROC							glBindFramebuffer;
		extern PFNGLDELETEFRAMEBUFFERSPROC						glDeleteFramebuffers;
		extern PFNGLGENFRAMEBUFFERSPROC							glGenFramebuffers;
		extern PFNGLCHECKFRAMEBUFFERSTATUSPROC					glCheckFramebufferStatus;
		extern PFNGLFRAMEBUFFERTEXTUREPROC						glFramebufferTexture;
		extern PFNGLFRAMEBUFFERTEXTURE1DPROC					glFramebufferTexture1D;
		extern PFNGLFRAMEBUFFERTEXTURE2DPROC					glFramebufferTexture2D;
		extern PFNGLFRAMEBUFFERTEXTURE3DPROC					glFramebufferTexture3D;
		extern PFNGLDRAWBUFFERSPROC								glDrawBuffers;
		// Transform Feedback
		extern PFNGLBEGINTRANSFORMFEEDBACKPROC					glBeginTransformFeedback;
		extern PFNGLENDTRANSFORMFEEDBACKPROC					glEndTransformFeedback;
		extern PFNGLBINDBUFFERRANGEPROC							glBindBufferRange;
		extern PFNGLBINDBUFFERBASEPROC							glBindBufferBase;
		extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC				glTransformFeedbackVaryings;
		// GL_ARB_occlusion_query / GL_ARB_occlusion_query2(GL_ANY_SAMPLES_PASSED) GL 3.3
		extern PFNGLGENQUERIESPROC								glGenQueries;
		extern PFNGLDELETEQUERIESPROC							glDeleteQueries;
		extern PFNGLISQUERYPROC									glIsQuery;
		extern PFNGLBEGINQUERYPROC								glBeginQuery;
		extern PFNGLENDQUERYPROC								glEndQuery;
		extern PFNGLGETQUERYIVPROC								glGetQueryiv;
		extern PFNGLGETQUERYOBJECTIVPROC						glGetQueryObjectiv;
		extern PFNGLGETQUERYOBJECTUIVPROC						glGetQueryObjectuiv;
		// GL_ARB_copy_buffer
		extern PFNGLCOPYBUFFERSUBDATAPROC						glCopyBufferSubData;

		// GL 4.0
#if GVE_OPENGL_VERSION >= 400
		extern PFNGLPATCHPARAMETERIPROC							glPatchParameteri;
#endif
		// GL 4.3
#if GVE_OPENGL_VERSION >= 430
		extern PFNGLBINDIMAGETEXTUREPROC						glBindImageTexture;
		extern PFNGLDISPATCHCOMPUTEPROC							glDispatchCompute;
#endif
		/*extern PFNGLGENVERTEXARRAYSPROC						glGenVertexArrays;
		extern PFNGLDELETEVERTEXARRAYSPROC					glDeleteVertexArrays;
		extern PFNGLBINDVERTEXARRAYPROC						glBindVertexArray;

		extern PFNGLGENBUFFERSPROC							glGenBuffers;
		extern PFNGLDELETEBUFFERSPROC						glDeleteBuffers;
		extern PFNGLBINDBUFFERPROC							glBindBuffer;
		extern PFNGLBUFFERDATAPROC							glBufferData;

		extern PFNGLCREATEPROGRAMPROC						glCreateProgram;
		extern PFNGLDELETEPROGRAMPROC						glDeleteProgram;
		extern PFNGLUSEPROGRAMPROC							glUseProgram;
		extern PFNGLLINKPROGRAMPROC							glLinkProgram;
		extern PFNGLVALIDATEPROGRAMPROC						glValidateProgram;
		extern PFNGLGETPROGRAMIVPROC						glGetProgramiv;
		extern PFNGLGETPROGRAMINFOLOGPROC					glGetProgramInfoLog;

		extern PFNGLCREATESHADERPROC						glCreateShader;
		extern PFNGLDELETESHADERPROC						glDeleteShader;
		extern PFNGLSHADERSOURCEPROC						glShaderSource;
		extern PFNGLCOMPILESHADERPROC						glCompileShader;
		extern PFNGLATTACHSHADERPROC						glAttachShader;
		extern PFNGLDETACHSHADERPROC						glDetachShader;
		extern PFNGLGETSHADERIVPROC							glGetShaderiv;
		extern PFNGLGETSHADERINFOLOGPROC					glGetShaderInfoLog;

		extern PFNGLGETUNIFORMLOCATIONPROC					glGetUniformLocation;
		extern PFNGLUNIFORM1FPROC							glUniform1f;
		extern PFNGLUNIFORM1IPROC							glUniform1i;

		extern PFNGLGETATTRIBLOCATIONPROC					glGetAttribLocation;
		extern PFNGLVERTEXATTRIBPOINTERPROC					glVertexAttribPointer;
		extern PFNGLENABLEVERTEXATTRIBARRAYPROC				glEnableVertexAttribArray;
		extern PFNGLDISABLEVERTEXATTRIBARRAYPROC			glDisableVertexAttribArray;

		extern PFNGLACTIVETEXTUREPROC						glActiveTexture;*/
	}
}



