#pragma region Include

#pragma once


#include <Header/Header.hpp>
#include <Header/ErrorHandling.hpp>
#include <Header/Function.hpp>

#include <Logic/Helpers.hpp>

#include <Input/Input.hpp>

#include <Geometry/Collision.hpp>

#include <Infrastructure/OpenGL.hpp>

#include <Utilities/JSON/JSON.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;

#pragma endregion


#pragma region MACRO

#pragma region FABRIC

#define FABRIC(BASE, TYPE, COLLECTION_NAME, ELEMENT_NAME) \
protected: \
	Vector<Reference<TYPE>> COLLECTION_NAME; \
public: \
	template<class Type> \
	inline void Add(const Reference<Type>& ELEMENT_NAME##_) \
	{ \
		COLLECTION_NAME.push_back(ELEMENT_NAME##_); \
	} \
	template<class Type, class...Arguments> \
	inline Link<Type> Create(Arguments...arguments) \
	{ \
		auto ELEMENT_NAME = Make<Type>(GetThis<BASE>(), arguments...); \
		Add(ELEMENT_NAME); \
		return ELEMENT_NAME; \
	} \
	inline void Remove(const Link<TYPE>& ELEMENT_NAME##_) \
	{ \
		auto owned = MakeReference(ELEMENT_NAME##_); \
		auto it = std::find_if( \
			COLLECTION_NAME.begin(), \
			COLLECTION_NAME.end(), \
			[&](const Reference<TYPE>& i) \
			{ \
				return i == owned; \
			} \
		); \
		COLLECTION_NAME.erase(it); \
	}

#pragma endregion

#pragma region DEPENDENT

#define DEPENDENT(OWNER_TYPE, OWNER_NAME) \
	class Dependent \
	{ \
	protected: \
		const Link<OWNER_TYPE> OWNER_NAME; \
	public: \
		inline Dependent(const Reference<OWNER_TYPE> OWNER_NAME##_): \
			OWNER_NAME(OWNER_NAME##_) \
		{ \
		} \
	public: \
		inline Reference<OWNER_TYPE> Get##OWNER_TYPE() const \
		{ \
			return MakeReference(OWNER_NAME); \
		} \
	};

#pragma endregion

#pragma endregion


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
		List<Link<Selection>> selections;
	protected:
		void OnSelectionDestroyed()
		{
		}
	public:
		inline void Add(Reference<Selection> selection_)
		{
			selections.push_back(MakeLink(selection_));

			// selection_->
		}
		inline void Remove(Reference<Selection> selection_)
		{
			// selections.remove(MakeLink(selection_));
		}
	public:
		Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_) const
		{
			/*
			auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
			auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
			auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
			auto d = normalize(p2 - p1);

			Float32 minDistance = Selection::NO_SELECTION;
			Reference<Selection> picked = nullptr;

			for(auto &selection : selections)
			{
				auto distance = selection->GetDistance(p1, d);
				if(distance < minDistance)
				{
					minDistance = distance;
					picked = selection;
				}
			}

			return picked;
			*/

			throw Exception();
		}
		template <class Filter_> Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_, Filter_ filter_) const
		{
			/*
			auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
			auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
			auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
			auto d = normalize(p2 - p1);

			Float32 minDistance = Selection::NO_SELECTION;
			Reference<Selection> picked = nullptr;

			for(auto &selection : selections)
			{
				if(filter_(selection))
				{
					auto distance = selection->GetDistance(p1, d);
					if(distance < minDistance)
					{
						minDistance = distance;
						picked = selection;
					}
				}
			}

			return picked;
			*/
			throw Exception();
		}
		template <class Filter_, class Comparer_> Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_, Filter_ filter_, Comparer_ comparer_) const
		{
			/*
			auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
			auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
			auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
			auto d = normalize(p2 - p1);

			Float32 minDistance = Selection::NO_SELECTION;
			Reference<Selection> picked = nullptr;

			for(auto &selection : selections)
			{
				if(filter_(selection))
				{
					auto distance = selection->GetDistance(p1, d);
					if(comparer_(picked, minDistance, selection, distance))
					{
						minDistance = distance;
						picked = selection;
					}
				}
			}

			return picked;
			*/
			throw Exception();
		}
	};
}

namespace Brothel
{
	class Exception;

	class Entity;

	class Selection;

	class Game;

	class GameState;
	namespace GameStates
	{
		class None;
	}

	class CameraController;

	class Event;
	namespace Events
	{
		class FinishEvent;
		class Service;
		namespace Services
		{
			class Test;
		}
	}

	class Building;
	class Room;
	class Section; // stub for room

	class Inmate;
	class Bunk; // Bed for inmate

	class Worker;
	class Workplace;

	namespace Rooms
	{
		class Bedroom;
		namespace Bedrooms
		{
			class Miserable;
		}

		class Bathroom;

		class Workroom;

		class Hole;
	}

	class Character;
	class Personnel;
	class Guest;


	class Exception:
		public GVE::Exception
	{
	public:
		inline Exception() = default;
		inline Exception(const Text& text_): GVE::Exception(text_)
		{
		}
	};

	class Entity:
		public This<Entity>
	{
	public:
		inline Entity(const Reference<Entity>& this_):
			This(this_)
		{
		}
		virtual ~Entity() = default;
	public:
		virtual void Update();
	};

	class Selection:
		public Entity,
		public Selectable::Selection
	{
	public:
		inline Selection(const Reference<Selection>& this_):
			Entity(Cast<Entity>(this_))
		{
			// game_->selector_Main->Add(this_);
		}
	public:
		// virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override;
	};

	class Game:
		public Entity
	{
	public:
		DEPENDENT(Game, game);
	public:
		const Reference<WinAPI::Window> window;
		const Reference<Graphics::OpenGL::Engine> graphicsEngine;
	public:
		const Reference<Graphics::OpenGL::Scene> graphicsScene_Main;
		const Reference<Graphics::OpenGL::Lights::Direction> graphicsLight_Sunlight;
		const Reference<Graphics::OpenGL::Material> graphicsMaterial_Flat;
		const Reference<Graphics::Camera> graphicsCamera_Main;

		const Reference<Selectable::Selector> selector_Main;
		Reference<GameState> state;
		const Reference<CameraController> cameraController;

		Vector<Reference<Guest>> guestsQueue;
		Float32 guestGenerationCooldown = 0.0f;
		/*Vector<Reference<Guest>> processedQuests;
		Float32 guestProcessingCooldown = 60.0f * 6;*/
	public:
		inline Game(const Reference<Game>& this_, const Reference<GreatVEngine::WinAPI::DeviceContext>& deviceContext_, const Reference<WinAPI::Window>& window_);
	public:
		FABRIC(Game, Entity, entities, entity);
	protected:
		void ProcessGuests();
	public:
		inline void Change(const Reference<GameState>& state_)
		{
			state = state_;
		}
		virtual void Update() override;
	};

	class GameState
	{
	protected:
		const Reference<Game> game;
	public:
		inline GameState(const Reference<Game>& game_):
			game(game_)
		{
		}
	public:
		virtual void Update()
		{
		}
	};
	namespace GameStates
	{
		class None:
			public GameState
		{
		public:
			inline None(const Reference<Game>& game_):
				GameState(game_)
			{
			}
		public:
			virtual void Update() override;
		};
		class RotateCamera:
			public GameState
		{
		public:
			inline RotateCamera(const Reference<Game>& game_):
				GameState(game_)
			{
			}
		public:
			virtual void Update() override;
		};
		class ShiftCamera:
			public GameState
		{
		public:
			inline ShiftCamera(const Reference<Game>& game_):
				GameState(game_)
			{
			}
		public:
			virtual void Update() override;
		};
		class PersonnelAssign:
			public GameState
		{
		public:
			const Reference<Personnel> personnel;
		public:
			inline PersonnelAssign(const Reference<Game>& game_, const Reference<Personnel>& personnel_):
				GameState(game_),
				personnel(personnel_)
			{
			}
		public:
			virtual void Update() override;
		};
		class BuildRoomSelection:
			public GameState
		{
		public:
			inline BuildRoomSelection(const Reference<Game>& game_):
				GameState(game_)
			{
			}
		public:
			virtual void Update() override;
		};
		template<class T> class BuildSectionSelection:
			public GameState
		{
		public:
			inline BuildSectionSelection(const Reference<Game>& game_):
				GameState(game_)
			{
			}
		public:
			virtual void Update() override;
		};
	}

	class CameraController
	{
	public:
		enum class Mode
		{
			None,
			Move,
			Rotate,
			Pull,
			PersonnelPick
		};
	public:
		Game*const	game;
		Mode		mode = Mode::None;

		Vec3		position = Vec3(0.0f),
					smoothedPosition = Vec3(0.0f);
		Vec3		angle = Vec3(0.0f),
					smoothedAngle = Vec3(0.0f);
		Float32		distance = 20.0f,
					smoothedDistance = 20.0f;
		bool		isLastPullPositionObtained = false;
		Vec3		lastPullPosition;

		Reference<Personnel> pickedPersonnel = nullptr;

		const Reference<Graphics::Camera> graphicsCamera;
	public:
		inline CameraController(Game* game_, const Reference<Graphics::Camera> graphicsCamera_):
			game(game_),
			graphicsCamera(graphicsCamera_)
		{
		}
	protected:
		inline void PerformMoveMode();
		inline void PerformRotateMode();
		inline void PerformPullMode();
		inline void PerformPick();
	public:
		inline void Update();
	};

	class Event:
		public Entity
	{
	public:
		class Performer :
			public This<Performer>
		{
		public:
			inline Performer(const Reference<Performer>& this_):
				This(this_)
			{
			}
			virtual ~Performer() = default;
		public:
			inline void Perform(const Reference<Event>& event_);
			virtual void Perform(const Reference<Events::FinishEvent>& event_);
			virtual void Perform(const Reference<Events::Service>& event_);
			virtual void Perform(const Reference<Events::Services::Test>& event_);
		};
		friend Performer;
	// public:
	// 	using OnFinished = Helper::Subscrption::OnEvent<void, void>;
	protected:
	public:
		inline Event(const Reference<Event>& this_):
			Entity(this_)
		{
		}
		virtual ~Event() override = default;
	protected:
		virtual void Accept(const Reference<Performer>& performer_);
	};
	namespace Events
	{
		class FinishEvent:
			public Event
		{
		protected:
			const Reference<Event> event;
		public:
			inline FinishEvent(const Reference<FinishEvent>& this_, const Reference<Event>& event_);
			virtual ~FinishEvent() override = default;
		protected:
			virtual void Accept(const Reference<Performer>& performer_) override;
		public:
			inline Reference<Event> GetEvent() const
			{
				return event;
			}
		};
		class Service:
			public Event
		{
		public:
			inline Service(const Reference<Service>& this_):
				Event(this_)
			{
			}
			virtual ~Service() override = default;
		protected:
			virtual void Accept(const Reference<Performer>& performer_) override;
		};
		namespace Services
		{
			class Test:
				public Service,
				public Game::Dependent
			{
			protected:
				const Reference<Guest> guest;
				const Reference<Personnel> personnel;
			public:
				Float32 progress = 0.0f;
			public:
				inline Test(const Reference<Test>& this_, const Reference<Game>& game_, const Reference<Guest> guest_, const Reference<Personnel>& personnel_);
				virtual ~Test() override = default;
			protected:
				virtual void Accept(const Reference<Performer>& performer_) override;
			public:
				virtual void Update() override;
			public:
				inline Reference<Guest> GetGuest() const
				{
					return guest;
				}
				inline Reference<Personnel> GetPersonnel() const
				{
					return personnel;
				}
			};
		}
	}

	class Building:
		public Entity,
		public Game::Dependent,
		public Helper::Transformation::HMat3
	{
	public:
		DEPENDENT(Building, building);
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	public:
		inline Building(const Reference<Building>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
			Entity(this_),
			Game::Dependent(game_),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
			rooms(),
			graphicsObject(new Graphics::OpenGL::Object(game_->graphicsEngine))
		{
			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(20.0f, 1.0f, 10.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, -0.5f, 0.0f));

				game_->graphicsScene_Main->Add(graphicsObject);
			}
		}
		virtual ~Building() = default;
	public:
		FABRIC(Building, Room, rooms, room);
	};
	class Room:
		public Selection,
		public Game::Dependent,
		public Building::Dependent,
		public Helper::Transformation::HMat3
	{
	public:
		DEPENDENT(Room, room);
	protected:
		const Link<Building> building;
	public:
		inline Room(const Reference<Room>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
			Selection(Cast<Selection>(this_)),
			Game::Dependent(building_->GetGame()),
			Building::Dependent(building_),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
			building(building_)
		{
			auto ownedGame = GetGame();

			ownedGame->selector_Main->Add(GetThis<Selection>());
		}
		virtual ~Room() override = default;
	};
	class Section:
		public Room
	{
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	public:
		inline Section(const Reference<Section>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
			Room(Cast<Room>(this_), building_, pos_, ang_),
			graphicsObject(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
		{
			auto ownedGame = GetGame();

			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(4.0f, 1.0f, 8.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 0.5f, 0.0f));

				ownedGame->graphicsScene_Main->Add(graphicsObject);
			}
		}
		virtual ~Section() = default;
	public:
		virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
		{
			if(Cast::Ray::BoxProcess({position_, direction_}, {graphicsObject->GetPosition(), graphicsObject->GetAngle(), Vec3(4.0f, 1.0f, 8.0f)}))
			{
				return Cast::Ray::GetLast().distance;
			}

			return Selection::GetDistance(position_, direction_);
		}
	};

	class Inmate // : public Shared<Inmate> // cause shit on creating shared_ptr through Make<T>()
	{
	protected:
		Reference<Bunk> bunk = nullptr;
	public:
		inline Inmate() = default;
		virtual ~Inmate() = default;
	protected:
		friend Rooms::Bedroom;
		inline void SetBunk(const Reference<Bunk>& bunk_)
		{
			bunk = bunk_;
		}
	public:
		inline Reference<Bunk> GetBunk() const
		{
			return bunk;
		}
	};
	class Bunk:
		public Selection,
		public Game::Dependent,
		public Room::Dependent,
		public Helper::Transformation::HMat3
	{
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	protected:
		Reference<Inmate> inmate = nullptr;
	public:
		inline Bunk(const Reference<Bunk>& this_, const Reference<Room>& room_, const Vec3& pos_, const Vec3& ang_):
			Selection(Cast<Selection>(this_)),
			Game::Dependent(room_->GetGame()),
			Room::Dependent(room_),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), room_.get()),
			graphicsObject(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
		{
			auto ownedGame = GetGame();

			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(2.0f, 0.5f, 6.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 0.25f, 0.0f));

				ownedGame->graphicsScene_Main->Add(graphicsObject);
			}
		}
		virtual ~Bunk() = default;
	protected:
		friend Rooms::Bedroom;
		inline void SetInmate(const Reference<Inmate>& inmate_)
		{
			inmate = inmate_;
		}
	public:
		inline Reference<Inmate> GetInmate() const
		{
			return inmate;
		}
	};

	class Worker
	{
	protected:
		Reference<Workplace> workplace = nullptr;
	public:
		inline Worker() = default;
		virtual ~Worker() = default;
	protected:
		friend Rooms::Workroom;
		inline void SetWorkplace(const Reference<Workplace>& workplace_)
		{
			workplace = workplace_;
		}
	public:
		inline Reference<Workplace> GetWorkplace() const
		{
			return workplace;
		}
	};
	class Workplace:
		public Selection,
		public Game::Dependent,
		public Room::Dependent,
		public Helper::Transformation::HMat3
	{
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	protected:
		Reference<Worker> worker = nullptr;
	public:
		inline Workplace(const Reference<Workplace>& this_, const Reference<Room>& room_, const Vec3& pos_, const Vec3& ang_):
			Selection(Cast<Selection>(this_)),
			Game::Dependent(room_->GetGame()),
			Room::Dependent(room_),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), room_.get()),
			graphicsObject(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
		{
			auto ownedGame = GetGame();

			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(0.2f, 3.0f, 0.2f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 1.5f, 0.0f));

				ownedGame->graphicsScene_Main->Add(graphicsObject);
			}
		}
		virtual ~Workplace() = default;
	protected:
		friend Rooms::Workroom;
		inline void SetWorker(const Reference<Worker>& worker_)
		{
			worker = worker_;
		}
	public:
		inline Reference<Worker> GetWorker() const
		{
			return worker;
		}
	};

	namespace Rooms
	{
		class Bedroom:
			public Room
		{
		public:
			using Bunks = Vector<Reference<Bunk>>;
		public:
			Bunks bunks;
		public:
			inline Bedroom(const Reference<Bedroom>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
				Room(this_, building_, pos_, ang_),
				bunks()
			{
			}
			virtual ~Bedroom() override = default;
		public:
			inline void AttachInmate(const Reference<Inmate>& inmate_)
			{
				for(auto &bunk : bunks)
				{
					if(inmate_->GetBunk() && (inmate_->GetBunk()->GetRoom() == bunk->GetRoom()))
					{
						return;
					}

					if(bunk && !bunk->GetInmate())
					{
						if(auto inmateBunk = inmate_->GetBunk())
						{
							inmateBunk->SetInmate(nullptr);
						}

						inmate_->SetBunk(bunk);

						if(auto inmateBunk = inmate_->GetBunk())
						{
							inmateBunk->SetInmate(inmate_);
						}

						return;
					}
				}

				// throw Exception("No free bunks");
			}
			// TODO: inline void DetachInmate(const Reference<Inmate>& inmate_)
		};
		namespace Bedrooms
		{
			class Miserable:
				public Bedroom
			{
			public:
				Reference<Graphics::OpenGL::Object> graphicsObject;
			public:
				inline Miserable(const Reference<Miserable>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_);
				virtual ~Miserable() override = default;
			public:
				virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
				{
					if(Cast::Ray::BoxProcess({position_, direction_}, {graphicsObject->GetPosition(), graphicsObject->GetAngle(), Vec3(4.0f, 4.0f, 8.0f)}))
					{
						return Cast::Ray::GetLast().distance;
					}

					return Selection::GetDistance(position_, direction_);
				}
			};
		}

		class Workroom:
			public Room
		{
		public:
			using Workplaces = Vector<Reference<Workplace>>;
		protected:
			Workplaces workplaces;
		public:
			inline Workroom(const Reference<Workroom>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
				Room(this_, building_, pos_, ang_),
				workplaces()
			{
			}
			virtual ~Workroom() override = default;
		public:
			inline void AttachWorker(const Reference<Worker>& worker_)
			{
				for(auto &workplace : workplaces)
				{
					if(worker_->GetWorkplace() && (worker_->GetWorkplace()->GetRoom() == workplace->GetRoom()))
					{
						return;
					}

					if(workplace && !workplace->GetWorker())
					{
						if(auto workerWorkplace = worker_->GetWorkplace())
						{
							workerWorkplace->SetWorker(nullptr);
						}

						worker_->SetWorkplace(workplace);

						if(auto workerWorkplace = worker_->GetWorkplace())
						{
							workerWorkplace->SetWorker(worker_);
						}

						return;
					}
				}

				// throw Exception("No free workplaces");
			}
			// TODO: inline void DetachInmate(const Reference<Inmate>& inmate_)
		};

		class Hole:
			public Workroom
		{
		public:
			Reference<Graphics::OpenGL::Object> graphicsObject;
		public:
			inline Hole(const Reference<Hole>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_);
			virtual ~Hole() override = default;
		public:
			virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
			{
				if(Cast::Ray::BoxProcess({position_, direction_}, {graphicsObject->GetPosition(), graphicsObject->GetAngle(), Vec3(4.0f, 4.0f, 8.0f)}))
				{
					return Cast::Ray::GetLast().distance;
				}

				return Selection::GetDistance(position_, direction_);
			}
		};
	}

	class Character:
		public Selection,
		public Game::Dependent,
		public Helper::Transformation::HMat3,
		public Event::Performer
	{
	public:
		using Age = Size;
		enum class Gender
		{
			Female,
			Male,
		};
	public:
		const Age age;
		const Gender gender;
	public:
		Reference<Event> event = nullptr;
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	public:
		inline Character(const Reference<Character>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_, const Age& age_, const Gender& gender_):
			Selection(Cast<Selection>(this_)),
			Game::Dependent(game_),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
			Performer(Cast<Performer>(this_)),
			age(age_),
			gender(gender_),
			graphicsObject(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
		{
			auto ownedGame = GetGame();

			ownedGame->selector_Main->Add( GetThis<Selection>() );

			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(1.0f, 2.0f, 1.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 1.0f, 0.0f));

				ownedGame->graphicsScene_Main->Add(graphicsObject);
			}
		}
		virtual ~Character() = default;
	public:
		virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
		{
			if(Cast::Ray::BoxProcess({position_, direction_}, {graphicsObject->GetPosition(), graphicsObject->GetAngle(), Vec3(1.0f, 2.0f, 1.0f)}))
			{
				return Cast::Ray::GetLast().distance;
			}

			return Selection::GetDistance(position_, direction_);
		}
	protected:
		virtual void Perform(const Reference<Events::FinishEvent>& event_) override;
	public:
		inline Reference<Event> GetEvent() const
		{
			return event;
		}
		/*inline void SetEvent(const Reference<Event>& event_)
		{
			event = event_;
		}*/
	};
	class Personnel:
		public Character,
		public Inmate,
		public Worker
	{
	public:
		using Stamina = Float32; // [0,∞]
		using Mood = Float32; // [0,1]
		using Passion = Float32; // [-1,+1]
		using Hygiene = Float32; // [0,1]
	protected: // hidden params
		Stamina maxStamina = 100.0f, stamina = maxStamina;
		bool onWorkCooldown = false;
	public: // visible params
		Mood mood = 0.5f;
		Passion passion = 0.0f;
		Hygiene hygiene = 0.5f;
	protected:
		// Reference<Graphics::OpenGL::Object> graphicsObjectStateIndicator; // TODO
		const Reference<Graphics::OpenGL::Object> graphicsObjectPassionIndicator;
		// Reference<Graphics::OpenGL::Object> graphicsObjectHygieneIndicator; // TODO
	public:
		inline Personnel(const Reference<Personnel>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_, const Age& age_, const Gender& gender_):
			Character(Cast<Character>(this_), game_, pos_, ang_, age_, gender_),
			graphicsObjectPassionIndicator(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
		{
			auto ownedGame = GetGame();

			graphicsObjectPassionIndicator;
			{
				auto geometry = Geometry::CreateSphere(0.5f, Vec2(1.0f), UVec2(32));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

				graphicsObjectPassionIndicator->SetModel(model);
				graphicsObjectPassionIndicator->SetParent(this);
				graphicsObjectPassionIndicator->SetLocalPosition(Vec3(0.0f, 4.0f, 0.0f));

				ownedGame->graphicsScene_Main->Add(graphicsObjectPassionIndicator);
			}
		}
		virtual ~Personnel() override = default;
	protected:
		inline void ProcessWorking()
		{
			auto rest = [&]()
			{
				if(auto ownedBunk = GetBunk())
				{
					SetLocalPosition(ownedBunk->GetPosition());
					stamina = Clamp(stamina + 0.1f, 0.0f, maxStamina);
				}
				else
				{
					stamina = Clamp(stamina + 0.02f, 0.0f, maxStamina);
				}

				if(onWorkCooldown)
				{
					if(stamina >= maxStamina)
					{
						onWorkCooldown = false;
					}
				}
			};
			auto work = [&]()
			{
				if(auto ownedWorkplace = GetWorkplace())
				{
					if(onWorkCooldown)
					{
						rest();
					}
					else
					{
						SetLocalPosition(ownedWorkplace->GetPosition());
						stamina = Clamp(stamina - 0.2f, 0.0f, maxStamina);

						if(stamina <= 0.0f)
						{
							onWorkCooldown = true;
						}
					}
				}
				else
				{
					rest();
				}
			};

			if(stamina > 0.0f)
			{
				work();
			}
			else
			{
				rest();
			}
		}
	public:
		virtual void Update() override;
	protected:
		virtual void Perform(const Reference<Events::Services::Test>& event_) override;
	};
	class Guest:
		public Character
	{
	public:
		inline Guest(const Reference<Guest>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_, const Age& age_, const Gender& gender_):
			Character(Cast<Character>(this_), game_, pos_, ang_, age_, gender_)
		{
		}
		virtual ~Guest() override = default;
	protected:
		virtual void Perform(const Reference<Events::FinishEvent>& event_) override;
		virtual void Perform(const Reference<Events::Services::Test>& event_) override;
	};
}


#pragma region Performer

inline void Brothel::Event::Performer::Perform(const Reference<Event>& event_)
{
	event_->Accept(GetThis<Performer>());
}

#pragma endregion

#pragma region Events

#pragma region Services

inline Brothel::Events::FinishEvent::FinishEvent(const Reference<FinishEvent>& this_, const Reference<Event>& event_):
	Event(this_),
	event(event_)
{
}

#pragma endregion

#pragma region Services

#pragma region Test

inline Brothel::Events::Services::Test::Test(const Reference<Test>& this_, const Reference<Game>& game_, const Reference<Guest> guest_, const Reference<Personnel>& personnel_):
	Service(this_),
	Dependent(game_),
	guest(guest_),
	personnel(personnel_)
{
}

#pragma endregion

#pragma endregion

#pragma endregion


#pragma region Game

inline Brothel::Game::Game(const Reference<Game>& this_, const Reference<WinAPI::DeviceContext>& deviceContext_, const Reference<WinAPI::Window>& window_):
	Entity(Cast<Entity>(this_)),
	window(window_),
	graphicsEngine(new Graphics::OpenGL::Engine(deviceContext_)),
	graphicsScene_Main(new Graphics::OpenGL::Scene(graphicsEngine, window_->GetSize())),
	graphicsLight_Sunlight(new Graphics::OpenGL::Lights::Direction()),
	graphicsMaterial_Flat(new Graphics::OpenGL::Material(graphicsEngine)),
	graphicsCamera_Main(new Graphics::Camera()),
	cameraController(new CameraController(this, graphicsCamera_Main)),
	state(new GameStates::None(this_)),
	selector_Main(new Selectable::Selector())
{
	graphicsLight_Sunlight;
	{
		graphicsLight_Sunlight->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		graphicsLight_Sunlight->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLight_Sunlight->SetAmbient(0.5f);

		graphicsScene_Main->Add(graphicsLight_Sunlight);
	}
	
	graphicsMaterial_Flat;
	{
		graphicsMaterial_Flat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		graphicsMaterial_Flat->SetValue("materialAlbedo", Vec3(1.0f));
		graphicsMaterial_Flat->SetValue("materialRoughness", 0.5f);
		graphicsMaterial_Flat->SetValue("materialMetalness", 0.0f);
	}

	graphicsCamera_Main;
	{
		graphicsCamera_Main->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera_Main->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera_Main->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window_->GetAspect(), 0.1f, 10000.0f));
	}
}

