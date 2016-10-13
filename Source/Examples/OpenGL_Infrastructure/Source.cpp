#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Input/Input.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;
using namespace GVE::Graphics;


void func()
{
	WinAPI::Window::Size resolution(800, 600);
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
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 1000.0f));
	}

	auto geometry1 = Geometry::CreateTorus(8.0f, 2.0f, Vec2(8.0f, 2.0f), UVec2(128, 24));
	auto geometry2 = Geometry::CreateBox(Vec3(200.0f, 1.0f, 200.0f), Vec3(10.0f, 0.1f, 10.0f), UVec3(50));


	auto engine = MakeReference(new OpenGL::Engine(deviceContext));
	
	auto shape1 = MakeReference(new OpenGL::Shape(engine, geometry1));
	auto shape2 = MakeReference(new OpenGL::Shape(engine, geometry2));
	auto material = MakeReference(new OpenGL::Material(engine));
	{
		material->Technique(OpenGL::Material::TechniqueType::Basic) = OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/1."), "vs", "", "", "", "fs");
		material->Map(OpenGL::Material::MapType::Color)			= OpenGL::Map::Load2D(engine, Filepath("Media/Images/Tiles1_D.png"));
		material->Map(OpenGL::Material::MapType::Topology)		= OpenGL::Map::Load2D(engine, Filepath("Media/Images/Tiles1_T.png"));
		material->Map(OpenGL::Material::MapType::Reflections)	= OpenGL::Map::Load2D(engine, Filepath("Media/Images/Tiles1_R.png"));
	}


	auto model1 = MakeReference(new OpenGL::Model(engine, shape1, material));
	auto model2 = MakeReference(new OpenGL::Model(engine, shape2, material));

	auto object1 = MakeReference(new OpenGL::Object());
	{
		object1->SetModel(model1);
	}
	auto object2 = MakeReference(new OpenGL::Object());
	{
		object2->SetLocalPosition(Vec3(0.0f, -5.0f, 0.0f));
		object2->SetModel(model2);
	}

	auto lightDirection1 = MakeReference(new OpenGL::Lights::Direction());
	{
		// lightDirection1->Hide();
		lightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		lightDirection1->SetColor(Vec4(Vec3(1.0f), 0.4f));
		lightDirection1->SetAmbient(0.2f);
	}
	auto lightPoint1 = MakeReference(new OpenGL::Lights::Point());
	{
		// lightPoint1->Hide();
		lightPoint1->SetLocalPosition(Vec3(20.0f, 0.0f, 0.0f));
		lightPoint1->SetColor(Vec4(normalize(Rnd3()), 1.0f));
		lightPoint1->SetRangeFar(50.0f);
	}
	auto lightSpot1 = MakeReference(new OpenGL::Lights::Spot());
	{
		lightSpot1->Hide();
		lightSpot1->SetColor(Vec4(normalize(Rnd3()), 1.0f));
	}

	auto environmentCubemap1 = OpenGL::Environments::Cubemap::LoadCube(engine, Filepath("Media/Images/Rocks.dds"));
	{
		// environmentCubemap1->Hide();
		environmentCubemap1->SetLocalScale(Vec3(100.0f));
		environmentCubemap1->SetLocalPosition(Vec3(0.0f, 50.0f, 0.0f));
		environmentCubemap1->SetColor(Vec4(Vec3(1.0f), 1.0f));
	}

	auto scene = MakeReference(new OpenGL::Scene(engine, window->GetSize()));
	{
		// scene->Add(object1);
		scene->Add(object2);
		scene->Add(lightDirection1);
		scene->Add(lightPoint1);
		scene->Add(lightSpot1);
		scene->Add(environmentCubemap1);
	}


	// auto geometryBlank = Geometry::CreateTorus(3.0f, 1.0f, Vec2(4, 1), UVec2(128, 24));
	auto geometryBlank = Geometry::CreateSphere(5.0f, Vec2(2.0f), UVec2(24));
	{
		for(Size x = 0; x <= 5; ++x)
		for(Size y = 0; y <= 5; ++y)
		for(Size z = 0; z <= 5; ++z)
		{
			auto material = MakeReference(new OpenGL::Material(engine));
			{
				material->Technique(Material::TechniqueType::Basic) = OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Blank/Basic/1."), "vs", "", "", "", "fs");
				material->SetValue("materialColor", Vec3(1.0f));
				material->SetValue("materialSpecular", Mix(Vec3(1.0f), Vec3(208.0f, 109.0f, 81.0f)/255.0f, (Float32)y / 5.0f));
				material->SetValue("materialGloss", (Float32)z / 5.0f);
				material->SetValue("materialRoughness", (Float32)x / 5.0f);
			}

			auto shape = MakeReference(new OpenGL::Shape(engine, geometryBlank));
			auto model = MakeReference(new OpenGL::Model(engine, shape, material));

			auto object = MakeReference(new OpenGL::Object());
			{
				object->SetLocalPosition(Vec3(x*20.0f - 50.0f, y*20.0f, z*20.0f - 50.0f));
				object->SetModel(model);
			}

			scene->Add(object);
		}
	}

	Array<Float32, 10> stage;
	Array<Float32, 10> speed;
	Array<Vec3, 10> prevPos;
	Array<Vec3, 10> nextPos;
	Array<Reference<OpenGL::Lights::Point>, 10> light;
	{
		for(Size i = 0; i < stage.size(); ++i)
		{
			light[i] = MakeReference(new OpenGL::Lights::Point());
			// light[i]->Hide();
			light[i]->SetLocalPosition(Vec3(20.0f, 0.0f, 0.0f));
			light[i]->SetColor(Vec4(normalize(Rnd3()), 1.0f));
			light[i]->SetRangeFar(50.0f);
			light[i]->SetAmbient(0.5f);
			scene->Add(light[i]);

			stage[i] = Rnd(1.0f);
			speed[i] = 0.001f + Rnd(0.01f);
			prevPos[i] = Rnd3(120.0f) - Vec3(50.0f, 0.0f, 50.0f);
			nextPos[i] = prevPos[i];
		}
	}

	while(!KeyState(Keys::ESC))
	{
		Input::Keyboard::Loop();
		window->Loop();

		for(Size i = 0; i < stage.size(); ++i)
		{
			stage[i] += speed[i];
			if(stage[i] >= 1.0f)
			{
				prevPos[i] = nextPos[i];
				nextPos[i] = Rnd3(120.0f) - Vec3(50.0f, 0.0f, 50.0f);
				stage[i] = 0.0f;
				speed[i] = 0.001f + Rnd(0.01f);
			}
			light[i]->SetLocalPosition(Mix(prevPos[i], nextPos[i], stage[i]));
		}

		Float32 speed = 0.2f;
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
			camera->Rotate(Vec3(+1, 0, 0)*1.0f);
		}
		if(KeyState(Keys::DOWN))
		{
			camera->Rotate(Vec3(-1, 0, 0)*1.0f);
		}
		if(KeyState(Keys::RIGHT))
		{
			camera->Rotate(Vec3(0, +1, 0)*1.0f);
		}
		if(KeyState(Keys::LEFT))
		{
			camera->Rotate(Vec3(0, -1, 0)*1.0f);
		}
		if(KeyState(Keys::E))
		{
			camera->Rotate(Vec3(0, 0, +1)*1.0f);
		}
		if(KeyState(Keys::Q))
		{
			camera->Rotate(Vec3(0, 0, -1)*1.0f);
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
		/*if(KeyState(Keys::UP))
		{
			object2->LocalRotate(Vec3(+1, 0, 0)*1.0f);
		}
		if(KeyState(Keys::DOWN))
		{
			object2->LocalRotate(Vec3(-1, 0, 0)*1.0f);
		}
		if(KeyState(Keys::RIGHT))
		{
			object2->LocalRotate(Vec3(0, +1, 0)*1.0f);
		}
		if(KeyState(Keys::LEFT))
		{
			object2->LocalRotate(Vec3(0, -1, 0)*1.0f);
		}
		if(KeyState(Keys::E))
		{
			object2->LocalRotate(Vec3(0, 0, +1)*1.0f);
		}
		if(KeyState(Keys::Q))
		{
			object2->LocalRotate(Vec3(0, 0, -1)*1.0f);
		}
		if(KeyState(Keys::W))
		{
			object2->LocalMove(Vec3(0, 0, +1)*0.1f);
		}
		if(KeyState(Keys::S))
		{
			object2->LocalMove(Vec3(0, 0, -1)*0.1f);
		}
		if(KeyState(Keys::D))
		{
			object2->LocalMove(Vec3(+1, 0, 0)*0.1f);
		}
		if(KeyState(Keys::A))
		{
			object2->LocalMove(Vec3(-1, 0, 0)*0.1f);
		}
		if(KeyState(Keys::SPACE))
		{
			object2->LocalMove(Vec3(0, +1, 0)*0.1f);
		}
		if(KeyState(Keys::L_CTRL))
		{
			object2->LocalMove(Vec3(0, -1, 0)*0.1f);
		}*/

		object1->SetLocalAngle(object1->GetLocalAngle() + Vec3(0.5f, 1.0f, 0.25f));

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
