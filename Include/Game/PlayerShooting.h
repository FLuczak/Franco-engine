#pragma once
#include "Engine/Core/Component.hpp"
#include "Engine/Editor/EditorVariables.h"

class PlayerShooting : public Component
{
public:
	SERIALIZE_FIELD(float, shootingSpeed);
	SERIALIZE_FIELD(float, bulletSpeed);
	SERIALIZE_FIELD(float, distanceFromPlayer);
	SERIALIZE_FILE_PATH(.ent, bulletPrefab);

	explicit PlayerShooting(Entity& entityToSet): Component(entityToSet)
	{
		shootingSpeed = timer;
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;

private:
	float timer = 0.0f;
};
REGISTER_COMPONENT(PlayerShooting);