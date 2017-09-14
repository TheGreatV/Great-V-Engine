#pragma region Include
#pragma once


#include "ErrorHandling.hpp"


#include <Geometry/Geometry.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace BulletPhysics
	{
		class World;
		class Shape;
		namespace Bodies
		{
			class Rigid;
			class Soft;
		}
		namespace Constraints
		{
			class Slider;
			class Hinge;
			class Corkscrew;
			// TODO: other kinds of constraints
		}


		class World
		{
			friend Shape;
			friend Bodies::Rigid;
			friend Bodies::Soft;
		public:
			using CollisionConfiguration = btCollisionConfiguration;
			using CollisionConfiguration_SoftRigid = btSoftBodyRigidBodyCollisionConfiguration;
			using Dispatcher = btCollisionDispatcher;
			using Broadphase = btBroadphaseInterface;
			using Broadphase_Default = btAxisSweep3;
			using ConstraintSolver = btConstraintSolver;
			using ConstraintSolver_SequentialImpulse = btSequentialImpulseConstraintSolver;
			using Handle = btSoftRigidDynamicsWorld;
			using Group = UInt16;
			using Mask = UInt16;
			using RigidBodyIndex = std::tuple<Group, Mask>;
		public:
			static const Vec3 POSITION_MIN;
			static const Vec3 POSITION_MAX;
			static const Size HANDLES_MAX = 32766;
		protected:
			const Pointer<CollisionConfiguration> configuration;
			const Pointer<Dispatcher> dispatcher;
			const Pointer<Broadphase> broadphase;
			const Pointer<ConstraintSolver> solver;
			const Pointer<Handle> handle;
			Dictionary<Reference<Bodies::Rigid>, RigidBodyIndex> bodiesRigid;
		public:
			inline World();
			inline World(const World&) = delete;
			inline ~World() = default;
		public:
			inline World& operator = (const World&) = delete;
		public:
			inline void Add(Reference<Bodies::Rigid> body_, const Group& group_, const Mask& mask_);
			inline void Remove(Reference<Bodies::Rigid> body_);
		public:
			inline void SetGravity(const Vec3& value_);
			inline void Render(const Float32& timeStep_);
		};
		class Shape
		{
			friend World;
			friend Bodies::Rigid;
			friend Bodies::Soft;
		public:
			using Handle = btCollisionShape; // btBoxShape -> btPolyhedralConvexShape -> btConvexInternalShape -> btConvexShape -> btCollisionShape
			using Handle_Box = btBoxShape;
			using Handle_Capsule = btCapsuleShape;
			using VertexArray_Indexed = btTriangleIndexVertexArray;
			using VertexArray_Bvh = btBvhTriangleMeshShape;
			using Mass = Float32;
			using Inertia = btVector3;
			using Vertices = Geometry::Bytes;
			using Indices = Geometry::Bytes;
		public:
			inline static Reference<Shape> CreateBox(const Vec3& size_, const Mass& mass_);
			inline static Reference<Shape> CreateCapsule(const Float32& radius_, const Float32& height_, const Mass& mass_);
			inline static Reference<Shape> CreateGeometry(Reference<Geometry> geometry_, const Mass& mass_, const Mat4& transform_ = One4());
		protected:
			const Mass mass;
			Inertia inertia;
			const Pointer<Handle> handle;
			const Vertices vertices;
			const Indices indices;
		protected:
			inline Shape(Pointer<Handle>&& handle_, const Mass& mass_);
			inline Shape(Pointer<Handle>&& handle_, const Mass& mass_, Vertices&& vertices_, Indices&& indices_);
		public:
			inline Shape(const Shape&) = delete;
			inline ~Shape() = default;
		public:
			inline Shape& operator = (const Shape&) = delete;
		public:
			inline Mass GetMass() const
			{
				return mass;
			}
		};
		namespace Bodies
		{
			class Rigid
			{
				friend World;
				friend Shape;
				friend Bodies::Soft;
			public:
				using Mass = Shape::Mass;
				using Transform = btTransform;
				using MotionState = btMotionState;
				using MotionState_Default = btDefaultMotionState;
				using Handle = btRigidBody;
			public:
				// static inline void*			operator new(size_t i){return _mm_malloc(i, 16);}
				// static inline void*			operator new[](size_t i)
				// {
				// 	return _mm_malloc(sizeof(Rigid)*i, 16);
				// }
				// static inline void			operator delete(void* p)
				// {
				// 	_mm_free(p);
				// }
				// static inline void			operator delete[](void* p)
				// {
				// 	_mm_free(p);
				// }
			protected:
				const Mass mass;
				const Reference<Shape> shape;
				const Pointer<MotionState> motionState;
				const Pointer<Handle> handle;
			public:
				inline Rigid(Reference<Shape> shape_);
				inline Rigid(Reference<Shape> shape_, const Mass& mass_);
				inline Rigid(Reference<Shape> shape_, const Vec3& position_);
				inline Rigid(Reference<Shape> shape_, const Vec3& position_, const Mass& mass_);
				inline Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_);
				inline Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_, const Mass& mass_);
				inline Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_, const Vec3& centerOfMass_);
				inline Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_, const Vec3& centerOfMass_, const Mass& mass_);
				inline Rigid(const Rigid&) = delete;
				inline ~Rigid() = default;
			public:
				inline void Loop();
			public:
				inline Mass GetMass() const;
				inline Vec3 GetPosition() const;
				inline Vec3 GetAngle() const;
			public:
				inline Vec3 GetVelocity() const;
				inline void SetVelocity(const Vec3& value_);
			public:
				inline void SetPositionFactor(const Vec3& factor_);
				inline void SetAngleFactor(const Vec3& factor_);
			public:
				inline void AddForce(const Vec3& position_, const Vec3& value_);
				inline void AddCentralForce(const Vec3& value_);
				inline void AddImpulse(const Vec3& position_, const Vec3& value_);
				inline void AddCentralImpulse(const Vec3& value_);
				inline void AddTorque(const Vec3& value_);
				inline void AddTorqueImpulse(const Vec3& value_);
			};
		}
		namespace Constraints
		{
			class Slider
			{
			};
			class Hinge
			{
			};
			// TODO: other kinds of constraints
		}

		inline btVector3 ToBtVector3(const Vec3& source_);
		inline btMatrix3x3 ToBtMatrix3x3(const Mat3& source_);
		inline btTransform ToBtTransform(const Vec3& position_);
		inline btTransform ToBtTransform(const Vec3& position_, const Vec3& angle_);

		inline Vec3 FromBtVector3(const btVector3& source_);
	}
}


