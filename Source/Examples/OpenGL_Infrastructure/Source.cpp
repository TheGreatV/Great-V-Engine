#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>

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
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, 800.0f/600.0f, 0.1f, 1000.0f));
	}

	auto geometry = Geometry::CreateTorus(8.0f, 2.0f, Vec2(8.0f, 2.0f), UVec2(128, 24));
	auto geometry2 = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(50));


	auto context = MakeReference(new OpenGL::WinAPI::ExtendedContext(deviceContext));
	{
		context->Set();
	}

	auto shape = MakeReference(new OpenGL::Shape(context, geometry));
	auto shape2 = MakeReference(new OpenGL::Shape(context, geometry2));
	auto material = MakeReference(new OpenGL::Material(context));
	{
		material->Technique(OpenGL::Material::TechniqueType::Basic) = OpenGL::Technique::Load(context, "../../../../../Media/Shaders/Test/1.", "vert", "", "", "", "frag");
		material->Map(OpenGL::Material::MapType::Color)			= OpenGL::Map::Load2D(context, "../../../../../Media/Images/Tiles1_D.png");
		material->Map(OpenGL::Material::MapType::Topology)		= OpenGL::Map::Load2D(context, "../../../../../Media/Images/Tiles1_T.png");
		material->Map(OpenGL::Material::MapType::Reflections)	= OpenGL::Map::Load2D(context, "../../../../../Media/Images/Tiles1_R.png");
	}

	auto model = MakeReference(new OpenGL::Model(context, shape, material));
	auto model2 = MakeReference(new OpenGL::Model(context, shape2, material));

	auto object = MakeReference(new OpenGL::Object());
	{
		object->SetModel(model);
	}
	auto object2 = MakeReference(new OpenGL::Object());
	{
		object2->SetModel(model2);
	}

	auto scene = MakeReference(new OpenGL::Scene(context));
	{
		scene->Add(object);
		scene->Add(object2);
	}

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		window->Loop();

		object->SetLocalAngle(object->GetLocalAngle() + Vec3(0.5f, 1.0f, 0.25f));

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
