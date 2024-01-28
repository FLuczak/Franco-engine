#include "Engine/Physics/PhysicsBody.hpp"

void PhysicsBody::Start()
{
	Component::Start();
}

void PhysicsBody::Update(float deltaTime)
{
	Component::Update(deltaTime);
}

void PhysicsBody::OnDestroy()
{
	Component::OnDestroy();
}

void PhysicsBody::RegisterCollider(BaseCollider& collider,  PhysicsBody& body)
{
	if (&body != this)return;
	colliders.emplace_back(collider);
}
