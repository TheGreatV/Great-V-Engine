#pragma region Include
#pragma once


#include "Header.hpp"


#include <System/System.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace Physics
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


		class Engine
		{
		public:
			inline Engine() = default;
			inline Engine(const Engine&) = delete;
			inline ~Engine() = default;
		public:
			inline Engine& operator = (const Engine&) = delete;
		};
		class World
		{
		public:
			inline World() = default;
			inline World(const World&) = delete;
			inline ~World() = default;
		public:
			inline World& operator = (const World&) = delete;
		};
		class Shape:
			public Helper::Transformation::Dimension3::BoundBox
		{
		};
		class Material
		{
		};
		class Body
		{
		};
		namespace Bodies
		{
			class Rigid:
				public Body,
				public Helper::Transformation::Dimension3::ViewMatrix,
				public Helper::Transformation::Dimension3::Velocity,
				public Helper::Transformation::Dimension3::Torque
			{
			public:
				using User = Helper::Transformation::Dimension3::HierarchyMatrix;
			protected:
				Reference<User> user;
			public:
				inline Rigid();
				inline Rigid(const Position::Value& position_);
				inline Rigid(const Position::Value& position_, const Angle::Value& angle_);
				inline Rigid(const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_);
				inline Rigid(const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_, const Torque::Value& omega_);
				inline Rigid(const Rigid&) = delete;
				inline ~Rigid() = default;
			public:
				inline Rigid& operator = (const Rigid&) = delete;
			public:
				inline Reference<User> GetUser() const
				{
					return user;
				}
				inline void SetUser(const Reference<User>& user_)
				{
					user = user_;
				}
				inline void Update(const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_, const Torque::Value& omega_)
				{
					SetPosition(position_);
					SetAngle(angle_);
					SetVelocity(velocity_);
					SetTorque(omega_);

					if(user)
					{
						user->SetLocalPosition(position_);
						user->SetLocalAngle(angle_);
					}
				}
			};
		}
		namespace Constraints
		{
			class Slider
			{
			};
		}
	}
}


#pragma region Bodies
#pragma region Rigid
inline GreatVEngine::Physics::Bodies::Rigid::Rigid():
	Rigid(Position::Value(0.0f))
{
}
inline GreatVEngine::Physics::Bodies::Rigid::Rigid(const Position::Value& position_) :
	Rigid(position_, Vec3(0.0f))
{
}
inline GreatVEngine::Physics::Bodies::Rigid::Rigid(const Position::Value& position_, const Angle::Value& angle_) :
	Rigid(position_, angle_, Vec3(0.0f))
{
}
inline GreatVEngine::Physics::Bodies::Rigid::Rigid(const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_) :
	Rigid(position_, angle_, velocity_, Vec3(0.0f))
{
}
inline GreatVEngine::Physics::Bodies::Rigid::Rigid(const Position::Value& position_, const Angle::Value& angle_, const Velocity::Value& velocity_, const Torque::Value& omega_):
	ViewMatrix(position_, angle_),
	Velocity(velocity_),
	Torque(omega_)
{
}
#pragma endregion
#pragma endregion


#pragma region
#pragma endregion


