#include <Engine/EngineGame.hpp>
#include "Engine/Engine.hpp"
#include "Game/CameraComponent.hpp"
#include "Game/NavMeshObstacle.h"
#include "Game/NavmeshSurface.h"
#include "Game/World.hpp"

void Game::Start()
{
	started = true;
	world.Start();

	std::vector<geometry2d::Polygon> floors;
	std::vector<geometry2d::Polygon> obstacles;

	for (auto& entity : world.GetEntitiesOfType<NavmeshSurface>())
	{
		const auto surface = entity.get().GetComponent<NavmeshSurface>();
		floors.push_back(surface->GetPolygon());
	}

	for (auto& entity : world.GetEntitiesOfType<NavMeshObstacle>())
	{
		const auto obstacle = entity.get().GetComponent<NavMeshObstacle>();
		obstacles.push_back(obstacle->GetPolygon());
	}

	Engine.navMesh = AI::NavMesh(0.75f, floors, obstacles);
	Engine.navMesh.Bake();
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
	started = false;
}
