#pragma once
#include "Engine/Component.hpp"
#include "Engine/EditorVariables.h"
#include "SFML/System/Vector2.hpp"

class DoorComponent : public Component
{
public:
	SERIALIZE_FIELD(float, doorModifier);
	sf::Vector2f doorDirection;
	explicit DoorComponent(Entity& entityToSet): Component(entityToSet)
	{
		doorModifier = 200.0f;
	}

	void Update(float deltaTime) override;
	void OnCollisionEnter(CollisionEvent& event) override;
};
REGISTER_COMPONENT(DoorComponent);
