#pragma once

#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>
#include <Input/Input.hpp>
#include <Geometry/Collision.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;


namespace Path
{
	class Finder;
	class Location;
	class Area;
	class Gateway;

	class Finder
	{
	protected:
		Vector<Reference<Area>> areas;
	protected:
		inline virtual Reference<Vector<Location>> FindPath(const Reference<Area>& startArea_, const Location& startLocation_, const Reference<Area>& finishArea_, const Location& finishLocation_);
	public:
		inline void Add(const Reference<Area>& area_)
		{
			areas.push_back(area_);
		}
		inline Reference<Vector<Location>> FindPath(const Vec3& startPosition_, const Vec3& finishPosition_);
	};
	class Location:
		public Helper::Transformation::Pos3
	{
	public:
		inline Location(const Position::Value& position_):
			Position(position_)
		{
		}
	};
	class Area
	{
	public:
		Vec3 position;
		Vector<Reference<Gateway>> gateways;
		inline Area(const Vec3& position_):
			position(position_),
			gateways()
		{
		}
		virtual ~Area() = default;
	};
	class Gateway
	{
	public:
		Reference<Area> a, b;
		Location location;
		Gateway(const Reference<Area>& a_, const Reference<Area>& b_, const Location& location_):
			a(a_), b(b_),
			location(location_)
		{
		}
		virtual ~Gateway() = default;
	};

	inline Reference<Gateway> Connect(const Reference<Area>& a_, const Reference<Area>& b_, const Location& location_);

	inline Reference<Vector<Location>> Finder::FindPath(const Reference<Area>& startArea_, const Location& startLocation_, const Reference<Area>& finishArea_, const Location& finishLocation_)
	{
		struct Node
		{
			Reference<Gateway> from = nullptr;
			size_t length = SIZE_MAX;
			Node() = default;
			Node(const Reference<Gateway>& from_, const size_t& length_):
				from(from_),
				length(length_)
			{
			}
		};
		Dictionary<Reference<Area>, Node> areas;

		areas[startArea_] = Node(nullptr, 0);

		bool flag = true;
		do
		{
			flag = false;
			for(auto &it : areas)
			{
				auto &area = it.first;
				auto &node = it.second;

				for(auto &gateway : area->gateways)
				{
					auto otherArea = gateway->a == area ? gateway->b : gateway->a;
					auto length = node.length + 1;

					auto i = areas.find(otherArea);
					if(i == areas.end() || i->second.length > length)
					{
						areas[otherArea] = Node(gateway, length);
						flag = true;
					}
				}
			}
		}
		while(flag);

		auto it = areas.find(finishArea_);
		if(it != areas.end())
		{
			auto backtrace = MakeReference<Vector<Location>>();

			auto area = finishArea_;
			auto node = areas[area];
			backtrace->push_back(finishLocation_); // finish location

			while(node.from)
			{
				area = node.from->a == area ? node.from->b : node.from->a;
				backtrace->push_back(node.from->location); // intermediate location
				node = areas[area];
			}

			std::reverse(backtrace->begin(), backtrace->end());

			return backtrace;
		}
		else
		{
			return nullptr;
		}
	}
	inline Reference<Vector<Location>> Finder::FindPath(const Vec3& startPosition_, const Vec3& finishPosition_)
	{
		Reference<Area> startArea = nullptr;
		{
			Float32 minimalDistance = FLT_MAX;
			for(auto &area : areas)
			{
				auto currentDistance = glm::distance(startPosition_, area->position);
				if(currentDistance < minimalDistance)
				{
					startArea = area;
					minimalDistance = currentDistance;
				}
			}
		}
		Reference<Area> finishArea = nullptr;
		{
			Float32 minimalDistance = FLT_MAX;
			for(auto &area : areas)
			{
				auto currentDistance = glm::distance(finishPosition_, area->position);
				if(currentDistance < minimalDistance)
				{
					finishArea = area;
					minimalDistance = currentDistance;
				}
			}
		}

		if(startArea && finishArea)
		{
			auto path = FindPath(startArea, Location(startPosition_), finishArea, Location(finishPosition_));
			return path;
		}

		return nullptr;
	}


	inline Reference<Gateway> Connect(const Reference<Area>& a_, const Reference<Area>& b_, const Location& location_)
	{
		auto gateway = MakeReference<Gateway>(a_, b_, location_);
		a_->gateways.push_back(gateway);
		b_->gateways.push_back(gateway);
		return gateway;
	}
}


namespace DarkPlace
{
	class Game;
	class Entity;

	class Player;
	class Test;


