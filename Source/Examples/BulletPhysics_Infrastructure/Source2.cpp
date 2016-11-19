#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>
#include <Input/Input.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;


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

	auto physicsEngine = MakeReference(new Physics::BulletPhysics::Engine);
	auto physicsWorld = MakeReference(new Physics::BulletPhysics::World(physicsEngine));

	auto physicsShape = Physics::BulletPhysics::Shape::CreateGeometry(Geometry::CreateBox(Vec3(400.0f, 1.0f, 400.0f), Vec3(1.0f), UVec3(1)), 0.0f);
	auto physicsBodyRigid = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(physicsShape));
	{
		physicsWorld->Add(physicsBodyRigid);
	}

	auto graphicsCamera = MakeReference(new Graphics::Camera());
	{
		graphicsCamera->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}

	auto graphicsEngine = MakeReference(new Graphics::OpenGL::Engine(deviceContext));

	auto graphicsScene = MakeReference(new Graphics::OpenGL::Scene(graphicsEngine, window->GetSize()));

	auto graphicsLightDirection1 = MakeReference(new Graphics::OpenGL::Lights::Direction());
	{
		graphicsLightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		graphicsLightDirection1->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLightDirection1->SetAmbient(0.5f);
		graphicsScene->Add(graphicsLightDirection1);
	}

	auto graphicsEnvironmentGlobalmap1 = Graphics::OpenGL::Environments::Globalmap::LoadCube(graphicsEngine, Filepath("Media/Images/Rocks.dds"));
	{
		graphicsScene->Add(graphicsEnvironmentGlobalmap1);
	}

	auto graphicsMaterialFlat = MakeReference(new Graphics::OpenGL::Material(graphicsEngine));
	{
		graphicsMaterialFlat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		graphicsMaterialFlat->SetValue("materialColor", Vec3(1.0f));
		graphicsMaterialFlat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		graphicsMaterialFlat->SetValue("materialGloss", 0.5f);
		graphicsMaterialFlat->SetValue("materialRoughness", 0.5f);
	}
	auto graphicsObject = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(400.0f, 1.0f, 400.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject->SetModel(model);
		graphicsScene->Add(graphicsObject);
		physicsBodyRigid->SetUser(graphicsObject);
	}

	// auto physicsShape2 = Physics::BulletPhysics::Shape::CreateBox(Vec3(10.0f), 100.0f);
	auto physicsShape2 = Physics::BulletPhysics::Shape::CreateCapsule(5.0f, 10.0f, 100.0f);
	auto physicsBodyRigid2 = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(physicsShape2, Vec3(0.0f, 50.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)));
	{
		// physicsBodyRigid2->SetAngleFactor(Vec3(0.0f, 0.0f, 0.0f));
		physicsWorld->Add(physicsBodyRigid2);
	}
	auto graphicsObject2 = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateCapsule(5.0f, 10.0f, Vec2(2.0f, 3.0f), UVec2(32, 16));
		// auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject2->SetModel(model);
		graphicsScene->Add(graphicsObject2);
		physicsBodyRigid2->SetUser(graphicsObject2);
	}

	/*
	for(int i = 0; i < 50; ++i)
	{
		auto physicsShape = Physics::BulletPhysics::Shape::CreateBox(Vec3(10.0f), 100.0f);
		auto physicsBodyRigid = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(
			physicsShape,
			Vec3(Rnd(-40.0f, +40.0f), 50.0f + Rnd(100.0f), Rnd(-40.0f, +40.0f)),
			Vec3(Rnd(360.0f), 0.0f, 0.0f)));
		{
			physicsWorld->Add(physicsBodyRigid);
		}
		auto graphicsObject = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
		{
			auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(1));
			auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
			auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
			graphicsObject->SetModel(model);
			graphicsScene->Add(graphicsObject);
			physicsBodyRigid->SetUser(graphicsObject);
		}
	}
	for(int i = 0; i < 50; ++i)
	{
		auto physicsShape = Physics::BulletPhysics::Shape::CreateBox(Vec3(10.0f), 100.0f);
		auto physicsBodyRigid = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(
			physicsShape,
			RotateY3(i*10.0f) * Vec3(0.0f, 100.0f + i*5.0f, 100.0f),
			Vec3(0.0f, i*10.0f, 0.0f)));
		{
			physicsWorld->Add(physicsBodyRigid);
		}
		auto graphicsObject = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
		{
			auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(1));
			auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
			auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
			graphicsObject->SetModel(model);
			graphicsScene->Add(graphicsObject);
			physicsBodyRigid->SetUser(graphicsObject);
		}
	}
	*/
	/*auto sponza = Graphics::OpenGL::Object::Load(graphicsEngine, Filepath("Media/Models/Sponza.gvs"), graphicsMaterialFlat);
	{
		graphicsScene->Add(sponza);
	}*/

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();
		Input::Mouse::SetPosition(Vec2(Input::Mouse::GetDesktopSize()) / 2.0f);

		if(KeyState(Keys::NUM_DIGIT6))
		{
			physicsBodyRigid2->AddCentralImpulse(Vec3(+100.0f, 0.0f, 0.0f));
		}
		if(KeyState(Keys::NUM_DIGIT4))
		{
			physicsBodyRigid2->AddCentralImpulse(Vec3(-100.0f, 0.0f, 0.0f));
		}
		// if(KeyState(Keys::NUM_DIGIT8))
		// {
		// 	physicsBodyRigid2->AddCentralImpulse(Vec3(0.0f, 0.0f, 100.0f));
		// }
		// if(KeyState(Keys::NUM_DIGIT2))
		// {
		// 	physicsBodyRigid2->AddCentralImpulse(Vec3(0.0f, 0.0f, -100.0f));
		// }

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
				graphicsCamera->Rotate(Vec3(+1, 0, 0)*angle);
			}
			if(KeyState(Keys::DOWN))
			{
				graphicsCamera->Rotate(Vec3(-1, 0, 0)*angle);
			}
			if(KeyState(Keys::RIGHT))
			{
				graphicsCamera->Rotate(Vec3(0, +1, 0)*angle);
			}
			if(KeyState(Keys::LEFT))
			{
				graphicsCamera->Rotate(Vec3(0, -1, 0)*angle);
			}
			if(KeyState(Keys::E))
			{
				graphicsCamera->Rotate(Vec3(0, 0, +1)*angle);
			}
			if(KeyState(Keys::Q))
			{
				graphicsCamera->Rotate(Vec3(0, 0, -1)*angle);
			}
			if(KeyState(Keys::W))
			{
				graphicsCamera->Move(Vec3(0, 0, +1)*speed);
			}
			if(KeyState(Keys::S))
			{
				graphicsCamera->Move(Vec3(0, 0, -1)*speed);
			}
			if(KeyState(Keys::D))
			{
				graphicsCamera->Move(Vec3(+1, 0, 0)*speed);
			}
			if(KeyState(Keys::A))
			{
				graphicsCamera->Move(Vec3(-1, 0, 0)*speed);
			}
			if(KeyState(Keys::SPACE))
			{
				graphicsCamera->Move(Vec3(0, +1, 0)*speed);
			}
			if(KeyState(Keys::L_CTRL))
			{
				graphicsCamera->Move(Vec3(0, -1, 0)*speed);
			}
		}

		graphicsCamera->Rotate(Vec3(
			-Input::Mouse::GetPositionDelta().y * 0.1f,
			Input::Mouse::GetPositionDelta().x * 0.1f ,
			0.0f));

		physicsWorld->Render(1.0f / 60.0f);
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
