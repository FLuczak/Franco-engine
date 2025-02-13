#include "Engine/Physics/PhysicsWorld.h"

#include <execution>

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Entity.hpp"
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
    for (auto& body : physicsBodies)
    {
        if (!body.get().collisionEvents.contains(toRemove.GetEntity().GetId()))continue;
        body.get().collisionEvents.erase(toRemove.GetEntity().GetId());
    }

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
    	if (body.bodyType == PhysicsBodyType::STATIC) continue;
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
            auto& elementCollider = element.collider.get();

            if (collider.GetEntity().GetId() == elementCollider.GetEntity().GetId()) continue;
            if (collider.physicsBody->bodyType == PhysicsBodyType::STATIC && elementCollider.physicsBody->bodyType == PhysicsBodyType::STATIC) continue;
            if (!Collision::AABBIntersect(element.aabb, aabb)) continue;
            list.emplace_back(elementCollider,collider,element.shape,collider.colliderShape);
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

void PhysicsWorld::ResolveCollision(PhysicsBody& bodyA, PhysicsBody& bodyB, const CollisionInfo& collisionInfo)
{
    if (bodyA.bodyType == PhysicsBodyType::STATIC && bodyB.bodyType == PhysicsBodyType::STATIC)return;

    const sf::Vector2f normalTimesDepth = collisionInfo.normal * collisionInfo.depth;

    const float massA = bodyA.InverseMass();
    const float massB = bodyB.InverseMass();

    const sf::Vector2f temp = normalTimesDepth / (massA + massB);

    bodyA.GetTransform().position += (-temp * massA);
    bodyB.GetTransform().position += (temp * massB);

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
      //  bodyB.velocity += impulse;
    }
    else if (bodyB.bodyType == PhysicsBodyType::STATIC)
    {
        const sf::Vector2f impulse = impulseMagnitude * collisionInfo.normal;
       // bodyA.velocity -= impulse;
    }
    else
    {
        impulseMagnitude /= (massA + massB);
        const sf::Vector2f impulse = (impulseMagnitude)*collisionInfo.normal;

      //  bodyA.velocity -= impulse * massB;
      //  bodyB.velocity += impulse * massA;
    }
}

void PhysicsWorld::CheckAndResolveCollisionForPolygonDisk(BaseCollider& polygon, BaseCollider& disk)
{
    const PolygonCollider& aCollider = dynamic_cast<PolygonCollider&>(polygon);

    CollisionInfo collisionInfo(polygon,disk);

    const DiskCollider& bCollider = dynamic_cast<DiskCollider&>(disk);

    if (Collision::DiskIntersectsPolygon(disk.GetTransform().position + disk.offset, bCollider.radius, aCollider.transformed, &collisionInfo))
    {
   		HandleEnterOngoingCollisionEvents(polygon, disk, collisionInfo);
        HandleEnterOngoingCollisionEvents(disk, polygon, collisionInfo);
        ResolveCollision(*polygon.physicsBody, *disk.physicsBody, collisionInfo);
    }
	else
    {
        HandleCollisionTrackingLeaveEvent(polygon, disk);
        HandleCollisionTrackingLeaveEvent(disk, polygon);
    }
}

void PhysicsWorld::CheckAndResolveCollisionForDiskDisk(BaseCollider& disk1, BaseCollider& disk2)
{
    const DiskCollider& aCollider = dynamic_cast<DiskCollider&>(disk1);

    CollisionInfo collisionInfo(disk1,disk2);

    const DiskCollider& bCollider = dynamic_cast<DiskCollider&>(disk2);

    if (Collision::DisksIntersects(disk1.GetTransform().position + sf::Vector2f(aCollider.offset.x, aCollider.offset.y), aCollider.radius, disk2.GetTransform().position + sf::Vector2f(bCollider.offset.x, bCollider.offset.y), bCollider.radius, &collisionInfo))
    {
        HandleEnterOngoingCollisionEvents(disk1, disk2, collisionInfo);
        HandleEnterOngoingCollisionEvents(disk2, disk1, collisionInfo);
        ResolveCollision(*disk1.physicsBody, *disk2.physicsBody, collisionInfo);
    }
	else
    {
        HandleCollisionTrackingLeaveEvent(disk1, disk2);
        HandleCollisionTrackingLeaveEvent(disk2, disk1);
    }
}

