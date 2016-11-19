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

	auto physicsShape = Physics::BulletPhysics::Shape::CreateBox(Vec3(10.0f), 100.0f);
	auto physicsBodyRigid = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(physicsShape, Vec3(0.0f, 100.0f, 0.0f)));
	{
		physicsWorld->Add(physicsBodyRigid);
	}
	auto physicsShape2 = Physics::BulletPhysics::Shape::CreateBox(Vec3(100.0f, 10.0f, 100.0f), 0.0f); // Physics::BulletPhysics::Shape::CreateGeometry(Geometry::CreateBox(Vec3(100.0f, 1.0f, 100.0f), Vec3(1.0f), UVec3(1)), 0.0f);
	auto physicsBodyRigid2 = MakeReference(new Physics::BulletPhysics::Bodies::Rigid(physicsShape2));
	{
		physicsWorld->Add(physicsBodyRigid2);
	}

	auto graphicsCamera = MakeReference(new Graphics::Camera());
	{
		graphicsCamera->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}

	auto graphicsEngine = MakeReference(new Graphics::OpenGL::Engine(deviceContext));

	auto graphicsScene = MakeReference(new Graphics::OpenGL::Scene(graphicsEngine, window->GetSize()));
	{
		// scene->LoadDecals(
		// 	Filepath("Media/Images/Decals/GlassHole_Albedo.png"),
		// 	Filepath("Media/Images/Decals/GlassHole_Topology.png"),
		// 	Filepath("Media/Images/Decals/GlassHole_Material.png"));
	}

	auto graphicsLightDirection1 = MakeReference(new Graphics::OpenGL::Lights::Direction());
	{
		// lightDirection1->Hide();
		graphicsLightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		graphicsLightDirection1->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLightDirection1->SetAmbient(0.5f);
		graphicsScene->Add(graphicsLightDirection1);
	}

	auto graphicsEnvironmentGlobalmap1 = Graphics::OpenGL::Environments::Globalmap::LoadCube(graphicsEngine, Filepath("Media/Images/Cubemap2.dds"));
	{
		// environmentGlobalmap1->Hide();
		graphicsScene->Add(graphicsEnvironmentGlobalmap1);
	}

	auto graphicsMaterialFlat = MakeReference(new Graphics::OpenGL::Material(graphicsEngine));
	{
		graphicsMaterialFlat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		graphicsMaterialFlat->SetValue("materialColor", Vec3(1.0f));
		graphicsMaterialFlat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		graphicsMaterialFlat->SetValue("materialGloss", 0.2f);
		graphicsMaterialFlat->SetValue("materialRoughness", 0.5f);
	}
	auto graphicsObject = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject->SetModel(model);
		graphicsScene->Add(graphicsObject);
		// physicsBodyRigid->SetUser(graphicsObject);
	}
	auto graphicsObject1 = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject1->SetModel(model);
		graphicsScene->Add(graphicsObject1);
		// physicsBodyRigid->SetUser(graphicsObject);
	}
	auto graphicsObject2 = MakeReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(100.0f, 10.0f, 100.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = MakeReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject2->SetLocalPosition(Vec3(0.0f, -5.0f, 0.0f));
		graphicsObject2->SetModel(model);
		graphicsScene->Add(graphicsObject2);
		// physicsBodyRigid2->SetUser(graphicsObject2);
	}

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

	/*auto sponza = Graphics::OpenGL::Object::Load(graphicsEngine, Filepath("Media/Models/Sponza.gvs"), graphicsMaterialFlat);
	{
		graphicsScene->Add(sponza);
	}*/


	/*btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration(); // btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	// btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	btDiscreteDynamicsWorld* dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody* groundRigidBody = new btRigidBody(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(groundRigidBody);

	btCollisionShape* fallShape = new btBoxShape(btVector3(5.0f, 5.0f, 5.0f));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);

	btDefaultMotionState* fallMotionState = new btDefaultMotionState(BulletPhysics::ToBtTransform(Vec3(0.0f, 100.0f, 0.0f), Vec3(30.0f, 0.0f, 0.0f)));//btTransform(btQuaternion(btVector3(1,0,0), radians(30.0f)), btVector3(0, 100, 0)));
	btRigidBody* fallRigidBody = new btRigidBody(mass, fallMotionState, fallShape, fallInertia);
	dynamicsWorld->addRigidBody(fallRigidBody);

	btDefaultMotionState* fallMotionState1 = new btDefaultMotionState(BulletPhysics::ToBtTransform(Vec3(5.0f, 120.0f, 0.0f), Vec3(30.0f, 0.0f, 0.0f)));//btTransform(btQuaternion(btVector3(1,0,0), radians(30.0f)), btVector3(0, 100, 0)));
	btRigidBody* fallRigidBody1 = new btRigidBody(mass, fallMotionState1, fallShape, fallInertia);
	dynamicsWorld->addRigidBody(fallRigidBody1);*/


	while(!KeyState(Keys::ESC))
	{
		Input::Keyboard::Loop();
		window->Loop();

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


		/*dynamicsWorld->stepSimulation(1 / 60.f, 10);

		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);
		graphicsObject->SetLocalPosition(Vec3(
			trans.getOrigin().getX(),
			trans.getOrigin().getY(),
			trans.getOrigin().getZ()
			));
		graphicsObject->SetLocalAngle(
			GetAngle(Mat3(
			trans.getBasis()[0].getX(), trans.getBasis()[1].getX(), trans.getBasis()[2].getX(),
			trans.getBasis()[0].getY(), trans.getBasis()[1].getY(), trans.getBasis()[2].getY(),
			trans.getBasis()[0].getZ(), trans.getBasis()[1].getZ(), trans.getBasis()[2].getZ()))
			);
		fallRigidBody1->getMotionState()->getWorldTransform(trans);
		graphicsObject1->SetLocalPosition(Vec3(
			trans.getOrigin().getX(),
			trans.getOrigin().getY(),
			trans.getOrigin().getZ()
			));
		graphicsObject1->SetLocalAngle(
			GetAngle(Mat3(
			trans.getBasis()[0].getX(), trans.getBasis()[1].getX(), trans.getBasis()[2].getX(),
			trans.getBasis()[0].getY(), trans.getBasis()[1].getY(), trans.getBasis()[2].getY(),
			trans.getBasis()[0].getZ(), trans.getBasis()[1].getZ(), trans.getBasis()[2].getZ()))
			);*/

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
