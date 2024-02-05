#include <Engine/EngineGame.hpp>

#include "SimpleDelegates.h"
#include "Engine/Engine.hpp"
#include "Game/CameraComponent.hpp"
#include "Game/SpriteRenderComponent.hpp"
#include "Game/World.hpp"

void Game::Start()
{
	started = true;
	world.Start();

	float radius = 0.75f;
}

Game::Game() : gameWindow(Engine.window), world(Engine.world)
{
	currentGame = this;
	auto& camera = world.Instantiate("MainCamera");
	camera.AddComponent<CameraComponent>(true);
}

void Game::Tick(float dt)
{
	if (!started)
	{
		world.TickInEditor();
		return;
	}

	physicsWorld.Simulate(dt);
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