	class Game:
		public This<Game>
	{
	public:
		class Dependent
		{
		protected:
			const Link<Game> game;
		public:
			inline Dependent(const Reference<Game>& game_);
		public:
			inline Link<Game> GetGame() const;
		};
	public:
		const Reference<WinAPI::Window> window;
		const Reference<Graphics::OpenGL::Engine> graphicsEngine;
		const Reference<Physics::BulletPhysics::Engine> physicsEngine;
	public:
		const Reference<Graphics::OpenGL::Scene> graphicsScene_Main;
		const Reference<Graphics::OpenGL::Lights::Direction> graphicsLight_Sunlight;
		const Reference<Graphics::OpenGL::Material> graphicsMaterial_Flat;
		const Reference<Graphics::Camera> graphicsCamera_Main;

		const Reference<Graphics::OpenGL::Material> graphicsMaterial_Brick3;
		const Reference<Graphics::OpenGL::Material> graphicsMaterial_Brick4;

		const Reference<Graphics::OpenGL::Environments::Globalmap> graphicsEnvironment_Main;

		Reference<Graphics::OpenGL::Object> test;
	public:
		const Reference<Physics::BulletPhysics::World> physicsWorld_Main;
	protected:
		Vector<Reference<Entity>> entities;
	public:
		inline Game(const Reference<Game>& this_, const Reference<WinAPI::DeviceContext>& deviceContext_, const Reference<WinAPI::Window>& window_);
	public:
		template<class T, class...A> Link<T> Add(A...a);
		template<class T> void Remove(const Reference<T>& entity_);
	public:
		void Update();
	};
	class Entity:
		public This<Entity>,
		public Game::Dependent
	{
	public:
		inline Entity(const Reference<Entity>& this_, const Reference<Game>& game_);
	public:
		virtual void Update();
	};

	class Player:
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		class Motions
		{
		public:
			const Reference<Player> player;
			Float32 breathStage = 0.0f;
			Float32 walkingStage = 0.0f;
			bool isWalking = false, walkingDirection = false;
		public:
			inline Motions(const Reference<Player>& player_):
				player(player_)
			{
			}
		protected:
			inline void UpdateWalking();
		public:
			inline void Update();
			inline Vec3 GetPositionOffset();
			inline Vec3 GetAngleOffset();
		};
	public:
		const Reference<Graphics::OpenGL::Object> graphicsObject;
		const Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody;
	public:
		bool isSpectatorMode = true;
		bool isMoving = false;
		Float32 movingTemp = 1.0f;
		const Pointer<Motions> motions;
	public:
		inline Player(const Reference<Player>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_);
	public:
		virtual void Update() override;
	};
	class Test:
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		const Reference<Graphics::OpenGL::Object> graphicsObject;
		const Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody;
	public:
		inline Test(const Reference<Test>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_);
	};
	class Test2:
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		const Reference<Graphics::OpenGL::Object> graphicsObject;
		const Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody;
		const Reference<Graphics::OpenGL::Lights::Point> graphicsLight;
	public:
		inline Test2(const Reference<Test2>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_);
	};

	class Enemy:
		public Entity,
		public Helper::Transformation::HMat3
	{
	public:
		const Reference<Graphics::OpenGL::Object> graphicsObject;
		const Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody;
	public:
		inline Enemy(const Reference<Enemy>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_);
	public:
		virtual void Update() override;
		virtual void MoveInDirection(const Vec3& direction_);

		Vec3 dir;
		inline void SetDir(const Vec3& dir_) // TODO: remove
		{
			dir = dir_;
		}
	};

	namespace Mazes
	{
		class Tile:
			public Entity,
			public Path::Area,
			public Helper::Transformation::HMat3
		{
		public:
			const Reference<Graphics::OpenGL::Object> graphicsObject;
		public:
			inline Tile(const Reference<Tile>& this_, const Reference<Game>& game_, const Position::Value& pos_):
				Entity(this_, game_),
				Area(pos_),
				HierarchyMatrix(pos_, Vec3(0.0f), Vec3(1.0f), nullptr),
				graphicsObject(new Graphics::OpenGL::Object(game_->graphicsEngine))
			{
				graphicsObject;
				{
					auto geometry = Geometry::CreateSphere(1.0f, Vec2(1.0f), UVec2(32));
					auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
					auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat);

					graphicsObject->SetModel(model);
					graphicsObject->SetParent(this);

					game_->graphicsScene_Main->Add(graphicsObject);
				}
			}
		};

		class Simple:
			public Entity
		{
		public:
			static Link<Simple> Generate(const Reference<Game>& game_);
		public: // protected:
			const Reference<Path::Finder> pathFinder;
			Dictionary<Size, Dictionary<Size, Reference<Tile>>> tiles;
			Vector<Reference<Path::Gateway>> gateways;
		public:
			inline Simple(const Reference<Simple>& this_, const Reference<Game>& game_):
				Entity(this_, game_),
				pathFinder(MakeReference<Path::Finder>()),
				tiles()
			{
			}
		public:
			virtual void Update() override;
		};
	}
}


