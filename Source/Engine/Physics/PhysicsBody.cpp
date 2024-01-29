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

void PhysicsBody::HandleCollisionEvents()
{
    for (auto it = collisionEvents.begin(); it != collisionEvents.end();)
    {
        if (it->second.type == CollisionEventType::Entering)
        {
            GetEntity().OnCollisionEnter(it->second);
        }

        if (it->second.type == CollisionEventType::Colliding)
        {
            GetEntity().OnCollisionStay(it->second);
        }

        if (it->second.type == CollisionEventType::Leaving)
        {
            GetEntity().OnCollisionLeave(it->second);
            it = collisionEvents.erase(it);
            continue;
        }

        ++it;
    }
}

void PhysicsBody::RegisterCollider(BaseCollider& collider,  PhysicsBody& body)
{
	if (&body != this)return;
	colliders.emplace_back(collider);
}
