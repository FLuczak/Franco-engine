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

void PhysicsWorld::IntegrateForces(float dt) const
{
    for (auto& physicsBody : physicsBodies)
    {
	    auto& body = physicsBody.get();
        body.forces = sf::Vector2f(0, 9.81f);
    	if (body.bodyType == PhysicsBodyType::STATIC) return;
        if (body.bodyType == PhysicsBodyType::DYNAMIC)
        {
            const sf::Vector2f acceleration = body.forces / body.mass;
            body.velocity += acceleration * dt;
            body.forces = {};
        }
        body.GetTransform().position += body.velocity * dt;
    }
}

void PhysicsWorld::Simulate(float dt)
{

    m_tick_timer += dt;
    m_timeThreshold = 1.0f / m_timesPerSecond;

    if (m_tick_timer >= m_timeThreshold)
    {
        m_executedFrame = false;
    }

    while (m_tick_timer >= m_timeThreshold)
    {
       // UpdateDiskColliders();
       // UpdatePolygonColliders();

       // Physics::PhysicsCore::bvh.BuildBVH(bvhElements);

        IntegrateForces(m_timeThreshold);
      //  CheckAndResolveCollisions();
        m_tick_timer -= m_timeThreshold;
        m_executedFrame = true;
    }

}

void PhysicsWorld::DetectCollisions()
{

}
