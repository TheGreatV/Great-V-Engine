#pragma region Include
#pragma once


#include "Header.hpp"


#include <Header/ErrorHandling.hpp>


#include <string>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
		class Exception:
			public GreatVEngine::Exception
		{
		public:
			using Code = GLenum;
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

		inline void DebugTest()
		{
#if GVE_DEBUG
			ErrorTest();
#endif
		}
	}
}


#pragma region Exception
inline GreatVEngine::OpenGL::Exception::Exception(const Code& code_):
	GreatVEngine::Exception(GetErrorText(code_)),
	code(code_)
{
}
inline GreatVEngine::OpenGL::Exception::Exception(const Text& text_):
	GreatVEngine::Exception(text_)
{
}
#pragma endregion


#pragma region
#pragma endregion
