#include "Brothel.hpp"


const GVE::Float32 Selectable::Selection::NO_SELECTION = FLT_MAX;


#pragma region Event

#pragma region Performer

void Brothel::Event::Performer::Perform(const Reference<Events::FinishEvent>& event_)
{
}
void Brothel::Event::Performer::Perform(const Reference<Events::Service>& event_)
{
}
void Brothel::Event::Performer::Perform(const Reference<Events::Services::Test>& event_)
{
}

#pragma endregion


void Brothel::Event::Accept(const Reference<Performer>& performer_)
{
	performer_->Perform(GetThis<Event>());
}

#pragma endregion

#pragma region Events

#pragma region FinishEvent

void Brothel::Events::FinishEvent::Accept(const Reference<Performer>& performer_)
{
	performer_->Perform(GetThis<FinishEvent>());
}

#pragma endregion

#pragma region Service

void Brothel::Events::Service::Accept(const Reference<Performer>& performer_)
{
	performer_->Perform(GetThis<Service>());
}

#pragma endregion


#pragma region Services

#pragma region Test

void Brothel::Events::Services::Test::Accept(const Reference<Performer>& performer_)
{
	performer_->Perform(GetThis<Services::Test>());
}
void Brothel::Events::Services::Test::Update()
{
	progress += 1.0f;
	// ???

	if(progress >= 100.0f)
	{
		auto finish = Make<FinishEvent>(Cast<Event>(GetThis()));

		Cast<Event::Performer>(guest)->Perform(finish);
		Cast<Event::Performer>(personnel)->Perform(finish);

		GetGame()->Remove(GetThis());
	}
	else
	{
		Accept(guest);
		Accept(personnel);
	}
}

#pragma endregion

#pragma endregion

#pragma endregion


#pragma region Entity

void Brothel::Entity::Update()
{
}

#pragma endregion

#pragma region Game

void Brothel::Game::ProcessGuests()
{
	if(guestGenerationCooldown <= 0.0f)
	{
		auto i = guestsQueue.size();

		if(KeyState(Keys::SPACE) && i < 3)
		{
			auto guest = Create<Guest>(Vec3(-5.0f - i * 2.0f, 0.0f, +2.0f), Vec3(0.0f), 20, Guest::Gender::Male);

			auto ownedGuest = MakeReference(guest);
			selector_Main->Remove(ownedGuest);
			Remove(ownedGuest);

			// guestsQueue.push_back(ownedGuest);

			guestGenerationCooldown = 30.0f;
		}
	}
	else
	{
		--guestGenerationCooldown;
	}

	if(!guestsQueue.empty())
	{
		auto guest = guestsQueue[0];

		auto personnel = [&]()
		{
			for(auto &entity : entities)
			{
				if(auto &personnel = UpCast<Personnel>(entity))
				{
					if(
						personnel->GetWorkplace() &&
						!personnel->GetEvent()
					) {
						// TODO: check here if personnel is acceptable
						return personnel;
					}
				}
			}

			return Reference<Personnel>(nullptr);
		}();

		if(personnel)
		{
			auto event = Create<Events::Services::Test>(guest, personnel);

			guestsQueue.erase(guestsQueue.begin());
		}
	}

	/*if(guestGenerationCooldown <= 0.0f)
	{
		auto i = guestsQueue.size();

		if(i < 3)
		{
			auto guest = Create<Guest>(Vec3(-5.0f - i * 2.0f, 0.0f, +2.0f), Vec3(0.0f), 20, Guest::Gender::Male);

			auto ownedGuest = MakeReference(guest);
			guestsQueue.push_back(ownedGuest);

			guestGenerationCooldown = 60.0f * 5;
		}
	}
	else
	{
		--guestGenerationCooldown;
	}

	if(guestProcessingCooldown <= 0.0f)
	{
		if(!guestsQueue.empty())
		{
			auto guest = guestsQueue[0];
		}

		guestProcessingCooldown = 60.0f * 6;
	}
	else
	{
		--guestProcessingCooldown;
	}*/
}

