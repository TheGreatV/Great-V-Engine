#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>
#include <Input/Input.hpp>
#include <Geometry/Collision.hpp>

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
	{
		physicsWorld->SetGravity(Vec3(0.0f));
	}

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

	/*auto objectBunny = Graphics::OpenGL::Object::Load(graphicsEngine, Filepath("Media/Models/Scene.gvs"), graphicsMaterialFlat);
	auto limb = objectBunny->operator[](0);
	auto model = limb->GetModel();
	auto shape = model->GetShape();
	auto geometry = shape->geometry;

	auto file = fopen(Filepath("Media/Models/Scene.bin").c_str(), "wb");
	{
		UInt32 verticesCount = static_cast<UInt32>(geometry->GetVerticesCount());
		auto vertices = Vector<Float32>(verticesCount * (3 + 3*3 + 2));

		Size v = 0;
		for(auto &vertex : geometry->vertices)
		{
			auto id = v * (3 + 3*3 + 2);

			vertices[id + 0] = vertex.pos.x;
			vertices[id + 1] = vertex.pos.y;
			vertices[id + 2] = vertex.pos.z;

			vertices[id + 3] = vertex.tan.x;
			vertices[id + 4] = vertex.tan.y;
			vertices[id + 5] = vertex.tan.z;
			vertices[id + 6] = vertex.bin.x;
			vertices[id + 7] = vertex.bin.y;
			vertices[id + 8] = vertex.bin.z;
			vertices[id + 9] = vertex.nor.x;
			vertices[id + 10] = vertex.nor.y;
			vertices[id + 11] = vertex.nor.z;

			vertices[id + 12] = vertex.tex.x;
			vertices[id + 13] = vertex.tex.y;

			++v;
		}

		UInt32 indicesCount = static_cast<UInt32>(geometry->GetIndicesCount());
		auto indices = Vector<UInt32>(indicesCount);

		Size i = 0;
		for(auto &index : geometry->indices)
		{
			indices[i] = index;
			++i;
		}

		fwrite(&verticesCount, 1, sizeof(UInt32), file);
		fwrite(vertices.data(), 1, sizeof(Float32)* (3 + 3*3 + 2) * verticesCount, file);

		fwrite(&indicesCount, 1, sizeof(UInt32), file);
		fwrite(indices.data(), 1, sizeof(UInt32)* indicesCount, file);

		fclose(file);
	}

	return;*/


	auto graphicsObject = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(400.0f, 1.0f, 400.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject->SetModel(model);
		graphicsScene->Add(graphicsObject);
		physicsBodyRigid->SetUser(graphicsObject);
	}

	auto physicsShape2 = Physics::BulletPhysics::Shape::CreateBox(Vec3(10.0f, 15.0f, 20.0f), 100.0f);
	// auto physicsShape2 = Physics::BulletPhysics::Shape::CreateCapsule(5.0f, 10.0f, 100.0f);
	auto physicsBodyRigid2 = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(physicsShape2, Vec3(0.0f, 50.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)));
	{
		// physicsBodyRigid2->SetAngleFactor(Vec3(0.0f, 0.0f, 0.0f));
		physicsWorld->Add(physicsBodyRigid2);
	}
	auto graphicsObject2 = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(10.0f, 15.0f, 20.0f), Vec3(1.0f), UVec3(1));
		// auto geometry = Geometry::CreateCapsule(5.0f, 10.0f, Vec2(2.0f, 3.0f), UVec2(32, 16));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject2->SetModel(model);
		graphicsScene->Add(graphicsObject2);
		physicsBodyRigid2->SetUser(graphicsObject2);
	}

	auto graphicsSphere = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		graphicsSphere->SetLocalPosition(Vec3(0.0f, 10.0f, 0.0f));

		auto geometry = Geometry::CreateSphere(5.0f, Vec2(1.0f), UVec2(32));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsSphere->SetModel(model);
		graphicsScene->Add(graphicsSphere);
	}
	auto graphicsPlane = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		graphicsPlane->SetLocalPosition(Vec3(-20.0f, 10.0f, 10.0f));
		graphicsPlane->SetLocalAngle(Vec3(-20.0f, 10.0f, 15.0f));

		auto geometry = Geometry::CreatePlain(Vec2(10.0f), Vec2(1.0f), UVec2(2));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsPlane->SetModel(model);
		graphicsScene->Add(graphicsPlane);
	}
	auto graphicsBox = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		graphicsBox->SetLocalPosition(Vec3(+20.0f, 10.0f, 10.0f));
		graphicsBox->SetLocalAngle(Vec3(+10.0f, 20.0f, 30.0f));

		auto geometry = Geometry::CreateBox(Vec3(8.0f, 10.0f, 12.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsBox->SetModel(model);
		graphicsScene->Add(graphicsBox);
	}

	auto p = graphicsSphere->GetPosition();
	auto r = 5.0f;
	auto p1 = p - Vec3(0.0f, 20.0f, 50.0f);
	auto p2 = p + Vec3(0.0f, 20.0f, 50.0f);
	// auto p = Vec3(0.0f, 10.0f, 0.0f);
	// auto n = Vec3(0.0f, 1.0f, 0.0f);
	// auto p1 = Vec3(10.0f, 20.0f, -50.0f);
	// auto p2 = Vec3(10.0f, -20.0f, 50.0f);

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();
		Input::Mouse::SetPosition(Vec2(WinAPI::GetDesktopSize()) / 2.0f);

		if(ButtonState(Buttons::Left))
		{
			p1 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(Vec3(3.0f, -2.0f, 10.0f), 1.0f));
			p2 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(Vec3(3.0f, -2.0f, 30.0f), 1.0f));
		}

		graphicsScene->DrawLine(p1, p2, Vec4(1, 1, 0, 1));
		graphicsScene->DrawCircle(p1, 1.0f, Vec4(1, 1, 0, 1));
		graphicsScene->DrawCircle(p2, 1.0f, Vec4(1, 1, 0, 1));

		using Caster = Cast::Interval;

		// if(Caster::SphereProcess({p1, p2}, {p, r}))
		// if(Caster::QuadProcess({p1, normalize(p2 - p1)}, {graphicsPlane->GetLocalPosition(), graphicsPlane->GetLocalAngle(), Vec2(10.0f)}))
		// if(Caster::QuadProcess({p1, p2}, {graphicsPlane->GetLocalPosition(), graphicsPlane->GetLocalAngle(), Vec2(10.0f)}))
		// if(Caster::BoxProcess({p1, normalize(p2 - p1)}, {graphicsBox->GetLocalPosition(), graphicsBox->GetLocalAngle(), Vec3(8.0f, 10.0f, 12.0f)}))
		if(Caster::BoxProcess({p1, p2}, {graphicsBox->GetLocalPosition(), graphicsBox->GetLocalAngle(), Vec3(8.0f, 10.0f, 12.0f)}))
		{
			auto cp = Caster::GetLast().position;
			auto cn = Caster::GetLast().normal;

			graphicsScene->DrawLine(p1, cp, Vec4(1, 0, 0, 1));
			graphicsScene->DrawCircle(cp, 1.5f, Vec4(1, 0, 0, 1));
			graphicsScene->DrawLine(cp, cp + cn*5.0f, Vec4(0, 1, 0, 1));
		}


		auto RMat = [](Vec3 a)
		{
			return RotateZXY3(a);
		};
		auto MMat = [](Vec3 p, Vec3 a)
		{
			return Move4(p) * RotateZXY4(a);
		};

		Vec3 tForce(
			-Input::Mouse::GetPositionDelta().y * 1.0f,
			Input::Mouse::GetPositionDelta().x * 1.0f,
			0.0f);

		if(KeyState(Keys::W))
		{
			tForce.z = -10.0f;
		}
		if(KeyState(Keys::R))
		{
			tForce.z = +10.0f;
		}

		physicsBodyRigid2->AddImpulse(
			VecXYZ(MMat(physicsBodyRigid2->GetPosition(), physicsBodyRigid2->GetAngle()) * Vec4(Vec3(0.0f, 0.0f, +10.0f), 1.0f)),
			RMat(physicsBodyRigid2->GetAngle()) * Vec3(+tForce.y, 0.0f, 0.0f));
		physicsBodyRigid2->AddImpulse(
			VecXYZ(MMat(physicsBodyRigid2->GetPosition(), physicsBodyRigid2->GetAngle()) * Vec4(Vec3(0.0f, 0.0f, -10.0f), 1.0f)),
			RMat(physicsBodyRigid2->GetAngle()) * Vec3(-tForce.y, 0.0f, 0.0f));

		physicsBodyRigid2->AddImpulse(
			VecXYZ(MMat(physicsBodyRigid2->GetPosition(), physicsBodyRigid2->GetAngle()) * Vec4(Vec3(0.0f, 0.0f, -10.0f), 1.0f)),
			RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, +tForce.x, 0.0f));
		physicsBodyRigid2->AddImpulse(
			VecXYZ(MMat(physicsBodyRigid2->GetPosition(), physicsBodyRigid2->GetAngle()) * Vec4(Vec3(0.0f, 0.0f, +10.0f), 1.0f)),
			RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, -tForce.x, 0.0f));

		physicsBodyRigid2->AddImpulse(
			VecXYZ(MMat(physicsBodyRigid2->GetPosition(), physicsBodyRigid2->GetAngle()) * Vec4(Vec3(-10.0f, 0.0f, 0.0f), 1.0f)),
			RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, +tForce.z, 0.0f));
		physicsBodyRigid2->AddImpulse(
			VecXYZ(MMat(physicsBodyRigid2->GetPosition(), physicsBodyRigid2->GetAngle()) * Vec4(Vec3(+10.0f, 0.0f, 0.0f), 1.0f)),
			RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, -tForce.z, 0.0f));

		Float32 speed = 200.0f;
		if(KeyState(Keys::E))
		{
			physicsBodyRigid2->AddCentralImpulse(RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, 0.0f, +speed));
		}
		if(KeyState(Keys::D))
		{
			physicsBodyRigid2->AddCentralImpulse(RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, 0.0f, -speed));
		}
		if(KeyState(Keys::S))
		{
			physicsBodyRigid2->AddCentralImpulse(RMat(physicsBodyRigid2->GetAngle()) * Vec3(-speed, 0.0f, 0.0f));
		}
		if(KeyState(Keys::F))
		{
			physicsBodyRigid2->AddCentralImpulse(RMat(physicsBodyRigid2->GetAngle()) * Vec3(+speed, 0.0f, 0.0f));
		}
		if(KeyState(Keys::SPACE))
		{
			physicsBodyRigid2->AddCentralImpulse(RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, +speed, 0.0f));
		}
		if(KeyState(Keys::L_CTRL))
		{
			physicsBodyRigid2->AddCentralImpulse(RMat(physicsBodyRigid2->GetAngle()) * Vec3(0.0f, -speed, 0.0f));
		}

		graphicsCamera->SetPosition(VecXYZ(
			(Move4(physicsBodyRigid2->GetPosition())*
			RotateZXY4(physicsBodyRigid2->GetAngle()))*
			Vec4(0.0f, 4.0f, 6.0f, 1.0f))); // Vec4(0.0f, 20.0f, -60.0f, 1.0f)));
		graphicsCamera->SetAngle(physicsBodyRigid2->GetAngle());

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