void PhysicsWorld::CheckAndResolveCollisionForPolygonPolygon(BaseCollider& polygon1, BaseCollider& polygon2)
{
    const PolygonCollider& aCollider = dynamic_cast<PolygonCollider&>(polygon1);

    CollisionInfo collisionInfo(polygon1, polygon2);

    const PolygonCollider& bCollider = dynamic_cast<PolygonCollider&>(polygon2);

    if (Collision::PolygonsIntersect( aCollider.transformed,bCollider.transformed, &collisionInfo))
    {
        HandleEnterOngoingCollisionEvents(polygon1, polygon2, collisionInfo);
        HandleEnterOngoingCollisionEvents(polygon2, polygon1, collisionInfo);
        ResolveCollision(*polygon1.physicsBody, *polygon2.physicsBody, collisionInfo);
    }
	else
    {
        HandleCollisionTrackingLeaveEvent(polygon1, polygon2);
    }
}

void PhysicsWorld::HandleEnterOngoingCollisionEvents(const BaseCollider& trackingCollider,BaseCollider& collidingCollider, const CollisionInfo& collisionInfo)
{
	std::unordered_map<entity_id, CollisionEvent>& events = trackingCollider.physicsBody->collisionEvents;
	size_t id = collidingCollider.GetEntity().GetId();
    if (!events.contains(id))
    {
      events.insert(std::pair(id, CollisionEvent(CollisionEventType::Entering, collidingCollider, collisionInfo.depth, collisionInfo.normal)));
    }
}

void PhysicsWorld::HandleCollisionTrackingLeaveEvent(const BaseCollider& trackingCollider, const BaseCollider& collidingEntity)
{
    std::unordered_map<entity_id, CollisionEvent>& events = trackingCollider.physicsBody->collisionEvents;
    size_t id = collidingEntity.GetEntity().GetId();
    if (events.contains(id))
    {
        if (events.at(id).type == CollisionEventType::Entering)return;
        events.at(id).type = CollisionEventType::Leaving;
    }
}

void PhysicsWorld::HandleLeavingEventsForBroadPhase(const std::list<BroadPhaseCollisionPair>& broadPhasePairs) const
{
	for (auto& element : physicsBodies)
	{
		auto& events = element.get().collisionEvents;
        auto copyEvents = events;


        for (auto& pair : broadPhasePairs)
        {
            std::erase_if(copyEvents, [&pair](const std::pair<entity_id, CollisionEvent>& obj)
            {
                return obj.first == pair.colliders.first.get().GetEntity().GetId() || obj.first == pair.colliders.second.get().GetEntity().GetId();
            });
        }

        for (auto& leavingEvents : copyEvents)
        {
            events.at(leavingEvents.first).type = CollisionEventType::Leaving;
        }
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
	m_executedFrame = false;

    if (m_tick_timer >= m_timeThreshold)
    {
        bvhElements.clear();
        UpdateBvhElements();
        bvh.BuildBVH(bvhElements);
        IntegrateForces(m_timeThreshold);
        CheckAndResolveCollisions();
        m_tick_timer -= m_timeThreshold;

    	m_executedFrame = true;
    }

    if (!physicsBodies.empty())
    {
        for (size_t i = physicsBodies.size() - 1; i > 0; i--)
        {
            auto& body = physicsBodies[i];
            auto& physicsBody = body.get();

            if (physicsBody.GetEntity().active == false)continue;
            if (physicsBody.collisionEvents.empty())continue;
            physicsBody.HandleCollisionEvents();
        }
    }

    bvh.DebugDraw(bvh.GetRootIndex(),Engine.debugRenderer);
}

void PhysicsWorld::DetectCollisions()
{

}
