#pragma region Include
#pragma once


#include "Header.hpp"
#include <APIs/WinAPI/Window.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace Input
	{
		class Keyboard
		{
		public:
			struct Key
			{
				bool state, press;
			};
		public:
			enum class KeyCode
			{
				ESC = 0,
				F1 = 1,
				F2 = 2,
				F3 = 4,
				F4 = 5,
				F5 = 6,
				F6 = 7,
				F7 = 8,
				F8 = 9,
				F9 = 10,
				F10 = 11,
				F11 = 12,
				F12 = 13,
				PrtScr = 14,
				SCROLL_LOCK = 15,
				PAUSE = 16,
				TILDE = 17,
				DIGIT1 = 18,
				DIGIT2 = 19,
				DIGIT3 = 20,
				DIGIT4 = 21,
				DIGIT5 = 22,
				DIGIT6 = 23,
				DIGIT7 = 24,
				DIGIT8 = 25,
				DIGIT9 = 26,
				DIGIT0 = 27,
				MINUS = 28,
				PLUS = 29,
				BACKSPACE = 30,
				TAB = 31,
				Q = 32,
				W = 33,
				E = 34,
				R = 35,
				T = 36,
				Y = 37,
				U = 38,
				I = 39,
				O = 40,
				P = 41,
				L_CLIP = 42,		// {
				R_CLIP = 43,		// }
				ENTER = 44,
				CAPS_LOCK = 45,
				A = 46,
				S = 47,
				D = 48,
				F = 49,
				G = 50,
				H = 51,
				J = 52,
				K = 53,
				L = 54,
				COLON = 55,		// ;
				QUOTE = 56,		// '
				DASH = 57,		// \0
				BACKSLASH = 58,		// \0
				Z = 59,
				X = 60,
				C = 61,
				V = 62,
				B = 63,
				N = 64,
				M = 65,
				COMM = 66,		// ,
				DOT = 67,		// .
				SLASH = 68,		// /
				L_SHIFT = 69,
				R_SHIFT = 70,
				SHIFT = 71,
				L_ALT = 72,
				R_ALT = 73,
				ALT = 74,
				L_CTRL = 75,
				R_CTRL = 76,
				CTRL = 77,
				L_WIN = 78,
				R_WIN = 79,
				WIN = 80,
				SPACE = 81,
				MENU = 82,
				INS = 83,
				DEL = 84,
				HOME = 85,
				END = 86,
				PAGE_UP = 87,
				PAGE_DOWN = 88,
				UP = 89,
				DOWN = 90,
				RIGHT = 91,
				LEFT = 92,
				NUM_LOCK = 93,
				NUM_DIV = 94,
				NUM_MUL = 95,
				NUM_SUB = 96,
				NUM_ADD = 97,
				NUM_DIGIT1 = 98,
				NUM_DIGIT2 = 99,
				NUM_DIGIT3 = 100,
				NUM_DIGIT4 = 101,
				NUM_DIGIT5 = 102,
				NUM_DIGIT6 = 103,
				NUM_DIGIT7 = 104,
				NUM_DIGIT8 = 105,
				NUM_DIGIT9 = 106,
				NUM_DIGIT0 = 107,
				__COUNT__
			};
		protected:
			static Key key[(Size)KeyCode::__COUNT__];
		public:
			inline static void Loop()
			{
				key[(Size)KeyCode::ESC].state = GetAsyncKeyState(VK_ESCAPE) != 0;
				key[(Size)KeyCode::F1].state = GetAsyncKeyState(VK_F1) != 0;
				key[(Size)KeyCode::F2].state = GetAsyncKeyState(VK_F2) != 0;
				key[(Size)KeyCode::F3].state = GetAsyncKeyState(VK_F3) != 0;
				key[(Size)KeyCode::F4].state = GetAsyncKeyState(VK_F4) != 0;
				key[(Size)KeyCode::F5].state = GetAsyncKeyState(VK_F5) != 0;
				key[(Size)KeyCode::F6].state = GetAsyncKeyState(VK_F6) != 0;
				key[(Size)KeyCode::F7].state = GetAsyncKeyState(VK_F7) != 0;
				key[(Size)KeyCode::F8].state = GetAsyncKeyState(VK_F8) != 0;
				key[(Size)KeyCode::F9].state = GetAsyncKeyState(VK_F9) != 0;
				key[(Size)KeyCode::F10].state = GetAsyncKeyState(VK_F10) != 0;
				key[(Size)KeyCode::F11].state = GetAsyncKeyState(VK_F11) != 0;
				key[(Size)KeyCode::F12].state = GetAsyncKeyState(VK_F2) != 0;
				key[(Size)KeyCode::PrtScr].state = GetAsyncKeyState(VK_SNAPSHOT) != 0;
				key[(Size)KeyCode::SCROLL_LOCK].state = GetAsyncKeyState(VK_SCROLL) != 0;
				key[(Size)KeyCode::PAUSE].state = GetAsyncKeyState(VK_PAUSE) != 0;

				key[(Size)KeyCode::TILDE].state = GetAsyncKeyState(VK_OEM_3) != 0;
				key[(Size)KeyCode::DIGIT1].state = GetAsyncKeyState('1') != 0;
				key[(Size)KeyCode::DIGIT2].state = GetAsyncKeyState('2') != 0;
				key[(Size)KeyCode::DIGIT3].state = GetAsyncKeyState('3') != 0;
				key[(Size)KeyCode::DIGIT4].state = GetAsyncKeyState('4') != 0;
				key[(Size)KeyCode::DIGIT5].state = GetAsyncKeyState('5') != 0;
				key[(Size)KeyCode::DIGIT6].state = GetAsyncKeyState('6') != 0;
				key[(Size)KeyCode::DIGIT7].state = GetAsyncKeyState('7') != 0;
				key[(Size)KeyCode::DIGIT8].state = GetAsyncKeyState('8') != 0;
				key[(Size)KeyCode::DIGIT9].state = GetAsyncKeyState('9') != 0;
				key[(Size)KeyCode::DIGIT0].state = GetAsyncKeyState('0') != 0;
				key[(Size)KeyCode::MINUS].state = GetAsyncKeyState(VK_OEM_MINUS) != 0;
				key[(Size)KeyCode::PLUS].state = GetAsyncKeyState(VK_OEM_PLUS) != 0;
				key[(Size)KeyCode::BACKSPACE].state = GetAsyncKeyState(VK_BACK) != 0;

				key[(Size)KeyCode::TAB].state = GetAsyncKeyState(VK_TAB) != 0;
				key[(Size)KeyCode::Q].state = GetAsyncKeyState('Q') != 0;
				key[(Size)KeyCode::W].state = GetAsyncKeyState('W') != 0;
				key[(Size)KeyCode::E].state = GetAsyncKeyState('E') != 0;
				key[(Size)KeyCode::R].state = GetAsyncKeyState('R') != 0;
				key[(Size)KeyCode::T].state = GetAsyncKeyState('T') != 0;
				key[(Size)KeyCode::Y].state = GetAsyncKeyState('Y') != 0;
				key[(Size)KeyCode::U].state = GetAsyncKeyState('U') != 0;
				key[(Size)KeyCode::I].state = GetAsyncKeyState('I') != 0;
				key[(Size)KeyCode::O].state = GetAsyncKeyState('O') != 0;
				key[(Size)KeyCode::P].state = GetAsyncKeyState('P') != 0;
				key[(Size)KeyCode::L_CLIP].state = GetAsyncKeyState(VK_OEM_4) != 0;
				key[(Size)KeyCode::R_CLIP].state = GetAsyncKeyState(VK_OEM_6) != 0;
				key[(Size)KeyCode::ENTER].state = GetAsyncKeyState(VK_RETURN) != 0;

				key[(Size)KeyCode::CAPS_LOCK].state = GetAsyncKeyState(VK_CAPITAL) != 0;
				key[(Size)KeyCode::A].state = GetAsyncKeyState('A') != 0;
				key[(Size)KeyCode::S].state = GetAsyncKeyState('S') != 0;
				key[(Size)KeyCode::D].state = GetAsyncKeyState('D') != 0;
				key[(Size)KeyCode::F].state = GetAsyncKeyState('F') != 0;
				key[(Size)KeyCode::G].state = GetAsyncKeyState('G') != 0;
				key[(Size)KeyCode::H].state = GetAsyncKeyState('H') != 0;
				key[(Size)KeyCode::J].state = GetAsyncKeyState('J') != 0;
				key[(Size)KeyCode::K].state = GetAsyncKeyState('K') != 0;
				key[(Size)KeyCode::L].state = GetAsyncKeyState('L') != 0;
				key[(Size)KeyCode::COLON].state = GetAsyncKeyState(VK_OEM_1) != 0;
				key[(Size)KeyCode::QUOTE].state = GetAsyncKeyState(VK_OEM_7) != 0;
				key[(Size)KeyCode::DASH].state = GetAsyncKeyState(VK_OEM_5) != 0;

				key[(Size)KeyCode::BACKSLASH].state = GetAsyncKeyState(VK_OEM_102) != 0;
				key[(Size)KeyCode::Z].state = GetAsyncKeyState('Z') != 0;
				key[(Size)KeyCode::X].state = GetAsyncKeyState('X') != 0;
				key[(Size)KeyCode::C].state = GetAsyncKeyState('C') != 0;
				key[(Size)KeyCode::V].state = GetAsyncKeyState('V') != 0;
				key[(Size)KeyCode::B].state = GetAsyncKeyState('B') != 0;
				key[(Size)KeyCode::N].state = GetAsyncKeyState('N') != 0;
				key[(Size)KeyCode::M].state = GetAsyncKeyState('M') != 0;
				key[(Size)KeyCode::COMM].state = GetAsyncKeyState(VK_OEM_COMMA) != 0;
				key[(Size)KeyCode::DOT].state = GetAsyncKeyState(VK_OEM_PERIOD) != 0;
				key[(Size)KeyCode::SLASH].state = GetAsyncKeyState(VK_OEM_2) != 0;

				key[(Size)KeyCode::L_SHIFT].state = GetAsyncKeyState(VK_LSHIFT) != 0;
				key[(Size)KeyCode::R_SHIFT].state = GetAsyncKeyState(VK_RSHIFT) != 0;
				key[(Size)KeyCode::SHIFT].state = key[(Size)KeyCode::L_SHIFT].state || key[(Size)KeyCode::R_SHIFT].state;
				key[(Size)KeyCode::L_ALT].state = GetAsyncKeyState(VK_LMENU) != 0;
				key[(Size)KeyCode::R_ALT].state = GetAsyncKeyState(VK_RMENU) != 0;
				key[(Size)KeyCode::ALT].state = key[(Size)KeyCode::L_ALT].state || key[(Size)KeyCode::R_ALT].state;
				key[(Size)KeyCode::L_CTRL].state = GetAsyncKeyState(VK_LCONTROL) != 0;
				key[(Size)KeyCode::R_CTRL].state = GetAsyncKeyState(VK_RCONTROL) != 0;
				key[(Size)KeyCode::CTRL].state = key[(Size)KeyCode::L_CTRL].state || key[(Size)KeyCode::R_CTRL].state;
				key[(Size)KeyCode::L_WIN].state = GetAsyncKeyState(VK_LWIN) != 0;
				key[(Size)KeyCode::R_WIN].state = GetAsyncKeyState(VK_RWIN) != 0;
				key[(Size)KeyCode::WIN].state = key[(Size)KeyCode::L_WIN].state || key[(Size)KeyCode::R_WIN].state;
				key[(Size)KeyCode::SPACE].state = GetAsyncKeyState(VK_SPACE) != 0;
				key[(Size)KeyCode::MENU].state = GetAsyncKeyState(VK_APPS) != 0;

				key[(Size)KeyCode::INS].state = GetAsyncKeyState(VK_INSERT) != 0;
				key[(Size)KeyCode::DEL].state = GetAsyncKeyState(VK_DELETE) != 0;
				key[(Size)KeyCode::HOME].state = GetAsyncKeyState(VK_HOME) != 0;
				key[(Size)KeyCode::END].state = GetAsyncKeyState(VK_END) != 0;
				key[(Size)KeyCode::PAGE_UP].state = GetAsyncKeyState(VK_PRIOR) != 0;
				key[(Size)KeyCode::PAGE_DOWN].state = GetAsyncKeyState(VK_NEXT) != 0;

				key[(Size)KeyCode::UP].state = GetAsyncKeyState(VK_UP) != 0;
				key[(Size)KeyCode::DOWN].state = GetAsyncKeyState(VK_DOWN) != 0;
				key[(Size)KeyCode::RIGHT].state = GetAsyncKeyState(VK_RIGHT) != 0;
				key[(Size)KeyCode::LEFT].state = GetAsyncKeyState(VK_LEFT) != 0;

				key[(Size)KeyCode::NUM_LOCK].state = GetAsyncKeyState(VK_NUMLOCK) != 0;
				key[(Size)KeyCode::NUM_DIV].state = GetAsyncKeyState(VK_DIVIDE) != 0;
				key[(Size)KeyCode::NUM_MUL].state = GetAsyncKeyState(VK_MULTIPLY) != 0;
				key[(Size)KeyCode::NUM_SUB].state = GetAsyncKeyState(VK_SUBTRACT) != 0;
				key[(Size)KeyCode::NUM_ADD].state = GetAsyncKeyState(VK_ADD) != 0;
				//NM(Size)KeyCode::_ENTER.state		= GetAsyncKeyState(VK_UP);
				//NM(Size)KeyCode::_DOT.state		= GetAsyncKeyState(VK_DELETE);
				key[(Size)KeyCode::NUM_DIGIT1].state = GetAsyncKeyState(VK_NUMPAD1) != 0;
				key[(Size)KeyCode::NUM_DIGIT2].state = GetAsyncKeyState(VK_NUMPAD2) != 0;
				key[(Size)KeyCode::NUM_DIGIT3].state = GetAsyncKeyState(VK_NUMPAD3) != 0;
				key[(Size)KeyCode::NUM_DIGIT4].state = GetAsyncKeyState(VK_NUMPAD4) != 0;
				key[(Size)KeyCode::NUM_DIGIT5].state = GetAsyncKeyState(VK_NUMPAD5) != 0;
				key[(Size)KeyCode::NUM_DIGIT6].state = GetAsyncKeyState(VK_NUMPAD6) != 0;
				key[(Size)KeyCode::NUM_DIGIT7].state = GetAsyncKeyState(VK_NUMPAD7) != 0;
				key[(Size)KeyCode::NUM_DIGIT8].state = GetAsyncKeyState(VK_NUMPAD8) != 0;
				key[(Size)KeyCode::NUM_DIGIT9].state = GetAsyncKeyState(VK_NUMPAD9) != 0;
				key[(Size)KeyCode::NUM_DIGIT0].state = GetAsyncKeyState(VK_NUMPAD0) != 0;
			}
			inline static bool GetKeyState(const KeyCode& keyCode_)
			{
				return key[(Size)keyCode_].state;
			}
		};
		class Mouse
		{
		public:
			using RawPosition = POINT;
			using Position = Vec2;
			using PositionDelta = Vec2;
		public:
			enum class Button: Size
			{
				Left = 0,
				Right = 1,
				Middle = 2,
				Count = 3
			};
		protected:
			static Position position;
			static PositionDelta positionDelta;
			static Keyboard::Key button[(Size)Button::Count];
		public:
			inline static Position PositionToNormal(const RawPosition& position_)
			{
				return Vec2(
					(Float32)position_.x,
					(Float32)((LONG)WinAPI::GetDesktopSize().y - position_.y));
			}
			inline static RawPosition PositionToRaw(const Position& position_)
			{
				return RawPosition{
					(LONG)position_.x,
					(LONG)((Float32)WinAPI::GetDesktopSize().y - position_.y)};
			}
			inline static RawPosition GetRawPosition()
			{
				RawPosition pos;
				GetCursorPos(&pos);
				return pos;
			}
		public:
			inline static void Loop()
			{
				auto newPosition = PositionToNormal(GetRawPosition());

				positionDelta = newPosition - position;

				position = newPosition;

				button[(Size)Button::Left].state = GetAsyncKeyState(VK_LBUTTON) != 0;
				button[(Size)Button::Middle].state = GetAsyncKeyState(VK_MBUTTON) != 0;
				button[(Size)Button::Right].state = GetAsyncKeyState(VK_RBUTTON) != 0;
			}
			inline static bool GetButtonState(const Button& button_)
			{
				return button[(Size)button_].state;
			}
			inline static Position GetPosition()
			{
				return position;
			}
			inline static void SetPosition(const Position& position_)
			{
				position = position_;
				auto rawPosition = PositionToRaw(position);
				SetCursorPos(rawPosition.x, rawPosition.y);
			}
			inline static PositionDelta GetPositionDelta()
			{
				return positionDelta;
			}
		};

		inline void Loop()
		{
			Keyboard::Loop();
			Mouse::Loop();
		}
	}

	using Buttons = Input::Mouse::Button;
	using Keys = Input::Keyboard::KeyCode;

	inline bool ButtonState(const Buttons& button_)
	{
		return Input::Mouse::GetButtonState(button_);
	}
	inline bool KeyState(const Keys& key_)
	{
		return Input::Keyboard::GetKeyState(key_);
	}
}


#pragma region
#pragma endregion


