#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Input/Input.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;
using namespace GVE::Graphics;
namespace UI = UserInterface;


void func()
{
	WinAPI::Window::Size resolution(800, 600);
	// WinAPI::Window::Size resolution(1280, 600);
	// WinAPI::Window::Size resolution(1920, 1080);

	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference(new GreatVEngine::WinAPI::WindowClass(instance, "class"));
	auto window = MakeReference(new GreatVEngine::WinAPI::Window(windowClass, "window", resolution));
	auto deviceContext = MakeReference(new GreatVEngine::WinAPI::DeviceContext(window));
	{
		deviceContext->SetPixelFormat();
	}


	auto camera = MakeReference(new Graphics::Camera());
	{
		camera->SetPosition(Vec3(0.0f, 8.0f, -20.0f));
		camera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}

	auto engine = MakeReference(new Graphics::OpenGL::Engine(deviceContext));

	auto scene = MakeReference(new Graphics::OpenGL::Scene(engine, window->GetSize()));
	{
		// scene->LoadDecals(
		// 	Filepath("Media/Images/Decals/GlassHole_Albedo.png"),
		// 	Filepath("Media/Images/Decals/GlassHole_Topology.png"),
		// 	Filepath("Media/Images/Decals/GlassHole_Material.png"));
	}

	auto lightDirection1 = MakeReference(new Graphics::OpenGL::Lights::Direction());
	{
		// lightDirection1->Hide();
		lightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		lightDirection1->SetColor(Vec4(Vec3(1.0f), 0.8f));
		lightDirection1->SetAmbient(0.5f);
		scene->Add(lightDirection1);
	}

	auto environmentGlobalmap1 = Graphics::OpenGL::Environments::Globalmap::LoadCube(engine, Filepath("Media/Images/Cubemap2.dds"));
	{
		// environmentGlobalmap1->Hide();
		scene->Add(environmentGlobalmap1);
	}

	/*auto materialSkeletal = MakeReference(new OpenGL::Material(engine));
	{
		materialSkeletal->Technique(Material::TechniqueType::Basic) = OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Blank/Basic/Skeletal.glsl."), "vs", "", "", "", "fs");
		materialSkeletal->SetValue("materialColor", Vec3(1.0f));
		materialSkeletal->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		materialSkeletal->SetValue("materialGloss", 0.8f);
		materialSkeletal->SetValue("materialRoughness", 0.2f);
	}*/
	auto materialFlat = MakeReference(new Graphics::OpenGL::Material(engine));
	{
		materialFlat->Technique(Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		materialFlat->SetValue("materialColor", Vec3(1.0f));
		materialFlat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		materialFlat->SetValue("materialGloss", 0.5f);
		materialFlat->SetValue("materialRoughness", 0.5f);
	}
	auto sponza = Graphics::OpenGL::Object::Load(engine, Filepath("Media/Models/Sponza.gvs"), materialFlat);
	{
		scene->Add(sponza);
	}
	/*auto bones = OpenGL::Object::Load(engine, Filepath("Media/Models/Bones.gvs"), materialSkeletal);
	{
		// bones->SetLocalPosition(Vec3(+45.0f, 0.0f, 0.0f));
		scene->Add(bones);
	}*/
	/*auto biker = OpenGL::Object::Load(engine, Filepath("Media/Models/Biker.gvs"), materialSkeletal);
	{
		scene->Add(biker);
	}*/

	while(!KeyState(Keys::ESC))
	{
		Input::Keyboard::Loop();
		window->Loop();

		// biker->GetBones()[10]->SetLocalAngle(biker->GetBones()[10]->GetLocalAngle() + Vec3(0.0f, 1.0f, 0.0f));

		Float32 angle = 2.0f;
		Float32 speed = 0.2f;
		{
			if(KeyState(Keys::L_SHIFT))
			{
				speed = 5.0f;
			}
			if(KeyState(Keys::L_ALT))
			{
				speed = 0.05f;
			}
			if(KeyState(Keys::UP))
			{
				camera->Rotate(Vec3(+1, 0, 0)*angle);
			}
			if(KeyState(Keys::DOWN))
			{
				camera->Rotate(Vec3(-1, 0, 0)*angle);
			}
			if(KeyState(Keys::RIGHT))
			{
				camera->Rotate(Vec3(0, +1, 0)*angle);
			}
			if(KeyState(Keys::LEFT))
			{
				camera->Rotate(Vec3(0, -1, 0)*angle);
			}
			if(KeyState(Keys::E))
			{
				camera->Rotate(Vec3(0, 0, +1)*angle);
			}
			if(KeyState(Keys::Q))
			{
				camera->Rotate(Vec3(0, 0, -1)*angle);
			}
			if(KeyState(Keys::W))
			{
				camera->Move(Vec3(0, 0, +1)*speed);
			}
			if(KeyState(Keys::S))
			{
				camera->Move(Vec3(0, 0, -1)*speed);
			}
			if(KeyState(Keys::D))
			{
				camera->Move(Vec3(+1, 0, 0)*speed);
			}
			if(KeyState(Keys::A))
			{
				camera->Move(Vec3(-1, 0, 0)*speed);
			}
			if(KeyState(Keys::SPACE))
			{
				camera->Move(Vec3(0, +1, 0)*speed);
			}
			if(KeyState(Keys::L_CTRL))
			{
				camera->Move(Vec3(0, -1, 0)*speed);
			}
		}

		scene->Render(camera);

		deviceContext->SwapBuffers();
	}
}

int WINAPI				WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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

	return 0;
}
