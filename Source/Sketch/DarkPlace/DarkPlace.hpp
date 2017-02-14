
#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>
#include <Input/Input.hpp>
#include <Geometry/Collision.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;


namespace DarkPlace
{
	template<class T> class This;
	
	class Game;
	class Entity;

	class Player;
	class Test;


	template<class T> class This
	{
	protected:
		Link<T> self;
	public:
		inline This(const Reference<T>& this_);
	public:
		inline Reference<T> GetThis() const;
	};

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

		const Reference<Graphics::OpenGL::Material> graphicsMaterial_Brick1;
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
		const Reference<Graphics::OpenGL::Object> graphicsObject;
		const Reference<Physics::BulletPhysics::Bodies::Rigid> physicsBody;
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
	public:
		inline Test2(const Reference<Test2>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_);
	};


	template<class T, class...A> Reference<T> Make(A...a);
}


#pragma region DarkPlace

#pragma region This

template<class T>
inline DarkPlace::This<T>::This(const Reference<T>& this_):
	self(this_)
{
}
template<class T>
inline Reference<T> DarkPlace::This<T>::GetThis() const
{
	return Reference<T>(self);
}

#pragma endregion

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

	graphicsMaterial_Brick1(new Graphics::OpenGL::Material(graphicsEngine)),

	physicsWorld_Main(new Physics::BulletPhysics::World(physicsEngine))
{
	physicsWorld_Main;
	{
		physicsWorld_Main->SetGravity(Vec3(0.0f, -9.8f, 0.0f));
	}

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

	graphicsMaterial_Brick1;
	{
		graphicsMaterial_Brick1->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Simple/Basic/SeparateMaps.glsl."), "vs", "", "", "", "fs");

		// graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Color) = Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick1_D.png"));
		// graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Topology) = Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick1_T.png"));
		// graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Reflections) = Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick1_R.png"));
		
		graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Albedo)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Albedo.png"));
		graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Normals)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Normals.png"));
		graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Height)		= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Height.png"));
		graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Occlusion)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Occlusion.png"));
		graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Roughness)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Roughness.png"));
		graphicsMaterial_Brick1->Map(Graphics::Material::MapType::Metalness)	= Graphics::OpenGL::Map::Load2D(graphicsEngine, Filepath("Media/Images/Materials/Brick3/Metalness.png"));

		// graphicsMaterial_Brick1->SetValue("materialColor", Vec3(1.0f));
		// graphicsMaterial_Brick1->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		// graphicsMaterial_Brick1->SetValue("materialGloss", 0.1f);
		// graphicsMaterial_Brick1->SetValue("materialRoughness", 0.8f);
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

inline DarkPlace::Player::Player(const Reference<Player>& this_, const Reference<Game>& game_, const Vec3& pos_, const Vec3& ang_):
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
		graphicsObject->SetParent(this);

		game_->graphicsScene_Main->Add(graphicsObject);
	}

	physicsBody;
	{
		physicsBody->SetAngleFactor(Vec3(0.0f));
		// physicsBody->SetUser(this_);

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
		Physics::BulletPhysics::Shape::CreateBox(Vec3(20.0f, 1.0f, 20.0f), 0.0f),
		VecXYZ(GetMMat() * Vec4(0.0f, -0.5f, 0.0f, 1.0f)), ang_))
{
	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(20.0f, 1.0f, 20.0f), Vec3(1.0f), UVec3(1));
		auto shape = MakeReference<Graphics::OpenGL::Shape>(game_->graphicsEngine, geometry);
		auto model = MakeReference<Graphics::OpenGL::Model>(game_->graphicsEngine, shape, game_->graphicsMaterial_Brick1);

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
	physicsBody(new Physics::BulletPhysics::Bodies::Rigid(Physics::BulletPhysics::Shape::CreateBox(Vec3(5.0f), 1.0f), pos_, ang_))
{
	graphicsObject;
	{
		auto geometry = Geometry::CreateBox(Vec3(5.0f), Vec3(1.0f), UVec3(1));
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

#pragma endregion


template<class T, class...A> Reference<T> DarkPlace::Make(A...a)
{
	auto memory = static_cast<T*>(malloc(sizeof(T)));
	auto instance = WrapReference(memory);
	new(memory)T(instance, a...);
	return instance;
}

#pragma endregion


#pragma region
#pragma endregion



















