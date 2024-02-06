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
    for (auto& collisionEvent : collisionEvents)
    {
        if (collisionEvent.second.type == CollisionEventType::Entering)
        {
            GetEntity().OnCollisionEnter(collisionEvent.second);
        }

        if (collisionEvent.second.type == CollisionEventType::Colliding)
        {
            GetEntity().OnCollisionStay(collisionEvent.second);
        }

        if (collisionEvent.second.type == CollisionEventType::Leaving)
        {
            GetEntity().OnCollisionLeave(collisionEvent.second);
        }
    }

    for (auto it = collisionEvents.begin(); it != collisionEvents.end(); )
    {
        if (it->second.type == CollisionEventType::Entering)
        {
            it->second.type = CollisionEventType::Colliding;
        }

        if (it->second.type == CollisionEventType::Leaving)
        {
            it = collisionEvents.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PhysicsBody::RegisterCollider(BaseCollider& collider,  PhysicsBody& body)
{
	if (&body != this)return;
	colliders.emplace_back(collider);
}
