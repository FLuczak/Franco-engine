#include "Game/BaseCollider.h"

#include "Engine/Entity.hpp"
#include "Engine/Physics/PhysicsBody.hpp"

BaseCollider::BaseCollider(Entity& entityToSet): Component(entityToSet){}

void BaseCollider::Start()
{
	Component::Start();
	PhysicsBody* body = GetEntity().GetComponent<PhysicsBody>();
	if (body == nullptr)return;
	physicsBody = body;
	OnColliderCreated(*this, *body);
}

void BaseCollider::Update(float deltaTime)
{
	Component::Update(deltaTime);
}

void BaseCollider::OnDestroy()
{
	Component::OnDestroy();
}
