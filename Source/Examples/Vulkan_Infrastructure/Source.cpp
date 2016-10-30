#include <iostream>


#include <Input/Input.hpp>
#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <Infrastructure/Vulkan.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;
using namespace GVE::Graphics;


void func()
{
	Log::Clear();

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
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 1000.0f));
	}

	auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(50));
	auto geometry2 = Geometry::CreateTorus(8.0f, 2.0f, Vec2(8.0f, 2.0f), UVec2(128, 24));


	auto engine = MakeReference(new Vulkan::Engine(instance->GetHangle(), window->GetHandle()));

	auto shape = MakeReference(new Vulkan::Shape(engine, geometry));
	auto shape2 = MakeReference(new Vulkan::Shape(engine, geometry2));

	auto material = MakeReference(new Vulkan::Material(engine));
	{
		material->Technique(Material::TechniqueType::Basic) =  Vulkan::Technique::Load(engine, Filepath("Media/Shaders/Vulkan_Infrastructure/1.spir-v."), "vs", "", "", "", "fs");
		// material->Technique(Material::TechniqueType::Basic) = Vulkan::Technique::Load(engine, "../../../../../Media/Shaders/TestV/1.", "vert.spv", "", "", "", "frag.spv");
		material->Map(Material::MapType::Color) = Vulkan::Map::Load2D(engine, Filepath("Media/Images/Tiles1_D.png"));
		material->Map(Material::MapType::Topology) = Vulkan::Map::Load2D(engine, Filepath("Media/Images/Tiles1_T.png"));
		material->Map(Material::MapType::Reflections) = Vulkan::Map::Load2D(engine, Filepath("Media/Images/Tiles1_R.png"));
	}

	auto model = MakeReference(new Vulkan::Model(engine, shape, material));
	auto model2 = MakeReference(new Vulkan::Model(engine, shape2, material));

	auto object = MakeReference(new Vulkan::Object);
	{
		object->SetModel(model);
	}
	auto object2 = MakeReference(new Vulkan::Object);
	{
		object2->SetModel(model2);
	}

	auto scene = MakeReference(new Vulkan::Scene(engine));
	{
		scene->Add(object);
		scene->Add(object2);
	}

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		window->Loop();
		Input::Keyboard::Loop();

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


		object2->SetLocalAngle(object2->GetLocalAngle() + Vec3(0.5f, 1.0f, 0.25f));

		scene->Render(camera);
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

	system("pause");
}