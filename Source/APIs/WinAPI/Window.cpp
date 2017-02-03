#include "Window.hpp"


GreatVEngine::Reference<GreatVEngine::WinAPI::Instance> GreatVEngine::WinAPI::Instance::Get()
{
	return WrapReference(new Instance(GetModuleHandleA(NULL)));
}
GreatVEngine::WinAPI::Instance::Instance(HINSTANCE handle_):
	handle(handle_)
{
}
GreatVEngine::WinAPI::Instance::Handle GreatVEngine::WinAPI::Instance::GetHangle() const
{
	return handle;
}


GreatVEngine::WinAPI::WindowClass::WindowClass(Reference<Instance> instance_, const Name& name_):
	WindowClass(instance_, name_, DefWindowProcA)
{
}
GreatVEngine::WinAPI::WindowClass::WindowClass(Reference<Instance> instance_, const Name& name_, Dispatcher dispatcher_):
	instance(instance_),
	name(name_)
{
	WNDCLASSA windowClass;
	{
		memset(&windowClass, 0, sizeof(windowClass));

		windowClass.lpszClassName = name.c_str();
		windowClass.hInstance = instance->GetHangle();
		windowClass.lpfnWndProc = dispatcher_;
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		if(!RegisterClassA(&windowClass))
		{
			ErrorTest();
		}
	}
}
GreatVEngine::WinAPI::WindowClass::~WindowClass()
{
	if(!UnregisterClassA(name.c_str(), instance->GetHangle()))
	{
		ErrorTest();
	}
}
GreatVEngine::Reference<GreatVEngine::WinAPI::Instance> GreatVEngine::WinAPI::WindowClass::GetInstance() const
{
	return instance;
}
GreatVEngine::WinAPI::WindowClass::Name GreatVEngine::WinAPI::WindowClass::GetName() const
{
	return name;
}


GreatVEngine::WinAPI::Window::Window(Reference<const WindowClass> windowClass_, const Name& name_):
	Window(windowClass_, name_, Size(800, 600))
{
}
GreatVEngine::WinAPI::Window::Window(Reference<const WindowClass> windowClass_, const Name& name_, const Size& size_):
	Window(windowClass_, name_, size_, styleDefault)
{
}
GreatVEngine::WinAPI::Window::Window(Reference<const WindowClass> windowClass_, const Name& name_, const Size& size_, const Style& style_):
	windowClass(windowClass_),
	name(name_),
	size(size_),
	style(style_),
	handle(CreateWindowA(
		windowClass->GetName().c_str(),
		name.c_str(),
		style,
		0, 0, size.x, size.y,
		NULL,
		NULL,
		windowClass->GetInstance()->GetHangle(),
		NULL
	))
{
	if(!handle)
	{
		ErrorTest();
	}
}
GreatVEngine::WinAPI::Window::~Window()
{
	if(!DestroyWindow(handle))
	{
		ErrorTest();
	}
}
void GreatVEngine::WinAPI::Window::Loop()
{
	RECT rect;
	POINT p;
	GetClientRect(handle, &rect);
	p.x = rect.left; p.y = rect.bottom;
	ClientToScreen(handle, &p);
	position.x = (GreatVEngine::Size)p.x;
	position.y = GetDesktopSize().y - (GreatVEngine::Size)p.y;

	MSG msg;
	{
		while(PeekMessageA(&msg, handle, 0, 0, PM_REMOVE))
		{
			ErrorTest();

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}


GreatVEngine::WinAPI::DeviceContext::DeviceContext(Reference<Window> window_):
	window(window_),
	handle(GetDC(window_->GetHandle()))
{
	if(!handle)
	{
		ErrorTest();
	}
}
GreatVEngine::Reference<GreatVEngine::WinAPI::Window> GreatVEngine::WinAPI::DeviceContext::GetWindow() const
{
	return window;
}
GreatVEngine::WinAPI::DeviceContext::Handle GreatVEngine::WinAPI::DeviceContext::GetHandle() const
{
	return handle;
}
void GreatVEngine::WinAPI::DeviceContext::SetPixelFormat()
{
	PIXELFORMATDESCRIPTOR pixelFormatDescriptorInfo;
	{
		memset(&pixelFormatDescriptorInfo, 0, sizeof(pixelFormatDescriptorInfo));

		pixelFormatDescriptorInfo.nSize = sizeof(pixelFormatDescriptorInfo);
		pixelFormatDescriptorInfo.nVersion = 1;
		pixelFormatDescriptorInfo.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pixelFormatDescriptorInfo.iPixelType = PFD_TYPE_RGBA;
		pixelFormatDescriptorInfo.cColorBits = 32;
		pixelFormatDescriptorInfo.cDepthBits = 32;
		pixelFormatDescriptorInfo.cStencilBits = 0;
	}

	auto pixelFormat = ChoosePixelFormat(handle, &pixelFormatDescriptorInfo);
	if(!pixelFormat)
	{
		ErrorTest();
	}

	if(!::SetPixelFormat(handle, pixelFormat, &pixelFormatDescriptorInfo))
	{
		ErrorTest();
	}
}
void GreatVEngine::WinAPI::DeviceContext::SwapBuffers() const
{
	if(!::SwapBuffers(handle))
	{
		ErrorTest();
	}
}



