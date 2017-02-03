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

	auto graphicsCamera = MakeReference<Graphics::Camera>();
	{
		graphicsCamera->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}

	auto graphicsEngine = MakeReference<Graphics::OpenGL::Engine>(deviceContext);

	auto graphicsScene = MakeReference<Graphics::OpenGL::Scene>(graphicsEngine, window->GetSize());

	auto graphicsLightDirection1 = MakeReference<Graphics::OpenGL::Lights::Direction>();
	{
		graphicsLightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		graphicsLightDirection1->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLightDirection1->SetAmbient(0.5f);
		graphicsScene->Add(graphicsLightDirection1);
	}

	auto graphicsMaterialFlat = MakeReference<Graphics::OpenGL::Material>(graphicsEngine);
	{
		graphicsMaterialFlat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		graphicsMaterialFlat->SetValue("materialColor", Vec3(1.0f));
		graphicsMaterialFlat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		graphicsMaterialFlat->SetValue("materialGloss", 0.1f);
		graphicsMaterialFlat->SetValue("materialRoughness", 0.8f);
	}
	auto graphicsObject = MakeReference<Graphics::OpenGL::Object>(graphicsEngine);
	{
		auto geometry = Geometry::CreateBox(Vec3(400.0f, 1.0f, 400.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(graphicsEngine, shape, graphicsMaterialFlat);
		graphicsObject->SetLocalPosition(Vec3(0.0f, -0.5f, 0.0f));
		graphicsObject->SetModel(model);
		graphicsScene->Add(graphicsObject);
	}

	// Helper::Transformation::HMat3 t(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr);
	// graphicsObject->SetParent(&t);

	// auto game = MakeReference<Brothel::Game>(deviceContext, window->GetSize());

	auto selector = MakeReference<Selectable::Selector>();

	/*
	using namespace Brothel;
	auto building = MakeReference<Building>(Vec3(0.0f), Vec3(0.0f));

	auto section = MakeReference<Section>(building, Vec3(0.0f), Vec3(0.0f), graphicsEngine, graphicsScene, graphicsMaterialFlat);
	{
		selector->Add(section);
	}
	auto section2 = MakeReference<Section>(building, Vec3(4.0f, 0.0f, 0.0f), Vec3(0.0f), graphicsEngine, graphicsScene, graphicsMaterialFlat);
	{
		selector->Add(section2);
	}
	auto section3 = MakeReference<Section>(building, Vec3(-4.0f, 0.0f, 0.0f), Vec3(0.0f), graphicsEngine, graphicsScene, graphicsMaterialFlat);
	{
		selector->Add(section3);
	}
	auto miserableBedroom = Rooms::Bedrooms::Miserable::Make(building, Vec3(8.0f, 0.0f, 0.0f), Vec3(0.0f), graphicsEngine, graphicsScene, graphicsMaterialFlat);
	{
		selector->Add(miserableBedroom);
	}
	auto personnel = MakeReference<Personnel>(Vec3(0.0f, 0.0f, -5.0f), Vec3(0.0f), 18, Character::Gender::Female, graphicsEngine, graphicsScene, graphicsMaterialFlat);
	{
		miserableBedroom->AttachInmate(personnel);
		selector->Add(personnel);
	}
	auto guest = MakeReference<Guest>(Vec3(2.0f, 0.0f, -5.0f), Vec3(0.0f), 18, Character::Gender::Male, graphicsEngine, graphicsScene, graphicsMaterialFlat);
	{
		selector->Add(guest);
	}
	*/

	// auto miserableBedroom = MakeReference(new Rooms::Bedrooms::Miserable(Vec3(0.0f), Vec3(0.0f)));
	// {
	// 	for(auto &bunk : miserableBedroom->bunks)
	// 	{
	// 		bunk->room = miserableBedroom;
	// 	}
	// 
	// 	miserableBedroom->building = building;
	// }
	// 
	// auto personnel = MakeReference(new Personnel(Vec3(0.0f), Vec3(0.0f), 18, Character::Gender::Female));


	Vec3 camPos = Vec3(0.0f, 2.0f, 0.0f);
	Vec3 camAng = Vec3(30.0f, 45.0f, 0.0f);
	Float32 camDist = 20.0f;
	Vec3 p1, p2;

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();
		// Input::Mouse::SetPosition(Vec2(Input::Mouse::GetDesktopSize()) / 2.0f);

		if(ButtonState(Buttons::Right))
		{
			camAng.x -= Input::Mouse::GetPositionDelta().y * 0.1f;
			camAng.y += Input::Mouse::GetPositionDelta().x * 0.2f;
		}

		graphicsCamera->SetPosition(camPos + RotateZXY3(camAng) * Vec3(0.0f, 0.0f, -camDist));
		graphicsCamera->SetAngle(camAng);

		/*for(auto &i : selector->selections)
		{
			if(auto section = std::dynamic_pointer_cast<Section>(i)) if(section->graphicsObject)
			{
				section->graphicsObject->SetColor(Vec4(1.0f));
			}

			if(auto miserableBedroom = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(i)) if(miserableBedroom->graphicsObject)
			{
				miserableBedroom->graphicsObject->SetColor(Vec4(1.0f));
			}
			
			if(auto character = std::dynamic_pointer_cast<Character>(i)) if(character->graphicsObject)
			{
				character->graphicsObject->SetColor(Vec4(1.0f));
			}
		}*/

		auto m = Vec2(window->ToLocalPosition(IVec2(Input::Mouse::GetPosition())));
		graphicsScene->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
		auto p = (m / Vec2(window->GetSize())) * 2.0f - 1.0f;
		auto picked = selector->Pick(graphicsCamera, p);
		/*if(picked)
		{
			if(auto section = std::dynamic_pointer_cast<Section>(picked)) if(section->graphicsObject)
			{
				section->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if(auto miserableBedroom = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(picked)) if(miserableBedroom->graphicsObject)
			{
				miserableBedroom->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if(auto character = std::dynamic_pointer_cast<Character>(picked)) if(character->graphicsObject)
			{
				character->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}*/

		// p1 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
		// p2 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));

		// personnel->Loop();

		graphicsScene->Render(graphicsCamera);

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