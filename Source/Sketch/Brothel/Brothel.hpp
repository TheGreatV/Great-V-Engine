#pragma region Include

#pragma once


#include <Header/Header.hpp>
#include <Header/ErrorHandling.hpp>
#include <Header/Function.hpp>

#include <Logic/Helpers.hpp>

#include <Input/Input.hpp>

#include <Geometry/Collision.hpp>

#include <Infrastructure/OpenGL.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;

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
		}
		template <class Filter_> Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_, Filter_ filter_) const
		{
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
		}
		template <class Filter_, class Comparer_> Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_, Filter_ filter_, Comparer_ comparer_) const
		{
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
		}
	};
}

namespace Brothel
{
	class Exception;

	class Selection;

	class Game;

	class GameState;
	namespace GameStates
	{
		class None;
	}

	class CameraController;

	class Entity;

	class Building;
	class Room;
	class Section; // stub for room

	class Inmate;
	class Bunk; // Bed for personel

	namespace Rooms
	{
		class Bedroom;
		class Bathroom;
	}

	class Character;
	class Personnel;
	class Guest;


	template<class T, class...A> Reference<T> Make(A...a)
	{
		auto memory = static_cast<T*>(malloc(sizeof(T)));
		auto instance = WrapReference(memory);
		new(memory)T(instance, a...);
		return instance;
	}


	class Exception:
		public GVE::Exception
	{
	public:
		inline Exception() = default;
		inline Exception(const Text& text_): GVE::Exception(text_)
		{
		}
	};

	class Selection:
		public Selectable::Selection
	{
	public:
		// virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override;
	};

	class Game
	{
	public:
		class Dependent
		{
		protected:
			const Reference<Game> game;
		public:
			inline Dependent(const Reference<Game>& game_):
				game(game_)
			{
			}
		public:
			inline Reference<Game> GetGame() const
			{
				return game;
			}
		};
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
	public:
		Vector<Reference<Room>> rooms;
		Vector<Reference<Character>> characters;
	public:
		inline Game(const Reference<Game>& this_, const Reference<GreatVEngine::WinAPI::DeviceContext>& deviceContext_, const Reference<WinAPI::Window>& window_);
	public:
		inline void Change(const Reference<GameState>& state_)
		{
			state = state_;
		}
		inline void Loop();
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
		virtual void Loop()
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
			virtual void Loop() override;
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
			virtual void Loop() override;
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
			virtual void Loop() override;
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
			virtual void Loop() override;
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
			virtual void Loop() override;
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
			virtual void Loop() override;
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
		inline void Loop();
	};

	class Entity:
		public Game::Dependent,
		public Selection
	{
	public:
		inline Entity(const Reference<Entity>& this_, const Reference<Game>& game_):
			Dependent(game_)
		{
			game_->selector_Main->Add(this_);
		}
		virtual ~Entity() = default;
	};

