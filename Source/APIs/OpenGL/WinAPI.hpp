#pragma region Include
#pragma once


#include <APIs/WinAPI/Window.hpp>


#include "Header.hpp"
#include "ErrorHandling.hpp"
#include "Functions.hpp"
#include "Wrapper.hpp"
#include "Infrastructure.hpp"


#include <string>


#include <Windows.h>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
#if GVE_PLATFORM == GVE_PLATFORM_WIN
		namespace WinAPI
		{
			class Context:
				public OpenGL::Context
			{
			public:
				using DeviceContext = GreatVEngine::WinAPI::DeviceContext;
				using Handle = HGLRC;
			protected:
				const Reference<DeviceContext> deviceContext;
				const Handle handle;
			public:
				Context(Reference<DeviceContext> deviceContext_);
				Context(const Context&) = delete;
				virtual ~Context() override;
			public:
				inline Context& operator = (const Context&) = delete;
			public:
				inline Handle GetHandle() const
				{
					return handle;
				}
			public:
				virtual void Set() override;
				virtual void Reset() override;
			};
			class ExtendedContext:
				public OpenGL::Context
			{
			public:
				using DeviceContext = GreatVEngine::WinAPI::DeviceContext;
			private:
				class Initer
				{
				public:
					Size texturesCount;
				public:
					Initer();
				};
			private:
				static const Initer initer;
				static const GLint attribs[];
			public:
				using Handle = HGLRC;
			protected:
				const Reference<DeviceContext> deviceContext;
				const Handle handle;
			public:
				ExtendedContext(Reference<DeviceContext> deviceContext_);
				~ExtendedContext();
			public:
				virtual void Set() override;
				virtual void Reset() override;
			};
		}
#endif // GVE_PLATFORM == GVE_PLATFORM_WIN
	}
}


#pragma region
#pragma endregion
