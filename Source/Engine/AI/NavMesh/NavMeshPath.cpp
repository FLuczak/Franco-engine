#include "Engine/AI/NavMesh/NavMeshPath.hpp"

#include <algorithm>

#include "Engine/Physics/geometry2d.hpp"
#include "SFML/SFMLMath.hpp"


AI::NavMeshPath::NavMeshPath(std::vector < const AI::Node*> &nodesToSet)
{
    for (const AI::Node* node :nodesToSet)
    {
        points.push_back(node->position);
    }

    aiNodes = nodesToSet;
}

AI::NavMeshPath::NavMeshPath(const std::vector<sf::Vector2f>& pointsToSet)
{
    points = pointsToSet;
}

/**
 * \brief Given a point on the path, calculate the float from 0-1 being a percentage
 * of how far along the path the point is
 * \param point - a point on the path
 * \return - a float in range 0-1 indicating how far along the path the point is
 */
float AI::NavMeshPath::GetPercentageAlongPath(const sf::Vector2f& point) const
{
    if (points.size() < 2) return 0.0f;

    float totalLength = 0.0f;
    std::vector<float> accumulatedLengths(points.size());

    for (size_t i = 0; i < points.size() - 1; i++)
    {
        sf::Vector2f delta = points[i + 1] - points[i];
        const float segmentLength = sf::getLength(delta);
        totalLength += segmentLength;
        accumulatedLengths[i + 1] = accumulatedLengths[i] + segmentLength;
    }

    int segmentIndex = 0;
    float closestDistance = std::numeric_limits<float>::max();

    //find the closest point to the given point
    for (int i = 0; i < points.size() - 1; i++)
    {
        auto closestPoint = geometry2d::GetNearestPointOnLineSegment(point, points[i + 1], points[i]);
        const float distance =  sf::distance(point, closestPoint);

        if (distance < closestDistance)
        {
            closestDistance = distance;
            segmentIndex = i;
        }
    }

    // Calculate the percentage along the closest segment.
    const float t = sf::dot(point - points[segmentIndex], points[segmentIndex + 1] - points[segmentIndex]) /sf::length2(points[segmentIndex + 1] - points[segmentIndex]);
    // Calculate the accumulated length up to the closest segment.
    const float accumulatedLengthUpToSegment = accumulatedLengths[segmentIndex] + t * (accumulatedLengths[segmentIndex + 1] - accumulatedLengths[segmentIndex]);
    // Calculate the total percentage along the entire path.
    const float totalT = accumulatedLengthUpToSegment / totalLength;
    return totalT;
}

/**
 * \brief Given a disconnected point from a path return the closest point
 * on the path to the given point
 * \param point - an arbitrary point
 * \return - the closest point on the path
 */
sf::Vector2f AI::NavMeshPath::GetClosestPointOnPath(sf::Vector2f point) const
{
    if (points.size() <= 1) return {};
    std::vector<sf::Vector2f> closestPoints = {};

    for (size_t i = 0 ; i < points.size()-1;i++)
    {
        closestPoints.push_back(geometry2d::GetNearestPointOnLineSegment(point, points[i], points[i + 1]));
    }

    std::sort(closestPoints.begin(), closestPoints.end(),[point](const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return sf::distance2(a, point) < sf::distance2(b, point);
    });

    return closestPoints[0];
}


/**
 * \brief Given a float t, return a point in the given percentage along the path
 * in a similar fasion to lerp functions: t being < 0 yields the start of the path,
 * t being > 1 yields the end of the path and every number between yields a point on the path
 * \param t - a float (preferably a percentage along the path)
 * \return - a point on the path
 */
sf::Vector2f AI::NavMeshPath::FindPointOnPath(float t) const
{
    if (points.empty()) return {0, 0};
    if (t <= 0.0f)
    {
        return points.front();  // Return the first point for position 0 or less
    }
    if (t >= 1.0f)
    {
        return points.back();  // Return the last point for position 1 or more
    }

    float totalLength = 0.0f;
    std::vector<float> segmentLengths;

    for (int i = 0; i < points.size() - 1; i++)
    {
        float segmentLength = sf::distance(points[i], points[i+1]);
        totalLength += segmentLength;
        segmentLengths.push_back(segmentLength);
    }

    const float targetLength = t * totalLength;

    // Find the segment containing the target length
    size_t segmentIndex = 0;
    float accumulatedLength = 0.0f;
    for (size_t i = 0; i < segmentLengths.size(); i++)
    {
        accumulatedLength += segmentLengths[segmentIndex];
        if (accumulatedLength >= targetLength)
        {
            break;
        }
        segmentIndex++;
    }

    if (segmentIndex >= points.size() - 1) return points.back();
    const float segmentT = (targetLength - accumulatedLength + segmentLengths[segmentIndex]) / segmentLengths[segmentIndex];
    return sf::lerp(points[segmentIndex], points[segmentIndex + 1], segmentT);
}










