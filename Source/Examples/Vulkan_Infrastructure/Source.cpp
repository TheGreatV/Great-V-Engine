#include <iostream>


#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <Infrastructure/Vulkan.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;
using namespace GVE::Graphics;


void func()
{
	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference(new GreatVEngine::WinAPI::WindowClass(instance, "class"));
	auto window = MakeReference(new GreatVEngine::WinAPI::Window(windowClass, "window"));
	auto deviceContext = MakeReference(new GreatVEngine::WinAPI::DeviceContext(window));
	{
		deviceContext->SetPixelFormat();
	}


	auto camera = MakeReference(new Graphics::Camera());
	{
		camera->SetPosition(Vec3(0.0f, 8.0f, -20.0f));
		camera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f));
	}

	// auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(50));
	auto geometry = Geometry::CreateTorus(8.0f, 2.0f, Vec2(8.0f, 2.0f), UVec2(128, 24));


	auto engine = MakeReference(new Vulkan::Engine(instance->GetHangle(), window->GetHandle()));


	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		window->Loop();

		// object->SetLocalAngle(object->GetLocalAngle() + Vec3(0.5f, 1.0f, 0.25f));

		// scene->Render(camera);

		// deviceContext->SwapBuffers();
	}
}


void main()
{
	try
	{
		func();
	}
	catch(GVE::WinAPI::Exception e)
	{
		std::cout << "WinAPI Exception: " << e.GetText().c_str() << std::endl;
	}
	catch(GVE::Vulkan::Exception e)
	{
		std::cout << "Vulkan Exception: " << e.GetText().c_str() << std::endl;
	}
	catch(GVE::OpenIL::Exception e)
	{
		std::cout << "OpenIL Exception: " << e.GetText().c_str() << std::endl;
	}
	catch(GVE::Exception e)
	{
		std::cout << "GVE Exception: " << e.GetText().c_str() << std::endl;
	}
}