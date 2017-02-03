#pragma region Include
#pragma once


#include "Header.hpp"


#include <string>
#pragma endregion


namespace GreatVEngine
{
	class Exception
	{
	public:
		using Text = String;
	protected:
		const Text text = "Unknown exception";
	public:
		inline Exception() = default;
		inline Exception(const Text& errorText_):
			text(errorText_)
		{

		}
		inline Exception(const Exception&) = default;
		inline ~Exception() = default;
	public:
		inline Exception& operator = (const Exception& source) = delete;
	public:
		inline Text GetText() const
		{
			return text;
		}
	};
}


#pragma region
#pragma endregion
