#pragma once
#include "Engine/Component.hpp"
#include "Engine/EditorVariables.h"
#include "SFML/System/Vector2.hpp"

class PlayerMovement : public Component
{
public:
	SERIALIZE_FIELD(float, moveSpeed);
	explicit PlayerMovement(Entity& entityToSet): Component(entityToSet){}
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	sf::Vector2f inputs{};
};
REGISTER_COMPONENT(PlayerMovement);