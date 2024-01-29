#include "Engine/Physics/Collision.hpp"

#include "Engine/Physics/PhysicsUtility.hpp"
#include "SFML/SFMLMath.hpp"


/**
 * \brief A function that checks if 2 disks intersect
 * \param oneCenter - disk one center
 * \param oneRadius - disk one radius
 * \param twoCenter - disk two center
 * \param twoRadius - disk two radius
 * \param collisionInfo - A pointer to collision struct to receive additional collision information such as collision depth
 * or its normal. Can be a nullptr.
 * \return - a bool if two disks of given centers and radii intersect
 */
bool Collision::DisksIntersects(const sf::Vector2f oneCenter, const float oneRadius, const sf::Vector2f twoCenter, const float twoRadius, CollisionInfo* collisionInfo)
{
    const float distance = sf::distance(oneCenter, twoCenter);

    if (distance > twoRadius + oneRadius)
    {
        return false;
    }

    if (collisionInfo != nullptr)
    {
        if (distance != 0.0f)
        {
        	sf::Vector2f toCenter = twoCenter - oneCenter;
            collisionInfo->normal = sf::normalize(toCenter);
            collisionInfo->depth = (oneRadius + twoRadius) - distance;
        }
        else
        {
            collisionInfo->normal = {0, 0};
            collisionInfo->depth = (oneRadius + twoRadius) - distance;
        }
    }

    return true;
} 

/**
 * \brief A function that checks if polygon intersects a disk- it is extended by finding the collision normal and its depth used for the
 * collision resolution. Based on this tutorial: 
 * \param diskCenter - center of the disk
 * \param diskRadius - radius of the disk
 * \param polygon - a vector of vertices of a polygon
 * \param collisionInfo - collision info reference
 * \return - whether or not the polygon collides with the disk
 */
bool Collision::DiskIntersectsPolygonNormalDepth(sf::Vector2f diskCenter, const float diskRadius, const geometry2d::Polygon& polygon, CollisionInfo& collisionInfo)
{
    collisionInfo.normal = {0, 0};
    collisionInfo.depth = std::numeric_limits<float>::max();

    sf::Vector2f axis = {0, 0};
    float axisDepth = 0.0f;

    for (int i = 0; i < polygon.size(); i++)
    {
        const sf::Vector2f vertexOne = polygon[i];
        const sf::Vector2f vertexTwo = polygon[(i + 1) % polygon.size()];

        const sf::Vector2f edge = vertexTwo - vertexOne;
        //create axis perpendicular to the edge

    	axis = sf::Vector2f(-edge.y, edge.x);
        axis = sf::normalize(axis);

        const AxisProjection polygonProjection = ProjectVertices(polygon, axis);
        const AxisProjection  circleProjection = ProjectDisk(diskCenter, diskRadius,axis);

        // Check for overlap on the axis, if no overlap, return false (no collision).
        if (polygonProjection.min >= circleProjection.max || circleProjection.min >= polygonProjection.max)
        {
            return false;
        }

        axisDepth = std::min(circleProjection.max - polygonProjection.min, polygonProjection.max - circleProjection.min);
        if (axisDepth < collisionInfo.depth)
        {
            collisionInfo.depth = axisDepth;
            collisionInfo.normal = axis;
        }
    }

    //Handle the case where the center is inside the polygon or very close to it
    {
        const sf::Vector2f closestPoint = Physics::PhysicsUtility::FindClosestPolygonVertexToPoint(diskCenter, polygon);

        axis = closestPoint - diskCenter;
        axis = sf::normalize(axis);

        const AxisProjection polygonProjection = ProjectVertices(polygon, axis);
        const AxisProjection circleProjection = ProjectDisk(diskCenter, diskRadius, axis);

        if (polygonProjection.min >= circleProjection.max || circleProjection.min >= polygonProjection.max)
        {
            return false;
        }

        axisDepth = std::min(circleProjection.max - polygonProjection.min, polygonProjection.max - circleProjection.min);

        if (axisDepth < collisionInfo.depth)
        {
            collisionInfo.depth = axisDepth;
            collisionInfo.normal = axis;
        }

        const sf::Vector2f direction = diskCenter - geometry2d::ComputeCenterOfPolygon(polygon);

        if (sf::dot(collisionInfo.normal, direction) < 0.0f)
        {
            collisionInfo.normal = -collisionInfo.normal;
        }
    }

    return true;
}

/**
 * \brief A function that checks if a disk intersects with a polygon
 * \param diskCenter - a disk collider
 * \param radius - radius of the disk
 * \param polygon - a polygon collider
 * \param collisionInfo - A pointer to collision struct to receive additional collision information such as collision depth
 * or its normal. Can be a nullptr.
 * \return - a bool whether a disk intersects with polygon
 */
bool Collision::DiskIntersectsPolygon(sf::Vector2f diskCenter, const float radius, const geometry2d::Polygon& polygon, CollisionInfo* collisionInfo)
{
    const bool centerInside = geometry2d::IsPointInsidePolygon(diskCenter, polygon);

    if (collisionInfo == nullptr)
    {
        for (size_t i = 0; i < polygon.size(); i++)
        {
            const sf::Vector2f edgeStart = polygon[i];
            const sf::Vector2f edgeEnd = polygon[(i + 1) % polygon.size()];

            if (geometry2d::DoesSegmentIntersectDisk(edgeStart, edgeEnd, diskCenter, radius))
            {
                return true;
            }
        }
    }
    else
    {
        return DiskIntersectsPolygonNormalDepth(diskCenter,radius,polygon, *collisionInfo);
    }

    return centerInside;
}