#pragma region

inline btVector3 GreatVEngine::BulletPhysics::ToBtVector3(const Vec3& source_)
{
	return btVector3(source_.x, source_.y, source_.z);
}
inline btMatrix3x3 GreatVEngine::BulletPhysics::ToBtMatrix3x3(const Mat3& source_)
{
	return btMatrix3x3(
		source_[0][0], source_[1][0], source_[2][0],
		source_[0][1], source_[1][1], source_[2][1],
		source_[0][2], source_[1][2], source_[2][2]);
}
inline btTransform GreatVEngine::BulletPhysics::ToBtTransform(const Vec3& position_)
{
	return ToBtTransform(position_, Vec3(0.0f));
}
inline btTransform GreatVEngine::BulletPhysics::ToBtTransform(const Vec3& position_, const Vec3& angle_)
{
	btTransform transform;
	
	transform.setIdentity();
	transform.setOrigin(ToBtVector3(position_));
	transform.setBasis(ToBtMatrix3x3(RotateZXY3(angle_)));

	return transform;
}

inline GreatVEngine::Vec3 GreatVEngine::BulletPhysics::FromBtVector3(const btVector3& source_)
{
	return Vec3(source_.x(), source_.y(), source_.z());
}

#pragma endregion
#pragma region World
inline GreatVEngine::BulletPhysics::World::World():
	configuration(new CollisionConfiguration_SoftRigid()),
	dispatcher(new Dispatcher(configuration.get())),
	broadphase(new Broadphase_Default(ToBtVector3(POSITION_MIN), ToBtVector3(POSITION_MAX), HANDLES_MAX)),
	solver(new ConstraintSolver_SequentialImpulse()),
	handle(new Handle(dispatcher.get(), broadphase.get(), solver.get(), configuration.get()))
{
}

inline void GreatVEngine::BulletPhysics::World::Add(Reference<Bodies::Rigid> body_, const Group& group_, const Mask& mask_)
{
	if(bodiesRigid.find(body_) == bodiesRigid.end())
	{
		bodiesRigid.insert({body_, RigidBodyIndex(group_, mask_)});
		handle->addRigidBody(body_->handle.get(), group_, mask_);
	}
	else
	{
		throw Exception("Failed to add rigid body twice");
	}
}
inline void GreatVEngine::BulletPhysics::World::Remove(Reference<Bodies::Rigid> body_)
{
	auto it = bodiesRigid.find(body_);
	if(it != bodiesRigid.end())
	{
		handle->removeRigidBody(body_->handle.get());
		bodiesRigid.erase(it);
	}
	else
	{
		throw Exception("Failed to remove not added body");
	}
}

