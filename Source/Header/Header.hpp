#pragma region Include
#pragma once

#include <cstdint>
#include <string>
#include <locale>
#include <codecvt>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <functional>
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

	template<class T> using Pointer = std::unique_ptr<T>;
	template<class T> using Reference = std::shared_ptr<T>;
	template<class T> using Link = std::weak_ptr<T>;
	template<class T> using Shared = std::enable_shared_from_this<T>;

	template<class T, size_t S> using Array = std::array<T, S>;
	template<class T> using Vector = std::vector<T>;
	template<class T> using List = std::list<T>;
	template<class T, class S> using Dictionary = std::map<T, S>;
	template<class T> using Set = std::set<T>;
	template<class T> using Initializer = std::initializer_list<T>;

	using String = std::string;
	using WString = std::wstring;

	inline WString ToWString(const String& source_)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		return converter.from_bytes(source_);
	}

	using Filename = String;

	inline Filename Filepath(const Filename& filename_)
	{
		return "../../../../../" + filename_;
	}

	template<class T> inline Pointer<T> WrapPointer(T* t)
	{
		return Pointer<T>(t);
	}
	// template<class T> inline Pointer<T> WrapPointer(std::shared_ptr<T> t)
	// {
	// 	return Pointer<T>(t);
	// }
	template<class T> inline Reference<T> WrapReference(T* t)
	{
		return Reference<T>(t);
	}
	template<class T, class...A> inline Reference<T> MakeReference(A...a)
	{
		return std::make_shared<T>(a...);
	}
	template<class T> inline Reference<T> MakeReference(const Link<T>& t)
	{
		return Reference<T>(t);
	}
	// template<class T> inline Reference<T> MakeReference(std::unique_ptr<T> t)
	// {
	// 	return Reference<T>(t);
	// }
	template<class T> inline Link<T> WrapLink(T t)
	{
		return Link<T>(t);
	}
	template<class T> inline Reference<T> Share(Pointer<T> t)
	{
		return std::make_shared<T>(t);
	}
	template<class T> inline Reference<T> Share(Link<T> t)
	{
		return std::make_shared<T>(t);
	}

	template<class A, class B> inline Reference<A> Cast(Reference<B>& b_)
	{
		return std::static_pointer_cast<A>(b_);
	}
	template<class A, class B> inline Reference<A> Cast(const Reference<B>& b_)
	{
		return std::static_pointer_cast<A>(b_);
	}
	template<class A, class B> inline Reference<A> UpCast(Reference<B>& b_)
	{
		return std::dynamic_pointer_cast<A>(b_);
	}
	template<class A, class B> inline Reference<A> UpCast(const Reference<B>& b_)
	{
		return std::dynamic_pointer_cast<A>(b_);
	}

	const Size BITS_IN_BYTE = 8;

	enum class Format
	{
		R8_UInt,
		R8G8B8_UInt,
		R8G8B8A8_UInt,
		B8G8R8_UInt,
		B8G8R8A8_UInt,
		R16_UInt,
		R16G16B16_UInt,
		R16G16B16A16_UInt,
		R32_SFloat,
		R32G32B32_SFloat,
		R32G32B32A32_SFloat,
	};
}


#pragma region
#pragma endregion


