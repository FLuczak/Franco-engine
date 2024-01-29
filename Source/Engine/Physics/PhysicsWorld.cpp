#include "Engine/Physics/PhysicsWorld.h"

#include "Engine/Engine.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Physics/DiskCollider.hpp"
#include "Engine/Physics/PolygonCollider.h"

class PolygonCollider;
class DiskCollider;

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

void PhysicsWorld::GetCollisionBroadPhaseBVH(unsigned index, const geometry2d::AABB& aabb, BaseCollider& collider,std::list<BroadPhaseCollisionPair>& list)
{
    const Physics::BVH::BVHNode& node = bvh.GetNodes()[index];

    if (!Collision::AABBIntersect(aabb, node.aabb)) return;
    if (node.IsLeaf())
    {
        for (size_t i = node.firstElementIndex; i < node.firstElementIndex + node.elementCount; i++)
        {
            const Physics::BVH::BVHElement& element = bvh.GetElementOfIndex(static_cast<unsigned>(i));
            if (collider.GetEntity().GetId() == element.collider.get().GetEntity().GetId()) continue;
            if (!Collision::AABBIntersect(element.aabb, aabb)) continue;
            list.emplace_back(element.collider.get(),collider,element.shape,collider.colliderShape);
        }
    }
    else
    {
        GetCollisionBroadPhaseBVH(node.leftChildIndex, aabb, collider, list);
        GetCollisionBroadPhaseBVH(node.rightChildIndex, aabb, collider, list);
    }

}

std::list<BroadPhaseCollisionPair> PhysicsWorld::GetBroadPhaseCollisionPairs()
{
    std::list<BroadPhaseCollisionPair> toReturn{};
	
    for (auto& body : physicsBodies)
    {
	    for (auto& collider : body.get().GetColliders())
	    {
            GetCollisionBroadPhaseBVH(bvh.GetRootIndex(), collider.get().AABB(), collider.get(), toReturn);
	    }
    }

    return toReturn;
}

void PhysicsWorld::CheckAndResolveCollisionForPolygonDisk(BaseCollider& polygon, BaseCollider& disk)
{
    const PolygonCollider& aCollider = dynamic_cast<PolygonCollider&>(polygon);

    CollisionInfo collisionInfo(polygon,disk);

    const DiskCollider& bCollider = dynamic_cast<DiskCollider&>(disk);

    if (Collision::DiskIntersectsPolygon(disk.GetTransform().position + bCollider.offset, bCollider.radius, aCollider.GetTransformedPolygon(), &collisionInfo))
    {
        ResolveCollision(*polygon.physicsBody, *disk.physicsBody, collisionInfo);
    }
}

void PhysicsWorld::ResolveCollision( PhysicsBody& bodyA,  PhysicsBody& bodyB,const CollisionInfo& collisionInfo)
{
    const sf::Vector2f normalTimesDepth = collisionInfo.normal * collisionInfo.depth;
    const sf::Vector2f temp = normalTimesDepth / (bodyA.InverseMass() + bodyB.InverseMass());

    bodyA.GetTransform().position += (-temp * bodyA.InverseMass());

    const sf::Vector2f relativeVelocity = bodyB.velocity - bodyA.velocity;
    const float restitutionCoefficient = std::min(bodyA.restitution, bodyB.restitution);
    float impulseMagnitude = (-(1.0f + restitutionCoefficient) * sf::dot(relativeVelocity, collisionInfo.normal));

    if (sf::dot(relativeVelocity, collisionInfo.normal) > 0.0f)
    {
        return;
    }

    if (bodyA.bodyType == PhysicsBodyType::STATIC)
    {
	    const sf::Vector2f impulse = impulseMagnitude * collisionInfo.normal;
        bodyB.velocity += impulse;
    }
    else if (bodyB.bodyType == PhysicsBodyType::STATIC)
    {
	    const sf::Vector2f impulse = impulseMagnitude * collisionInfo.normal;
        bodyA.velocity -= impulse;
    }
    else
    {
        impulseMagnitude /= (bodyA.InverseMass() + bodyB.InverseMass());
        const sf::Vector2f impulse = (impulseMagnitude)*collisionInfo.normal;

        bodyB.velocity -= impulse * bodyA.InverseMass();
    }
}

void PhysicsWorld::CheckAndResolveCollisionForDiskDisk(BaseCollider& disk1, BaseCollider& disk2)
{
    const DiskCollider& aCollider = dynamic_cast<DiskCollider&>(disk1);

    CollisionInfo collisionInfo(disk1,disk2);

    const DiskCollider& bCollider = dynamic_cast<DiskCollider&>(disk2);

    if (Collision::DisksIntersects(disk1.GetTransform().position + aCollider.offset, aCollider.radius, disk2.GetTransform().position + bCollider.offset, bCollider.radius, &collisionInfo))
    {
        ResolveCollision(*disk1.physicsBody, *disk1.physicsBody, collisionInfo);
    }
}

void PhysicsWorld::CheckAndResolveCollisionForPolygonPolygon(BaseCollider& polygon1, BaseCollider& polygon2)
{
    const PolygonCollider& aCollider = dynamic_cast<PolygonCollider&>(polygon1);

    CollisionInfo collisionInfo(polygon1, polygon2);

    const PolygonCollider& bCollider = dynamic_cast<PolygonCollider&>(polygon2);

    if (Collision::PolygonsIntersect( aCollider.GetTransformedPolygon(),bCollider.GetTransformedPolygon(), &collisionInfo))
    {
        ResolveCollision(*polygon1.physicsBody, *polygon2.physicsBody, collisionInfo);
    }
}

void PhysicsWorld::DetectAndResolveNarrowPhaseCollisions(const std::list<BroadPhaseCollisionPair>& list)
{
    for (const auto& pair : list)
    {
        if (pair.shapes.first == ColliderShape::DISK)
        {
            if (pair.shapes.first == pair.shapes.second)
            {
                CheckAndResolveCollisionForDiskDisk(pair.colliders.first.get(), pair.colliders.second.get());
            }
            else
            {
                CheckAndResolveCollisionForPolygonDisk(pair.colliders.second.get(), pair.colliders.first.get());
            }
        }
        else
        {
            if (pair.shapes.first == pair.shapes.second)
            {
                CheckAndResolveCollisionForPolygonPolygon( pair.colliders.first.get(), pair.colliders.second.get());
            }
            else
            {
                CheckAndResolveCollisionForPolygonDisk( pair.colliders.first.get(), pair.colliders.second.get());
            }
        }
    }
}

void PhysicsWorld::CheckAndResolveCollisions()
{
	const std::list<BroadPhaseCollisionPair>& broadPhasePairs = GetBroadPhaseCollisionPairs();
    if (broadPhasePairs.empty())return;
    DetectAndResolveNarrowPhaseCollisions( broadPhasePairs);
}

void PhysicsWorld::UpdateBvhElements()
{
    for (auto& body : physicsBodies)
    {
        for (auto& collider : body.get().GetColliders())
        {
	        auto& baseCollider = collider.get();
            bvhElements.emplace_back(baseCollider, baseCollider.colliderShape, baseCollider.AABB());
        }
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
        bvhElements.clear();
        UpdateBvhElements();
        bvh.BuildBVH(bvhElements);
        IntegrateForces(m_timeThreshold);
        CheckAndResolveCollisions();
        m_tick_timer -= m_timeThreshold;
        m_executedFrame = true;
    }

    bvh.DebugDraw(bvh.GetRootIndex(),Engine.debugRenderer);
}

void PhysicsWorld::DetectCollisions()
{

}
