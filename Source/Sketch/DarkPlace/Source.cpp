#pragma once
#include "DarkPlace.hpp"
#include "Utilities/JSON/JSON.hpp"


GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


void func()
{
	// auto image = OpenIL::Image::Load2D(Filepath("Media/Images/Layered.psd"));


	// WinAPI::Window::Size resolution(800, 600);
	WinAPI::Window::Size resolution(1280, 600);
	// WinAPI::Window::Size resolution(1920, 1080);

	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference<GreatVEngine::WinAPI::WindowClass>(instance, "class");
	auto window = MakeReference<GreatVEngine::WinAPI::Window>(windowClass, "window", resolution);
	auto deviceContext = MakeReference<GreatVEngine::WinAPI::DeviceContext>(window);
	{
		deviceContext->SetPixelFormat();
	}

	auto game = Make<DarkPlace::Game>(deviceContext, window); // deviceContext, window);
	
	auto player = game->Add<DarkPlace::Player>(Vec3(0.0f, 20.0f, 0.0f), Vec3(0.0f));
	
	auto enemy = game->Add<DarkPlace::Enemy>(Vec3(-10.0f, 10.0f, 0.0f), Vec3(0.0f));
	auto enemy2 = game->Add<DarkPlace::Enemy>(Vec3(-20.0f, 10.0f, 0.0f), Vec3(0.0f));

	auto test = game->Add<DarkPlace::Test>(Vec3(0.0f), Vec3(0.0f));
	auto test2 = game->Add<DarkPlace::Test2>(Vec3(0.0f, 14.0f, 0.0f), Vec3(40.0f, 0.0f, 0.0f));
	
	auto maze = DarkPlace::Mazes::Simple::Generate(game);

	// auto pathFinder = MakeReference<Path::Finder>();
	// Vector<Reference<DarkPlace::Mazes::Tile>> tiles;
	// for(int x = 0; x < 10; ++x)
	// {
	// 	auto tile = MakeReference(game->Add<DarkPlace::Mazes::Tile>(
	// 		RotateY3(x*20.0f) * Vec3(0.0f, 0.0f, 20.0f + x * 5.0f)
	// 		));
	// 	tiles.push_back(tile);
	// 	pathFinder->Add(tile);
	// }
	// for(int x = 0; x < 10 - 1; ++x)
	// {
	// 	Path::Connect(tiles[x], tiles[x + 1], Path::Location((tiles[x]->GetPosition() + tiles[x + 1]->GetPosition()) * 0.5f));
	// }


	/*auto decal = MakeReference<Graphics::OpenGL::Decal>();
	{
		decal->SetLocalPosition(Vec3(5.0f, 0.0f, 0.0f));
		decal->SetLocalAngle(Vec3(90.0f, 0.0f, 0.0f));
		decal->SetLocalScale(Vec3(8.0f));

		game->graphicsScene_Main->LoadDecals(
			Filepath("Media/Images/Decals/Hearth_Albedo.png"),
			Filepath("Media/Images/Decals/Hearth_Topology.png"),
			Filepath("Media/Images/Decals/Hearth_Material.png"));

		game->graphicsScene_Main->Add(decal);
	}*/


	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		auto sharedPlayer = MakeReference(player);
		auto sharedEnemy = MakeReference(enemy);
		auto sharedMaze = MakeReference(maze);

		auto path = sharedMaze->pathFinder->FindPath(
			sharedEnemy->GetPosition(),
			sharedPlayer->GetPosition()
		);
		if(path)
		{
			if(path->size() > 0)
			{
				for(auto &i : *path)
				{
					game->graphicsScene_Main->DrawCircle(i.GetPosition(), 1.0f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));
				}

				if(Length((*path)[0].GetPosition() - sharedEnemy->GetPosition()) > 10.0f)
				{
					game->graphicsScene_Main->DrawCircle(Vec2(100.0f), 50.0f, Vec4(1, 0, 0, 1));

					auto target = (*path)[0];
					auto direction = Normalize(target.GetPosition() - sharedEnemy->GetPosition());
					sharedEnemy->SetDir(direction);
				}
				else
				{
					game->graphicsScene_Main->DrawCircle(Vec2(100.0f), 40.0f, Vec4(0, 1, 0, 1));

					if(path->size() > 1)
					{
						game->graphicsScene_Main->DrawCircle(Vec2(100.0f), 50.0f, Vec4(0, 0, 1, 1));

						auto target = (*path)[1];
						auto direction = Normalize(target.GetPosition() - sharedEnemy->GetPosition());
						sharedEnemy->SetDir(direction);
					}
				}
			}
		}

		// auto path = pathFinder->FindPath(sharedEnemy->GetPosition(), tiles[9]->GetPosition());
		// if(path)
		// {
		// 	for(auto &i : *path)
		// 	{
		// 		game->graphicsScene_Main->DrawCircle(i.GetPosition(), 1.0f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));
		// 	}
		// }
		// 
		// for(auto &tile : tiles)
		// {
		// 	for(auto &gateway : tile->gateways)
		// 	{
		// 		auto otherArea = gateway->a == Cast<Path::Area>(tile) ? gateway->b : gateway->a;
		// 		if(auto otherTile = UpCast<DarkPlace::Mazes::Tile>(otherArea))
		// 		{
		// 			game->graphicsScene_Main->DrawLine(tile->GetPosition(), otherTile->GetPosition(), Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		// 		}
		// 	}
		// }

		game->Update();

		Sleep(1000 / 60);

		deviceContext->SwapBuffers();
	}
}

void main()
{
	try
	{
		func();
	}
	catch(GVE::WinAPI::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "WinAPI Exception", MB_OK);
	}
	catch(GVE::OpenGL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenGL Exception", MB_OK);
	}
	catch(GVE::OpenIL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenIL Exception", MB_OK);
	}
	catch(GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	return;
}