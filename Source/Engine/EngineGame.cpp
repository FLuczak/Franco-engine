#include <Engine/EngineGame.hpp>

#include "SimpleDelegates.h"
#include "Game/CameraComponent.hpp"
#include "Game/SpriteRenderComponent.hpp"
#include "Game/World.hpp"

void Game::Start()
{
	started = true;
	world.Start();
}

Game::Game(sf::RenderWindow& window) : gameWindow(window), world(World::GetInstance())
{
	currentGame = this;
	auto& camera = world.Instantiate("MainCamera");
	camera.AddComponent<CameraComponent>(camera);
}

void Game::Tick(float dt)
{
	if (!started)
	{
		world.TickInEditor();
		return;
	}
	world.Tick(dt);
}

void Game::Render()
{
}

void Game::Inspect()
{
}


void Game::End()
{
}
