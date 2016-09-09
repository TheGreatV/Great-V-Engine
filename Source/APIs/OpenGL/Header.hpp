#pragma region Include
#pragma once


#include <Header/Header.hpp>


#if GVE_PLATFORM == GVE_PLATFORM_WIN
#include <Windows.h>
#endif


#include <gl/GL.h>
#include <gl/glext.h>
#include <gl/wglext.h>
#pragma endregion


#pragma region Macro
#define GVE_OPENGL_GEOMETRY_SHADER 1
#define GVE_OPENGL_TESSELLATION_SHADER 1
#define GVE_OPENGL_COMPUTE_SHADER 1

#if GVE_OPENGL_GEOMETRY_SHADER || GVE_OPENGL_TESSELLATION_SHADER || GVE_OPENGL_COMPUTE_SHADER
#if GVE_OPENGL_TESSELLATION_SHADER || GVE_OPENGL_COMPUTE_SHADER
#if GVE_OPENGL_COMPUTE_SHADER
#define GVE_OPENGL_VERSION 430
#else
#define GVE_OPENGL_VERSION 410
#endif
#else
#define GVE_OPENGL_VERSION 330
#endif
#else
#define GVE_OPENGL_VERSION 300
#endif
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
		class Context;
	}
}


#pragma region
#pragma endregion
