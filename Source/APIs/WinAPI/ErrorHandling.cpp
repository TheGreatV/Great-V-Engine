#include "ErrorHandling.hpp"


bool GreatVEngine::WinAPI::IsSuccessful(const Exception::Code& code)
{
	return code == NO_ERROR;
}
GreatVEngine::WinAPI::Exception::Text GreatVEngine::WinAPI::GetErrorText(const Exception::Code& code)
{
	LPSTR messageBuffer = nullptr;

	auto messageSize = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL
	);

	Exception::Text text(messageBuffer, messageSize);

	LocalFree(messageBuffer);

	return text;
}
void GreatVEngine::WinAPI::ErrorTest()
{
	auto code = GetLastError();

	ErrorTest(code);
}
void GreatVEngine::WinAPI::ErrorTest(const Exception::Code& code)
{
	if(!IsSuccessful(code))
	{
		throw Exception(code);
	}
}







