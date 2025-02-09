#pragma once

#include "Engine/Core/Component.hpp"
#include "Engine/Editor/EditorVariables.h"
#include "SFML/System/Vector2.hpp"

class PlayerMovement : public Component
{
public:
	SERIALIZE_FIELD(float, moveSpeedHorizontal);
	SERIALIZE_FIELD(float, moveSpeedVertical);
	explicit PlayerMovement(Entity& entityToSet): Component(entityToSet), moveSpeedHorizontal(0), moveSpeedVertical(0)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	sf::Vector2f inputs{};
};
REGISTER_COMPONENT(PlayerMovement);