#include "Engine/Physics/PhysicsWorld.h"

#include "Engine/Entity.hpp"

void PhysicsWorld::HandlePhysicsBodyAdded(PhysicsBody& toAdd)
{
	physicsBodies.emplace_back(toAdd);
}

void PhysicsWorld::HandlePhysicsBodyRemove(PhysicsBody& toRemove)
{
	erase_if(physicsBodies, [&toRemove](const std::reference_wrapper<PhysicsBody>& ref)
	{
		return &ref.get() == &toRemove;
	});
}

PhysicsWorld::PhysicsWorld()
{
	PhysicsBody::OnPhysicsBodyAdded.bind(this, &PhysicsWorld::HandlePhysicsBodyAdded);
	PhysicsBody::OnPhysicsBodyRemoved.bind(this, &PhysicsWorld::HandlePhysicsBodyRemove);
}

void PhysicsWorld::Simulate(float dt)
{
	for (auto& physicsBody : physicsBodies)
	{
		physicsBody.get().GetTransform().position += physicsBody.get().velocity * dt;
	}
}

void PhysicsWorld::DetectCollisions()
{

}
