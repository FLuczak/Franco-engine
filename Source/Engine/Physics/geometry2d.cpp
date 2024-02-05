

#include "Engine/Physics/geometry2d.hpp"

#include <cassert>

#include "SFML/SFMLMath.hpp"
#include "SFML/System/Vector2.hpp"

bool geometry2d::IsPointLeftOfLine(const sf::Vector2f& point, const sf::Vector2f& line1, const sf::Vector2f& line2)
{
    return (line2.x - line1.x) * (point.y - line1.y) - (line2.y - line1.y) * (point.x - line1.x) > 0;
}

bool geometry2d::DoSegmentsIntersect(sf::Vector2f one, sf::Vector2f two, sf::Vector2f three, sf::Vector2f four)
{
    const double denominator = ((two.x - one.x) * (four.y - three.y)) - ((two.y - one.y) * (four.x - three.x));
    // Check for parallel lines
    if (denominator == 0) return false;

    const double numerator1 = ((one.y - three.y) * (four.x - three.x)) - ((one.x - three.x) * (four.y - three.y));
    const double numerator2 = ((one.y - three.y) * (two.x - one.x)) - ((one.x - three.x) * (two.y - one.y));

    const double reverse_denominator = 1.0f / static_cast<float>(denominator);

    const double r = static_cast<float>(numerator1) * reverse_denominator;
    const double s = static_cast<float>(numerator2) * reverse_denominator;
    return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

bool geometry2d::IsPointRightOfLine(const sf::Vector2f& point, const sf::Vector2f& line1, const sf::Vector2f& line2)
{
    double p[2] = {point.x, point.y};
    double la[2] = {line1.x, line1.y};
    double lb[2] = {line2.x, line2.y};
    return true;
   // return RobustPredicates::orient2d(p, la, lb) < 0;
}

bool geometry2d::IsClockwise(const Polygon& polygon)
{
    size_t n = polygon.size();
    assert(n > 2);
    float signedArea = 0.f;

    for (size_t i = 0; i < n; ++i)
    {
        const auto& p0 = polygon[i];
        const auto& p1 = polygon[(i + 1) % n];

        signedArea += (p0.x * p1.y - p1.x * p0.y);
    }

    // Technically we now have 2 * the signed area.
    // But for the "is clockwise" check, we only care about the sign of this number,
    // so there is no need to divide by 2.
    return signedArea < 0.f;
}

bool geometry2d::IsPointInsidePolygon(const sf::Vector2f& point, const Polygon& polygon)
{
    // Adapted from: https://wrfranklin.org/Research/Short_Notes/pnpoly.html

    size_t i, j;
    size_t n = polygon.size();
    bool inside = false;

    for (i = 0, j = n - 1; i < n; j = i++)
    {
        if ((polygon[i].y > point.y != polygon[j].y > point.y) &&
            (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
            inside = !inside;
    }

    return inside;
}

sf::Vector2f geometry2d::GetNearestPointOnLineSegment(const sf::Vector2f& p, const sf::Vector2f& segmentA, const sf::Vector2f& segmentB)
{
    float t = dot(p - segmentA, segmentB - segmentA) / sf::distance2(segmentA, segmentB);
    if (t <= 0) return segmentA;
    if (t >= 1) return segmentB;
    return (1 - t) * segmentA + t * segmentB;
}

bool geometry2d::DoesSegmentIntersectDisk(sf::Vector2f start, sf::Vector2f end, sf::Vector2f center, float radius)
{
    // Calculate the direction vector of the line segment
    const float dx = end.x - start.x;
    const float dy = end.y - start.y;

    // Calculate the vector from one endpoint of the segment to the circle's center
    const float a = center.x - start.x;
    const float b = center.y - start.y;

    // Calculate the squared length of the line segment
    const float segmentLengthSquared = dx * dx + dy * dy;

    // Calculate the dot product of the direction vector and the vector to the circle's center
    const float dotProduct = a * dx + b * dy;

    // Calculate the closest point on the line segment to the circle's center
    float closestX, closestY;
    if (dotProduct <= 0)
    {
        closestX = start.x;
        closestY = start.y;
    }
    else if (dotProduct >= segmentLengthSquared)
    {
        closestX = end.x;
        closestY = end.y;
    }
    else
    {
        closestX = start.x + (dotProduct / segmentLengthSquared) * dx;
        closestY = start.y + (dotProduct / segmentLengthSquared) * dy;
    }

    // Calculate the distance between the closest point and the circle's center
    const float distanceSquared = (center.x - closestX) * (center.x - closestX) + (center.y - closestY) * (center.y - closestY);
    return distanceSquared <= (radius * radius);
}

sf::Vector2f geometry2d::GetLineIntersectionPoint(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
{
    // Line AB represented as a1x + b1y = c1
    const float a1 = B.y - A.y;
    const float b1 = A.x - B.x;
    const float c1 = a1 * (A.x) + b1 * (A.y);

    // line CD represented as a2x + b2y = c2
    const float a2 = D.y - C.y;
    const float b2 = C.x - D.x;
    const float c2 = a2 * (C.x) + b2 * (C.y);

    const float determinant = a1 * b2 - a2 * b1;

    if (determinant == 0)
    {
        return {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    }
    else
    {
        float x = (b2 * c1 - b1 * c2) / determinant;
        float y = (a1 * c2 - a2 * c1) / determinant;
        return {x, y};
    }
}

sf::Vector2f geometry2d::ComputeCenterOfPolygon(const Polygon& polygon)
{
    sf::Vector2f total(0,0);
    for (const sf::Vector2f& p : polygon) total += p;
    return total / static_cast<float>(polygon.size());
}