void Brothel::Game::Update()
{
	ProcessGuests();

	cameraController->Update();

	auto s = state;
	s->Update();

	auto entitiesToUpdate = entities;
	for(auto &entity : entitiesToUpdate)
	{
		entity->Update();
	}
}

#pragma endregion

#pragma region Character

void Brothel::Character::Perform(const Reference<Events::FinishEvent>& event_)
{
	if(event == event_->GetEvent())
	{
		event = nullptr;
	}
	else
	{
		throw Exception();
	}
}

#pragma endregion

#pragma region Personnel

void Brothel::Personnel::Update()
{
	ProcessWorking();

	graphicsObjectPassionIndicator->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Brothel::Personnel::Perform(const Reference<Events::Services::Test>& event_)
{
	if(!event)
	{
		event = event_;
	}

	if(event == event_)
	{
		GetGame()->graphicsScene_Main->DrawCircle(GetPosition() + Vec3(0.0f, 3.0f, 0.0f), 1.0f, Vec4(1, 0, 0, 1));
	}
	else
	{
		throw Exception();
	}
}

#pragma endregion

#pragma region Guest

void Brothel::Guest::Perform(const Reference<Events::FinishEvent>& event_)
{
	Character::Perform(event_);

	GetGame()->Remove(GetThis<Guest>());
}
void Brothel::Guest::Perform(const Reference<Events::Services::Test>& event_)
{
	if(!event)
	{
		event = event_;
	}

	if(event == event_)
	{
		SetLocalPosition(event_->GetPersonnel()->GetPosition() + Vec3(-1.0f, 0.0f, 0.0f));

		GetGame()->graphicsScene_Main->DrawCircle(GetPosition() + Vec3(0.0f, 3.0f, 0.0f), 1.0f, Vec4(1, 1, 0, 1));
		GetGame()->graphicsScene_Main->DrawLine(GetPosition() + Vec3(0.0f, 3.0f, 0.0f), event_->GetGuest()->GetPosition() + Vec3(0.0f, 3.0f, 0.0f), Vec4(1, 1, 0, 1));
	}
	else
	{
		throw Exception();
	}
}

#pragma endregion


#pragma region

void Brothel::GameStates::None::Update()
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

	/*for(auto &i : game->selector_Main->selections)
	{
		if(auto casted = UpCast<Section>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = UpCast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = UpCast<Character>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}
	}*/

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

void Brothel::GameStates::RotateCamera::Update()
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

void Brothel::GameStates::ShiftCamera::Update()
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

void Brothel::GameStates::PersonnelAssign::Update()
{
	auto mouseLeft = Input::Mouse::GetButtonState(Buttons::Left);
	auto mouseMiddle = Input::Mouse::GetButtonState(Buttons::Middle);
	auto mouseRight = Input::Mouse::GetButtonState(Buttons::Right);
	
	/*for(auto &i : game->selector_Main->selections)
	{
		if(auto casted = UpCast<Section>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = UpCast<Rooms::Bedrooms::Miserable>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}

		if(auto casted = UpCast<Character>(i)) if(casted->graphicsObject)
		{
			casted->graphicsObject->SetColor(Vec4(1.0f));
		}
	}*/

	auto m = Vec2(game->window->ToLocalPosition(IVec2(Input::Mouse::GetPosition()))); // game->graphicsScene_Main->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));
	auto p = (m / Vec2(game->window->GetSize())) * 2.0f - 1.0f;

	auto picked = game->selector_Main->Pick(game->graphicsCamera_Main, p);
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
				if(auto bedroom = UpCast<Rooms::Bedroom>(picked)) // must be Bedroom
				{
					bedroom->AttachInmate(personnel);
				}
				else if(auto workroom = UpCast<Rooms::Workroom>(picked)) // must be Bedroom
				{
					workroom->AttachWorker(personnel);
				}
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

void Brothel::GameStates::BuildRoomSelection::Update()
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

#pragma endregion


#pragma region
#pragma endregion


