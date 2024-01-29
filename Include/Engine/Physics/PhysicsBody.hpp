#pragma once
#include "Collision.hpp"
#include "SimpleDelegates.h"
#include "Engine/Component.hpp"
#include "Engine/Entity.hpp"
#include "Game/BaseCollider.h"
#include "SFML/System/Vector2.hpp"

class BaseCollider;

enum class PhysicsBodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC
};

class PhysicsBody final : public Component
{
public:
	static inline fluczak::sdel::Delegate<void(PhysicsBody&)> OnPhysicsBodyAdded;
	static inline fluczak::sdel::Delegate<void(PhysicsBody&)> OnPhysicsBodyRemoved; 

	float InverseMass() const
	{
		if (bodyType == PhysicsBodyType::STATIC) return 0.0f;
		return 1.0f / mass;
	}

	sf::Vector2f forces {};
	sf::Vector2f velocity{};

	SERIALIZE_FIELD(float, mass)
	SERIALIZE_FIELD(float, restitution)
	SERIALIZE_FIELD(PhysicsBodyType, bodyType);

	explicit PhysicsBody(Entity& entityToSet) : Component(entityToSet)
	{
		OnPhysicsBodyAdded(*this);
		BaseCollider::OnColliderCreated.bind(this, &PhysicsBody::RegisterCollider);
	}
	const std::vector<std::reference_wrapper<BaseCollider>>& GetColliders(){ return colliders; }
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	~PhysicsBody() override
	{
		OnPhysicsBodyRemoved(*this);
	}
private:
	bool physicsTickedThisFrame = false;
	std::vector<std::reference_wrapper<BaseCollider>> colliders;
	std::unordered_map<entity_id, CollisionEvent> collisionEvents;

	void RegisterCollider(BaseCollider& collider, PhysicsBody& body);
	friend class PhysicsWorld;
};

REGISTER_COMPONENT(PhysicsBody);


