#pragma region Include
#pragma once


#include "ErrorHandling.hpp"
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
		protected:
			const Reference<Instance> instance;
			const Name name;
		public:
			WindowClass(Reference<Instance> instance_, const Name& name_);
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
		protected:
			const Reference<const WindowClass> windowClass;
			const Name name;
			const Handle handle;
		public:
			Window(Reference<const WindowClass> windowClass_, const Name& name_);
			~Window();
		public:
			Name GetName() const;
			Handle GetHandle() const;
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

