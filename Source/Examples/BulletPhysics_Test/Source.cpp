#include <iostream>
#include <Windows.h>

#include <APIs/BulletPhysics/Wrapper.hpp>
namespace GVE = GreatVEngine;
using namespace GVE;


void wrapped()
{
	auto world = MakePointer(new BulletPhysics::World());
	{
		world->SetGravity(Vec3(0.0f, -9.8f, 0.0f));
	}

	auto geometry = Geometry::CreateBox(Vec3(100.0f, 1.0f, 100.0f), Vec3(1.0f), UVec3(1));

	auto shape = BulletPhysics::Shape::CreateBox(Vec3(10.0f), 100.0f);
	auto shape2 = BulletPhysics::Shape::CreateGeometry(geometry, 0.0f);

	auto bodyRigid = MakeReference(new BulletPhysics::Bodies::Rigid(shape, Vec3(0.0f, 100.0f, 0.0f)));
	{
		world->Add(bodyRigid);
	}
	auto bodyRigid2 = MakeReference(new BulletPhysics::Bodies::Rigid(shape2, 0.0f));
	{
		world->Add(bodyRigid2);
	}

	// Main loop
	// for(int i = 0; i < 10; ++i)
	while(true)
	{
		world->Render(1.0f); // 1s

		std::cout << "position: " <<
			bodyRigid->GetPosition().x << ", " <<
			bodyRigid->GetPosition().y << ", " <<
			bodyRigid->GetPosition().z << std::endl;

		Sleep(1000 / 60);
	}

	world->Remove(bodyRigid);
	world->Remove(bodyRigid2);
}
void raw()
{
	// Create dynamic world
	auto collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	auto collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	auto broadphase = new btAxisSweep3(btVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX), btVector3(FLT_MAX, FLT_MAX, FLT_MAX), 32766);
	auto solver = new btSequentialImpulseConstraintSolver();
	auto dynamicsWorld = new btSoftRigidDynamicsWorld(
		collisionDispatcher,
		broadphase,
		solver,
		collisionConfiguration);
	{
		dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	}


	// Create Shape
	auto shape = new btBoxShape(btVector3(10.0f, 10.0f, 10.0f));
	{
		shape->calculateLocalInertia(100.0f, btVector3(1.0f, 1.0f, 1.0f));
	}


	// Create Body
	btTransform transform;
	{
		transform.setIdentity();
		transform.setOrigin(btVector3(0.0f, 100.0f, 0.0f));
		transform.setBasis(btMatrix3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f));
	}
	auto motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		100.0f,
		motionState,
		shape,
		btVector3(1.0f, 1.0f, 1.0f));
	auto body = new btRigidBody(rbInfo);
	{
		dynamicsWorld->addRigidBody(body, 0x0001, 0xFFFF);
	}


	// Main loop
	for(int i = 0; i < 10; ++i)
	{
		dynamicsWorld->stepSimulation(1.0f); // 1s

		auto transform = body->getWorldTransform();
		std::cout << "position: " <<
			transform.getOrigin().x() << ", " <<
			transform.getOrigin().y() << ", " <<
			transform.getOrigin().z() << std::endl;
	}


	// Delete body
	dynamicsWorld->removeRigidBody(body);
	delete body;
	delete motionState;


	// Delete Shape
	delete shape;


	// Delete World
	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete collisionDispatcher;
	delete collisionConfiguration;
}


void main()
{
	wrapped();
	// raw();

	std::system("pause");
}