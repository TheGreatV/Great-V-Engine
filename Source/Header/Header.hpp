#pragma region Include
#pragma once

#define NOMINMAX 1

#include <cstdint>
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#pragma endregion


#pragma region Macro
#define GVE_DEBUG			_DEBUG


#define GVE_PLATFORM_WIN	1
#define GVE_PLATFORM_LINUX	2
#define GVE_PLATFORM_MAC	3

#define GVE_PLATFORM		GVE_PLATFORM_WIN
#pragma endregion


namespace GreatVEngine
{
	using Size = std::size_t;

	template<class T> using Reference = std::shared_ptr<T>;
	template<class T> using Link = std::weak_ptr<T>;

	template<class T, size_t S> using Array = std::array<T, S>;
	template<class T> using Vector = std::vector<T>;
	template<class T> using List = std::list<T>;
	template<class T, class S> using Dictionary = std::map<T, S>;
	template<class T> using Set = std::set<T>;
	template<class T> using Initializer = std::initializer_list<T>;

	using String = std::string;

	using Filename = String;

	template<class T>
	inline Reference<T> MakeReference(T* t)
	{
		return Reference<T>(t);
	}
	template<class T>
	inline Link<T> MakeLink(T* t)
	{
		return Link<T>(t);
	}
	template<class T>
	inline Reference<T> Share(Link<T> t)
	{
		return std::make_shared(t);
	}
}


#pragma region
#pragma endregion


