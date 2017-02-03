#include "Input.hpp"


GreatVEngine::Vec2 GreatVEngine::Input::Mouse::position = PositionToNormal(GetRawPosition());
GreatVEngine::Vec2 GreatVEngine::Input::Mouse::positionDelta = Vec2(0.0);
GreatVEngine::Input::Keyboard::Key GreatVEngine::Input::Mouse::button[(Size)Button::Count];

GreatVEngine::Input::Keyboard::Key GreatVEngine::Input::Keyboard::key[(Size)KeyCode::__COUNT__];