#pragma endregion

#pragma region CameraController

inline void Brothel::CameraController::PerformMoveMode()
{
	auto mouseMove = Input::Mouse::GetPositionDelta();
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);

	if(mouseMiddle)
	{
		auto rMat = RotateZXY3(angle);
		position += rMat * Vec3(-mouseMove*Vec2(4.0f, 3.0f)*0.01f, 0.0f);
	}
	else
	{
		mode = Mode::None;
	}
}
inline void Brothel::CameraController::PerformRotateMode()
{
	auto mouseMove = Input::Mouse::GetPositionDelta();
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);

	if(mouseRight)
	{
		angle += Vec3(
			-mouseMove.y * 0.1f,
			mouseMove.x * 0.1f,
			0.0f);
	}
	else
	{
		mode = Mode::None;
	}
}
inline void Brothel::CameraController::PerformPullMode()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);

	if(mouseLeft)
	{
		auto m = Vec2(game->window->ToLocalPosition(IVec2(Input::Mouse::GetPosition()))); // game->graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
		auto p = (m / Vec2(game->window->GetSize())) * 2.0f - 1.0f;

		auto t = VecXYZ(graphicsCamera->GetPIMat() * Vec4(p, 1.0f, 1.0f));
		auto p1 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f)); // game->graphicsScene_Main->DrawCircle(p1, 1.0f, Vec4(1, 0, 0, 1));
		auto p2 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f)); // game->graphicsScene_Main->DrawCircle(p2, 1.0f, Vec4(1, 0, 0, 1));
		auto d = normalize(p2 - p1);

		if(Cast::Ray::PlaneProcess({p1, d}, {Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0f)})) // TODO: replace plane data with floor level
		{
			auto collisionPosition = Cast::Ray::GetLast().position - position; // game->graphicsScene_Main->DrawCircle(collisionPosition, 1.0f, Vec4(1, 0, 0, 1));
			if(isLastPullPositionObtained)
			{
				auto difference = collisionPosition - lastPullPosition;
				position -= difference;
			}
			else
			{
				// if(glm::length(collisionPosition) > 20.0f)
				// {
				// 	isLastPullPositionObtained = false;
				// 	mode = Mode::None;
				// }
				// else
				{
					isLastPullPositionObtained = true;
				}
			}
			lastPullPosition = collisionPosition;
		}
		else
		{
			isLastPullPositionObtained = false;
			mode = Mode::None;
		}
	}
	else
	{
		isLastPullPositionObtained = false;
		mode = Mode::None;
	}
}
inline void Brothel::CameraController::PerformPick()
{

}
inline void Brothel::CameraController::Update()
{
	// smooth camera
	smoothedPosition = position; // Mix(smoothedPosition, position, 1.0f / 4.0f);
	smoothedAngle = angle;
	smoothedDistance = Mix(smoothedDistance, distance, 1.0f / 8.0f);

	auto ang = smoothedAngle;
	auto pos = smoothedPosition + RotateZXY3(ang) * Vec3(0.0f, 0.0f, -smoothedDistance);

	graphicsCamera->SetPosition(pos);
	graphicsCamera->SetAngle(ang);
}

