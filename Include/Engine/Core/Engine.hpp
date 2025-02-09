#pragma once
#include "Engine/Editor/AssetExplorer.h"
#include "Engine/Editor/DebugRenderer.hpp"
#include "EngineGame.hpp"
#include "Engine/Editor/Inspector.hpp"
#include "Engine/AI/BehaviorTrees/Editor/BehaviorTreeEditor.hpp"
#include "Engine/AI/FiniteStateMachines/Editor/FiniteStateMachineEditor.hpp"
#include "Engine/AI/NavMesh/NavMesh.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Engine : public Inspectable
{
public:
	Engine();
	std::unique_ptr<Game> game;
	void Inspect() override;
	sf::RenderWindow window;
	DebugRenderer debugRenderer;
	Inspector inspector;
	AI::FiniteStateMachineEditor fsmEditor;
	AI::BehaviorTreeEditor btEditor;
	AI::NavMesh navMesh;
	AssetExplorer explorer;
	World world;
private:
	nlohmann::json temporaryWorld;
};

extern Engine Engine;


