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

	using String = std::string;
	using WString = std::wstring;

	using Filename = String;

	template<class Type> using Memory = Type*;
	template<class Type> using Pointer = std::unique_ptr<Type>;
	template<class Type> using Link = std::weak_ptr<Type>;
	template<class Type> using Reference = std::shared_ptr<Type>;
	template<class Type> using Shared = std::enable_shared_from_this<Type>;

	template<class Type, Size Size> using Array = std::array<Type, Size>;
	template<class Type> using Vector = std::vector<Type>;
	template<class Type> using List = std::list<Type>;
	template<class Key, class Value> using Dictionary = std::map<Key, Value>;
	template<class Type> using Set = std::set<Type>;
	template<class Type> using Initializer = std::initializer_list<Type>;


	// Types
	template<class Type> class This;


	// Functions
	template<class To, class From> inline Reference<To> Cast(const Reference<From>& from_);
	template<class To, class From> inline Reference<const To> Cast(const Reference<const From>& from_);
	template<class To, class From> inline Reference<To> UpCast(const Reference<From>& from_);
	template<class To, class From> inline Reference<const To> UpCast(const Reference<const From>& from_);

	template<class Type> inline typename std::remove_reference<Type>::type&& Move(Type&& type_);
	template<class Type> inline Type&& Forward(typename std::remove_reference<Type>::type& type_);

	template<class Type> inline Pointer<Type> WrapPointer(Type*const type_);
	template<class Type> inline Pointer<const Type> WrapPointer(const Type*const type_);
	template<class Type, class...Arguments> inline Pointer<Type> MakePointer(Arguments&&...arguments_);

	template<class Type> inline Link<Type> MakeLink(const Reference<Type>& reference_);

	template<class Type> inline Reference<Type> WrapReference(Type*const type_);
	template<class Type> inline Reference<const Type> WrapReference(const Type*const type_);
	template<class Type> inline Reference<Type> MakeReference(const Link<Type>& link_);
	template<class Type, class...Arguments> inline Reference<Type> MakeReference(Arguments&&...arguments_);

	template<class Type> inline Memory<Type> AllocateMemory();
	template<class Type> inline void ReleaseMemory(const Memory<Type>& memory_);

	template<class Type, class...Arguments> inline Reference<Type> Make(Arguments&&...arguments_);

	template<class Basic> class SmartDestructor:
		public Basic
	{
		template<class Type, class...Arguments> friend inline Reference<Type> Make(Arguments&&...arguments_);
	protected:
		bool isConstructed;
	};


	inline WString ToWString(const String& source_)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		return converter.from_bytes(source_);
	}

	inline Filename Filepath(const Filename& filename_)
	{
		return "../../../../../" + filename_;
	}


	const Size BITS_IN_BYTE = 8;


	// Classes definition
	template<class Type> class This
	{
	protected:
		const Link<Type> self;
	public:
		inline This(const Reference<Type>& this_);
	public:
		template<class Child> inline Reference<Child> GetThis() const;
		inline Reference<Type> GetThis() const;
	};

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


#pragma region GreatVEngine

#pragma region This

template<class Type>
inline GreatVEngine::This<Type>::This(const Reference<Type>& this_):
	self(this_)
{
}

template<class Type> template<class Child>
inline GreatVEngine::Reference<Child> GreatVEngine::This<Type>::GetThis() const
{
	return Cast<Child>(MakeReference(self));
}

template<class Type>
inline GreatVEngine::Reference<Type> GreatVEngine::This<Type>::GetThis() const
{
	return MakeReference(self);
}

#pragma endregion


template<class To, class From>
inline GreatVEngine::Reference<To> GreatVEngine::Cast(const Reference<From>& from_)
{
	return std::static_pointer_cast<To>(from_);
}

template<class To, class From>
inline GreatVEngine::Reference<const To> GreatVEngine::Cast(const Reference<const From>& from_)
{
	return std::static_pointer_cast<const To>(from_);
}

template<class To, class From>
inline GreatVEngine::Reference<To> GreatVEngine::UpCast(const Reference<From>& from_)
{
	return std::dynamic_pointer_cast<To>(from_);
}

template<class To, class From>
inline GreatVEngine::Reference<const To> GreatVEngine::UpCast(const Reference<const From>& from_)
{
	return std::dynamic_pointer_cast<const To>(from_);
}


template<class Type>
inline typename std::remove_reference<Type>::type&& GreatVEngine::Move(Type&& type_)
{
	return std::move(type_);
}

template<class Type> inline Type&& GreatVEngine::Forward(typename std::remove_reference<Type>::type& type_)
{
	return std::forward<Type>(type_);
}


template<class Type>
inline GreatVEngine::Pointer<Type> GreatVEngine::WrapPointer(Type*const type_)
{
	return Move(Pointer<Type>(type_));
}

template<class Type>
inline GreatVEngine::Pointer<const Type> GreatVEngine::WrapPointer(const Type*const type_)
{
	return Move(Pointer<const Type>(type_));
}

template<class Type, class...Arguments>
inline GreatVEngine::Pointer<Type> GreatVEngine::MakePointer(Arguments&&...arguments_)
{
	return Move(std::make_unique<Type>(Forward<Arguments>(arguments_)...));
}

template<class Type>
inline GreatVEngine::Link<Type> GreatVEngine::MakeLink(const Reference<Type>& reference_)
{
	return Move(Link<Type>(reference_));
}

template<class Type>
inline GreatVEngine::Reference<Type> GreatVEngine::WrapReference(Type*const type_)
{
	return Move(Reference<Type>(type_));
}

template<class Type>
inline GreatVEngine::Reference<const Type> GreatVEngine::WrapReference(const Type*const type_)
{
	return Move(Reference<const Type>(type_));
}

template<class Type>
inline GreatVEngine::Reference<Type> GreatVEngine::MakeReference(const Link<Type>& link_)
{
	return Move(Reference<Type>(link_));
}

template<class Type, class...Arguments>
inline GreatVEngine::Reference<Type> GreatVEngine::MakeReference(Arguments&&...arguments_)
{
	return Move(std::make_shared<Type>(std::forward<Arguments>(arguments_)...));
}


template<class Type>
inline GreatVEngine::Memory<Type> GreatVEngine::AllocateMemory()
{
	auto rawMemory = std::malloc(sizeof(Type));
	auto memory = static_cast<Memory<Type>>(rawMemory);
	return memory;
}

template<class Type>
inline void GreatVEngine::ReleaseMemory(const Memory<Type>& memory_)
{
	std::free(memory_);
}


template<class Type, class...Arguments>
inline GreatVEngine::Reference<Type> GreatVEngine::Make(Arguments&&...arguments_)
{
	auto holder = AllocateMemory<SmartDestructor<Type>>();
	holder->isConstructed = false;

	auto shared = Move(Reference<Type>(static_cast<Type*>(holder), [](Type* memory_){
		auto holder = static_cast<SmartDestructor<Type>*>(memory_);
		
		if(holder->isConstructed)
		{
			holder->~SmartDestructor<Type>();
		}

		ReleaseMemory(holder);
	}));

	new(holder)Type(shared, Forward<Arguments&&>(arguments_)...);
	holder->isConstructed = true;

	return Move(shared);
}

#pragma endregion



#pragma region
#pragma endregion


