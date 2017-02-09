#include "Brothel.hpp"


const GVE::Float32 Selectable::Selection::NO_SELECTION = FLT_MAX;


void Brothel::GameStates::None::Loop()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);

	if(KeyState(Keys::B))
	{
		game->Change(MakeReference<BuildRoomSelection>(game));
		return;
	}

	if(!mouseLeft && !mouseMiddle && mouseRight)
	{
		game->Change(MakeReference<GameStates::RotateCamera>(game));
		return;
	}
	if(!mouseLeft && mouseMiddle && !mouseRight)
	{
		game->Change(MakeReference<GameStates::ShiftCamera>(game));
		return;
	}

	auto m = Vec2(game->window->ToLocalPosition(IVec2(Input::Mouse::GetPosition()))); // game->graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
	auto p = (m / Vec2(game->window->GetSize())) * 2.0f - 1.0f;

	for(auto &i : game->selector_Main->selections)
	{
		if(auto casted = std::dynamic_pointer_cast<Section>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Character>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}
	}

	auto picked = game->selector_Main->Pick(game->graphicsCamera_Main, p, [](const Reference<Selectable::Selection>&){
		return true;
	}, [](const Reference<Selectable::Selection>& old_, const Float32& oldDistance_, const Reference<Selectable::Selection>& new_, const Float32& newDistance_){
		return (newDistance_ != Selection::NO_SELECTION) && (
			(old_ && !UpCast<Character>(old_) && UpCast<Character>(new_)) ||
			(newDistance_ < oldDistance_));
	});
	if(picked)
	{
		if(auto casted = std::dynamic_pointer_cast<Section>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Character>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if(picked)
	{
		if(auto personnel = UpCast<Personnel>(picked))
		{
			if(mouseLeft && !mouseMiddle && !mouseRight)
			{
				game->Change(MakeReference<GameStates::PersonnelAssign>(game, personnel));
				return;
			}
		}
	}
}

void Brothel::GameStates::RotateCamera::Loop()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);
	auto mouseMove = Input::Mouse::GetPositionDelta();

	if(!mouseLeft && !mouseMiddle && mouseRight)
	{
		game->cameraController->angle += Vec3(
			-mouseMove.y * 0.1f,
			mouseMove.x * 0.1f,
			0.0f);
		return;
	}
	else
	{
		game->Change(MakeReference<GameStates::None>(game));
		return;
	}
}

void Brothel::GameStates::ShiftCamera::Loop()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);
	auto mouseMove = Input::Mouse::GetPositionDelta();

	if(!mouseLeft && mouseMiddle && !mouseRight)
	{
		auto rMat = RotateZXY3(game->cameraController->angle);
		game->cameraController->position += rMat * Vec3(-mouseMove*Vec2(4.0f, 3.0f)*0.01f, 0.0f);
		return;
	}
	else
	{
		game->Change(MakeReference<GameStates::None>(game));
		return;
	}
}

void Brothel::GameStates::PersonnelAssign::Loop()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);
	
	for(auto &i : game->selector_Main->selections)
	{
		if(auto casted = std::dynamic_pointer_cast<Section>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Character>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}
	}

	auto m = Vec2(game->window->ToLocalPosition(IVec2(Input::Mouse::GetPosition()))); // game->graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
	auto p = (m / Vec2(game->window->GetSize())) * 2.0f - 1.0f;

	auto picked = game->selector_Main->Pick(game->graphicsCamera_Main, p, [](const Reference<Selectable::Selection>& selection_){
		auto casted = UpCast<Rooms::Bedroom>(selection_);
		return casted != nullptr;
	});
	if(picked)
	{
		if(auto casted = std::dynamic_pointer_cast<Section>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Rooms::Bedrooms::Miserable>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}

		if(auto casted = std::dynamic_pointer_cast<Character>(picked)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	if(!mouseMiddle && !mouseRight)
	{
		if(!mouseLeft)
		{
			if(picked)
			{
				auto room = UpCast<Rooms::Bedroom>(picked); // must be Bedroom
				room->AttachInmate(personnel);
			}

			game->Change(MakeReference<GameStates::None>(game));
			return;
		}
	}
	else
	{
		game->Change(MakeReference<GameStates::None>(game));
		return;
	}
}

void Brothel::GameStates::BuildRoomSelection::Loop()
{
	if(KeyState(Keys::DIGIT1))
	{
		game->Change(MakeReference<GameStates::BuildSectionSelection<Rooms::Bedrooms::Miserable>>(game));
		return;
	}

	if(KeyState(Keys::BACKSPACE))
	{
		game->Change(MakeReference<GameStates::None>(game));
		return;
	}
}


Brothel::Room::~Room()
{
	game->rooms.erase(std::find_if(game->rooms.begin(), game->rooms.end(), [this](const Reference<Room>& room_){
		return room_.get() == this;
	}));
}

