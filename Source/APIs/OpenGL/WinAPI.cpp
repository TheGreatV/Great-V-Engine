#include "WinAPI.hpp"


#if GVE_PLATFORM == GVE_PLATFORM_WIN

#define GVE_OPENGL_GET_PROC(x) if((x = reinterpret_cast<decltype(x)>(wglGetProcAddress(#x))) == nullptr) throw Exception("Failed to get function pointer for \""#x"\"");


#pragma region Context
GreatVEngine::OpenGL::WinAPI::Context::Context(Reference<DeviceContext> deviceContext_):
	OpenGL::Context(),
	deviceContext(deviceContext_),
	handle(wglCreateContext(deviceContext->GetHandle()))
{
	if(!handle)
	{
		ErrorTest();
	}
}
GreatVEngine::OpenGL::WinAPI::Context::~Context()
{
	wglDeleteContext(handle);
}

void GreatVEngine::OpenGL::WinAPI::Context::Set()
{
	if(!wglMakeCurrent(deviceContext->GetHandle(), handle))
	{
		ErrorTest();
		GreatVEngine::WinAPI::ErrorTest();
	}
}
void GreatVEngine::OpenGL::WinAPI::Context::Reset()
{
	if(!wglMakeCurrent(nullptr, nullptr))
	{
		ErrorTest();
		GreatVEngine::WinAPI::ErrorTest();
	}
}
#pragma endregion

#pragma region ExtendedContext
#pragma region Initer
GreatVEngine::OpenGL::WinAPI::ExtendedContext::Initer::Initer()
{
	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference(new GreatVEngine::WinAPI::WindowClass(instance, "class"));
	auto window = MakeReference(new GreatVEngine::WinAPI::Window(windowClass, "window"));
	auto deviceContext = MakeReference(new GreatVEngine::WinAPI::DeviceContext(window));
	deviceContext->SetPixelFormat();
	auto context = MakeReference(new WinAPI::Context(deviceContext));
	context->Set();


	GVE_OPENGL_GET_PROC(wglCreateContextAttribsARB);

	GVE_OPENGL_GET_PROC(glEnablei);
	GVE_OPENGL_GET_PROC(glDisablei);
	// Draw Func
	GVE_OPENGL_GET_PROC(glColorMaski);
	GVE_OPENGL_GET_PROC(glBlendColor);
	GVE_OPENGL_GET_PROC(glBlendFunci);
	GVE_OPENGL_GET_PROC(glBlendFuncSeparate);
	GVE_OPENGL_GET_PROC(glBlendFuncSeparatei);
	GVE_OPENGL_GET_PROC(glBlendEquation);
	GVE_OPENGL_GET_PROC(glBlendEquationi);
	GVE_OPENGL_GET_PROC(glDrawArraysInstanced);
	GVE_OPENGL_GET_PROC(glDrawElementsInstanced);
	// TextureFunc
	GVE_OPENGL_GET_PROC(glActiveTexture);
	GVE_OPENGL_GET_PROC(glGetTexParameterIiv);
	GVE_OPENGL_GET_PROC(glGenerateMipmap);
	GVE_OPENGL_GET_PROC(glTexImage3D);
	// Shader func
	GVE_OPENGL_GET_PROC(glCreateProgram);
	GVE_OPENGL_GET_PROC(glDeleteProgram);
	GVE_OPENGL_GET_PROC(glLinkProgram);
	GVE_OPENGL_GET_PROC(glValidateProgram);
	GVE_OPENGL_GET_PROC(glUseProgram);
	GVE_OPENGL_GET_PROC(glGetProgramiv);
	GVE_OPENGL_GET_PROC(glGetProgramInfoLog);
	GVE_OPENGL_GET_PROC(glCreateShader);
	GVE_OPENGL_GET_PROC(glDeleteShader);
	GVE_OPENGL_GET_PROC(glShaderSource);
	GVE_OPENGL_GET_PROC(glCompileShader);
	GVE_OPENGL_GET_PROC(glAttachShader);
	GVE_OPENGL_GET_PROC(glDetachShader);
	GVE_OPENGL_GET_PROC(glGetShaderiv);
	GVE_OPENGL_GET_PROC(glGetShaderInfoLog);
	GVE_OPENGL_GET_PROC(glGetUniformLocation);
	GVE_OPENGL_GET_PROC(glUniform1i);
	GVE_OPENGL_GET_PROC(glUniform1f);
	GVE_OPENGL_GET_PROC(glUniform2f);
	GVE_OPENGL_GET_PROC(glUniform3f);
	GVE_OPENGL_GET_PROC(glUniform4f);
	GVE_OPENGL_GET_PROC(glUniformMatrix3fv);
	GVE_OPENGL_GET_PROC(glUniformMatrix4fv);
	GVE_OPENGL_GET_PROC(glGetUniformBlockIndex);
	GVE_OPENGL_GET_PROC(glGetActiveUniformBlockiv);
	GVE_OPENGL_GET_PROC(glGetActiveUniformBlockName);
	GVE_OPENGL_GET_PROC(glUniformBlockBinding);
	GVE_OPENGL_GET_PROC(glBindAttribLocation);
	GVE_OPENGL_GET_PROC(glGetActiveAttrib);
	GVE_OPENGL_GET_PROC(glGetActiveUniform);
	GVE_OPENGL_GET_PROC(glGetAttribLocation);
	GVE_OPENGL_GET_PROC(glVertexAttribPointer);
	GVE_OPENGL_GET_PROC(glEnableVertexAttribArray);
	GVE_OPENGL_GET_PROC(glDisableVertexAttribArray);
	// Buffer Func
	GVE_OPENGL_GET_PROC(glGenVertexArrays);
	GVE_OPENGL_GET_PROC(glDeleteVertexArrays);
	GVE_OPENGL_GET_PROC(glBindVertexArray);
	GVE_OPENGL_GET_PROC(glBindBuffer);
	GVE_OPENGL_GET_PROC(glDeleteBuffers);
	GVE_OPENGL_GET_PROC(glGenBuffers);
	GVE_OPENGL_GET_PROC(glIsBuffer);
	GVE_OPENGL_GET_PROC(glBufferData);
	GVE_OPENGL_GET_PROC(glBufferSubData);
	GVE_OPENGL_GET_PROC(glGetBufferSubData);
	GVE_OPENGL_GET_PROC(glMapBuffer);
	GVE_OPENGL_GET_PROC(glUnmapBuffer);
	GVE_OPENGL_GET_PROC(glGetBufferParameteriv);
	GVE_OPENGL_GET_PROC(glGetBufferPointerv);
	// Framebuffer Func
	GVE_OPENGL_GET_PROC(glClearBufferiv);
	GVE_OPENGL_GET_PROC(glClearBufferuiv);
	GVE_OPENGL_GET_PROC(glClearBufferfv);
	GVE_OPENGL_GET_PROC(glClearBufferfi);
	GVE_OPENGL_GET_PROC(glBindFramebuffer);
	GVE_OPENGL_GET_PROC(glDeleteFramebuffers);
	GVE_OPENGL_GET_PROC(glGenFramebuffers);
	GVE_OPENGL_GET_PROC(glCheckFramebufferStatus);
	GVE_OPENGL_GET_PROC(glFramebufferTexture);
	GVE_OPENGL_GET_PROC(glFramebufferTexture1D);
	GVE_OPENGL_GET_PROC(glFramebufferTexture2D);
	GVE_OPENGL_GET_PROC(glFramebufferTexture3D);
	GVE_OPENGL_GET_PROC(glDrawBuffers);
	// Transform Feedback
	GVE_OPENGL_GET_PROC(glBeginTransformFeedback);
	GVE_OPENGL_GET_PROC(glEndTransformFeedback);
	GVE_OPENGL_GET_PROC(glBindBufferRange);
	GVE_OPENGL_GET_PROC(glBindBufferBase);
	GVE_OPENGL_GET_PROC(glTransformFeedbackVaryings);
	// GL_ARB_occlusion_query / GL_ARB_occlusion_query2(GL_ANY_SAMPLES_PASSED) GL 3.3
	GVE_OPENGL_GET_PROC(glGenQueries);
	GVE_OPENGL_GET_PROC(glDeleteQueries);
	GVE_OPENGL_GET_PROC(glIsQuery);
	GVE_OPENGL_GET_PROC(glBeginQuery);
	GVE_OPENGL_GET_PROC(glEndQuery);
	GVE_OPENGL_GET_PROC(glGetQueryiv);
	GVE_OPENGL_GET_PROC(glGetQueryObjectiv);
	GVE_OPENGL_GET_PROC(glGetQueryObjectuiv);
	// GL_ARB_copy_buffer
	GVE_OPENGL_GET_PROC(glCopyBufferSubData);

	// GL 4.0
#if GVE_OPENGL_VERSION >= 400
	GVE_OPENGL_GET_PROC(glPatchParameteri);
#endif
	// GL 4.3
#if GVE_OPENGL_VERSION >= 430
	GVE_OPENGL_GET_PROC(glBindImageTexture);
	GVE_OPENGL_GET_PROC(glDispatchCompute);
#endif

	context->Reset();

	// delete context;
	// delete deviceContext;
	// delete window;
	// delete windowClass;
	// delete instance;
}
#pragma endregion

