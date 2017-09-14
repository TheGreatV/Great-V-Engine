#pragma region Include

#pragma once


#include <Header/Header.hpp>
#include <Header/ErrorHandling.hpp>
#include <Header/Function.hpp>

#include <Logic/Helpers.hpp>

#include <Input/Input.hpp>

#include <Geometry/Collision.hpp>

#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;

#pragma endregion


GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


/*template<class Type>
class LinkCollection
{
public:
	using Elements = List<Link<Type>>;
protected:
	Elements elements;
public:
	inline LinkCollection() = default;
	inline LinkCollection(const LinkCollection&) = delete;
	inline ~LinkCollection() = default;
public:
	inline LinkCollection& operator = (const LinkCollection&) = delete;
public:
	inline void Add(const Reference<Type>& element_)
	{
		auto link = MakeLink(element_);
		auto it = elements.insert(elements.end(), link);

		// TODO: need to unsubscribe before LinkCollection die
		auto callback = Function<void, void>([this, it](){
			elements.erase(it);
		});

		element_->Subscribe_OnDestruction(callback);
	}
	inline typename Elements::const_iterator begin() const
	{
		return elements.cbegin();
	}
	inline typename Elements::const_iterator end() const
	{
		return elements.cend();
	}
};*/


/*
class LinkCollection
{
public:
	class Entity;
protected:
	Vector<Link<Entity>> entities;
protected:
	inline void Remove(const Link<Entity>& entity_);
public:
	inline void Add(const Reference<Entity>& entity_);
};
class LinkCollection::Entity:
	public This<Entity>,
	public Helper::Subscrption::OnDestructionEvent
{
// protected:
// 	Helper::Subscrption::OnEvent<Reference<Entity>> onDestruction;
public:
	// virtual ~Entity()
	// {
	// 	// auto owned = GetThis<Entity>();
	// 	// 
	// 	// onDestruction(owned);
	// }
};
inline void LinkCollection::Add(const Reference<Entity>& entity_)
{
	auto link = MakeLink(entity_);
	
	entities.push_back(link);
	
	entity_->Subscribe_OnDestruction(Function<void, void>([link]() {
	}));
}
inline void LinkCollection::Remove(const Link<Entity>& entity_)
{
	auto it = std::find(entities.begin(), entities.end(), entity_);

	if (it != entities.end())
	{
		entities.erase(it);
	}
}
*/


namespace Desert
{
	namespace Selectable
	{
		class Selection;
		class Selector;
	}
	namespace Inventory
	{
		class Slot;
		class Dock;
	}

	class Entity;
	class Game;
	class State;
	namespace States
	{
		class Empty;
		class Reversable;
		class FreeCamera;
		class ShipTracker;
	}
	
	class Slot;
	class Dock;
	class Item;
	
	class Ship;
	namespace Ships
	{
	}
	class Equipment;
	namespace Equipments
	{
		class Engine;
		namespace Engines
		{
			class Default;
		}
	}


