#pragma once
#include "SimpleDelegates.h"
#include "Engine/Component.hpp"
#include "Engine/EditorVariables.h"
#include "Engine/Physics/geometry2d.hpp"
#include "SFML/SFMLMath.hpp"

class PhysicsBody;

enum class ColliderShape
{
	POLYGON,
	DISK,
	AABB,
};

VISITABLE_STRUCT(sf::Vector2f, x, y);

class BaseCollider : public Component
{
public:
	static inline fluczak::sdel::Delegate<void(BaseCollider&,PhysicsBody&)> OnColliderCreated;
	SERIALIZE_FIELD(sf::Vector2f, offset)

	ColliderShape colliderShape;
	PhysicsBody* physicsBody = nullptr;

	BaseCollider(Entity& entityToSet);

	virtual geometry2d::AABB AABB() { return geometry2d::AABB({ 0,0 }, { 0,0 }); };
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
};

