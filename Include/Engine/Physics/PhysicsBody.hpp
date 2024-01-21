#pragma once
#include "SimpleDelegates.h"
#include "Engine/Component.hpp"
#include "SFML/System/Vector2.hpp"

class PhysicsBody final : public Component
{
public:
	static inline fluczak::sdel::Delegate<void(PhysicsBody&)> OnPhysicsBodyAdded;
	static inline fluczak::sdel::Delegate<void(PhysicsBody&)> OnPhysicsBodyRemoved; \

	sf::Vector2f velocity{};

	explicit PhysicsBody(Entity& entityToSet) : Component(entityToSet)
	{
		OnPhysicsBodyAdded(*this);
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	~PhysicsBody() override
	{
		OnPhysicsBodyRemoved(*this);
	}
};

REGISTER_COMPONENT(PhysicsBody);