#pragma region DarkPlace

#pragma region Game

#pragma region Dependent

inline DarkPlace::Game::Dependent::Dependent(const Reference<Game>& game_):
	game(game_)
{
}
inline Link<DarkPlace::Game> DarkPlace::Game::Dependent::GetGame() const
{
	return game;
}

#pragma endregion


inline DarkPlace::Game::Game(const Reference<Game>& this_, const Reference<GreatVEngine::WinAPI::DeviceContext>& deviceContext_, const Reference<WinAPI::Window>& window_):
	This(this_),
	window(window_),
	graphicsEngine(new Graphics::OpenGL::Engine(deviceContext_)),
	physicsEngine(new Physics::BulletPhysics::Engine()),

	graphicsScene_Main(new Graphics::OpenGL::Scene(graphicsEngine, window_->GetSize())),
	graphicsLight_Sunlight(new Graphics::OpenGL::Lights::Direction()),
	graphicsMaterial_Flat(new Graphics::OpenGL::Material(graphicsEngine)),
	graphicsCamera_Main(new Graphics::Camera()),

	graphicsMaterial_Brick3(new Graphics::OpenGL::Material(graphicsEngine)),
	graphicsMaterial_Brick4(new Graphics::OpenGL::Material(graphicsEngine)),

	graphicsEnvironment_Main(Graphics::OpenGL::Environments::Globalmap::LoadCube(graphicsEngine, Filepath("Media/Images/Rocks.dds"))),

	// test(Graphics::OpenGL::Object::Load(graphicsEngine, Filepath("Media/Models/Demo/Scene.gvs"), graphicsMaterial_Flat)),

	physicsWorld_Main(new Physics::BulletPhysics::World(physicsEngine))
{
	physicsWorld_Main;
	{
		physicsWorld_Main->SetGravity(Vec3(0.0f, -9.8f * 10.0f, 0.0f));
	}

	graphicsLight_Sunlight;
	{
		graphicsLight_Sunlight->SetLocalAngle(Vec3(60.0f, 0.0f, 0.0f));
		graphicsLight_Sunlight->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLight_Sunlight->SetAmbient(0.5f);

		graphicsScene_Main->Add(graphicsLight_Sunlight);
		// graphicsLight_Sunlight->Hide();
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
		graphicsCamera_Main->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window_->GetAspect(), 0.1f, 10000.0f));
	}

	graphicsMaterial_Brick3;
	{
		graphicsMaterial_Brick3->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		graphicsMaterial_Brick3->Map(Graphics::Material::MapType::Albedo)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Albedo.png"));
		graphicsMaterial_Brick3->Map(Graphics::Material::MapType::Normals)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Normals.png"));
		graphicsMaterial_Brick3->Map(Graphics::Material::MapType::Height)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Height.png"));
		graphicsMaterial_Brick3->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Occlusion.png"));
		graphicsMaterial_Brick3->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Roughness.png"));
		graphicsMaterial_Brick3->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Metalness.png"));
	}
	graphicsMaterial_Brick4;
	{
		graphicsMaterial_Brick4->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		graphicsMaterial_Brick4->Map(Graphics::Material::MapType::Albedo)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick4/Albedo.png"));
		graphicsMaterial_Brick4->Map(Graphics::Material::MapType::Normals)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick4/Normals.png"));
		graphicsMaterial_Brick4->Map(Graphics::Material::MapType::Height)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick4/Height.png"));
		graphicsMaterial_Brick4->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick4/Occlusion.png"));
		graphicsMaterial_Brick4->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick4/Roughness.png"));
		graphicsMaterial_Brick4->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick4/Metalness.png"));
	}

	graphicsEnvironment_Main;
	{
		// graphicsEnvironment_Main->Hide();

		graphicsScene_Main->Add(graphicsEnvironment_Main);
	}

	test;
	{
		// test = Graphics::OpenGL::Object::Load(graphicsEngine, Filepath("Media/Models/Demo/Scene.gvs"), graphicsMaterial_Flat);

		Float32 innerSize = 40.0f;
		Float32 outerSize = 50.0f;
		Float32 height = 1.0f;

		auto geometry = MakeReference<Geometry>(8, 24);

		geometry->vertices[0].pos = Vec3(-innerSize, height, -innerSize);
		geometry->vertices[0].nor = Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[1].pos = Vec3(-outerSize, height, -outerSize);
		geometry->vertices[1].nor = Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[2].pos = Vec3(innerSize, height, -innerSize);
		geometry->vertices[2].nor = Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[3].pos = Vec3(outerSize, height, -outerSize);
		geometry->vertices[3].nor = Vec3(0.0f, 1.0f, 0.0f);
		
		geometry->vertices[4].pos = Vec3(innerSize, height, innerSize);
		geometry->vertices[4].nor = Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[5].pos = Vec3(outerSize, height, outerSize);
		geometry->vertices[5].nor = Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[6].pos = Vec3(-innerSize, height, innerSize);
		geometry->vertices[6].nor = Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[7].pos = Vec3(-outerSize, height, outerSize);
		geometry->vertices[7].nor = Vec3(0.0f, 1.0f, 0.0f);

		geometry->indices[0]	= 0;
		geometry->indices[1]	= 1;
		geometry->indices[2]	= 3;
		geometry->indices[3]	= 0;
		geometry->indices[4]	= 3;
		geometry->indices[5]	= 2;

		geometry->indices[6]	= 2;
		geometry->indices[7]	= 3;
		geometry->indices[8]	= 5;
		geometry->indices[9]	= 2;
		geometry->indices[10]	= 5;
		geometry->indices[11]	= 4;

		geometry->indices[12]	= 4;
		geometry->indices[13]	= 5;
		geometry->indices[14]	= 7;
		geometry->indices[15]	= 4;
		geometry->indices[16]	= 7;
		geometry->indices[17]	= 6;

		geometry->indices[18]	= 6;
		geometry->indices[19]	= 7;
		geometry->indices[20]	= 1;
		geometry->indices[21]	= 6;
		geometry->indices[22]	= 1;
		geometry->indices[23]	= 0;

		auto shape = MakeReference<Graphics::OpenGL::Shape>(graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(graphicsEngine, shape, graphicsMaterial_Flat);

		test = MakeReference<Graphics::OpenGL::Object>(graphicsEngine);
		test->SetModel(model);

		graphicsScene_Main->Add(test);
	}
}

