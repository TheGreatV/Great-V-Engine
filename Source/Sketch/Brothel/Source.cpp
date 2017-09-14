#pragma once
#include "Brothel.hpp"


void test()
{
	auto content = System::LoadFileContent(Filepath("Media/Data/Brothel/Test.json"));

	while(content.back() == '\0') content.pop_back();

	auto node = GVE::JSON::Parse(content);

	/*Vec3
		pos = Vec3(0.0f),
		ang = Vec3(0.0f);*/
	
	/*if((GVE::JSON::Object::Type)node["Position"] == GVE::JSON::Object::Type::Map)
	{
		pos = Vec3(
			(float)(double)node["Position"]["X"],
			(float)(double)node["Position"]["Y"],
			(float)(double)node["Position"]["Z"]);
	}*/
	/*if((GVE::JSON::Object::Type)node["Angle"] == GVE::JSON::Object::Type::Map)
	{
		ang = Vec3(
			(float)(double)node["Angle"]["X"],
			(float)(double)node["Angle"]["Y"],
			(float)(double)node["Angle"]["Z"]);
	}*/

	// 
}


void func()
{
	test();

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


	// auto game = MakeReference<Brothel::Game>(deviceContext, window);
	auto game = Make<Brothel::Game>(deviceContext, window);

	auto building = game->Create<Brothel::Building>(Vec3(0.0f), Vec3(0.0f));

	auto section			= MakeReference(building)->Create<Brothel::Section>(Vec3(0.0f), Vec3(0.0f));
	auto miserableBedroom	= MakeReference(building)->Create<Brothel::Rooms::Bedrooms::Miserable>(Vec3(4.0f, 0.0f, 0.0f), Vec3(0.0f));
	auto miserableBedroom2	= MakeReference(building)->Create<Brothel::Rooms::Bedrooms::Miserable>(Vec3(4.0f, 0.0f, -8.0f - 4.0f), Vec3(0.0f, 180.0f, 0.0f));
	auto hole				= MakeReference(building)->Create<Brothel::Rooms::Hole>(Vec3(0.0f, 0.0f, -8.0f - 4.0f), Vec3(0.0f, 180.0f, 0.0f));

	auto personnel		= game->Create<Brothel::Personnel>(Vec3(-5.0f - 0.0f, 0.0f, -2.0f), Vec3(0.0f), 18, Brothel::Personnel::Gender::Female);
	auto personnel2		= game->Create<Brothel::Personnel>(Vec3(-5.0f - 2.0f, 0.0f, -2.0f), Vec3(0.0f), 18, Brothel::Personnel::Gender::Female);
	auto personnel3		= game->Create<Brothel::Personnel>(Vec3(-5.0f - 4.0f, 0.0f, -2.0f), Vec3(0.0f), 18, Brothel::Personnel::Gender::Female);
	
	auto guest = game->Create<Brothel::Guest>(Vec3(-4.0f - 1.0f, 0.0f, +2.0f), Vec3(0.0f), 18, Brothel::Guest::Gender::Male);
	game->Remove(guest);

	while(!KeyState(Keys::ESC))
	{
		// if(KeyState(Keys::SPACE) && !MakeReference(guest)->GetEvent())
		// {
		// 	auto event = game->Create<Brothel::Events::Services::Test>(MakeReference(guest), MakeReference(personnel));
		// }

		Input::Loop();
		window->Loop();

		game->Update();
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