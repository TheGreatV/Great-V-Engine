#pragma region Include
#pragma once


#include "Physics.hpp"


#include <APIs/BulletPhysics/Wrapper.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace Physics
	{
		namespace BulletPhysics
		{
			class Engine;
			class World;
			class Shape;
			class Material;
			namespace Bodies
			{
				class Rigid;
			}
			namespace Constraints
			{
				class Slider;
			}


			class Engine:
				public Physics::Engine
			{
				friend World;
				friend Shape;
				friend Material;
				friend Bodies::Rigid;
			};
			class Shape:
				public Physics::Shape
			{
				friend Engine;
				friend World;
				friend Material;
				friend Bodies::Rigid;
			protected:
				using Handle = GreatVEngine::BulletPhysics::Shape;
				using Mass = Handle::Mass;
			public:
				inline static Reference<Shape> CreateBox(const Vec3& size_, const Mass& mass_);
				inline static Reference<Shape> CreateCapsule(const Float32& radius_, const Float32& height_, const Mass& mass_);
				inline static Reference<Shape> CreateGeometry(Reference<Geometry> geometry_, const Mass& mass_, const Mat4& transform_ = One4());
			protected:
				const Reference<Handle> handle;
			protected:
				inline Shape(Reference<Handle> shape_);
			public:
				inline Mass GetMass() const
				{
					return handle->GetMass();
				}
			};
			class Body
			{
			protected:
				const Reference<Shape> shape;
			public:
				inline Body(Reference<Shape> shape_):
					shape(shape_)
				{
				}
			public:
				inline Reference<Shape> GetShape() const
				{
					return shape;
				}
			};
			namespace Bodies
			{
				class Rigid:
					public Shared<Rigid>,
					public Physics::Bodies::Rigid,
					public Body
				{
					friend Engine;
					friend World;
					friend Shape;
					friend Material;
				public:
					using Handle = GreatVEngine::BulletPhysics::Bodies::Rigid;
				protected:
					const Reference<Shape> shape;
					const Reference<Handle> handle;
				public:
					inline Rigid(Reference<Shape> shape_);
					inline Rigid(Reference<Shape> shape_, const Position::Value& position_);
					inline Rigid(Reference<Shape> shape_, const Position::Value& position_, const Angle::Value& angle_);
					inline Rigid(Reference<Shape> shape_, const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_);
					inline Rigid(Reference<Shape> shape_, const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_, const Torque::Value& omega_);
					inline ~Rigid() = default;
				public:
					using Group = std::bitset<16>; // Helper::Logic::BitGroup<UInt16>;
					using Mask = std::bitset<16>; // Helper::Logic::BitGroup<UInt16>;
					using OnGroupChanged = Helper::Subscrption::OnEvent<Reference<Rigid>>;
					using OnMaskChanged = Helper::Subscrption::OnEvent<Reference<Rigid>>;
				protected:
					Group group = 1;
					Mask mask = 0xFFFF;
					OnGroupChanged onGroupChanged;
					OnMaskChanged onMaskChanged;
				public:
					inline void Subscribe_OnGroupChanged(OnGroupChanged::Subscriber subscriber_)
					{
						onGroupChanged += subscriber_;
					}
					inline void Unsubscribe_OnGroupChanged(OnGroupChanged::Subscriber subscriber_)
					{
						onGroupChanged -= subscriber_;
					}
					inline void Subscribe_OnMaskChanged(OnMaskChanged::Subscriber subscriber_)
					{
						onMaskChanged += subscriber_;
					}
					inline void Unsubscribe_OnMaskChanged(OnMaskChanged::Subscriber subscriber_)
					{
						onMaskChanged -= subscriber_;
					}
				public:
					inline Group GetGroup() const
					{
						return group;
					}
					inline void SetGroup(const Size& groupId_)
					{
						group[groupId_] = true;

						onGroupChanged(shared_from_this());
					}
					inline void SetGroup(const Initializer<Size>& groupIds_)
					{
						for(auto &groupId : groupIds_)
						{
							group[groupId] = true;
						}

						onGroupChanged(shared_from_this());
					}
					inline void ResetGroup(const Size& groupId_)
					{
						group[groupId_] = false;

						onGroupChanged(shared_from_this());
					}
					inline void ResetGroup(const Initializer<Size>& groupIds_)
					{
						for(auto &groupId : groupIds_)
						{
							group[groupId] = false;
						}

						onGroupChanged(shared_from_this());
					}
					inline Mask GetMask() const
					{
						return mask;
					}
					inline void SetMask(const Size& maskId_)
					{
						mask[maskId_] = true;

						onMaskChanged(shared_from_this());
					}
					inline void SetMask(const Initializer<Size>& maskIds_)
					{
						for(auto &maskId : maskIds_)
						{
							mask[maskId] = true;
						}

						onMaskChanged(shared_from_this());
					}
					inline void ResetMask(const Size& maskId_)
					{
						mask[maskId_] = false;

						onMaskChanged(shared_from_this());
					}
					inline void ResetMask(const Initializer<Size>& maskIds_)
					{
						for(auto &maskId : maskIds_)
						{
							mask[maskId] = false;
						}

						onMaskChanged(shared_from_this());
					}
				public:
					inline Handle::Mass GetMass() const
					{
						return handle->GetMass();
					}
				public:
					inline void SetVelocity(const Velocity::Value& value_)
					{
						handle->SetVelocity(value_);
						velocity = value_; // TODO: check this
					}
				public:
					inline void SetPositionFactor(const Vec3& factor_)
					{
						handle->SetPositionFactor(factor_);
					}
					inline void SetAngleFactor(const Vec3& factor_)
					{
						handle->SetAngleFactor(factor_);
					}
				public:
					inline void AddForce(const Position::Value& position_, const Velocity::Value& force_)
					{
						handle->AddForce(position_, force_);
					}
					inline void AddCentralForce(const Velocity::Value& force_)
					{
						handle->AddCentralForce(force_);
					}
					inline void AddImpulse(const Position::Value& position_, const Velocity::Value& force_)
					{
						handle->AddImpulse(position_, force_);
					}
					inline void AddCentralImpulse(const Velocity::Value& force_)
					{
						handle->AddCentralImpulse(force_);
					}
					inline void AddTorque(const Torque::Value& torque_)
					{
						handle->AddTorque(torque_);
					}
					inline void AddTorqueImpulse(const Torque::Value& torque_)
					{
						handle->AddTorqueImpulse(torque_);
					}
				};
			}
			namespace Constraints
			{
				class Slider
				{
				};
			}
			class World:
				public Physics::World
			{
				friend Engine;
				friend Shape;
				friend Material;
				friend Bodies::Rigid;
			public:
				using Handle = GreatVEngine::BulletPhysics::World;
			protected:
				const Reference<Engine> engine;
				const Pointer<Handle> handle;
				Vector<Reference<Bodies::Rigid>> bodiesRigid;
			public:
				inline World(Reference<Engine> engine_);
				inline World(const World&) = delete;
				inline ~World();
			public:
				inline World& operator = (const World&) = delete;
			protected:
				inline void OnGroupChanged(Reference<Bodies::Rigid> body_);
				inline void OnMaskChanged(Reference<Bodies::Rigid> body_);
			public:
				inline void Add(Reference<Bodies::Rigid> body_);
				inline void Remove(Reference<Bodies::Rigid> body_);
				inline void Render(const Float32& timeStep_);
			public:
				inline void SetGravity(const Vec3& gravity_)
				{
					handle->SetGravity(gravity_);
				}
			};
		}
	}
}