template<class T, class...A>
Link<T> DarkPlace::Game::Add(A...a)
{
	auto entity = Make<T>(GetThis(), a...);

	entities.push_back(entity);

	return entity;
}
template<class T>
void DarkPlace::Game::Remove(const Reference<T>& entity_)
{
	auto it = std::find(entities.begin(), entities.end(), entity_);
	if(it != entities.end())
	{
		entities.erase(it);
	}
	else
	{
		throw Exception();
	}
}

#pragma endregion

#pragma region Entity

inline DarkPlace::Entity::Entity(const Reference<Entity>& this_, const Reference<Game>& game_):
	This(this_),
	Dependent(game_)
{
}

#pragma endregion

#pragma region Player

#pragma region Motions

inline void DarkPlace::Player::Motions::UpdateWalking()
{
	Float32 speed = 0.12f * player->movingTemp;
	Float32 max = 1.0f;

	if(isWalking)
	{
		if(walkingDirection)
		{
			walkingStage += speed;
			if(walkingStage >= max)
			{
				walkingStage = max;
				walkingDirection = !walkingDirection;
			}
		}
		else
		{
			walkingStage -= speed;
			if(walkingStage <= -max)
			{
				walkingStage = -max;
				walkingDirection = !walkingDirection;
			}
		}
	}
	else
	{
		if(walkingStage != 0.0f)
		{
			if(walkingStage > 0.0f)
			{
				walkingStage -= speed;
				if(walkingStage < 0.0f)
				{
					walkingStage = 0.0f;
				}
			}
			else
			{
				walkingStage += speed;
				if(walkingStage > 0.0f)
				{
					walkingStage = 0.0f;
				}
			}
		}
	}
}

inline void DarkPlace::Player::Motions::Update()
{
	breathStage += 5.0f;

	isWalking = player->isMoving; // KeyState(Keys::Q);
	
	UpdateWalking();
}
inline GVE::Vec3 DarkPlace::Player::Motions::GetPositionOffset()
{
	return Vec3(0.0f,
		(cos(radians(breathStage)) * 0.5f + 0.5f) * 0.05f +
		sin(radians(abs(walkingStage)*90.0f)) * 0.5f,
		0.0f);// +
		// player->GetRMat() * Vec3(walkStage*0.1f, 0.0f, 0.0f);
}
inline GVE::Vec3 DarkPlace::Player::Motions::GetAngleOffset()
{
	return Vec3(0.0f, 0.0f, walkingStage * 2.0f);
}


