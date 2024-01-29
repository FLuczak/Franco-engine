#pragma once
#include "geometry2d.hpp"
#include "SFML/System/Vector2.hpp"

class BaseCollider;
/**
 * \brief The type of collision event that could possibly happen
 * entering- for if the collision just began
 * colliding- for an ongoing collision
 * leaving -for an collision that is ending this frame
 */
enum class CollisionEventType
{
    Entering,
    Colliding,
    Leaving
};

struct CollisionEvent
{
    CollisionEvent(CollisionEventType typeToSet,BaseCollider& colliderToSet, float depthToSet, sf::Vector2f normalToSet) : type(typeToSet), otherCollider(colliderToSet), depth(depthToSet), normal(normalToSet) {}
    CollisionEvent() = default;
    CollisionEventType type;
    BaseCollider& otherCollider;
    float depth;
    sf::Vector2f normal;
};

class BaseCollider;
/**
 * \brief An object holding additional collision information, such as a collision normal
 * and its depth. Used for collision resolution
 */
struct CollisionInfo
{
	CollisionInfo(BaseCollider& collider_a, BaseCollider& collider_b): colliderA(collider_a),colliderB(collider_b)
	{
	}

	BaseCollider& colliderA;
    BaseCollider& colliderB;
    sf::Vector2f normal = {};
    float depth = 0.0f;
};

/**
 * \brief A struct for Separate axis theorem based collision detection with minimal and maximal projection values
 */
struct AxisProjection
{
    float min;
    float max;
};

class Collision
{
public:
    static bool PolygonsIntersect(const geometry2d::Polygon& one, const geometry2d::Polygon& two, CollisionInfo* collisionInfo = nullptr);
    static bool DisksIntersects(sf::Vector2f oneCenter, float oneRadius, sf::Vector2f twoCenter, float twoRadius, CollisionInfo* collisionInfo = nullptr);
    static bool DiskIntersectsPolygon(sf::Vector2f diskCenter, const float radius, const geometry2d::Polygon& polygon, CollisionInfo* collisionInfo = nullptr);
    static bool AABBIntersect(geometry2d::AABB a, geometry2d::AABB b);
private:
    static bool DiskIntersectsPolygonNormalDepth(sf::Vector2f diskCenter, float diskRadius, const geometry2d::Polygon& polygon, CollisionInfo& collisionInfo);
    static AxisProjection ProjectDisk(const sf::Vector2f diskCenter, const float radius, sf::Vector2f axis);
    static bool CheckSATIntersection(const geometry2d::Polygon& polygon, const geometry2d::Polygon& other,
    CollisionInfo* collisionInfo = nullptr);
    static AxisProjection ProjectVertices(const geometry2d::Polygon& polygon, sf::Vector2f axis);
};
