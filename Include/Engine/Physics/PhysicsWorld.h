#pragma once
#include <vector>

#include "BVH.hpp"
#include "Collision.hpp"
#include "PhysicsBody.hpp"
#include "Engine/Entity.hpp"
#include "SFML/System/Vector2.hpp"

struct BroadPhaseCollisionPair
{
	BroadPhaseCollisionPair(BaseCollider& a, BaseCollider& b, ColliderShape shapeA, ColliderShape shapeB)
	:colliders(std::pair(std::reference_wrapper(a), std::reference_wrapper(b))),
	shapes(std::pair(shapeA, shapeB))
	{

	}
	std::pair<std::reference_wrapper<BaseCollider>, std::reference_wrapper<BaseCollider>> colliders;
	std::pair<ColliderShape, ColliderShape> shapes{};
};

class PhysicsWorld
{
public:
	sf::Vector2f gravity = { 0,0 };
	void HandlePhysicsBodyAdded(PhysicsBody& toAdd);
	void HandlePhysicsBodyRemove(PhysicsBody& toRemove);
	PhysicsWorld();
	void IntegrateForces(float dt) const;
	void GetCollisionBroadPhaseBVH(unsigned index, const geometry2d::AABB& aabb, BaseCollider& collider, std::list<BroadPhaseCollisionPair>& list);
	std::list<BroadPhaseCollisionPair> GetBroadPhaseCollisionPairs();

	static void CheckAndResolveCollisionForPolygonDisk(BaseCollider& polygon, BaseCollider& disk);
	static void ResolveCollision(PhysicsBody& bodyA, PhysicsBody& bodyB, const CollisionInfo& collisionInfo);
	static void CheckAndResolveCollisionForDiskDisk(BaseCollider& disk1, BaseCollider& disk2);
	static void CheckAndResolveCollisionForPolygonPolygon(BaseCollider& polygon1, BaseCollider& polygon2);

	static void DetectAndResolveNarrowPhaseCollisions(const std::list<BroadPhaseCollisionPair>& list);
	void CheckAndResolveCollisions();
	void UpdateBvhElements();
	void Simulate(float dt);
	void DetectCollisions();

private:
	std::vector<Physics::BVH::BVHElement> bvhElements;
	std::vector<std::reference_wrapper<PhysicsBody>> physicsBodies{};
	float m_timesPerSecond = 50.0f;
	float m_tick_timer = 0.0f;
	float m_timeThreshold = 0.0f;
	bool m_executedFrame = false;
	Physics::BVH bvh;
};