	class Building:
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		Vector<Reference<Room>> rooms;
		Reference<Graphics::OpenGL::Object> graphicsObject;
	public:
		inline Building(const Reference<Building>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
			Entity(Cast<Entity>(this_), game_),
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
	};
	class Room:
		public Entity,
		public Helper::Transformation::HMat3
	{
	// public:
	// 	const Link<Room>
	// 		left	= Reference<Room>(nullptr),
	// 		right	= Reference<Room>(nullptr),
	// 		back	= Reference<Room>(nullptr);
	protected:
		const Link<Building> building;
	public:
		inline Room(const Reference<Room>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
			Entity(Cast<Entity>(this_), building_->GetGame()),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
			building(building_)
		{
			game->rooms.push_back(this_);
		}
		virtual ~Room() override;
	public:
		inline Reference<Building> GetBuilding() const
		{
			return Reference<Building>(building);
		}
	};
	class Section:
		public Room
	{
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	public:
		inline Section(const Reference<Section>& this_, const Reference<Building>& building_, const Vec3& pos_, const Vec3& ang_):
			Room(Cast<Room>(this_), building_, pos_, ang_),
			graphicsObject(new Graphics::OpenGL::Object(game->graphicsEngine))
		{
			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(4.0f, 1.0f, 8.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(game->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(game->graphicsEngine, shape, game->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 0.5f, 0.0f));

				game->graphicsScene_Main->Add(graphicsObject);
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
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
	protected:
		const Link<Room> room;
		Reference<Inmate> inmate = nullptr;
	public:
		inline Bunk(const Reference<Bunk>& this_, const Reference<Room>& room_, const Vec3& pos_, const Vec3& ang_):
			Entity(Cast<Entity>(this_), room_->GetGame()),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), room_.get()),
			room(room_),
			graphicsObject(new Graphics::OpenGL::Object(game->graphicsEngine))
		{
			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(2.0f, 0.5f, 6.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(game->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(game->graphicsEngine, shape, game->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 0.25f, 0.0f));

				game->graphicsScene_Main->Add(graphicsObject);
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
		inline Reference<Room> GetRoom() const
		{
			return Reference<Room>(room);
		}
		inline Reference<Inmate> GetInmate() const
		{
			return inmate;
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

				throw Exception("No free bunks");
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
		/*class Bathroom:
			public Room
		{
		};*/
		/*class Shower:
			public Room
		{
		};*/
	}

	class Character:
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		Reference<Graphics::OpenGL::Object> graphicsObject;
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
		inline Character(const Reference<Character>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_, const Age& age_, const Gender& gender_):
			Entity(Cast<Entity>(this_), game_),
			HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
			age(age_),
			gender(gender_),
			graphicsObject(new Graphics::OpenGL::Object(game->graphicsEngine))
		{
			graphicsObject;
			{
				auto geometry = Geometry::CreateBox(Vec3(1.0f, 2.0f, 1.0f), Vec3(1.0f), UVec3(1));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(game->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(game->graphicsEngine, shape, game->graphicsMaterial_Flat);

				graphicsObject->SetModel(model);
				graphicsObject->SetParent(this);
				graphicsObject->SetLocalPosition(Vec3(0.0f, 1.0f, 0.0f));

				game->graphicsScene_Main->Add(graphicsObject);
			}

			game->characters.push_back(this_);
		}
		virtual ~Character() = default;
	public:
		virtual void Loop()
		{
		}
	public:
		virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
		{
			if(Cast::Ray::BoxProcess({position_, direction_}, {graphicsObject->GetPosition(), graphicsObject->GetAngle(), Vec3(1.0f, 2.0f, 1.0f)}))
			{
				return Cast::Ray::GetLast().distance;
			}

			return Selection::GetDistance(position_, direction_);
		}
	};
	class Personnel:
		public Character,
		public Inmate
	{
	public:
		using State = Float32; // [0,1]
		using Passion = Float32; // [-1,+1]
		using Hygiene = Float32; // [0,1]
	public:
		State state = 0.5f;
		Passion passion = 0.0f;
		Hygiene hygiene = 0.5f;
	protected:
		// Reference<Graphics::OpenGL::Object> graphicsObjectStateIndicator; // TODO
		const Reference<Graphics::OpenGL::Object> graphicsObjectPassionIndicator;
		// Reference<Graphics::OpenGL::Object> graphicsObjectHygieneIndicator; // TODO
	public:
		inline Personnel(const Reference<Personnel>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_, const Age& age_, const Gender& gender_):
			Character(Cast<Character>(this_), game_, pos_, ang_, age_, gender_),
			graphicsObjectPassionIndicator(new Graphics::OpenGL::Object(game->graphicsEngine))
		{
			graphicsObjectPassionIndicator;
			{
				auto geometry = Geometry::CreateSphere(0.5f, Vec2(1.0f), UVec2(32));
				auto shape = MakeReference<Graphics::OpenGL::Shape>(game->graphicsEngine, geometry);
				auto model = MakeReference<Graphics::OpenGL::Model>(game->graphicsEngine, shape, game->graphicsMaterial_Flat);

				graphicsObjectPassionIndicator->SetModel(model);
				graphicsObjectPassionIndicator->SetParent(this);
				graphicsObjectPassionIndicator->SetLocalPosition(Vec3(0.0f, 4.0f, 0.0f));

				game->graphicsScene_Main->Add(graphicsObjectPassionIndicator);
			}
		}
		virtual ~Personnel() override = default;
	public:
		virtual void Loop() override
		{
			if(auto ownedBunk = GetBunk())
			{
				SetLocalPosition(ownedBunk->GetPosition());
			}

			graphicsObjectPassionIndicator->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
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
	};
}


#pragma region Game

inline Brothel::Game::Game(const Reference<Game>& this_, const Reference<WinAPI::DeviceContext>& deviceContext_, const Reference<WinAPI::Window>& window_):
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
		graphicsMaterial_Flat->SetValue("materialColor", Vec3(1.0f));
		graphicsMaterial_Flat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		graphicsMaterial_Flat->SetValue("materialGloss", 0.1f);
		graphicsMaterial_Flat->SetValue("materialRoughness", 0.8f);
	}

	graphicsCamera_Main;
	{
		graphicsCamera_Main->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera_Main->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera_Main->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window_->GetAspect(), 0.1f, 10000.0f));
	}
}
inline void Brothel::Game::Loop()
{
	cameraController->Loop();

	auto s = state;
	s->Loop();

	for(auto &room : rooms)
	{
		// room->Loop();
	}

	for(auto &character : characters)
	{
		character->Loop();
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
inline void Brothel::CameraController::Loop()
{
	/*auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);

	auto m = Vec2(game->window->ToLocalPosition(IVec2(Input::Mouse::GetPosition()))); // game->graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
	auto p = (m / Vec2(game->window->GetSize())) * 2.0f - 1.0f;

	switch(mode)
	{
		case Mode::None: {
			if(!mouseLeft && mouseMiddle && !mouseRight)
			{
				mode = Mode::Move;
				PerformMoveMode();
			}
			if(!mouseLeft && !mouseMiddle && mouseRight)
			{
				mode = Mode::Rotate;
				PerformRotateMode();
			}
			// if(mouseLeft && !mouseMiddle && !mouseRight)
			// {
			// 	mode = Mode::Pull;
			// 	PerformPullMode();
			// }

			for(auto &i : game->selector_Main->selections)
			{
				if(auto casted = std::dynamic_pointer_cast<Section>(i)) if(casted->graphicsObject)
				{
					casted->graphicsObject->SetColor(Vec4(1.0f));
				}

				if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
				{
					casted->graphicsObject->SetColor(Vec4(1.0f));
				}
			
				if(auto casted = std::dynamic_pointer_cast<Character>(i)) if(casted->graphicsObject)
				{
					casted->graphicsObject->SetColor(Vec4(1.0f));
				}
			}

			auto picked = game->selector_Main->Pick(graphicsCamera, p, [](const Reference<Selectable::Selection>&){
				return true;
			}, [](const Reference<Selectable::Selection>& old_, const Float32& oldDistance_, const Reference<Selectable::Selection>& new_, const Float32& newDistance_){
				return (newDistance_ != Selection::NO_SELECTION) && (
					(old_ && !UpCast<Character>(old_) && UpCast<Character>(new_)) ||
					(newDistance_ < oldDistance_));
			});
			if(picked)
			{
				if(auto casted = std::dynamic_pointer_cast<Section>(picked)) if(casted->graphicsObject)
				{
					casted->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				}

				if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(picked)) if(casted->graphicsObject)
				{
					casted->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				}

				if(auto casted = std::dynamic_pointer_cast<Character>(picked)) if(casted->graphicsObject)
				{
					casted->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
			
			if(mouseLeft && !mouseMiddle && !mouseRight)
			{
				if(picked)
				if(auto personnel = UpCast<Personnel>(picked))
				{
					pickedPersonnel = personnel;
					mode = Mode::PersonnelPick;
				}
				else
				{

				}
			}
		} break;
		case Mode::Move: {
			PerformMoveMode();
		} break;
		case Mode::Rotate: {
			PerformRotateMode();
		} break;
		case Mode::Pull: {
			PerformPullMode();
		} break;
		case Mode::PersonnelPick: {
			if(mouseLeft && !mouseMiddle && !mouseRight)
			{
			}
			else
			{
				if(pickedPersonnel)
				{
					auto picked = game->selector_Main->Pick(graphicsCamera, p, [](const Reference<Selectable::Selection>& selection_){
						auto casted = UpCast<Rooms::Bedroom>(selection_);
						return casted != nullptr;
					});

					if(picked)
					{
						auto room = UpCast<Rooms::Bedroom>(picked);
						room->AttachInmate(pickedPersonnel);
					}

					pickedPersonnel = nullptr;
				}
				mode = Mode::None;
			}
		} break;
	}*/

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
	graphicsObject(new Graphics::OpenGL::Object(game->graphicsEngine))
{
	auto t1 = Make<Bunk>(this_, Vec3(-1.0f, 0.0f, -1.0f), Vec3(0.0f));
	auto t2 = Make<Bunk>(this_, Vec3(-1.0f, 2.0f, -1.0f), Vec3(0.0f));
	bunks = {t1, t2};

	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(4.0f, 4.0f, 8.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(game->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(game->graphicsEngine, shape, game->graphicsMaterial_Flat);

		graphicsObject->SetModel(model);
		graphicsObject->SetParent(this);
		graphicsObject->SetLocalPosition(Vec3(0.0f, 2.0f, 0.0f));
		graphicsObject->SetLocalScale(Vec3(-1.0f));

		game->graphicsScene_Main->Add(graphicsObject);
	}
}

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region 

template<class T>
void Brothel::GameStates::BuildSectionSelection<T>::Loop()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);

	for(auto &i : game->selector_Main->selections)
	{
		if(auto casted = std::dynamic_pointer_cast<Section>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Character>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}
	}

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


