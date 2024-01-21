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
