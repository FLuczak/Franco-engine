#pragma once
#include "DebugRenderer.hpp"
#include "EngineGame.hpp"
#include "Inspector.hpp"
#include "AI/BehaviorTrees/Editor/BehaviorTreeEditor.hpp"
#include "AI/FiniteStateMachines/Editor/FiniteStateMachineEditor.hpp"
#include "Game/World.hpp"
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
	World world;
};

extern Engine Engine;


