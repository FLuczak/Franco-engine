#pragma once
#include "geometry2d.hpp"
#include "SFML/System/Vector2.hpp"

namespace Physics
{

/**
 * \brief An object holding additional collision information, such as a collision normal
 * and its depth. Used for collision resolution
 */
struct CollisionInfo
{
    sf::Vector2f normal;
    float depth;
};

class Collision
{
public:
    static bool PolygonsIntersect(const geometry2d::Polygon& one, const geometry2d::Polygon& two, CollisionInfo* collisionInfo = nullptr);
    static bool DisksIntersects(sf::Vector2f oneCenter, float oneRadius, sf::Vector2f twoCenter, float twoRadius, CollisionInfo* collisionInfo = nullptr);
    static bool DiskIntersectsPolygon(sf::Vector2f diskCenter, const float radius, const geometry2d::Polygon& polygon, CollisionInfo* collisionInfo = nullptr);
    static bool AABBIntersect(geometry2d::AABB a, geometry2d::AABB b);
private:
    /**
     * \brief A struct for Separate axis theorem based collision detection with minimal and maximal projection values
     */
    struct AxisProjection
    {
        float min;
        float max;
    };

    static bool DiskIntersectsPolygonNormalDepth(sf::Vector2f diskCenter, float diskRadius, const geometry2d::Polygon& polygon, CollisionInfo& collisionInfo);
    static AxisProjection ProjectDisk(const sf::Vector2f diskCenter, const float radius, sf::Vector2f axis);
    static bool CheckSATIntersection(const geometry2d::Polygon& polygon, const geometry2d::Polygon& other,
    CollisionInfo* collisionInfo = nullptr);
    static AxisProjection ProjectVertices(const geometry2d::Polygon& polygon, sf::Vector2f axis);
};

}  // namespace Physics
