#include "ErrorHandling.hpp"


bool GreatVEngine::OpenGL::IsSuccessful(const Exception::Code& code)
{
	return code == GL_NO_ERROR;
}
GreatVEngine::OpenGL::Exception::Text GreatVEngine::OpenGL::GetErrorText(const Exception::Code& code)
{
	switch(code)
	{
		case GL_INVALID_OPERATION: return "INVALID_OPERATION";
		case GL_INVALID_ENUM: return "INVALID_ENUM";
		case GL_INVALID_VALUE: return "INVALID_VALUE";
		case GL_OUT_OF_MEMORY: return "OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
	}

	throw Exception("Unknown error code");
}
void GreatVEngine::OpenGL::ErrorTest()
{
	auto code = glGetError();

	ErrorTest(code);
}
void GreatVEngine::OpenGL::ErrorTest(const Exception::Code& code)
{
	if(!IsSuccessful(code))
	{
		throw Exception(code);
	}
}