#pragma endregion


inline DarkPlace::Player::Player(const Reference<Player>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
	Entity(this_, game_),
	HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
	graphicsObject(new Graphics::OpenGL::Object(game_->graphicsEngine)),
	physicsBody(new Physics::BulletPhysics::Bodies::Rigid(Physics::BulletPhysics::Shape::CreateCapsule(2.0f, 4.0f, 1.0f), pos_, ang_)),
	motions(MakePointer<Motions>(this_))
{
	graphicsObject;
	{
		auto geometry = Geometry::CreateCapsule(2.0f, 4.0f, Vec2(1.0f), UVec2(32, 16));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat);

		graphicsObject->SetModel(model);
		// graphicsObject->SetParent(this);

		game_->graphicsScene_Main->Add(graphicsObject);
	}

	physicsBody;
	{
		physicsBody->SetAngleFactor(Vec3(0.0f));
		physicsBody->SetUser(graphicsObject);

		game_->physicsWorld_Main->Add(physicsBody);
	}
}

#pragma endregion

#pragma region Test

inline DarkPlace::Test::Test(const Reference<Test>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
	Entity(this_, game_),
	HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
	graphicsObject(new Graphics::OpenGL::Object(game_->graphicsEngine)),
	physicsBody(new Physics::BulletPhysics::Bodies::Rigid(
		Physics::BulletPhysics::Shape::CreateBox(Vec3(200.0f, 1.0f, 200.0f), 0.0f),
		VecXYZ(GetMMat() * Vec4(0.0f, -0.5f, 0.0f, 1.0f)), ang_))
{
	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(200.0f, 1.0f, 200.0f), Vec3(20.0f, 0.1f, 20.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Brick3);

		graphicsObject->SetModel(model);
		graphicsObject->SetParent(this);
		graphicsObject->SetLocalPosition(Vec3(0.0f, -0.5f, 0.0f));

		game_->graphicsScene_Main->Add(graphicsObject);
	}

	physicsBody;
	{
		game_->physicsWorld_Main->Add(physicsBody);
	}
}

#pragma endregion

#pragma region Test2

inline DarkPlace::Test2::Test2(const Reference<Test2>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
	Entity(this_, game_),
	HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
	graphicsObject(new Graphics::OpenGL::Object(game_->graphicsEngine)),
	physicsBody(new Physics::BulletPhysics::Bodies::Rigid(Physics::BulletPhysics::Shape::CreateBox(Vec3(5.0f), 1.0f), pos_, ang_)),
	graphicsLight(new Graphics::OpenGL::Lights::Point())
{
	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(5.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Brick4);

		graphicsObject->SetModel(model);
		graphicsObject->SetParent(this);

		game_->graphicsScene_Main->Add(graphicsObject);
	}

	physicsBody;
	{
		game_->physicsWorld_Main->Add(physicsBody);
		physicsBody->SetUser(this_);
	}

	graphicsLight;
	{
		graphicsLight->SetLocalPosition(Vec3(0.0f));
		graphicsLight->SetRangeFar(50.0f);
		// graphicsLight->Hide();

		graphicsLight->SetParent(this);

		game_->graphicsScene_Main->Add(graphicsLight);
	}
}

#pragma endregion

#pragma region Enemy

inline DarkPlace::Enemy::Enemy(const Reference<Enemy>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
	Entity(this_, game_),
	HierarchyMatrix(pos_, ang_, Vec3(1.0f), nullptr),
	graphicsObject(new Graphics::OpenGL::Object(game_->graphicsEngine)),
	physicsBody(new Physics::BulletPhysics::Bodies::Rigid(Physics::BulletPhysics::Shape::CreateCapsule(2.0f, 4.0f, 1.0f), pos_, ang_))
{
	graphicsObject;
	{
		auto geometry = Geometry::CreateCapsule(2.0f, 4.0f, Vec2(1.0f), UVec2(32, 16));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Flat);

		graphicsObject->SetModel(model);
		graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		// graphicsObject->SetParent(this);

		game_->graphicsScene_Main->Add(graphicsObject);
	}

	physicsBody;
	{
		physicsBody->SetAngleFactor(Vec3(0.0f));
		physicsBody->SetUser(graphicsObject);

		game_->physicsWorld_Main->Add(physicsBody);
	}
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion



















