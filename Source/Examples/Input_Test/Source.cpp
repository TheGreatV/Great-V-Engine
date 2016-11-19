#include <iostream>
#include <Input/Input.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;


void func()
{
	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		
		Input::Mouse::SetPosition(Vec2(Input::Mouse::GetDesktopSize()) / 2.0f);

		auto p = Input::Mouse::GetPosition();
		auto d = Input::Mouse::GetPositionDelta();

		std::cout <<
			"X: " << p.x << ",\t" <<
			"Y: " << p.y << ",\t" <<
			"dX: " << d.x << ",\t" <<
			"dY: " << d.y << std::endl;

		Sleep(1000 / 60);
	}
}

void main()
{
	try
	{
		func();
	}
	catch(GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	return;
}
