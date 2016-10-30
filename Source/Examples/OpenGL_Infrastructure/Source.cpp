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
	// WinAPI::Window::Size resolution(1920, 1080);

	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference(new GreatVEngine::WinAPI::WindowClass(instance, "class"));
	auto window = MakeReference(new GreatVEngine::WinAPI::Window(windowClass, "window", resolution));
	auto deviceContext = MakeReference(new GreatVEngine::WinAPI::DeviceContext(window));
	{
		deviceContext->SetPixelFormat();
	}


	auto uiItem = new UI::Item();
	{
		uiItem->SetSize(Vec2(100.0f, 200.0f));
		uiItem->Add(MakeReference(new UI::Item()));
	}

	auto camera = MakeReference(new Graphics::Camera());
	{
		camera->SetPosition(Vec3(0.0f, 8.0f, -20.0f));
		camera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
		// camera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Orthogonal(-40.0f, +40.0f, -30.0f, +30.0f, 0.0f, 1000.0f));
	}

	auto geometry1 = Geometry::CreateTorus(8.0f, 2.0f, Vec2(8.0f, 2.0f), UVec2(128, 24));
	auto geometry2 = Geometry::CreateBox(Vec3(200.0f, 1.0f, 200.0f), Vec3(10.0f, 0.1f, 10.0f), UVec3(50));


	auto engine = MakeReference(new OpenGL::Engine(deviceContext));
	
	auto shape1 = MakeReference(new OpenGL::Shape(engine, geometry1));
	auto shape2 = MakeReference(new OpenGL::Shape(engine, geometry2));
	auto material = MakeReference(new OpenGL::Material(engine));
	{
		material->Technique(OpenGL::Material::TechniqueType::Basic) = OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Simple/Basic/1."), "vs", "", "", "", "fs");
		material->Map(OpenGL::Material::MapType::Color)			= OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Brick1_D.png"));
		material->Map(OpenGL::Material::MapType::Topology)		= OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Brick1_T.png"));
		material->Map(OpenGL::Material::MapType::Reflections)	= OpenGL::Map::Load2D(engine, Filepath("Media/Images/Materials/Brick1_R.png"));
	}


	auto model1 = MakeReference(new OpenGL::Model(engine, shape1, material));
	auto model2 = MakeReference(new OpenGL::Model(engine, shape2, material));

	auto object1 = MakeReference(new OpenGL::Object());
	{
		object1->Hide();
		object1->SetModel(model1);
	}
	auto object2 = MakeReference(new OpenGL::Object());
	{
		object2->Hide();
		object2->SetLocalPosition(Vec3(0.0f, -5.0f, 0.0f));
		object2->SetModel(model2);
	}

	auto lightDirection1 = MakeReference(new OpenGL::Lights::Direction());
	{
		// lightDirection1->Hide();
		lightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		lightDirection1->SetColor(Vec4(Vec3(1.0f), 0.8f));
		lightDirection1->SetAmbient(0.5f);
	}
	auto lightPoint1 = MakeReference(new OpenGL::Lights::Point());
	{
		// lightPoint1->Hide();
		lightPoint1->SetLocalPosition(Vec3(20.0f, 0.0f, 0.0f));
		lightPoint1->SetColor(Vec4(normalize(Rnd3()), 1.0f));
		lightPoint1->SetRangeFar(50.0f);
	}
	auto lightPoint2 = MakeReference(new OpenGL::Lights::Point());
	{
		// lightPoint2->Hide();
		lightPoint2->SetLocalPosition(Vec3(-20.0f, 30.0f, -10.0f));
		lightPoint2->SetColor(Vec4(normalize(Rnd3()), 1.0f));
		lightPoint2->SetRangeFar(50.0f);
	}
	auto lightSpot1 = MakeReference(new OpenGL::Lights::Spot());
	{
		lightSpot1->Hide();
		lightSpot1->SetColor(Vec4(normalize(Rnd3()), 1.0f));
	}

	auto environmentCubemap1 = OpenGL::Environments::Cubemap::LoadCube(engine, Filepath("Media/Images/Rocks.dds"));
	{
		environmentCubemap1->SetLocalPosition(Vec3(-80.0f, 0.0f, 0.0f));
		// environmentCubemap1->Hide();
		environmentCubemap1->SetRange(Vec3(100.0f));
	}
	auto environmentCubemap2 = OpenGL::Environments::Cubemap::LoadCube(engine, Filepath("Media/Images/Cubemap1.dds"));
	{
		environmentCubemap2->SetLocalPosition(Vec3(+80.0f, 0.0f, 0.0f));
		// environmentCubemap2->Hide();
		// environmentCubemap2->SetRangeNear(Vec3(99.8f));
		// environmentCubemap2->SetRangeMiddle(Vec3(99.9f));
		// environmentCubemap2->SetRangeFar(Vec3(100.0f));
		environmentCubemap2->SetRange(Vec3(100.0f));
	}
	auto environmentGlobalmap1 = OpenGL::Environments::Globalmap::LoadCube(engine, Filepath("Media/Images/Cubemap2.dds"));
	{
		// environmentGlobalmap1->Hide();
	}

	auto decal1 = MakeReference(new OpenGL::Decal());
	{
		decal1->Hide();
		decal1->SetLocalPosition(Vec3(0.0f, 0.0f, 10.0f));
		decal1->SetLocalAngle(Vec3(90.0f, 0.0f, 0.0f));
		decal1->SetLocalScale(Vec3(10.0f, 10.0f, 10.0f));
	}
	auto decal2 = MakeReference(new OpenGL::Decal());
	{
		decal2->Hide();
		decal2->SetLocalPosition(Vec3(10.0f, 0.0f, 10.0f));
		decal2->SetLocalAngle(Vec3(90.0f, 30.0f, 0.0f));
		decal2->SetLocalScale(Vec3(15.0f, 15.0f, 10.0f));
		decal2->SetPriority(5);
		decal2->SetGroup(1);
	}

	auto scene = MakeReference(new OpenGL::Scene(engine, window->GetSize()));
	{
		scene->LoadDecals(
			Filepath("Media/Images/Decals/GlassHole_Albedo.png"),
			Filepath("Media/Images/Decals/GlassHole_Topology.png"),
			Filepath("Media/Images/Decals/GlassHole_Material.png"));

		scene->Add(object1);
		scene->Add(object2);
		scene->Add(lightDirection1);
		scene->Add(lightPoint1);
		scene->Add(lightPoint2);
		scene->Add(lightSpot1);
		scene->Add(environmentCubemap1);
		scene->Add(environmentCubemap2);
		scene->Add(environmentGlobalmap1);
		scene->Add(decal1);
		scene->Add(decal2);
	}

	auto materialFlat = MakeReference(new OpenGL::Material(engine));
	{
		materialFlat->Technique(Material::TechniqueType::Basic) = OpenGL::Technique::Load(engine, Filepath("Media/Shaders/Materials/Blank/Basic/1."), "vs", "", "", "", "fs");
		materialFlat->SetValue("materialColor", Vec3(1.0f));
		materialFlat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		materialFlat->SetValue("materialGloss", 0.6f);
		materialFlat->SetValue("materialRoughness", 0.15f);
	}
	auto bunny = OpenGL::Object::Load(Filepath("Media/Models/Bunny.gvs"), materialFlat);
	{
		bunny->SetLocalPosition(Vec3(-15.0f, 0.0f, 0.0f));
		scene->Add(bunny);
	}
	auto buddha = OpenGL::Object::Load(Filepath("Media/Models/Buddha.gvs"), materialFlat);
	{
		buddha->SetLocalPosition(Vec3(-5.0f, 0.0f, 0.0f));
		scene->Add(buddha);
	}
	auto dragon = OpenGL::Object::Load(Filepath("Media/Models/Dragon.gvs"), materialFlat);
	{
		dragon->SetLocalPosition(Vec3(+5.0f, 0.0f, 0.0f));
		scene->Add(dragon);
	}
	auto greece = OpenGL::Object::Load(Filepath("Media/Models/Greece.gvs"), materialFlat);
	{
		greece->SetLocalPosition(Vec3(+15.0f, 0.0f, 0.0f));
		scene->Add(greece);
	}
	auto mitsuba = OpenGL::Object::Load(Filepath("Media/Models/Mitsuba.gvs"), materialFlat);
	{
		mitsuba->SetLocalPosition(Vec3(+25.0f, 0.0f, 0.0f));
		mitsuba->SetLocalScale(Vec3(4.0f));
		scene->Add(mitsuba);
	}
	auto sportsCar = OpenGL::Object::Load(Filepath("Media/Models/SportsCar.gvs"), materialFlat);
	{
		sportsCar->SetLocalPosition(Vec3(-25.0f, 0.0f, 0.0f));
		sportsCar->SetLocalScale(Vec3(5.0f));
		scene->Add(sportsCar);
	}
	auto head = OpenGL::Object::Load(Filepath("Media/Models/Head.gvs"), materialFlat);
	{
		head->SetLocalPosition(Vec3(+35.0f, 0.0f, 0.0f));
		scene->Add(head);
	}
	/*auto sibenik = OpenGL::Object::Load(Filepath("Media/Models/Sibenik.gvs"), materialFlat);
	{
		sibenik->SetLocalPosition(Vec3(0.0f, 0.0f, 500.0f));
		sibenik->SetLocalScale(Vec3(10.0f));
		scene->Add(sibenik);
	}*/
	auto sponza = OpenGL::Object::Load(Filepath("Media/Models/Sponza.gvs"), materialFlat);
	{
		scene->Add(sponza);
	}

	// auto geometryBlank = Geometry::CreateTorus(3.0f, 1.0f, Vec2(4, 1), UVec2(128, 24));
	auto geometryBlank = Geometry::CreateSphere(2.0f, Vec2(2.0f), UVec2(24));
	// auto geometryBlank = Geometry::LoadMesh(Filepath("Media/Models/Teapot01-lib.gvm"));
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
				object->SetLocalPosition(Vec3(x*10.0f - 25.0f, 40.0f + y*10.0f, z*5.0f - 12.5f));
				// object->SetLocalScale(Vec3(5.0f));
				object->SetModel(model);
				if(x%2 == 0) 
					object->SetGroup(0);
				else
					object->ResetGroup(0);

				if(z % 2 == 0)
					object->SetGroup(1);
				else
					object->ResetGroup(1);
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
	
	float ang = 0.0f;

	while(!KeyState(Keys::ESC))
	{
		Input::Keyboard::Loop();
		window->Loop();

		/*
		if(KeyState(Keys::ENTER))
		{
			if(KeyState(Keys::NUM_DIGIT1)) uiItem->SetCenter(UI::Item::Homogeneous::Side::LeftBottom);
			if(KeyState(Keys::NUM_DIGIT2)) uiItem->SetCenter(UI::Item::Homogeneous::Side::Bottom);
			if(KeyState(Keys::NUM_DIGIT3)) uiItem->SetCenter(UI::Item::Homogeneous::Side::RightBottom);
			if(KeyState(Keys::NUM_DIGIT4)) uiItem->SetCenter(UI::Item::Homogeneous::Side::Left);
			if(KeyState(Keys::NUM_DIGIT5)) uiItem->SetCenter(UI::Item::Homogeneous::Side::Center);
			if(KeyState(Keys::NUM_DIGIT6)) uiItem->SetCenter(UI::Item::Homogeneous::Side::Right);
			if(KeyState(Keys::NUM_DIGIT7)) uiItem->SetCenter(UI::Item::Homogeneous::Side::LeftTop);
			if(KeyState(Keys::NUM_DIGIT8)) uiItem->SetCenter(UI::Item::Homogeneous::Side::Top);
			if(KeyState(Keys::NUM_DIGIT9)) uiItem->SetCenter(UI::Item::Homogeneous::Side::RightTop);
		}
		else
		{
			if(KeyState(Keys::NUM_DIGIT1)) uiItem->SetDocking(UI::Item::Homogeneous::Side::LeftBottom);
			if(KeyState(Keys::NUM_DIGIT2)) uiItem->SetDocking(UI::Item::Homogeneous::Side::Bottom);
			if(KeyState(Keys::NUM_DIGIT3)) uiItem->SetDocking(UI::Item::Homogeneous::Side::RightBottom);
			if(KeyState(Keys::NUM_DIGIT4)) uiItem->SetDocking(UI::Item::Homogeneous::Side::Left);
			if(KeyState(Keys::NUM_DIGIT5)) uiItem->SetDocking(UI::Item::Homogeneous::Side::Center);
			if(KeyState(Keys::NUM_DIGIT6)) uiItem->SetDocking(UI::Item::Homogeneous::Side::Right);
			if(KeyState(Keys::NUM_DIGIT7)) uiItem->SetDocking(UI::Item::Homogeneous::Side::LeftTop);
			if(KeyState(Keys::NUM_DIGIT8)) uiItem->SetDocking(UI::Item::Homogeneous::Side::Top);
			if(KeyState(Keys::NUM_DIGIT9)) uiItem->SetDocking(UI::Item::Homogeneous::Side::RightTop);
		}

		if(KeyState(Keys::NUM_MUL)) uiItem->SetPosition(uiItem->GetPosition() + Vec2(1.0f, 0.0f));
		if(KeyState(Keys::NUM_DIV)) uiItem->SetPosition(uiItem->GetPosition() - Vec2(1.0f, 0.0f));
		if(KeyState(Keys::NUM_ADD)) uiItem->SetAngle(uiItem->GetAngle() + 1.0f);
		if(KeyState(Keys::NUM_SUB)) uiItem->SetAngle(uiItem->GetAngle() - 1.0f);

		if(KeyState(Keys::PLUS)) ang += 1.0f;
		if(KeyState(Keys::MINUS)) ang -= 1.0f;

		auto canvas = Vec2(resolution) / 2.0f;
		auto transformation =
			Move3(canvas)*
			Rotate3(ang)*
			Scale3(canvas / 2.0f);

		auto leftBottom		= VecXY(transformation * Vec3(-1.0f, -1.0f, 1.0f));
		auto rightBottom	= VecXY(transformation * Vec3(+1.0f, -1.0f, 1.0f));
		auto leftTop		= VecXY(transformation * Vec3(-1.0f, +1.0f, 1.0f));
		auto rightTop		= VecXY(transformation * Vec3(+1.0f, +1.0f, 1.0f));

		scene->DrawLine(leftBottom, rightBottom);
		scene->DrawLine(leftTop, rightTop);
		scene->DrawLine(leftBottom, leftTop);
		scene->DrawLine(rightBottom, rightTop);

		uiItem->Draw([&](UI::Item* item, const Mat3& transformation_) {
			auto leftBottom		= VecXY(transformation_ * Vec3(-1.0f, -1.0f, 1.0f));
			auto rightBottom	= VecXY(transformation_ * Vec3(+1.0f, -1.0f, 1.0f));
			auto leftTop		= VecXY(transformation_ * Vec3(-1.0f, +1.0f, 1.0f));
			auto rightTop		= VecXY(transformation_ * Vec3(+1.0f, +1.0f, 1.0f));

			scene->DrawLine(leftBottom, rightBottom);
			scene->DrawLine(leftTop, rightTop);
			scene->DrawLine(leftBottom, leftTop);
			scene->DrawLine(rightBottom, rightTop);
		}, transformation, canvas);
		*/

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

		Float32 angle = 2.0f;
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