const GreatVEngine::OpenGL::WinAPI::ExtendedContext::Initer GreatVEngine::OpenGL::WinAPI::ExtendedContext::initer;
const GLint GreatVEngine::OpenGL::WinAPI::ExtendedContext::attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 3,
	WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, //WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0, 0
};

GreatVEngine::OpenGL::WinAPI::ExtendedContext::ExtendedContext(Reference<DeviceContext> deviceContext_):
	OpenGL::Context(),
	deviceContext(deviceContext_),
	handle(wglCreateContextAttribsARB(deviceContext->GetHandle(), nullptr, attribs))
{
	if(!handle)
	{
		ErrorTest();
	}
}
GreatVEngine::OpenGL::WinAPI::ExtendedContext::~ExtendedContext()
{
	wglDeleteContext(handle);
}

void GreatVEngine::OpenGL::WinAPI::ExtendedContext::Set()
{
	if(!wglMakeCurrent(deviceContext->GetHandle(), handle))
	{
		ErrorTest();
		GreatVEngine::WinAPI::ErrorTest();
	}
}
void GreatVEngine::OpenGL::WinAPI::ExtendedContext::Reset()
{
	if(!wglMakeCurrent(nullptr, nullptr))
	{
		ErrorTest();
		GreatVEngine::WinAPI::ErrorTest();
	}
}
#pragma endregion

#endif GVE_PLATFORM == GVE_PLATFORM_WIN