	namespace Selectable
	{
		class Selection
		{
		public:
			static const Float32 NO_SELECTION;
		public:
			virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const
			{
				return NO_SELECTION;
			}
		};
		class Selector
		{
		public:
			List<Reference<Selection>> selections;
		public:
			inline void Add(Reference<Selection> selection_)
			{
				selections.push_back(selection_);
			}
		public:
			Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_) const
			{
				auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
				auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
				auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
				auto d = normalize(p2 - p1);

				Float32 minDistance = Selection::NO_SELECTION;
				Reference<Selection> picked = nullptr;

				for (auto &selection : selections)
				{
					auto distance = selection->GetDistance(p1, d);
					if (distance < minDistance)
					{
						minDistance = distance;
						picked = selection;
					}
				}

				return picked;
			}
			template <class Condition_> Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_, Condition_ condition_) const
			{
				auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
				auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
				auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
				auto d = normalize(p2 - p1);

				Float32 minDistance = Selection::NO_SELECTION;
				Reference<Selection> picked = nullptr;

				for (auto &selection : selections)
				{
					if (condition_(selection))
					{
						auto distance = selection->GetDistance(p1, d);
						if (distance < minDistance)
						{
							minDistance = distance;
							picked = selection;
						}
					}
				}

				return picked;
			}
		};

		const Float32 Selection::NO_SELECTION = FLT_MAX;
	}
	namespace Inventory
	{
		enum class Rotation : UInt32
		{	// Values indicates indices in array and must NOT be changed!
			Front = 0,
			Right = 1,
			Back = 2,
			Left = 3,
		};

		class Slot:
			public This<Slot>
		{
		public:
			Array<Reference<Slot>, 4> links; // TODO: replace with Array<Link>
			Reference<Dock> dock = nullptr;
		public:
			inline Slot(const Reference<Slot>& this_):
				This(this_)
			{
			}
		};
		class Dock:
			public This<Dock>
		{
		public:
			Array<Reference<Dock>, 4>	links;
			Reference<Slot> slot = nullptr;
		public:
			inline Dock(const Reference<Dock>& this_):
				This(this_)
			{
			}
		protected:
			bool IsConnectPossible(const Reference<Slot>& slot_, const Rotation& rotation_, Set<const Dock*const>& checked_)
			{
				if (!slot_ || slot_->dock)
				{
					return false;
				}

				checked_.insert(this);

				for (Size i = static_cast<Size>(Rotation::Front); i <= static_cast<Size>(Rotation::Left); ++i)
				{
					auto &link = links[i];

					auto j = (i + static_cast<Size>(rotation_)) % 4;
					auto &linkedSlot = slot_->links[j];

					if (link && checked_.find(link.get()) == checked_.end()) // if link exist and not checked yet
					{
						if (!linkedSlot || !(link->IsConnectPossible(linkedSlot, rotation_, checked_)))
						{
							return false;
						}
					}
				}

				return true;
			}
		public:
			bool IsAttached() const
			{
				return slot != nullptr;
			}
			bool IsConnectPossible(const Reference<Slot>& slot_, const Rotation& rotation_ = Rotation::Front)
			{
				Set<const Dock*const> checked;

				return IsConnectPossible(slot_, rotation_, checked);
			}
			void Attach(const Reference<Slot>& slot_, const Rotation& rotation_ = Rotation::Front)
			{
				if (slot)
				{
					throw Exception();
				}

				slot = slot_;
				slot->dock = GetThis<Dock>();

				for (UInt32 i = static_cast<UInt32>(Rotation::Front); i <= static_cast<UInt32>(Rotation::Left); ++i)
				{
					auto &link = links[i];

					auto j = (i + static_cast<UInt32>(rotation_)) % 4;
					auto &linkedSlot = slot_->links[j];

					if (link && !link->slot)
					{
						link->Attach(linkedSlot, rotation_);
					}
				}
			}
			void Detach()
			{
				auto shared = GetThis<Dock>();

				if (!slot || slot->dock != shared)
				{
					throw Exception("");
				}

				slot->dock = nullptr;
				slot = nullptr;

				for (UInt32 i = static_cast<UInt32>(Rotation::Front); i <= static_cast<UInt32>(Rotation::Left); ++i)
				{
					auto &link = links[i];

					if (link && link->slot)
					{
						link->Detach();
					}
				}
			}
		};
	}

	class Entity: This<Entity>
	{
	protected:
		const Link<Game> game;
	public:
		inline Entity(const Reference<Entity>& this_, const Reference<Game>& game_):
			This(this_),
			game(game_)
		{
		}
		virtual ~Entity() = default;
	public:
		inline Reference<Game> GetGame() const
		{
			return MakeReference(game);
		}
	public:
		virtual void Update() const
		{
		}
	};
	class Game:
		public This<Game>
	{
	public:
		const WinAPI::Window::Size resolution;
		const Reference<WinAPI::Instance> instance;
		const Reference<WinAPI::WindowClass> windowClass;
		const Reference<WinAPI::Window> window;
		const Reference<WinAPI::DeviceContext> deviceContext;
	public:
		const Reference<Graphics::OpenGL::Engine> graphicsEngine;
		const Reference<Physics::BulletPhysics::Engine> physicsEngine;
	public:
		const Reference<Graphics::OpenGL::Scene> graphicsScene_Main;
		const Reference<Graphics::OpenGL::Lights::Direction> graphicsLight_Sunlight;
		const Reference<Graphics::OpenGL::Environments::Globalmap> graphicsEnvironment_Main;
		const Reference<Graphics::OpenGL::Material> graphicsMaterial_Flat;
		const Reference<Graphics::Camera> graphicsCamera_Main;
	public:
		const Reference<Physics::BulletPhysics::World> physicsWorld_Main;
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject_Test;
	public:
		Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody_Test;
	public:
		Vector<Reference<Entity>> entities;
	public:
		Reference<State> state;
		Reference<Selectable::Selector> selector = MakeReference<Selectable::Selector>();
	public:
		Game(const Reference<Game>& this_);
	public:
		template<class Entity_, class...Arguements_>
		inline Reference<Entity_> Add(Arguements_&&... arguments_)
		{
			auto ownedThis = GetThis<Game>();

			auto entity = Make<Entity_>(ownedThis, arguments_...);

			entities.push_back(entity);

			return entity;
		}
	public:
		void Loop();
	};
	class State:
		public This<State>
	{
	protected:
		const Link<Game> game;
	public:
		inline State(const Reference<State>& this_, const Reference<Game>& game_) :
			This(this_),
			game(game_)
		{
		}
		virtual ~State() = default;
	public:
		inline Reference<Game> GetGame() const
		{
			return MakeReference(game);
		}
	public:
		virtual Reference<State> Update() = 0;
	};
	namespace States
	{
		class Empty:
			public State
		{
		public:
			inline Empty(const Reference<Empty>& this_, const Reference<Game>& game_) :
				State(this_, game_)
			{
			}
			virtual ~Empty() = default;
		public:
			virtual Reference<State> Update() override;
		};
		class Reversable
		{
		protected:
			const Reference<State> previous;
		public:
			inline Reversable(const Reference<State>& previous_) :
				previous(previous_)
			{
			}
			virtual ~Reversable() = default;
		};
		class FreeCamera:
			public State,
			public Reversable,
			public Helper::Transformation::Dimension3::ViewMatrix
		{
		public:
			inline FreeCamera(const Reference<FreeCamera>& this_, const Vec3& position_, const Vec3& angle_, const Reference<State>& previous_, const Reference<Game>& game_) :
				State(this_, game_),
				Reversable(previous_),
				ViewMatrix(position_, angle_)
			{
			}
			virtual ~FreeCamera() override = default;
		public:
			virtual Reference<State> Update() override;
		};
		class ShipTracker:
			public State
		{
		protected:
			const Link<Ship> ship;
			Float32 distance = 100.0f;
			bool isPositionChanging = false;
			Vec3 position = Vec3(0.0f);
			bool isAngleChanging = false;
			Vec3 angle = Vec3(0.0f);
		public:
			inline ShipTracker(const Reference<ShipTracker>& this_, const Reference<Ship>& ship_, const Reference<Game>& game_):
				State(this_, game_),
				ship(ship_)
			{
			}
			virtual ~ShipTracker() = default;
		public:
			virtual Reference<State> Update() override;
		};
	}

	class Slot:
		public Entity,
		public Selectable::Selection,
		public Inventory::Slot,
		public Helper::Transformation::HMat3
	{
	public:
		const Link<Ship> ship;
		Reference<Graphics::OpenGL::Object> graphicsObject;
	public:
		inline Slot(const Reference<Slot>& this_, const Reference<Game>& game_, const Reference<Ship>& ship_, const Vec3& pos_):
			Entity(this_, game_),
			Selection(),
			Inventory::Slot(this_),
			HierarchyMatrix(pos_, Vec3(0.0f), Vec3(1.0f), nullptr),
			ship(ship_)
		{
			game_->selector->Add(this_);

			graphicsObject = MakeReference<Graphics::OpenGL::Object>(game_->graphicsEngine);
			{
				graphicsObject->SetLocalPosition(pos_);

				auto geometry = Geometry::CreateBox(Vec3(4.5f, 1.0f, 4.5f), Vec3(1.0f), UVec3(1));
				auto shape = WrapReference(new Graphics::OpenGL::Shape(game_->graphicsEngine, geometry));
				auto model = WrapReference(new Graphics::OpenGL::Model(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat));
				
				graphicsObject->SetModel(model);
				graphicsObject->SetLocalPosition(Vec3(0.0f, -0.5f, 0.0f));
				graphicsObject->SetParent(this);
				
				game_->graphicsScene_Main->Add(graphicsObject);
			}
		}
	public:
		virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
		{
			if (Cast::Ray::BoxProcess({ position_, direction_ }, { graphicsObject->GetPosition(), graphicsObject->GetAngle(), Vec3(4.5f, 1.0f, 4.5f) }))
			{
				return Cast::Ray::GetLast().distance;
			}

			return Selection::GetDistance(position_, direction_);
		}
	};
	class Dock :
		public Entity,
		public Selectable::Selection,
		public Inventory::Dock,
		public Helper::Transformation::Dimension3::HierarchyMatrix
	{
	public:
		const Link<Item> item;
	public:
		inline Dock(const Reference<Dock>& this_, const Reference<Game>& game_, const Vec3& pos_, const Reference<Item>& item_):
			Entity(this_, game_),
			Selectable::Selection(),
			Inventory::Dock(this_),
			HierarchyMatrix(pos_, Vec3(0.0f), Vec3(1.0f), nullptr),
			item(item_)
		{
			game_->selector->Add(this_);
		}
		virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
		{
			if (Cast::Ray::BoxProcess({ position_, direction_ }, { GetPosition(), GetAngle(), Vec3(5.0f) }))
			{
				return Cast::Ray::GetLast().distance;
			}

			return Selection::GetDistance(position_, direction_);
		}
		void Glue(const Reference<Slot>& slot_, const Inventory::Rotation& rotation_);
		void Glue(const Vec3& position_, const Inventory::Rotation& rotation_);
	};
	class Item: // TODO: merge with Equipment?
		public Entity,
		public Helper::Transformation::Dimension3::HierarchyMatrix
	{
	public:
		Inventory::Rotation rotation = Inventory::Rotation::Front;
		Vector<Reference<Dock>> docks;
	public:
		inline Item(const Reference<Item>& this_, const Reference<Game>& game_):
			Entity(this_, game_),
			HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr)
		{
		}
	};

	class Ship:
		public Entity,
		public Helper::Transformation::HMat3
	{
	protected:
		Vector<Reference<Slot>> slots; // TODO: make const
	public:
		inline Ship(const Reference<Ship>& this_, const Reference<Game>& game_, const Vec3& position_, const Vec3& angle_) :
			Entity(this_, game_),
			HierarchyMatrix(position_, angle_, Vec3(1.0f), nullptr)
		{
		}
	};
	namespace Ships
	{
		class Default:
			public Ship
		{
		protected:
			const Reference<Graphics::OpenGL::Object> graphicsObject;
			const Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody;
		public:
			inline Default(const Reference<Default>& this_, const Reference<Game>& game_, const Vec3& position_, const Vec3& angle_):
				Ship(this_, game_, position_, angle_),
				graphicsObject(MakeReference<Graphics::OpenGL::Object>(game_->graphicsEngine)),
				physicsBody(MakeReference<Physics::BulletPhysics::Bodies::Rigid>(Physics::BulletPhysics::Shape::CreateBox(Vec3(10.0f, 20.0f, 30.0f), 10.0f), position_, angle_))
			{
				slots.resize(6);
				{
					slots[0] = Make<Slot>(game_, this_, Vec3(-2.5f, 12.0f, -5.0f));
					{
						slots[0]->SetParent(this);
					}
					slots[1] = Make<Slot>(game_, this_, Vec3(+2.5f, 12.0f, -5.0f));
					{
						slots[1]->SetParent(this);
					}
					slots[2] = Make<Slot>(game_, this_, Vec3(-2.5f, 12.0f, 0.0f));
					{
						slots[2]->SetParent(this);
					}
					slots[3] = Make<Slot>(game_, this_, Vec3(+2.5f, 12.0f, 0.0f));
					{
						slots[3]->SetParent(this);
					}
					slots[4] = Make<Slot>(game_, this_, Vec3(-2.5f, 12.0f, +5.0f));
					{
						slots[4]->SetParent(this);
					}
					slots[5] = Make<Slot>(game_, this_, Vec3(+2.5f, 12.0f, +5.0f));
					{
						slots[5]->SetParent(this);
					}

					slots[0]->links[static_cast<Size>(Inventory::Rotation::Right)]	= slots[1];
					slots[1]->links[static_cast<Size>(Inventory::Rotation::Left)]	= slots[0];
					slots[0]->links[static_cast<Size>(Inventory::Rotation::Front)]	= slots[2];
					slots[1]->links[static_cast<Size>(Inventory::Rotation::Front)]	= slots[3];

					slots[2]->links[static_cast<Size>(Inventory::Rotation::Back)]	= slots[0];
					slots[3]->links[static_cast<Size>(Inventory::Rotation::Back)]	= slots[1];
					slots[2]->links[static_cast<Size>(Inventory::Rotation::Right)]	= slots[3];
					slots[3]->links[static_cast<Size>(Inventory::Rotation::Left)]	= slots[2];
					slots[2]->links[static_cast<Size>(Inventory::Rotation::Front)]	= slots[4];
					slots[3]->links[static_cast<Size>(Inventory::Rotation::Front)]	= slots[5];

					slots[4]->links[static_cast<Size>(Inventory::Rotation::Back)]	= slots[2];
					slots[5]->links[static_cast<Size>(Inventory::Rotation::Back)]	= slots[3];
					slots[4]->links[static_cast<Size>(Inventory::Rotation::Right)]	= slots[5];
					slots[5]->links[static_cast<Size>(Inventory::Rotation::Left)]	= slots[4];
				}

				graphicsObject;
				{
					auto geometry = Geometry::CreateBox(Vec3(10.0f, 20.0f, 30.0f), Vec3(1.0f), UVec3(1));
					auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
					auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat);

					graphicsObject->SetModel(model);
					graphicsObject->SetParent(this);

					game_->graphicsScene_Main->Add(graphicsObject);
				}

				physicsBody;
				{
					game_->physicsWorld_Main->Add(physicsBody);

					physicsBody->SetUser(this_);
				}
			}
		public:
			virtual void Update() const override
			{
				auto ownedGame = GetGame();

				Float32 speed = 20.0f;

				if (KeyState(Keys::W))
				{
					physicsBody->AddCentralImpulse(GetRMat() * Vec3(0.0f, 0.0f, speed));
				}
				if (KeyState(Keys::S))
				{
					physicsBody->AddCentralImpulse(GetRMat() * Vec3(0.0f, 0.0f, -speed));
				}
				if (KeyState(Keys::D))
				{
					physicsBody->AddCentralImpulse(GetRMat() * Vec3(speed, 0.0f, 0.0f));
				}
				if (KeyState(Keys::A))
				{
					physicsBody->AddCentralImpulse(GetRMat() * Vec3(-speed, 0.0f, 0.0f));
				}
				if (KeyState(Keys::SPACE))
				{
					physicsBody->AddCentralImpulse(GetRMat() * Vec3(0.0f, speed, 0.0f));
				}
				if (KeyState(Keys::L_CTRL))
				{
					physicsBody->AddCentralImpulse(GetRMat() * Vec3(0.0f, -speed, 0.0f));
				}

				auto p = physicsBody->GetPosition();
				auto mat = Move4(p) * RotateZXY4(physicsBody->GetAngle());
				auto rmat = physicsBody->GetRMat();

				if (KeyState(Keys::Q))
				{
					Float32 force = 10.0f;

					physicsBody->AddImpulse(VecXYZ(mat * Vec4(+1.0f, 0.0f, 0.0f, 1.0f)), rmat * Vec3(0.0f, +force, 0.0f));
					physicsBody->AddImpulse(VecXYZ(mat * Vec4(-1.0f, 0.0f, 0.0f, 1.0f)), rmat * Vec3(0.0f, -force, 0.0f));
				}
				if (KeyState(Keys::E))
				{
					Float32 force = 10.0f;

					physicsBody->AddImpulse(VecXYZ(mat * Vec4(+1.0f, 0.0f, 0.0f, 1.0f)), rmat * Vec3(0.0f, -force, 0.0f));
					physicsBody->AddImpulse(VecXYZ(mat * Vec4(-1.0f, 0.0f, 0.0f, 1.0f)), rmat * Vec3(0.0f, +force, 0.0f));
				}
				if (KeyState(Keys::F))
				{
					Float32 force = 10.0f;

					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, +1.0f, 1.0f)), rmat * Vec3(0.0f, +force, 0.0f));
					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, -1.0f, 1.0f)), rmat * Vec3(0.0f, -force, 0.0f));
				}
				if (KeyState(Keys::R))
				{
					Float32 force = 10.0f;

					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, +1.0f, 1.0f)), rmat * Vec3(0.0f, -force, 0.0f));
					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, -1.0f, 1.0f)), rmat * Vec3(0.0f, +force, 0.0f));
				}
				if (KeyState(Keys::C))
				{
					Float32 force = 10.0f;

					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, +1.0f, 1.0f)), rmat * Vec3(+force, 0.0f, 0.0f));
					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, -1.0f, 1.0f)), rmat * Vec3(-force, 0.0f, 0.0f));
				}
				if (KeyState(Keys::Z))
				{
					Float32 force = 10.0f;

					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, +1.0f, 1.0f)), rmat * Vec3(-force, 0.0f, 0.0f));
					physicsBody->AddImpulse(VecXYZ(mat * Vec4(0.0f, 0.0f, -1.0f, 1.0f)), rmat * Vec3(+force, 0.0f, 0.0f));
				}


				auto p0 = VecXYZ(mat * Vec4(0.0f, 0.0f, 0.0f, 1.0f));
				auto t1 = VecXYZ(mat * Vec4(0.0f, 10.0f, 0.0f, 1.0f));
				auto t2 = VecXYZ(mat * Vec4(0.0f, 0.0f, 0.0f, 1.0f)) + Vec3(0.0f, 10.0f, 0.0f);

				ownedGame->graphicsScene_Main->DrawCircle(p0, 0.5f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));
				ownedGame->graphicsScene_Main->DrawCircle(t1, 0.5f, Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				ownedGame->graphicsScene_Main->DrawCircle(t2, 0.5f, Vec4(1.0f, 1.0f, 0.0f, 1.0f));

				ownedGame->graphicsScene_Main->DrawLine(p0, t1, Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				ownedGame->graphicsScene_Main->DrawLine(p0, t2, Vec4(1.0f, 1.0f, 0.0f, 1.0f));

				auto l = Length(t2 - t1);
				if (l > 0.0f)
				{
					auto force = Normalize(t2 - t1) * pow(Clamp(l * 2.0f, 0.0f, 1.0f), 4.0f) / 2.0f * 1.5f;

					physicsBody->AddImpulse(t1, force);
				}

				auto p1 = VecXYZ(mat * Vec4(0.0f, 0.0f, 0.0f, 1.0f));
				auto t3 = VecXYZ(mat * Vec4(0.0f, -10.0f, 0.0f, 1.0f));
				auto t4 = VecXYZ(mat * Vec4(0.0f, 0.0f, 0.0f, 1.0f)) + Vec3(0.0f, -10.0f, 0.0f);

				ownedGame->graphicsScene_Main->DrawCircle(p1, 0.5f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));
				ownedGame->graphicsScene_Main->DrawCircle(t3, 0.5f, Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				ownedGame->graphicsScene_Main->DrawCircle(t4, 0.5f, Vec4(1.0f, 1.0f, 0.0f, 1.0f));

				ownedGame->graphicsScene_Main->DrawLine(p1, t3, Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				ownedGame->graphicsScene_Main->DrawLine(p1, t4, Vec4(1.0f, 1.0f, 0.0f, 1.0f));

				auto l2 = Length(t4 - t3);
				if (l2 > 0.0f)
				{
					auto force = Normalize(t4 - t3) * pow(Clamp(l2 * 2.0f, 0.0f, 1.0f), 4.0f) / 2.0f * 1.5f;

					physicsBody->AddImpulse(t3, force);
				}
			}
		};
	}

	class Equipment:
		public Item
	{
	public:
		using Mass = Float32;
	public:
		inline Equipment(const Reference<Equipment>& this_, const Reference<Game>& game_):
			Item(this_, game_)
		{
		}
	public:
		virtual Mass GetMass() const = 0;
	};
	namespace Equipments
	{
		class Engine:
			public Equipment
		{
		public:
			inline Engine(const Reference<Engine>& this_, const Reference<Game>& game_):
				Equipment(this_, game_)
			{
			}
		};
		namespace Engines
		{
			class Default :
				public Engine
			{
			public: // protected: // TODO:
				const Reference<Graphics::OpenGL::Object> graphicsObject;
			public:
				inline Default(const Reference<Default>& this_, const Reference<Game>& game_):
					Engine(this_, game_),
					graphicsObject(MakeReference<Graphics::OpenGL::Object>(game_->graphicsEngine))
				{
					docks.resize(2);
					{
						docks[0] = Make<Dock>(game_, Vec3(0.0f, -2.0f, -2.5f), this_);
						{
							docks[0]->SetParent(this);
						}
						docks[1] = Make<Dock>(game_, Vec3(0.0f, -2.0f, +2.5f), this_);
						{
							docks[1]->SetParent(this);
						}

						docks[0]->links[static_cast<Size>(Inventory::Rotation::Front)]	= docks[1];
						docks[1]->links[static_cast<Size>(Inventory::Rotation::Back)]	= docks[0];
					}

					graphicsObject;
					{
						auto geometry = Geometry::CreateBox(Vec3(4.0f, 4.0f, 9.0f), Vec3(1.0f), UVec3(1));
						auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
						auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat);

						graphicsObject->SetModel(model);
						graphicsObject->SetParent(this);

						game_->graphicsScene_Main->Add(graphicsObject);
					}
				}
			public:
				virtual Mass GetMass() const
				{
					return 100.0f;
				}
			};
		}
	}
}