#pragma endregion

#pragma region Rooms

#pragma region Bedrooms

#pragma region Miserable

inline Brothel::Rooms::Bedrooms::Miserable::Miserable(const Reference<Miserable>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
	Bedroom(Cast<Bedroom>(this_), building_, pos_, ang_),
	graphicsObject(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
{
	auto ownedGame = GetGame();

	auto t1 = Make<Bunk>(this_, Vec3(-1.0f, 0.0f, -1.0f), Vec3(0.0f));
	auto t2 = Make<Bunk>(this_, Vec3(-1.0f, 2.0f, -1.0f), Vec3(0.0f));
	bunks = {t1, t2};

	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(4.0f, 4.0f, 8.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

		graphicsObject->SetModel(model);
		graphicsObject->SetParent(this);
		graphicsObject->SetLocalPosition(Vec3(0.0f, 2.0f, 0.0f));
		graphicsObject->SetLocalScale(Vec3(-1.0f));

		ownedGame->graphicsScene_Main->Add(graphicsObject);
	}
}

#pragma endregion

#pragma endregion

#pragma region Hole

inline Brothel::Rooms::Hole::Hole(const Reference<Hole>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
	Workroom(Cast<Workroom>(this_), building_, pos_, ang_),
	graphicsObject(new Graphics::OpenGL::Object(GetGame()->graphicsEngine))
{
	auto ownedGame = GetGame();

	auto t1 = Make<Workplace>(this_, Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f));
	auto t2 = Make<Workplace>(this_, Vec3(0.0f, 0.0f, -0.0f), Vec3(0.0f));
	auto t3 = Make<Workplace>(this_, Vec3(0.0f, 0.0f, +2.0f), Vec3(0.0f));
	workplaces = {t1, t2, t3};

	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(4.0f, 4.0f, 8.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(ownedGame->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(ownedGame->graphicsEngine, shape, ownedGame->graphicsMaterial_Flat);

		graphicsObject->SetModel(model);
		graphicsObject->SetParent(this);
		graphicsObject->SetLocalPosition(Vec3(0.0f, 2.0f, 0.0f));
		graphicsObject->SetLocalScale(Vec3(-1.0f));

		ownedGame->graphicsScene_Main->Add(graphicsObject);
	}
}

#pragma endregion

#pragma endregion

#pragma region 

template<class T>
void Brothel::GameStates::BuildSectionSelection<T>::Update()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);

	/*for(auto &i : game->selector_Main->selections)
	{
		if(auto casted = UpCast<Section>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = UpCast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = UpCast<Character>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}
	}*/

	auto m = Vec2(game->window->ToLocalPosition(IVec2(Input::Mouse::GetPosition()))); // game->graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
	auto p = (m / Vec2(game->window->GetSize())) * 2.0f - 1.0f;

	auto picked = game->selector_Main->Pick(game->graphicsCamera_Main, p, [](const Reference<Selectable::Selection>& selection_){
		auto casted = UpCast<Section>(selection_);
		return casted != nullptr;
	});
	if(picked)
	{
		if(auto casted = std::dynamic_pointer_cast<Section>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Character>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	if(!mouseMiddle && !mouseRight)
	{
		if(mouseLeft)
		{
			if(picked)
			{
				auto section = UpCast<Section>(picked); // must be Section
				// TODO
				Make<T>(section->GetBuilding(), section->GetPosition(), section->GetAngle());
				// section;
			}

			game->Change(MakeReference<GameStates::None>(game));
			return;
		}
	}
	else
	{
		game->Change(MakeReference<GameStates::None>(game));
		return;
	}
}

#pragma endregion


#pragma region
#pragma endregion