#pragma region World

inline GreatVEngine::Physics::BulletPhysics::World::World(Reference<Engine> engine_):
	engine(engine_),
	handle(new Handle())
{
	// handle->SetGravity(Vec3(0.0f, -98.0f, 0.0f));
	handle->SetGravity(Vec3(0.0f, 0.0f, 0.0f));
}
inline GreatVEngine::Physics::BulletPhysics::World::~World()
{
	while(!bodiesRigid.empty())
	{
		auto &body = bodiesRigid.back();
		Remove(body);
	}
}
inline void GreatVEngine::Physics::BulletPhysics::World::OnGroupChanged(Reference<Bodies::Rigid> body_)
{
	auto it = std::find(bodiesRigid.begin(), bodiesRigid.end(), body_);
	if(it != bodiesRigid.end())
	{
		handle->Remove(body_->handle);
		handle->Add(
			body_->handle,
			(GreatVEngine::BulletPhysics::World::Group)body_->GetGroup().to_ulong(),
			(GreatVEngine::BulletPhysics::World::Mask)body_->GetMask().to_ulong());
	}
	else
	{
		throw Exception("Nonsence");
	}
}
inline void GreatVEngine::Physics::BulletPhysics::World::OnMaskChanged(Reference<Bodies::Rigid> body_)
{
	auto it = std::find(bodiesRigid.begin(), bodiesRigid.end(), body_);
	if(it != bodiesRigid.end())
	{
		handle->Remove(body_->handle);
		handle->Add(
			body_->handle,
			(GreatVEngine::BulletPhysics::World::Group)body_->GetGroup().to_ulong(),
			(GreatVEngine::BulletPhysics::World::Mask)body_->GetMask().to_ulong());
	}
	else
	{
		throw Exception("Nonsence");
	}
}
inline void GreatVEngine::Physics::BulletPhysics::World::Add(Reference<Bodies::Rigid> body_)
{
	if(std::find(bodiesRigid.begin(), bodiesRigid.end(), body_) == bodiesRigid.end())
	{
		body_->Subscribe_OnGroupChanged(Bodies::Rigid::OnGroupChanged::Subscriber(*this, &World::OnGroupChanged));
		body_->Subscribe_OnMaskChanged(Bodies::Rigid::OnGroupChanged::Subscriber(*this, &World::OnGroupChanged));

		bodiesRigid.push_back(body_);
		handle->Add(
			body_->handle,
			(GreatVEngine::BulletPhysics::World::Group)body_->GetGroup().to_ulong(),
			(GreatVEngine::BulletPhysics::World::Mask)body_->GetMask().to_ulong());
	}
	else
	{
		throw Exception("Failed to add rigid body twice");
	}
}
inline void GreatVEngine::Physics::BulletPhysics::World::Remove(Reference<Bodies::Rigid> body_)
{
	auto it = std::find(bodiesRigid.begin(), bodiesRigid.end(), body_);
	if(it != bodiesRigid.end())
	{
		body_->Unsubscribe_OnGroupChanged(Bodies::Rigid::OnGroupChanged::Subscriber(*this, &World::OnGroupChanged));
		body_->Unsubscribe_OnMaskChanged(Bodies::Rigid::OnGroupChanged::Subscriber(*this, &World::OnGroupChanged));

		bodiesRigid.erase(it);
		handle->Remove(body_->handle);
	}
	else
	{
		throw Exception("Failed to remove not added body");
	}
}
inline void GreatVEngine::Physics::BulletPhysics::World::Render(const Float32& timeStep_)
{
	handle->Render(timeStep_);

	for(auto &body : bodiesRigid)
	{
		body->handle->Loop();

		body->Update(
			body->handle->GetPosition(),
			body->handle->GetAngle(),
			body->handle->GetVelocity(),
			Vec3(0.0f));
	}
}

