#include "Engine/Physics/PhysicsUtility.hpp"

#include "SFML/SFMLMath.hpp"

/**
 * \brief A function that given an arbitrary point returns the closest vertex of the polygon to the point.
 * \param point - an arbitrary point in the world
 * \param polygon - a vector of vec2s - vertices of the polygon
 * \return - a vertex closest to the point
 */
sf::Vector2f Physics::PhysicsUtility::FindClosestPolygonVertexToPoint(sf::Vector2f point,  const geometry2d::Polygon& polygon)
{
    int result = -1;
    float minDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < polygon.size(); i++)
    {
        const float distance = sf::distance(polygon[i], point);
        if (distance < minDistance)
        {
            minDistance = distance;
            result = i;
        }
    }

    return polygon[result];
}
