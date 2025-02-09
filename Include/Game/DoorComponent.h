#pragma once
#include "Engine/Core/SimpleDelegates.h"
#include "Engine/Core/Component.hpp"
#include "Engine/Editor/EditorVariables.h"
#include "SFML/System/Vector2.hpp"

class DoorComponent : public Component
{
public:
	SERIALIZE_FIELD(float, doorModifier);
	SERIALIZE_FIELD(int, roomFrom);
	SERIALIZE_FIELD(int, roomTo);

	sf::Vector2f doorDirection;

	explicit DoorComponent(Entity& entityToSet): Component(entityToSet)
	{
		doorModifier = 150.0f;
	}

	fluczak::sdel::Delegate<void(DoorComponent&)> doorOpened{};
	void Update(float deltaTime) override;
	void OnCollisionEnter(CollisionEvent& event) override;
};
REGISTER_COMPONENT(DoorComponent);