#pragma endregion
#pragma region Shape
inline GreatVEngine::Reference<GreatVEngine::Physics::BulletPhysics::Shape> GreatVEngine::Physics::BulletPhysics::Shape::CreateBox(const Vec3& size_, const Mass& mass_)
{
	return WrapReference(new Shape(Handle::CreateBox(size_, mass_)));
}
inline GreatVEngine::Reference<GreatVEngine::Physics::BulletPhysics::Shape> GreatVEngine::Physics::BulletPhysics::Shape::CreateCapsule(const Float32& radius_, const Float32& height_, const Mass& mass_)
{
	return WrapReference(new Shape(Handle::CreateCapsule(radius_, height_, mass_)));
}
inline GreatVEngine::Reference<GreatVEngine::Physics::BulletPhysics::Shape> GreatVEngine::Physics::BulletPhysics::Shape::CreateGeometry(Reference<Geometry> geometry_, const Mass& mass_, const Mat4& transform_)
{
	return WrapPointer(new Shape(Handle::CreateGeometry(geometry_, mass_, transform_)));
}

inline GreatVEngine::Physics::BulletPhysics::Shape::Shape(Reference<Handle> handle_):
	handle(std::move(handle_))
{
}
#pragma endregion
#pragma region Bodies

#pragma region Rigid

inline GreatVEngine::Physics::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_):
	Rigid(shape_, Position::Value(0.0f))
{
}
inline GreatVEngine::Physics::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Position::Value& position_):
	Rigid(shape_, position_, Vec3(0.0f))
{
}
inline GreatVEngine::Physics::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Position::Value& position_, const Angle::Value& angle_):
	Rigid(shape_, position_, angle_, Vec3(0.0f))
{
}
inline GreatVEngine::Physics::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_):
	Rigid(shape_, position_, angle_, velocity_, Vec3(0.0f))
{
}
inline GreatVEngine::Physics::BulletPhysics::Bodies::Rigid::Rigid(Reference<Shape> shape_, const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_, const Torque::Value& omega_):
	Physics::Bodies::Rigid(position_, angle_, velocity_, omega_),
	Physics::BulletPhysics::Body(shape_),
	shape(shape_),
	handle(new Handle(shape_->handle, position_, angle_, shape->GetMass()))
{
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion

