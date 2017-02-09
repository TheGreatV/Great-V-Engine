
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
	template<class T> class Self;
	
	class Game;
	class Entity;


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
		Vector<Reference<Entity>> entities;
	public:
		inline Game(const Reference<Game>& this_);
	public:
		template<class T, class...A> Link<T> Add(A...a);
		template<class T> void Remove(const Reference<T>& entity_);
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


inline DarkPlace::Game::Game(const Reference<Game>& this_):
	This(this_)
{
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



















