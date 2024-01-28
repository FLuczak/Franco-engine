#pragma once
#include "SimpleDelegates.h"
#include "Engine/Component.hpp"

class PhysicsBody;

enum class ColliderShape
{
	POLYGON,
	CIRCLE,
	AABB
};

class BaseCollider : public Component
{
public:
	static inline fluczak::sdel::Delegate<void(BaseCollider&,PhysicsBody&)> OnColliderCreated;

	ColliderShape colliderShape;
	BaseCollider(Entity& entityToSet);
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
};

