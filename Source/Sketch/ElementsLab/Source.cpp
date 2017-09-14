#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>
#include <Input/Input.hpp>
#include <Geometry/Collision.hpp>
#include <APIs/WinAPI/Window.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;


GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


void func()
{
	using namespace Graphics::OpenGL;

	// WinAPI::Window::Size resolution(800, 600);
	WinAPI::Window::Size resolution(1280, 600);
	// WinAPI::Window::Size resolution(1920, 1080);

	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference<GreatVEngine::WinAPI::WindowClass>(instance, "class");
	auto window = MakeReference<GreatVEngine::WinAPI::Window>(windowClass, "window", resolution);
	auto deviceContext = MakeReference<GreatVEngine::WinAPI::DeviceContext>(window);
	{
		deviceContext->SetPixelFormat();
	}

	auto camera = MakeReference<Graphics::Camera>();
	{
		camera->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		camera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}
	auto engine = MakeReference<Engine>(deviceContext);
	auto scene = MakeReference<Scene>(engine, resolution);

	auto sunlight = MakeReference<Lights::Direction>();
	{
		sunlight->SetLocalAngle(Vec3(60.0f, 0.0f, 0.0f));
		sunlight->SetColor(Vec4(Vec3(1.0f), 0.8f));
		sunlight->SetAmbient(0.5f);

		scene->Add(sunlight);
	}
	/*auto reflectedSunlight = MakeReference<Lights::Direction>();
	{
		reflectedSunlight->SetLocalAngle(GetAngle(sunlight->GetRMat() * RotateX3(180.0f)));
		reflectedSunlight->SetColor(Vec4(Vec3(0.62f, 0.68f, 1.0f), 0.25f));
		reflectedSunlight->SetAmbient(0.4f);

		scene->Add(reflectedSunlight);
	}*/
	auto pointLight1 = MakeReference<Lights::Point>();
	{
		pointLight1->SetLocalPosition(Vec3(50.0f, 10.0f, 0.0f));
		pointLight1->SetRangeNear(1.0f);
		pointLight1->SetRangeMiddle(2.0f);
		pointLight1->SetRangeFar(40.0f);
		pointLight1->SetColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
		pointLight1->SetAmbient(0.4f);

		scene->Add(pointLight1);
	}
	auto pointLight2 = MakeReference<Lights::Point>();
	{
		pointLight2->SetLocalPosition(Vec3(-50.0f, 10.0f, 0.0f));
		pointLight2->SetRangeNear(1.0f);
		pointLight2->SetRangeMiddle(2.0f);
		pointLight2->SetRangeFar(40.0f);
		pointLight2->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		pointLight2->SetAmbient(0.4f);

		scene->Add(pointLight2);
	}
	auto environment = Environments::Globalmap::LoadCube(engine, Filepath("Media/Images/Rocks.dds"));
	{
		scene->Add(environment);
	}

	auto materialFlat = MakeReference<Material>(engine);
	{
		materialFlat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		materialFlat->SetValue("materialAlbedo", Vec3(1.0f));
		materialFlat->SetValue("materialRoughness", 0.5f);
		materialFlat->SetValue("materialMetalness", 0.0f);
	}
	auto materialCement = MakeReference<Material>(engine);
	{
		materialCement->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		materialCement->Map(Graphics::Material::MapType::Albedo)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Cement/Albedo.png"));
		materialCement->Map(Graphics::Material::MapType::Normals)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Cement/Normals.png"));
		materialCement->Map(Graphics::Material::MapType::Height)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Cement/Height.png"));
		materialCement->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Cement/Occlusion.png"));
		materialCement->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Cement/Roughness.png"));
		materialCement->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Cement/Metalness.png"));
	}
	auto materialAluminium = MakeReference<Material>(engine);
	{
		materialAluminium->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		materialAluminium->Map(Graphics::Material::MapType::Albedo)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Aluminium/Albedo.png"));
		materialAluminium->Map(Graphics::Material::MapType::Normals)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Aluminium/Normals.png"));
		materialAluminium->Map(Graphics::Material::MapType::Height)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Aluminium/Height.png"));
		materialAluminium->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Aluminium/Occlusion.png"));
		materialAluminium->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Aluminium/Roughness.png"));
		materialAluminium->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Aluminium/Metalness.png"));
	}
	auto materialRustedIron = MakeReference<Material>(engine);
	{
		materialRustedIron->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		materialRustedIron->Map(Graphics::Material::MapType::Albedo)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/RustedIron/Albedo.png"));
		materialRustedIron->Map(Graphics::Material::MapType::Normals)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/RustedIron/Normals.png"));
		materialRustedIron->Map(Graphics::Material::MapType::Height)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/RustedIron/Height.png"));
		materialRustedIron->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/RustedIron/Occlusion.png"));
		materialRustedIron->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/RustedIron/Roughness.png"));
		materialRustedIron->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/RustedIron/Metalness.png"));
	}
	auto materialGreasyMetal = MakeReference<Material>(engine);
	{
		materialGreasyMetal->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		materialGreasyMetal->Map(Graphics::Material::MapType::Albedo)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/GreasyMetal/Albedo.png"));
		materialGreasyMetal->Map(Graphics::Material::MapType::Normals)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/GreasyMetal/Normals.png"));
		materialGreasyMetal->Map(Graphics::Material::MapType::Height)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/GreasyMetal/Height.png"));
		materialGreasyMetal->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/GreasyMetal/Occlusion.png"));
		materialGreasyMetal->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/GreasyMetal/Roughness.png"));
		materialGreasyMetal->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/GreasyMetal/Metalness.png"));
	}
	auto materialDryDirt = MakeReference<Material>(engine);
	{
		materialDryDirt->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps(Tessellation).glsl."), "vs", "tc", "te", "", "fs");

		materialDryDirt->Map(Graphics::Material::MapType::Albedo)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/DryDirt/Albedo.png"));
		materialDryDirt->Map(Graphics::Material::MapType::Normals)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/DryDirt/Normals.png"));
		materialDryDirt->Map(Graphics::Material::MapType::Height)		= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/DryDirt/Height.png"));
		materialDryDirt->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/DryDirt/Occlusion.png"));
		materialDryDirt->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/DryDirt/Roughness.png"));
		materialDryDirt->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/DryDirt/Metalness.png"));
	}

	auto object = Object::Load(engine, Filepath("Media/Models/Demo/Scene.gvs"), materialFlat);
	{
		if(auto o = (*object)[3])
		{
			auto m = o->GetModel();
			o->SetModel(MakeReference<Model>(engine, m->GetShape(), materialCement));
		}
		if(auto o = (*object)[2])
		{
			auto m = o->GetModel();
			o->SetModel(MakeReference<Model>(engine, m->GetShape(), materialAluminium));
		}
		if(auto o = (*object)[4])
		{
			auto m = o->GetModel();
			o->SetModel(MakeReference<Model>(engine, m->GetShape(), materialCement));
		}
		if(auto o = (*object)[5])
		{
			auto m = o->GetModel();
			o->SetModel(MakeReference<Model>(engine, m->GetShape(), materialRustedIron));
		}
		if(auto o = (*object)[6])
		{
			auto m = o->GetModel();
			o->SetModel(MakeReference<Model>(engine, m->GetShape(), materialGreasyMetal));
		}
		if(auto o = (*object)[7])
		{
			auto m = o->GetModel();
			m->GetShape()->geometry->SetTopology(Geometry::Topology::Patches3);
			o->SetModel(MakeReference<Model>(engine, m->GetShape(), materialDryDirt));
		}

		scene->Add(object);
	}

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		for(Size i = 4; i < 8; ++i)
		{
			auto o = (*object)[i];

			o->LocalRotate(Vec3(0.2f, 0.15f, 0.05f));
		}

		Input::Mouse::SetPosition(Vec2(WinAPI::GetDesktopSize()) / 2.0f);
		Float32 motion = 0.1f * (KeyState(Keys::X) ? 0.1f : 1.0f);
		camera->Rotate(Vec3(
			-Input::Mouse::GetPositionDelta().y * motion,
			Input::Mouse::GetPositionDelta().x * motion,
			KeyState(Keys::Q) ? -2.0f : KeyState(Keys::E) ? +2.0f : 0.0f));

		Float32 speed = 1.0f * KeyState(Keys::L_SHIFT) ? 10.0f : KeyState(Keys::L_ALT) ? 0.1f : 1.0f;
		camera->Move(Vec3(
			KeyState(Keys::D) ? +speed : KeyState(Keys::A) ? -speed : 0.0f,
			KeyState(Keys::SPACE) ? +speed : KeyState(Keys::L_CTRL) ? -speed : 0.0f,
			KeyState(Keys::W) ? +speed : KeyState(Keys::S) ? -speed : 0.0f));

		// rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(10.0f, 0.0f, 0.0f), Vec4(1, 0, 0, 1));
		// rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(0.0f, 10.0f, 0.0f), Vec4(0, 1, 0, 1));
		// rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(0.0f, 0.0f, 10.0f), Vec4(0, 0, 1, 1));
		
		scene->Render(camera);

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