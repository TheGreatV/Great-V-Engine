#pragma once
#include "DarkPlace.hpp"
#include "Utilities/JSON/JSON.hpp"


GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


void func()
{
	// auto image = OpenIL::Image::Load2D(Filepath("Media/Images/Layered.psd"));


	WinAPI::Window::Size resolution(800, 600);
	// WinAPI::Window::Size resolution(1280, 600);
	// WinAPI::Window::Size resolution(1920, 1080);

	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference<GreatVEngine::WinAPI::WindowClass>(instance, "class");
	auto window = MakeReference<GreatVEngine::WinAPI::Window>(windowClass, "window", resolution);
	auto deviceContext = MakeReference<GreatVEngine::WinAPI::DeviceContext>(window);
	{
		deviceContext->SetPixelFormat();
	}

	auto game = DarkPlace::Make<DarkPlace::Game>(deviceContext, window); // deviceContext, window);
	
	auto player = game->Add<DarkPlace::Player>(Vec3(0.0f, 20.0f, 0.0f), Vec3(0.0f));
	auto test = game->Add<DarkPlace::Test>(Vec3(0.0f), Vec3(0.0f));
	auto test2 = game->Add<DarkPlace::Test2>(Vec3(0.0f, 14.0f, 0.0f), Vec3(40.0f, 0.0f, 0.0f));


	auto decal = MakeReference<Graphics::OpenGL::Decal>();
	{
		decal->SetLocalPosition(Vec3(5.0f, 0.0f, 0.0f));
		decal->SetLocalAngle(Vec3(90.0f, 0.0f, 0.0f));
		decal->SetLocalScale(Vec3(8.0f));

		game->graphicsScene_Main->LoadDecals(
			Filepath("Media/Images/Decals/Hearth_Albedo.png"),
			Filepath("Media/Images/Decals/Hearth_Topology.png"),
			Filepath("Media/Images/Decals/Hearth_Material.png"));

		game->graphicsScene_Main->Add(decal);
	}


	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		game->Update();

		Sleep(1000 / 60);

		deviceContext->SwapBuffers();
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
		MessageBoxA(NULL, e.GetText().c_str(), "WinAPI Exception", MB_OK);
	}
	catch(GVE::OpenGL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenGL Exception", MB_OK);
	}
	catch(GVE::OpenIL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenIL Exception", MB_OK);
	}
	catch(GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	return;
}