bool Collision::AABBIntersect(geometry2d::AABB a, geometry2d::AABB b)
{
    return !(a.GetMax().x < b.GetMin().x || a.GetMax().y < b.GetMin().y || a.GetMin().x > b.GetMax().x ||a.GetMin().y > b.GetMax().y);
}

/**
 * \brief Projects a disk onto a given axis. Returns an axis projection object for separate axis theory collision check
 * \param diskCenter - A position of the disk
 * \param radius - radius of the disk
 * \param axis - An axis the disk is projected on
 * \return - Axis projection of the disk onto the axis
 */
AxisProjection Collision::ProjectDisk(const sf::Vector2f diskCenter, const float radius, sf::Vector2f axis)
{
    const sf::Vector2f directionAndRadius = sf::normalize(axis)*radius;
    const sf::Vector2f p1 = diskCenter + directionAndRadius;
    const sf::Vector2f p2 = diskCenter - directionAndRadius;

    float min = sf::dot(p1, axis);
    float max = sf::dot(p2, axis);

    if (min > max)
    {
        const float t = min;
        min = max;
        max = t;
    }

    return { min, max };
}

/**
 * \brief A function that checks 2 polygons intersection based on separated axis theroem
 * function based on this implementation: https://www.youtube.com/watch?v=Zgf1DYrmSnk
 * \param polygon - a geometry2d::polygon (a vector of sf::Vector2fs)
 * \param other - a geometry2d::polygon (a vector of sf::Vector2fs)
 * \param collisionInfo - A pointer to collision struct to receive additional collision information such as collision depth
 * or its normal. Can be a nullptr.
 * \return - a bool whether or not these 2 polygons intersect
 */
bool Collision::CheckSATIntersection(const geometry2d::Polygon& polygon, const geometry2d::Polygon& other,CollisionInfo* collisionInfo)
{
    sf::Vector2f normal = sf::Vector2f(0, 0);
    float depth = std::numeric_limits<float>::max();

    for (size_t  i = 0 ; i < polygon.size();i++)
    {
        sf::Vector2f va = polygon[i];
        sf::Vector2f vb = polygon[(i+1)%polygon.size()];

        const sf::Vector2f edge = vb - va;
    	sf::Vector2f axis = sf::Vector2f(-edge.y,edge.x);
        axis = sf::normalize(axis);

        const AxisProjection projectionOne = ProjectVertices(polygon, axis);
        const AxisProjection projectionTwo = ProjectVertices(other, axis);

        if (projectionOne.min >= projectionTwo.max || projectionTwo.min >= projectionOne.max)
        {
            return false;
        }

        if (collisionInfo != nullptr)
        {
            const float axisDepth = std::min(projectionTwo.max - projectionOne.min, projectionOne.max - projectionTwo.min);

            if (axisDepth < depth)
            {
                depth = axisDepth;
                normal = axis;
            }
        }
    }

    for (size_t i = 0; i < other.size(); i++)
    {
        sf::Vector2f va = other[i];
        sf::Vector2f vb = other[(i + 1) % other.size()];

        const sf::Vector2f edge = vb - va;
    	sf::Vector2f axis =sf::Vector2f(-edge.y, edge.x);
        axis = sf::normalize(axis);

        const AxisProjection projectionOne = ProjectVertices(polygon, axis);
        const AxisProjection projectionTwo = ProjectVertices(other, axis);

        if (projectionOne.min >= projectionTwo.max || projectionTwo.min >= projectionOne.max)
        {
            return false;
        }

        if (collisionInfo != nullptr)
        {
            const float axisDepth = std::min(projectionTwo.max - projectionOne.min, projectionOne.max - projectionTwo.min);

            if (axisDepth < depth)
            {
                depth = axisDepth;
                normal = axis;
            }
        }
    }

    sf::Vector2f centerA = geometry2d::ComputeCenterOfPolygon(polygon);
    sf::Vector2f centerB = geometry2d::ComputeCenterOfPolygon(other);
    sf::Vector2f direction = centerA - centerB;
    if (sf::dot(direction, normal) < 0.0f)
    {
        normal *= -1.0f;
    }

    if (collisionInfo != nullptr)
    {
        collisionInfo->normal = normal;
        collisionInfo->depth = depth;
    }

    return true;
}

/**
 * \brief A function that checks if two polygons intersect
 * \param one - A vector of vertices of a polygon
 * \param two - A vector of vertices of another polygon
 * \param collisionInfo - A pointer to collision struct to receive additional collision information such as collision depth
 * or its normal. Can be a nullptr.
 * \return - A bool whether or not these 2 polygons intersect
 */
bool Collision::PolygonsIntersect(const geometry2d::Polygon& one, const geometry2d::Polygon& two, CollisionInfo* collisionInfo)
{
    if (!CheckSATIntersection(two, one, collisionInfo))
    {
        return false;
    }
    return true;
}

/**
 * \brief A projection function used for separated axis theorem intersection check
 *https://code.tutsplus.com/collision-detection-using-the-separating-axis-theorem--gamedev-169t
 * \param polygon - a geometry2d::Polygon (a vector of vec2s)
 * \param axis - an axis that a polygon is being projected onto (a vec2)
 * \return - an axisprojection struct which is 
 */
AxisProjection Collision::ProjectVertices(const geometry2d::Polygon& polygon,const sf::Vector2f axis)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    for (const sf::Vector2f& vec : polygon)
    {
        const float projection = sf::dot(vec, axis);

        if (projection < min){min = projection;}
        if (projection > max){max = projection;}
    }
      
    return {min, max};
}
