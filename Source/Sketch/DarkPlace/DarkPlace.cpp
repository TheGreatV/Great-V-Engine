#include "DarkPlace.hpp"


#pragma region DarkPlace

#pragma region Game

void DarkPlace::Game::Update()
{
	for(auto &entity : entities)
	{
		entity->Update();
	}

	physicsWorld_Main->Render(1.0f / 60.0f);

	graphicsScene_Main->Render(graphicsCamera_Main);
}

#pragma endregion

#pragma region Entity

void DarkPlace::Entity::Update()
{
}

#pragma endregion

#pragma region Player

void DarkPlace::Player::Update()
{
	auto rGame = MakeReference(game);

	Input::Mouse::SetPosition(Vec2(WinAPI::GetDesktopSize()) / 2.0f);
	Float32 moment = 0.1f * (KeyState(Keys::X) ? 0.1f : 1.0f);
	rGame->graphicsCamera_Main->Rotate(Vec3(
		-Input::Mouse::GetPositionDelta().y * moment,
		Input::Mouse::GetPositionDelta().x * moment,
		KeyState(Keys::Q) ? -1.0f : KeyState(Keys::E) ? +1.0f : 0.0f));

	Float32 speed = 1.0f * KeyState(Keys::L_SHIFT) ? 10.0f : KeyState(Keys::Z) ? 0.1f : 1.0f;
	rGame->graphicsCamera_Main->Move(Vec3(
		KeyState(Keys::D) ? +speed : KeyState(Keys::A) ? -speed : 0.0f,
		KeyState(Keys::SPACE) ? +speed : KeyState(Keys::L_CTRL) ? -speed : 0.0f,
		KeyState(Keys::W) ? +speed : KeyState(Keys::S) ? -speed : 0.0f));

	rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(10.0f, 0.0f, 0.0f), Vec4(1, 0, 0, 1));
	rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(0.0f, 10.0f, 0.0f), Vec4(0, 1, 0, 1));
	rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(0.0f, 0.0f, 10.0f), Vec4(0, 0, 1, 1));
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion























