#pragma once
#include "Engine/Core/SimpleDelegates.h"
#include "Engine/Core/Component.hpp"
#include "Engine/Editor/EditorVariables.h"

class Health : public Component
{
public:
	explicit Health(Entity& entityToSet): Component(entityToSet), maxHealth(0)
	{
	}

	fluczak::sdel::Delegate<void(int, int)> OnHealthChanged;
	fluczak::sdel::Delegate<void()> OnDied;

	void ChangeHealth(int amount);
	void SetHealth(int toSet);
	void OnCollisionEnter(CollisionEvent& event) override;
	void Start() override;
private:
	SERIALIZE_FIELD(int, maxHealth)
	int health = 0;
};
REGISTER_COMPONENT(Health);