Desert::Game::Game(const Reference<Game>& this_):
	This(this_),
	resolution(800, 600),
	instance(GVE::WinAPI::Instance::Get()),
	windowClass(MakeReference<GVE::WinAPI::WindowClass>(instance, "class")),
	window(MakeReference<GVE::WinAPI::Window>(windowClass, "window", resolution)),
	deviceContext(([&](){
		auto deviceContext = MakeReference<GreatVEngine::WinAPI::DeviceContext>(window);
			
		deviceContext->SetPixelFormat();
			
		return deviceContext;
	})()),

	graphicsEngine(MakeReference<Graphics::OpenGL::Engine>(deviceContext)),
	physicsEngine(MakeReference<Physics::BulletPhysics::Engine>()),

	graphicsScene_Main(MakeReference<Graphics::OpenGL::Scene>(graphicsEngine, window->GetSize())),
	graphicsLight_Sunlight(MakeReference<Graphics::OpenGL::Lights::Direction>()),
	graphicsEnvironment_Main(Graphics::OpenGL::Environments::Globalmap::LoadCube(graphicsEngine, Filepath("Media/Images/Rocks.dds"))),
	graphicsMaterial_Flat(MakeReference<Graphics::OpenGL::Material>(graphicsEngine)),
	graphicsCamera_Main(MakeReference<Graphics::Camera>()),

	physicsWorld_Main(MakeReference<Physics::BulletPhysics::World>(physicsEngine)),

	state(Make<States::Empty>(this_))
{
	graphicsLight_Sunlight;
	{
		graphicsLight_Sunlight->SetLocalAngle(Vec3(60.0f, 0.0f, 0.0f));
		graphicsLight_Sunlight->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLight_Sunlight->SetAmbient(0.4f);

		graphicsScene_Main->Add(graphicsLight_Sunlight);
		// graphicsLight_Sunlight->Hide();
	}

	graphicsEnvironment_Main;
	{
		// graphicsEnvironment_Main->Hide();

		graphicsScene_Main->Add(graphicsEnvironment_Main);
	}

	graphicsMaterial_Flat;
	{
		graphicsMaterial_Flat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		// graphicsMaterial_Flat->Technique(Graphics::Material::TechniqueType::Distortion) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Distortion/Common.glsl."), "vs", "", "", "", "fs");
		graphicsMaterial_Flat->SetValue("materialAlbedo", Vec3(1.0f));
		graphicsMaterial_Flat->SetValue("materialRoughness", 0.5f);
		graphicsMaterial_Flat->SetValue("materialMetalness", 0.0f);
	}

	graphicsCamera_Main;
	{
		graphicsCamera_Main->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera_Main->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera_Main->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}

	graphicsObject_Test = MakeReference<Graphics::OpenGL::Object>(graphicsEngine);
	{
		auto geometry = Geometry::CreateBox(Vec3(200.0f, 1.0f, 200.0f), Vec3(20.0f, 0.1f, 20.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(graphicsEngine, shape, graphicsMaterial_Flat);

		graphicsObject_Test->SetModel(model);
		graphicsObject_Test->SetLocalPosition(Vec3(0.0f, -0.5f, 0.0f));

		graphicsScene_Main->Add(graphicsObject_Test);
	}
	physicsBody_Test = MakeReference<Physics::BulletPhysics::Bodies::Rigid>(Physics::BulletPhysics::Shape::CreateBox(Vec3(200.0f, 1.0f, 200.0f), 0.0f), Vec3(0.0f, -0.5f, 0.0f), Vec3(0.0f));
	{
		physicsWorld_Main->Add(physicsBody_Test);
	}
}
void Desert::Game::Loop()
{
	while (!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		state = state->Update();


		// // //
		static bool grab = false;
		static Reference<Dock> pickedDock = nullptr;
		static Vec3 previousPosition;
		static Inventory::Rotation previousRotation;
		static Inventory::Rotation rotation = Inventory::Rotation::Front;

		auto m = Vec2(window->ToLocalPosition(IVec2(Input::Mouse::GetPosition())));

		graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));

		auto p = (m / Vec2(window->GetSize())) * 2.0f - 1.0f;

		if (KeyState(Keys::UP)) rotation = Inventory::Rotation::Front;
		if (KeyState(Keys::DOWN)) rotation = Inventory::Rotation::Back;
		if (KeyState(Keys::LEFT)) rotation = Inventory::Rotation::Left;
		if (KeyState(Keys::RIGHT)) rotation = Inventory::Rotation::Right;

		for (auto &i : selector->selections)
		{
			if (auto slot = UpCast<Slot>(i))
			{
				slot->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else if (auto dock = UpCast<Dock>(i))
			{
				auto item = MakeReference(dock->item);

				if (auto t = UpCast<Equipments::Engines::Default>(item))
				{
					t->graphicsObject->SetColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
				}
			}
			// auto dock = std::dynamic_pointer_cast<Dock>(i);
			// if (dock && dock->item && dock->item->graphicsObject)
			// {
			// 	dock->item->graphicsObject->SetColor(Vec4(1.0f));
			// }
		}

		if (auto picked = selector->Pick(graphicsCamera_Main, p))
		{
			if (auto slot = UpCast<Slot>(picked))
			{
				slot->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
			}
			else if (auto dock = UpCast<Dock>(picked))
			{
				auto item = MakeReference(dock->item);

				if (auto t = UpCast<Equipments::Engines::Default>(item))
				{
					t->graphicsObject->SetColor(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}

		if (pickedDock)
		{
			auto item = MakeReference(pickedDock->item);

			if (auto t = UpCast<Equipments::Engines::Default>(item))
			{
				t->graphicsObject->SetColor(Vec4(1.0f, 0.5f, 0.5f, 1.0f));
			}
		}

		if (Input::Mouse::GetButtonState(Input::Mouse::Button::Left))
		{
			if (!grab)
			{
				auto picked = selector->Pick(graphicsCamera_Main, p);

				if (auto dock = UpCast<Dock>(picked))
				{
					pickedDock = dock;

					if (dock->IsAttached())
					{
						dock->Detach();
					}

					auto item = MakeReference(dock->item);

					rotation = item->rotation;

					previousPosition = item->GetLocalPosition();
					previousRotation = rotation;
				}

				grab = true;
			}
			else if (pickedDock)
			{
				auto picked = selector->Pick(graphicsCamera_Main, p, [](Reference<Selectable::Selection> selection){
					return UpCast<Slot>(selection) != nullptr;
				});

				if (auto slot = UpCast<Slot>(picked))
				{
					pickedDock->Glue(slot, rotation);

					if (pickedDock->IsConnectPossible(slot, rotation))
					{
						slot->graphicsObject->SetColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
					}
					else
					{
						slot->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
					}
				}
			}
		}
		else
		{
			if (pickedDock)
			{
				auto picked = selector->Pick(graphicsCamera_Main, p, [](Reference<Selectable::Selection> selection){
					return UpCast<Slot>(selection) != nullptr;
				});

				if (auto slot = UpCast<Slot>(picked))
				{
					if (pickedDock->IsConnectPossible(slot, rotation))
					{
						pickedDock->Glue(slot, rotation);

						pickedDock->Attach(slot, rotation);
					}
					else
					{
						pickedDock->Glue(previousPosition, previousRotation);
					}
				}
			}

			pickedDock = nullptr;
			grab = false;
		}
		// // //


		for (auto &entity : entities)
		{
			entity->Update();
		}

		physicsWorld_Main->Render(1.0f / 60.0f);
		graphicsScene_Main->Render(graphicsCamera_Main);

		Sleep(1000 / 60);

		deviceContext->SwapBuffers();
	}
}

Reference<Desert::State> Desert::States::Empty::Update()
{
	if (KeyState(Keys::TILDE) && !KeyPress(Keys::TILDE))
	{
		PressKey(Keys::TILDE);

		auto ownedThis = GetThis<Empty>();
		auto ownedGame = GetGame();

		auto freeCamera = Make<States::FreeCamera>(ownedGame->graphicsCamera_Main->GetPosition(), ownedGame->graphicsCamera_Main->GetAngle(), ownedThis, ownedGame);

		return freeCamera;
	}

	return GetThis<Empty>();
}
Reference<Desert::State> Desert::States::FreeCamera::Update()
{
	auto ownedGame = GetGame();
	auto camera = ownedGame->graphicsCamera_Main;

	if (KeyState(Keys::TILDE) && !KeyPress(Keys::TILDE))
	{
		PressKey(Keys::TILDE);

		return previous;
	}

	Input::Mouse::SetPosition(Vec2(WinAPI::GetDesktopSize()) / 2.0f);
	Float32 motion = 0.1f * (KeyState(Keys::X) ? 0.1f : 1.0f);
	Rotate(Vec3(
		-Input::Mouse::GetPositionDelta().y * motion,
		Input::Mouse::GetPositionDelta().x * motion,
		KeyState(Keys::Q) ? -2.0f : KeyState(Keys::E) ? +2.0f : 0.0f));
	
	Float32 speed = 1.0f * KeyState(Keys::L_SHIFT) ? 10.0f : KeyState(Keys::L_ALT) ? 0.1f : 1.0f;
	Move(Vec3(
		KeyState(Keys::D) ? +speed : KeyState(Keys::A) ? -speed : 0.0f,
		KeyState(Keys::SPACE) ? +speed : KeyState(Keys::L_CTRL) ? -speed : 0.0f,
		KeyState(Keys::W) ? +speed : KeyState(Keys::S) ? -speed : 0.0f));

	auto currentPosision = GetPosition();
	auto currentAngle = GetAngle();

	camera->SetPosition(currentPosision);
	camera->SetAngle(currentAngle);

	return GetThis<FreeCamera>();
}
Reference<Desert::State> Desert::States::ShipTracker::Update()
{
	auto ownedGame = GetGame();
	auto ownedShip = MakeReference(ship);

	if (KeyState(Keys::TILDE) && !KeyPress(Keys::TILDE))
	{
		PressKey(Keys::TILDE);

		auto ownedThis = GetThis<Empty>();

		auto freeCamera = Make<States::FreeCamera>(ownedGame->graphicsCamera_Main->GetPosition(), ownedGame->graphicsCamera_Main->GetAngle(), ownedThis, ownedGame);

		return freeCamera;
	}

	if (ButtonState(Buttons::Middle) && !ButtonState(Buttons::Right))
	{
		isPositionChanging = true;
	}
	else
	{
		isPositionChanging = false;
	}

	if (isPositionChanging)
	{
		Float32 motion = 0.1f;
		
		position += RotateZXY3(angle) * Vec3(
			-Input::Mouse::GetPositionDelta().x * motion,
			-Input::Mouse::GetPositionDelta().y * motion,
			0.0f
		);
	}

	if (ButtonState(Buttons::Right) && !ButtonState(Buttons::Middle))
	{
		isAngleChanging = true;
	}
	else
	{
		isAngleChanging = false;
	}

	if (isAngleChanging)
	{
		Float32 motion = 0.35f;

		angle += Vec3(
			-Input::Mouse::GetPositionDelta().y * motion,
			Input::Mouse::GetPositionDelta().x * motion,
			0.0f
		);
	}

	auto graphicsCamera = ownedGame->graphicsCamera_Main;

	distance;
	{
		Float32 speed = 5.0f;
		Float32 delta = 0.0f;

		if (KeyState(Keys::PLUS))
		{
			delta += speed;
		}
		if (KeyState(Keys::MINUS))
		{
			delta -= speed;
		}

		distance = Clamp(distance + delta, 50.0f, 250.0f);
	}

	auto currentPosition = ownedShip->GetPosition() +
		(ownedShip->GetRMat() * position) +
		(ownedShip->GetRMat() * RotateZXY3(angle) * Vec3(0.0f, 0.0f, -distance));
	auto currentAngle = GetAngle(ownedShip->GetRMat() * RotateZXY3(angle));

	graphicsCamera->SetPosition(currentPosition);
	graphicsCamera->SetAngle(currentAngle);

	return GetThis<ShipTracker>();
}

void Desert::Dock::Glue(const Reference<Slot>& slot_, const Inventory::Rotation& rotation_)
{
	auto ownedItem = MakeReference(item);
	auto ownedShip = MakeReference(slot_->ship);

	auto angle = static_cast<Size>(rotation_)* 90.0f;

	ownedItem->SetLocalPosition(slot_->GetLocalPosition() - (RotateY3(angle) * GetLocalPosition()));
	ownedItem->SetParent(ownedShip.get());

	ownedItem->rotation = rotation_;
	ownedItem->SetLocalAngle(Vec3(0.0f, angle, 0.0f));
}
void Desert::Dock::Glue(const Vec3& position_, const Inventory::Rotation& rotation_)
{
	auto ownedItem = MakeReference(item);

	auto angle = static_cast<Size>(rotation_)* 90.0f;

	ownedItem->SetLocalPosition(position_);
	ownedItem->SetParent(nullptr);

	ownedItem->rotation = rotation_;
	ownedItem->SetLocalAngle(Vec3(0.0f, angle, 0.0f));
}


void func()
{
	/*
	class A:
		public Helper::Subscrption::OnDestructionEvent
	{
	};

	LinkCollection<A> lc;

	{
		auto a1 = MakeReference<A>();

		lc.Add(a1);

		std::cout << "----------" << std::endl;
		std::cout << "x" << std::endl;

		for (auto &i : lc)
		{
			std::cout << "x";
		}

		std::cout << std::endl;

		std::cout << "----------" << std::endl;
	}

	std::cout << "----------" << std::endl;
	std::cout << "" << std::endl;

	for (auto &i : lc)
	{
		std::cout << "x";
	}

	std::cout << std::endl;

	std::cout << "----------" << std::endl;
	*/

	auto game = Make<Desert::Game>();
	
	auto ship = game->Add<Desert::Ships::Default>(Vec3(0.0f, 15.0f, 0.0f), Vec3(0.0f));
	game->state = Make<Desert::States::ShipTracker>(ship, game);
	
	auto engine = game->Add<Desert::Equipments::Engines::Default>();
	engine->SetLocalPosition(Vec3(20.0f, 15.0f, 0.0f));

	auto engine2 = game->Add<Desert::Equipments::Engines::Default>();
	engine2->SetLocalPosition(Vec3(25.0f, 20.0f, 0.0f));

	game->Loop();
}

void main()
{
	try
	{
		func();
	}
	catch (GVE::WinAPI::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "WinAPI Exception", MB_OK);
	}
	catch (GVE::OpenGL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenGL Exception", MB_OK);
	}
	catch (GVE::OpenIL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenIL Exception", MB_OK);
	}
	catch (GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	// std::system("pause");

	return;
}