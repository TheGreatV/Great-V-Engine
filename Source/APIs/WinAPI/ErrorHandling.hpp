#pragma region Include
#pragma once


#include <Header/ErrorHandling.hpp>
#include "Header.hpp"


#include <string>
#pragma endregion


namespace GreatVEngine
{
	namespace WinAPI
	{
		class Exception:
			public GreatVEngine::Exception
		{
		public:
			using Code =  DWORD;
		protected:
			Code code = 0;
		public:
			inline Exception(const Code& code_);
			inline Exception(const Text& text_);
		public:
			inline Code GetCode() const
			{
				return code;
			}
		};


		bool IsSuccessful(const Exception::Code& code);
		Exception::Text GetErrorText(const Exception::Code& code);
		void ErrorTest() throw(...);
		void ErrorTest(const Exception::Code& errorCode) throw(...);
	}
}


#pragma region Exception
inline GreatVEngine::WinAPI::Exception::Exception(const Code& code_):
	GreatVEngine::Exception(GetErrorText(code_)),
	code(code_)
{
}
inline GreatVEngine::WinAPI::Exception::Exception(const Text& text_):
	GreatVEngine::Exception(text_)
{
}
#pragma endregion


#pragma region
#pragma endregion
