#pragma once
#include "Engine/Core/Component.hpp"

class Health;

class DestroyOnDeathComponent : public Component
{
public:
	explicit DestroyOnDeathComponent(Entity& entityToSet): Component(entityToSet)
	{
	}

	void Start() override;
private:
	void Destroy();
	Health* health = nullptr;
};
REGISTER_COMPONENT(DestroyOnDeathComponent);

