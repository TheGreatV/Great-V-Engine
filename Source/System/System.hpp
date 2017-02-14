#pragma region Include
#pragma once


#include "Header.hpp"
#include <Header/ErrorHandling.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace System
	{
		class BinaryFileReader
		{
		public:
			using Handle = FILE*;
			using Buffer = Vector<std::uint8_t>;
		public:
			inline static Reference<BinaryFileReader> LoadFile(const Filename& filename_);
		protected:
			const Handle handle;
			Buffer buffer;
		protected:
			inline BinaryFileReader(const Handle& handle_):
				handle(handle_)
			{
			}
		public:
			inline BinaryFileReader(const BinaryFileReader&) = delete;
			inline ~BinaryFileReader()
			{
				fclose(handle);
			}
		public:
			inline BinaryFileReader& operator = (const BinaryFileReader&) = delete;
		public:
			template<class Value>
			const Value* ReadRaw(const Size& count_ = 1)
			{
				buffer.resize(sizeof(Value)* count_);
				
				if(fread(buffer.data(), 1, sizeof(Value)* count_, handle) == sizeof(Value)* count_)
				{
					return reinterpret_cast<Value*>(buffer.data());
				}

				/*
				if(fread_s(buffer.data(), sizeof(Value) * count_, 1, sizeof(Value) * count_, handle) == sizeof(Value) * count_)
				{
					return reinterpret_cast<Value*>(buffer.data());
				}
				*/

				throw Exception(
					"Failed to read " + std::to_string(count_) + " value(s) of " + std::to_string(sizeof(Value)) +
					" bytes size each (total " + std::to_string(sizeof(Value) * count_) + " bytes) from file.");
			}
			template<class Value>
			const Value Read()
			{
				Value value;

				if(fread(&value, 1, sizeof(Value), handle) == sizeof(Value))
				{
					return value;
				}

				throw Exception("");
			}
			template<class Value>
			const Vector<Value> Read(const Size& count_)
			{
				Vector<Value> values(count_);

				if(fread(values.data(), 1, sizeof(Value)* count_, handle) == sizeof(Value)* count_)
				{
					return std::move(values);
				}

				throw Exception(
					"Failed to read " + std::to_string(count_) + " value(s) of " + std::to_string(sizeof(Value)) +
					" bytes size each (total " + std::to_string(sizeof(Value)* count_) + " bytes) from file.");
			}
		};


		inline String LoadFileContent(const Filename& filename_)
		{
			FILE* file = nullptr;
			if(fopen_s(&file, filename_.c_str(), "r") != 0)
			{
				throw Exception("failed to load file: " + filename_);
			}
			fseek(file, 0, FILE_END);
			auto size = ftell(file);

			rewind(file);

			String data; data.resize(size);
			fread((void*)data.data(), 1, size, file);

			fclose(file);

			return std::move(data);
		}

		template<typename Value> Vector<Value> LoadFileContentBinary(const Filename& filename_)
		{
			FILE* file = nullptr;
			if(fopen_s(&file, filename_.c_str(), "rb") != 0)
			{
				throw Exception("failed to load file: " + filename_);
			}
			fseek(file, 0, FILE_END);
			auto size = ftell(file);

			if(size % sizeof(Value) != 0)
			{
				throw Exception("File size (" + std::to_string(size) + ") not multiple of value size (" + std::to_string(sizeof(Value)) + ")");
			}

			rewind(file);

			Vector<Value> data(size / sizeof(Value));
			fread((void*)data.data(), 1, size, file);

			fclose(file);

			return std::move(data);
		}
	}
}


#pragma region BinaryFileReader
inline GreatVEngine::Reference<GreatVEngine::System::BinaryFileReader> GreatVEngine::System::BinaryFileReader::LoadFile(const Filename& filename_)
{
	Handle handle;
	if(fopen_s(&handle, filename_.c_str(), "rb") == 0)
	{
		return WrapReference(new BinaryFileReader(handle));
	}

	throw Exception("Failed to load file: " + filename_);
}
#pragma endregion


#pragma region
#pragma endregion


