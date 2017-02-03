#pragma once
#include "Brothel.hpp"


void func()
{
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

	auto game = MakeReference<Brothel::Game>(deviceContext, window);

	auto building = MakeReference<Brothel::Building>(game, Vec3(0.0f), Vec3(0.0f));
	auto section = MakeReference<Brothel::Section>(building, Vec3(0.0f), Vec3(0.0f));
	auto miserableBedroom = Brothel::Rooms::Bedrooms::Miserable::Make(building, Vec3(4.0f, 0.0f, 0.0f), Vec3(0.0f));
	auto personnel = MakeReference<Brothel::Personnel>(game, Vec3(-4.0f, 0.0f, -2.0f), Vec3(0.0f), 18, Brothel::Personnel::Gender::Female);
	auto guest = MakeReference<Brothel::Guest>(game, Vec3(-4.0f, 0.0f, +2.0f), Vec3(0.0f), 18, Brothel::Guest::Gender::Male);

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		game->Loop();
		// graphicsScene->Render(graphicsCamera);
		game->graphicsScene_Main->Render(game->graphicsCamera_Main);

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