inline void GreatVEngine::BulletPhysics::World::SetGravity(const Vec3& value_)
{
	handle->setGravity(ToBtVector3(value_));
}
inline void GreatVEngine::BulletPhysics::World::Render(const Float32& timeStep_)
{
	handle->stepSimulation(timeStep_);
}
#pragma endregion
#pragma region Shape
inline GreatVEngine::Reference<GreatVEngine::BulletPhysics::Shape> GreatVEngine::BulletPhysics::Shape::CreateBox(const Vec3& size_, const Mass& mass_)
{
	return WrapReference(new Shape(
		std::move(WrapPointer(new Handle_Box(ToBtVector3(size_ * 0.5f)))),
		mass_));
}
inline GreatVEngine::Reference<GreatVEngine::BulletPhysics::Shape> GreatVEngine::BulletPhysics::Shape::CreateCapsule(const Float32& radius_, const Float32& height_, const Mass& mass_)
{
	return WrapReference(new Shape(
		std::move(WrapPointer(new Handle_Capsule(radius_, height_))),
		mass_));
}
inline GreatVEngine::Reference<GreatVEngine::BulletPhysics::Shape> GreatVEngine::BulletPhysics::Shape::CreateGeometry(Reference<Geometry> geometry_, const Mass& mass_, const Mat4& transform_)
{
	auto vertices = geometry_->GetVertices(transform_, Geometry::VertexPackMode::Pos32F);
	auto indices = geometry_->GetIndices(Geometry::IndexPackMode::UInt32);

	auto indexedVertexArray = new VertexArray_Indexed(
		geometry_->GetIndicesCount() / 3, reinterpret_cast<int*>(indices->data()), geometry_->GetIndexSize(Geometry::IndexPackMode::UInt32) * 3,
		geometry_->GetVerticesCount(), reinterpret_cast<btScalar*>(vertices->data()), geometry_->GetVertexSize(Geometry::VertexPackMode::Pos32F));

	auto handle = new VertexArray_Bvh(indexedVertexArray, false);

	// motion = new btDefaultMotionState(transform);
	// btRigidBody::btRigidBodyConstructionInfo rbInfo(mass_, motion, shape, btVector3(1.0f, 1.0f, 1.0f));
	// body = new btRigidBody(rbInfo);
	// body->setUserPointer(this);
	// 
	// World::data.dynamicsWorld->addRigidBody(body, (uint16)group_, (uint16)group_);

	return WrapReference(new Shape(
		WrapPointer(handle),
		mass_,
		std::move(*vertices.get()),
		std::move(*indices.get())));
}

inline GreatVEngine::BulletPhysics::Shape::Shape(Pointer<Handle>&& handle_, const Mass& mass_):
	mass(mass_),
	handle(std::move(handle_))
{
	if(mass > 0.0f)
	{
		inertia = Inertia(0.0f, 0.0f, 0.0f);
		handle->calculateLocalInertia(mass, inertia);
	}
}
inline GreatVEngine::BulletPhysics::Shape::Shape(Pointer<Handle>&& handle_, const Mass& mass_, Vertices&& vertices_, Indices&& indices_):
	mass(mass_),
	handle(std::move(handle_)),
	vertices(std::move(vertices_)),
	indices(std::move(indices_))
{
	if(mass > 0.0f)
	{
		handle->calculateLocalInertia(mass, ToBtVector3(Vec3(1.0f)));
	}
}
#pragma endregion
#pragma region Bodies

#pragma region Rigid

inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_) :
	Rigid(shape_, Vec3(0.0f), shape_->GetMass())
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Mass& mass_):
	Rigid(shape_, Vec3(0.0f), mass_)
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Vec3& position_) :
	Rigid(shape_, position_, shape_->GetMass())
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Vec3& position_, const Mass& mass_) :
	Rigid(shape_, position_, Vec3(0.0f), mass_)
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_) :
	Rigid(shape_, position_, rotation_, shape_->GetMass())
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_, const Mass& mass_) :
	Rigid(shape_, position_, rotation_, Vec3(0.0f), mass_)
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_, const Vec3& centerOfMass_) :
	Rigid(shape_, position_, rotation_, centerOfMass_, shape_->GetMass())
{
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Vec3& position_, const Vec3& rotation_, const Vec3& centerOfMass_, const Mass& mass_):
	mass(mass_),
	shape(shape_),
	motionState(new MotionState_Default(ToBtTransform(position_, rotation_))), //, ToBtTransform(centerOfMass_))),
	handle(new Handle(Handle::btRigidBodyConstructionInfo(
		mass,
		motionState.get(),
		shape->handle.get(),
		shape->inertia)))
	// handle(new Handle(
	// 	mass,
	// 	motionState.get(),
	// 	shape->handle.get()))
{
	// handle->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
	// handle->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
	handle->setActivationState(DISABLE_DEACTIVATION);
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::Loop()
{
	auto a = handle->getAngularVelocity();
	handle->setAngularVelocity(ToBtVector3(Vec3(a.getX(), a.getY(), a.getZ()) * 0.98f));

	auto l = handle->getLinearVelocity();
	handle->setLinearVelocity(ToBtVector3(Vec3(l.getX(), l.getY(), l.getZ()) * 0.98f));
}
inline GreatVEngine::BulletPhysics::Bodies::Rigid::Mass GreatVEngine::BulletPhysics::Bodies::Rigid::GetMass() const
{
	return mass;
}
inline GreatVEngine::Vec3 GreatVEngine::BulletPhysics::Bodies::Rigid::GetPosition() const
{
	Transform transform; motionState->getWorldTransform(transform);

	// auto transform = handle->getWorldTransform();
	auto origin = transform.getOrigin();

	return Vec3(origin.getX(), origin.getY(), origin.getZ());
}
inline GreatVEngine::Vec3 GreatVEngine::BulletPhysics::Bodies::Rigid::GetAngle() const
{
	Transform transform; motionState->getWorldTransform(transform);

	// auto transform = handle->getWorldTransform();
	auto basis = transform.getBasis();

	return GreatVEngine::GetAngle(Mat3(
		// basis[0].getX(), basis[0].getY(), basis[0].getZ(),
		// basis[1].getX(), basis[1].getY(), basis[1].getZ(),
		// basis[2].getX(), basis[2].getY(), basis[2].getZ()));
		basis[0].getX(), basis[1].getX(), basis[2].getX(),
		basis[0].getY(), basis[1].getY(), basis[2].getY(),
		basis[0].getZ(), basis[1].getZ(), basis[2].getZ()));
}

inline GreatVEngine::Vec3 GreatVEngine::BulletPhysics::Bodies::Rigid::GetVelocity() const
{
	return FromBtVector3(handle->getLinearVelocity());
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::SetVelocity(const Vec3& value_)
{
	handle->setLinearVelocity(ToBtVector3(value_));
}

inline void GreatVEngine::BulletPhysics::Bodies::Rigid::SetPositionFactor(const Vec3& factor_)
{
	handle->setLinearFactor(ToBtVector3(factor_));
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::SetAngleFactor(const Vec3& factor_)
{
	handle->setAngularFactor(ToBtVector3(factor_));
}

inline void GreatVEngine::BulletPhysics::Bodies::Rigid::AddForce(const Vec3& position_, const Vec3& value_)
{
	handle->applyForce(ToBtVector3(value_), ToBtVector3(position_));
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::AddCentralForce(const Vec3& value_)
{
	handle->applyCentralForce(ToBtVector3(value_));
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::AddImpulse(const Vec3& position_, const Vec3& value_)
{
	handle->applyImpulse(ToBtVector3(value_), ToBtVector3(position_));
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::AddCentralImpulse(const Vec3& value_)
{
	handle->applyCentralImpulse(ToBtVector3(value_));
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::AddTorque(const Vec3& value_)
{
	handle->applyTorque(ToBtVector3(Vec3(value_.x, value_.y, -value_.z)));
}
inline void GreatVEngine::BulletPhysics::Bodies::Rigid::AddTorqueImpulse(const Vec3& value_)
{
	// auto tAng = GreatVEngine::GetAngle(
	// 	RotateX3(value_.x)*
	// 	RotateY3(value_.y)*
	// 	RotateZ3(value_.z));

	handle->applyTorqueImpulse(ToBtVector3(Vec3(value_.x, value_.y, -value_.z)));
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion
