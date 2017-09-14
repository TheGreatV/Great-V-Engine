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

	if(KeyState(Keys::L))
	{
		if(graphicsLight_Sunlight->IsVisible())
		{
			graphicsLight_Sunlight->Hide();
		}
		else
		{
			graphicsLight_Sunlight->Show();
		}
	}
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

	physicsBody->SetVelocity(physicsBody->GetVelocity() * Vec3(0.5f, 1.0f, 0.5f));
	SetLocalPosition(physicsBody->GetPosition());

	if(KeyState(Keys::MINUS))
	{
		isSpectatorMode = false;
	}
	if(KeyState(Keys::PLUS))
	{
		isSpectatorMode = true;
	}

	if(isSpectatorMode)
	{
		Input::Mouse::SetPosition(Vec2(WinAPI::GetDesktopSize()) / 2.0f);
		Float32 motion = 0.1f * (KeyState(Keys::X) ? 0.1f : 1.0f);
		rGame->graphicsCamera_Main->Rotate(Vec3(
			-Input::Mouse::GetPositionDelta().y * motion,
			Input::Mouse::GetPositionDelta().x * motion,
			KeyState(Keys::Q) ? -2.0f : KeyState(Keys::E) ? +2.0f : 0.0f));

		Float32 speed = 1.0f * KeyState(Keys::L_SHIFT) ? 10.0f : KeyState(Keys::L_ALT) ? 0.1f : 1.0f;
		rGame->graphicsCamera_Main->Move(Vec3(
			KeyState(Keys::D) ? +speed : KeyState(Keys::A) ? -speed : 0.0f,
			KeyState(Keys::SPACE) ? +speed : KeyState(Keys::L_CTRL) ? -speed : 0.0f,
			KeyState(Keys::W) ? +speed : KeyState(Keys::S) ? -speed : 0.0f));

		rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(10.0f, 0.0f, 0.0f), Vec4(1, 0, 0, 1));
		rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(0.0f, 10.0f, 0.0f), Vec4(0, 1, 0, 1));
		rGame->graphicsScene_Main->DrawLine(Vec3(0.0f), Vec3(0.0f, 0.0f, 10.0f), Vec4(0, 0, 1, 1));
	}
	else
	{

		Input::Mouse::SetPosition(Vec2(WinAPI::GetDesktopSize()) / 2.0f);
		auto rotationDelta = Vec2(
			-Input::Mouse::GetPositionDelta().y * 0.03f,
			Input::Mouse::GetPositionDelta().x * 0.04f);

		// Rotate(Vec3(rotationDelta, 0.0f));
		SetLocalAngle(GetLocalAngle() + Vec3(rotationDelta, 0.0f));

		movingTemp = 1.0f;
		{
			if(KeyState(Keys::L_SHIFT) && !KeyState(Keys::L_ALT))
			{
				movingTemp = 2.0f;
			}
			if(!KeyState(Keys::L_SHIFT) && KeyState(Keys::L_ALT))
			{
				movingTemp = 0.5f;
			}
		}

		Float32 moveSpeed = 10.0f * movingTemp;
		Vec3 moveDirection =
			(KeyState(Keys::W) ? Vec3(0.0f, 0.0f, +1.0f) : KeyState(Keys::S) ? Vec3(0.0f, 0.0f, -1.0f) : Vec3(0.0f)) + 
			(KeyState(Keys::D) ? Vec3(+1.0f, 0.0f, 0.0f) : KeyState(Keys::A) ? Vec3(-1.0f, 0.0f, 0.0f) : Vec3(0.0f)) +
			(KeyState(Keys::SPACE) ? Vec3(0.0f, 1.0f, 0.0f) : Vec3(0.0f));

		if(moveDirection != Vec3(0.0f))
		{
			isMoving = true;
			Vec3 moveForce = RotateY3(GetLocalAngle().y) * Normalize(moveDirection) * moveSpeed;
			physicsBody->AddCentralImpulse(moveForce);
		}
		else
		{
			isMoving = false;
		}

		motions->Update();

		rGame->graphicsCamera_Main->SetPosition(VecXYZ(GetMMat() * Vec4(Vec3(0.0f, 2.0f, 0.0f), 1.0f)) + motions->GetPositionOffset());
		rGame->graphicsCamera_Main->SetAngle(GetAngle() + motions->GetAngleOffset());
	}
}

#pragma endregion

#pragma region Enemy

void DarkPlace::Enemy::Update()
{
	auto rGame = MakeReference(game);

	physicsBody->SetVelocity(physicsBody->GetVelocity() * Vec3(0.5f, 1.0f, 0.5f));
	SetLocalPosition(physicsBody->GetPosition());

	if(glm::length(dir) > 0.0f)
	{
		MoveInDirection(dir);
		dir = Vec3(0.0f);
	}

	if(KeyState(Keys::G))
	{
		MoveInDirection(Vec3(0.0f, 0.0f, 1.0f));
	}
}
void DarkPlace::Enemy::MoveInDirection(const Vec3& direction_)
{
	Float32 moveSpeed = 10.0f;

	if(direction_ != Vec3(0.0f))
	{
		Vec3 moveForce = RotateY3(GetLocalAngle().y) * Normalize(direction_) * moveSpeed;
		physicsBody->AddCentralImpulse(moveForce);
	}
}


#pragma endregion

#pragma region Mazes

#pragma region Simple

GVE::Link<DarkPlace::Mazes::Simple> DarkPlace::Mazes::Simple::Generate(const Reference<Game>& game_)
{
	auto maze = game_->Add<Simple>(); // Make<Simple>();
	auto pMaze = MakeReference(maze);

	auto mask = Array<Array<bool, 10>, 10>();
	{
		for(Size x = 0; x < mask.size(); ++x)
		for(Size y = 0; y < mask[x].size(); ++y)
		{
			// mask[x][y] = Rnd() > 0.5f ? true : false;
			mask[x][y] = (x == 0) || (x == (mask.size() - 1)) || (y == 0) || (y == (mask[x].size() - 1));
		}
	}

	for(Size x = 0; x < mask.size(); ++x)
	for(Size y = 0; y < mask[x].size(); ++y)
	{
		if(mask[x][y])
		{
			auto tile = Make<Tile>(game_, Vec3(
				x * 5.0f,
				0.0f,
				y * 5.0f
			));
			pMaze->tiles[x][y] = tile;
			pMaze->pathFinder->Add(tile);
		}
	}

	for(Size x = 0; x < mask.size(); ++x)
	for(Size y = 0; y < mask[x].size(); ++y)
	{
		if(mask[x][y])
		{
			if(x > 0)
			{
				auto tx = x - 1;
				if(mask[tx][y])
				{
					auto gateway = MakeReference<Path::Gateway>(
						pMaze->tiles[x][y],
						pMaze->tiles[tx][y],
						Path::Location((pMaze->tiles[x][y]->GetPosition() + pMaze->tiles[tx][y]->GetPosition()) * 0.5f)
					);
					pMaze->gateways.push_back(gateway);
					pMaze->tiles[x][y]->gateways.push_back(gateway);
					pMaze->tiles[tx][y]->gateways.push_back(gateway);
				}
			}
			if(y > 0)
			{
				auto ty = y - 1;
				if(mask[x][ty])
				{
					auto gateway = MakeReference<Path::Gateway>(
						pMaze->tiles[x][y],
						pMaze->tiles[x][ty],
						Path::Location((pMaze->tiles[x][y]->GetPosition() + pMaze->tiles[x][ty]->GetPosition()) * 0.5f)
					);
					pMaze->gateways.push_back(gateway);
					pMaze->tiles[x][y]->gateways.push_back(gateway);
					pMaze->tiles[x][ty]->gateways.push_back(gateway);
				}
			}
		}
	}

	return maze;
}
void DarkPlace::Mazes::Simple::Update()
{
	auto rGame = MakeReference(game);

	for(auto &gateway : gateways)
	{
		rGame->graphicsScene_Main->DrawLine(
			gateway->a->position,
			gateway->b->position,
			Vec4(1.0f, 1.0f, 0.0f, 1.0)
		);
	}
}

#pragma endregion

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion























