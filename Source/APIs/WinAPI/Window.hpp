#pragma region Include
#pragma once


#include "ErrorHandling.hpp"


#include <Logic/Mathematics.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace WinAPI
	{
		class Instance
		{
		public:
			using Handle = HINSTANCE;
		public:
			static Reference<Instance> Get();
		protected:
			const HINSTANCE handle;
		protected:
			Instance(HINSTANCE handle_);
		public:
			Handle GetHangle() const;
		};

		class WindowClass
		{
		public:
			using Name = String;
			using Dispatcher = WNDPROC;
		protected:
			const Reference<Instance> instance;
			const Name name;
		public:
			WindowClass(Reference<Instance> instance_, const Name& name_);
			WindowClass(Reference<Instance> instance_, const Name& name_, Dispatcher dispatcher_);
			~WindowClass();
		public:
			Reference<Instance> GetInstance() const;
			Name GetName() const;
		};

		class Window
		{
		public:
			using Handle = HWND;
			using Name = String;
			using Style = DWORD;
			using Size = Size2;
		public:
			const static Style styleDefault = WS_SYSMENU | WS_VISIBLE;
			const static Style styleHidden = 0 & ~WS_VISIBLE;
		protected:
			const Reference<const WindowClass> windowClass;
			const Name name;
			const Style style;
			Size size;
			const Handle handle;
		public:
			Window(Reference<const WindowClass> windowClass_, const Name& name_);
			Window(Reference<const WindowClass> windowClass_, const Name& name_, const Size& size_);
			Window(Reference<const WindowClass> windowClass_, const Name& name_, const Size& size_, const Style& style_);
			~Window();
		public:
			inline Name GetName() const
			{
				return name;
			}
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Style GetStyle() const
			{
				return style;
			}
			inline Size GetSize() const
			{
				return size;
			}
			inline Float32 GetAspect() const
			{
				return (Float32)size.x / (Float32)size.y;
			}
			void Loop() const;
		};

		class DeviceContext
		{
		public:
			using Handle = HDC;
		protected:
			const Reference<Window> window;
			const Handle handle;
		public:
			DeviceContext(Reference<Window> window_);
		public:
			Reference<Window> GetWindow() const;
			Handle GetHandle() const;
		public:
			void SetPixelFormat();
		public:
			void SwapBuffers() const;
		};
	}
}


#pragma region
#pragma endregion

