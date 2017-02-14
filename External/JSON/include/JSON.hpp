#pragma region Include

#pragma once

#ifndef __TEST_OLOLO__
#define __TEST_OLOLO__ 1
#include <cstdint>
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <functional>

#pragma endregion


namespace JSON
{
	using Size = std::size_t;
	using String = std::string;

	template<class T> using Reference = std::shared_ptr<T>;
	template<class T> using Shared = std::enable_shared_from_this<T>;

	template<class T, size_t S> using Array = std::array<T, S>;
	template<class T> using Vector = std::vector<T>;
	template<class T> using List = std::list<T>;
	template<class T, class S> using Dictionary = std::map<T, S>;
	template<class T> using Set = std::set<T>;
	template<class T> using Initializer = std::initializer_list<T>;


	template<class T> inline Reference<T> WrapReference(T* t)
	{
		return Reference<T>(t);
	}
	template<class T, class...A> inline Reference<T> MakeReference(A...a)
	{
		return std::make_shared<T>(a...);
	}

	
	template<class T> inline T Move(T&& t)
	{
		return std::move(t);
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

	class Exception;
	/*class Undefined;
	class Null;
	class Object;
	namespace Objects
	{
		class Literal;
		namespace Literals
		{
		}
		// class Array;
	}*/
	class Object;
	using Null = std::nullptr_t;
	class Undefined
	{
	};


	class Exception
	{
	public:
		using Text = String;
	protected:
		const Text text = "Unknown exception";
	public:
		inline Exception() = default;
		inline Exception(const Text& text_);
		inline Exception(const Exception&) = default;
		inline Exception(Exception&& source_);
		inline ~Exception() = default;
	public:
		inline Exception& operator = (const Exception&) = delete;
		inline Exception& operator = (Exception&&) = delete;
	public:
		inline const Text& GetText() const;
	};
	/*class Null:
		public Object
	{
	protected:
		class Data;
	public:
		inline Null();
		inline Null(const Null&) = default;
		inline Null(Object&& source_);
		inline ~Null() = default;
	public:
		inline Null& operator = (const Null&) = default;
		inline Null& operator = (Null&& source_);
	};
	class Null::Data:
		public Object::Data
	{
	public:
		inline Data() = default;
		inline Data(const Data&) = default;
		inline Data(Data&& source_) = delete;
		virtual ~Data() = default;
	public:
		inline Data& operator = (const Data&) = default;
		inline Data& operator = (Data&& source_) = delete;
	public:
		virtual operator Text() const override;
	};*/
	/*class Object
	{
	public:
		using Name = String;
		using Text = String;
	protected:
		class Data;
	protected:
		Reference<Data> data = MakeReference<Data>();
	public:
		inline Object() = default;
		inline Object(const Object&) = default;
		inline Object(Object&& source_);
		inline ~Object() = default;
	protected:
		inline Object(const Reference<Data>& data_);
	public:
		inline Object& operator = (const Object&) = default;
		inline Object& operator = (Object&& source_);
	public:
		inline const Object& operator[](const Name& name_) const;
		inline Object& operator[](const Name& name_);
	public:
		inline operator Text() const;
	};
	class Object::Data
	{
	protected:
		Dictionary<Name, Object> childs;
	public:
		inline Data() = default;
		inline Data(const Data&) = default;
		inline Data(Data&& source_) = delete;
		virtual ~Data() = default;
	public:
		inline Data& operator = (const Data&) = default;
		inline Data& operator = (Data&& source_) = delete;
	public:
		virtual const Object& operator[](const Name& name_) const;
		virtual Object& operator[](const Name& name_);
	public:
		virtual operator Text() const;
	};
	namespace Objects
	{
		class Literal:
			public Object
		{
		};
		namespace Literals
		{
		}
	}*/
	class Object
	{
	public:
		using Name = String;
		using Text = String;
		using Index = int;
	public:
		enum class Type
		{
			Undefined,
			Null,
			Boolean,
			Number,
			String,
			Array,
			Map,
		};
	protected:
		class Undefined; // +
		class Null; // +
		class Boolean; // +
		class Number; // +
		class String; // +
		class Array; // -
		class Map; // +
	protected:
		Reference<Undefined> object = Cast<Undefined>(MakeReference<Map>());
	public:
		inline Object() = default;
		inline Object(const JSON::Undefined&);
		inline Object(const JSON::Null&);
		inline Object(const bool& source_);
		inline Object(const int& source_);
		inline Object(const std::double_t& source_);
		inline Object(const char* source_);
		inline Object(const JSON::String& source_);
		inline Object(const Initializer<Object>& initializer_);
		inline Object(const Vector<Object>& vector_);
		inline Object(const Dictionary<Name, Object>& vector_);
		inline Object(const Object&) = default;
		inline Object(Object&& source_);
		inline ~Object() = default;
	protected:
		inline Object(const Reference<Undefined>& object_);
	public:
		inline Object& operator = (const JSON::Undefined&);
		inline Object& operator = (const JSON::Null&);
		inline Object& operator = (const bool& source_);
		inline Object& operator = (const int& source_);
		inline Object& operator = (const std::double_t& source_);
		inline Object& operator = (const char* source_);
		inline Object& operator = (const JSON::String& source_);
		inline Object& operator = (const Initializer<Object>& initializer_);
		inline Object& operator = (const Vector<Object>& vector_);
		inline Object& operator = (const Dictionary<Name, Object>& vector_);
		inline Object& operator = (const Object&) = default;
		inline Object& operator = (Object&& source_);
	public:
		inline const Object& operator [] (const Name& name_) const;
		inline Object& operator [] (const Name& name_);
		inline const Object& operator [] (const char*const name_) const;
		inline Object& operator [] (const char*const name_);
		inline const Object& operator [] (const Index& index_) const;
		inline Object& operator [] (const Index& index_);
	public:
		inline operator Type() const;
		inline operator JSON::Null() const;
		inline operator bool() const;
		inline operator std::double_t() const;
		inline operator JSON::String() const;
	public:
		inline Text Stringify() const;
	};
	class Object::Undefined
	{
	public:
		inline Undefined() = default;
		inline Undefined(const Undefined&) = delete;
		inline Undefined(Undefined&&) = delete;
		virtual ~Undefined() = default;
	public:
		inline Undefined& operator = (const Undefined&) = delete;
		inline Undefined& operator = (Undefined&&) = delete;
	public:
		virtual const Object& operator [] (const Name& name_) const;
		virtual Object& operator [] (const Name& name_);
		virtual const Object& operator [] (const Size& index_) const;
		virtual Object& operator [] (const Size& index_);
	public:
		virtual operator Type() const;
		virtual operator JSON::Null() const;
		virtual operator bool() const;
		virtual operator std::double_t() const;
		virtual operator JSON::String() const;
	public:
		virtual Text Stringify() const;
	};
#pragma region Object::Null
	class Object::Null : public Object::Undefined
	{
	public:
		inline Null() = default;
		inline Null(const Null&) = delete;
		inline Null(Null&&) = delete;
		virtual ~Null() = default;
	public:
		inline Undefined& operator = (const Null&) = delete;
		inline Undefined& operator = (Null&&) = delete;
	public:
		virtual operator Type() const override;
		virtual operator JSON::Null() const override;
	public:
		virtual Text Stringify() const override;
	};
#pragma endregion
#pragma region Object::Boolean
	class Object::Boolean : public Object::Undefined
	{
	public:
		using Value = bool;
	protected:
		Value value;
	public:
		inline Boolean() = delete;
		inline Boolean(const Value& value_);
		inline Boolean(const Boolean&) = delete;
		inline Boolean(Boolean&&) = delete;
		virtual ~Boolean() = default;
	public:
		inline Boolean& operator = (const Boolean&) = delete;
		inline Boolean& operator = (Boolean&&) = delete;
	public:
		virtual operator Type() const override;
		virtual operator bool() const override;
	public:
		virtual Text Stringify() const override;
	};
#pragma endregion
#pragma region Object::Number
	class Object::Number : public Object::Undefined
	{
	public:
		using Value = std::double_t;
	protected:
		Value value;
	public:
		inline Number() = delete;
		inline Number(const Value& value_);
		inline Number(const int& value_);
		inline Number(const Number&) = delete;
		inline Number(Number&&) = delete;
		virtual ~Number() = default;
	public:
		inline Number& operator = (const Number&) = delete;
		inline Number& operator = (Number&&) = delete;
	public:
		virtual operator Type() const override;
		virtual operator std::double_t() const override;
	public:
		virtual Text Stringify() const override;
	};
#pragma endregion
#pragma region Object::String
	class Object::String : public Object::Undefined
	{
	public:
		using Value = JSON::String;
	protected:
		Value value;
	public:
		inline String() = delete;
		inline String(const Value& value_);
		inline String(const String&) = delete;
		inline String(String&&) = delete;
		virtual ~String() = default;
	public:
		inline String& operator = (const String&) = delete;
		inline String& operator = (String&&) = delete;
	public:
		virtual operator Type() const override;
		virtual operator JSON::String() const override;
	public:
		virtual Text Stringify() const override;
	};
#pragma endregion
#pragma region Object::Array
	class Object::Array : public Object::Undefined
	{
	protected:
		Vector<Object> childs;
	public:
		inline Array() = delete;
		inline Array(const Initializer<Object>& initializer_);
		inline Array(const Vector<Object>& vector_);
		inline Array(const Array&) = delete;
		inline Array(Array&&) = delete;
		virtual ~Array() = default;
	public:
		inline Array& operator = (const Array&) = delete;
		inline Array& operator = (Array&&) = delete;
	public:
		virtual const Object& operator [] (const Size& index_) const override;
		virtual Object& operator [] (const Size& index_) override;
	public:
		virtual operator Type() const override;
	public:
		virtual Text Stringify() const override;
	};
#pragma endregion
#pragma region Object::Map
	class Object::Map : public Object::Undefined
	{
	protected:
		Dictionary<Name, Object> childs;
	public:
		inline Map() = default;
		inline Map(const Dictionary<Name, Object>& dictionary_);
		inline Map(const Map&) = delete;
		inline Map(Map&&) = delete;
		virtual ~Map() = default;
	public:
		inline Map& operator = (const Map&) = delete;
		inline Map& operator = (Map&&) = delete;
	public:
		virtual const Object& operator [] (const Name& name_) const override;
		virtual Object& operator [] (const Name& name_) override;
	public:
		virtual operator Type() const override;
	public:
		virtual Text Stringify() const override;
	};
#pragma endregion


	Object Parse(const Object::Text& text_);
}


#pragma region JSON

#pragma region Exception

inline JSON::Exception::Exception(const Text& text_):
	text(text_)
{
}
inline JSON::Exception::Exception(Exception&& source_) :
	text(Move(source_.text))
{

}
inline const JSON::Exception::Text& JSON::Exception::GetText() const
{
	return text;
}

#pragma endregion

#pragma region Object

#pragma region Boolean

inline JSON::Object::Boolean::Boolean(const Value& value_):
	Undefined(),
	value(value_)
{
}

#pragma endregion

#pragma region Number

inline JSON::Object::Number::Number(const int& value_):
	Undefined(),
	value(static_cast<Value>(value_))
{
}
inline JSON::Object::Number::Number(const Value& value_):
	Undefined(),
	value(value_)
{
}

#pragma endregion

#pragma region String

inline JSON::Object::String::String(const Value& value_):
	Undefined(),
	value(value_)
{
}

#pragma endregion

#pragma region Array

inline JSON::Object::Array::Array(const Initializer<Object>& initializer_):
	Undefined(),
	childs(initializer_)
{
}
inline JSON::Object::Array::Array(const JSON::Vector<Object>& vector_):
	Undefined(),
	childs(vector_)
{
}

#pragma endregion

#pragma region Map

inline JSON::Object::Map::Map(const Dictionary<Name, Object>& dictionary_):
	Undefined(),
	childs(dictionary_)
{
}

#pragma endregion


inline JSON::Object::Object(const JSON::Undefined&) :
	object(MakeReference<Undefined>())
{
}
inline JSON::Object::Object(const JSON::Null&):
	object(MakeReference<Object::Null>())
{
}
inline JSON::Object::Object(const bool& source_) :
	object(MakeReference<Boolean>(source_))
{
}
inline JSON::Object::Object(const int& source_) :
	object(MakeReference<Number>(source_))
{
}
inline JSON::Object::Object(const std::double_t& source_) :
	object(MakeReference<Number>(source_))
{
}
inline JSON::Object::Object(const char* source_) :
	object(MakeReference<String>(source_))
{
}
inline JSON::Object::Object(const JSON::String& source_) :
	object(MakeReference<String>(source_))
{
}
inline JSON::Object::Object(const Initializer<Object>& initializer_):
	object(MakeReference<Array>(initializer_))
{
}
inline JSON::Object::Object(const Vector<Object>& vector_):
	object(MakeReference<Array>(vector_))
{
}
inline JSON::Object::Object(const Dictionary<Name, Object>& vector_) :
	object(MakeReference<Map>(vector_))
{
}
inline JSON::Object::Object(Object&& source_):
	object(Move(source_.object))
{
	source_.object = nullptr;
}
inline JSON::Object::Object(const Reference<Undefined>& object_):
	object(object_)
{
}

inline JSON::Object& JSON::Object::operator = (const JSON::Undefined&)
{
	object = MakeReference<Undefined>();

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const JSON::Null&)
{
	object = MakeReference<Null>();

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const bool& source_)
{
	object = MakeReference<Boolean>(source_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const int& source_)
{
	object = MakeReference<Number>(source_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const std::double_t& source_)
{
	object = MakeReference<Number>(source_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const char* source_)
{
	object = MakeReference<String>(source_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const JSON::String& source_)
{
	object = MakeReference<String>(source_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const Initializer<Object>& initializer_)
{
	object = MakeReference<Array>(initializer_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const Vector<Object>& vector_)
{
	object = MakeReference<Array>(vector_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (const Dictionary<Name, Object>& vector_)
{
	object = MakeReference<Map>(vector_);

	return *this;
}
inline JSON::Object& JSON::Object::operator = (Object&& source_)
{
	object = Move(source_.object);
	source_.object = nullptr;

	return *this;
}

inline const JSON::Object& JSON::Object::operator [] (const Name& name_) const
{
	return object->operator[](name_);
}
inline JSON::Object& JSON::Object::operator [] (const Name& name_)
{
	return object->operator[](name_);
}
inline const JSON::Object& JSON::Object::operator [] (const char*const name_) const
{
	return operator[](static_cast<Name>(name_));
}
inline JSON::Object& JSON::Object::operator [] (const char*const name_)
{
	return operator[](static_cast<Name>(name_));
}
inline const JSON::Object& JSON::Object::operator [] (const Index& index_) const
{
	return object->operator[](index_);
}
inline JSON::Object& JSON::Object::operator [] (const Index& index_)
{
	return object->operator[](index_);
}

inline JSON::Object::operator Type() const
{
	return object->operator Type();
}
inline JSON::Object::operator JSON::Null() const
{
	return object->operator JSON::Null();
}
inline JSON::Object::operator bool() const
{
	return object->operator bool();
}
inline JSON::Object::operator std::double_t() const
{
	return object->operator std::double_t();
}
inline JSON::Object::operator JSON::String() const
{
	return object->operator JSON::String();
}

inline JSON::Object::Text JSON::Object::Stringify() const
{
	return object->Stringify();
}

#pragma endregion

#pragma endregion


#endif

#pragma region
#pragma endregion




















