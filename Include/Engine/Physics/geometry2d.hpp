#pragma once

#include <vector>
#include "SFML/System/Vector2.hpp"
#include "Visit struct/visit_struct.hpp"
/// <summary>
/// A namespace containing functions related to 2D geometry.
/// </summary>


namespace geometry2d
{
    // Input data types
    using Polygon = std::vector<sf::Vector2f>;
    using PolygonList = std::vector<Polygon>;

    /// <summary>
    /// Simple struct representing an axis-aligned bounding box.
    /// </summary>
    struct AABB
    {
    private:
        sf::Vector2f m_min;
        sf::Vector2f m_max;

    public:
        AABB(const sf::Vector2f& minPos, const sf::Vector2f& maxPos) : m_min(minPos), m_max(maxPos)
        {

        }

        /// <summary>
        /// Computes and returns the four boundary vertices of this AABB.
        /// </summary>
        Polygon ComputeBoundary() const { return { m_min, sf::Vector2f(m_max.x, m_min.y), m_max, sf::Vector2f(m_min.x, m_max.y) }; }

        /// <summary>
        /// Computes and returns the center coordinate of this AABB.
        /// </summary>
        sf::Vector2f ComputeCenter() const { return (m_min + m_max) / 2.f; }

        /// <summary>
        /// Computes and returns the size of this AABB, wrapped in a vec2 (x=width, y=height).
        /// </summary>
        /// <returns></returns>
        sf::Vector2f ComputeSize() const { return m_max - m_min; }

        const sf::Vector2f& GetMin() const { return m_min; }
        const sf::Vector2f& GetMax() const { return m_max; }
    };

    /// <summary>
    /// Checks and returns whether a 2D point lies strictly to the left of an infinite directed line.
    /// </summary>
    /// <param name="point">A query point.</param>
    /// <param name="line1">A x point on the query line.</param>
    /// <param name="line2">A y point on the query line.</param>
    /// <returns>true if "point" lies strictly to the left of the infinite directed line through line1 and line2;
    /// false otherwise (i.e. if the point lies on or to the right of the line).</return>
    bool IsPointLeftOfLine(const sf::Vector2f& point, const sf::Vector2f& line1, const sf::Vector2f& line2);


    ///<summary> A method to check line segment-line segment intersection. 
   ///</summary>
   /// <param name= "one"> - point one of the  line segment </param>
   /// <param name= "two"> - point two of the  line segment </param>
   ///  <param name= "three"> - point one of the other line segment </param>
   ///  <param name= "four"> - point two of the other line segment </param>
   ///  /// <returns>true if two line segments intersect. False if they do not intersect</return>
    bool DoSegmentsIntersect(sf::Vector2f one, sf::Vector2f two, sf::Vector2f three, sf::Vector2f four);

    /// <summary>
    /// Checks and returns whether a 2D point lies strictly to the right of an infinite directed line.
    /// </summary>
    /// <param name="point">A query point.</param>
    /// <param name="line1">A x point on the query line.</param>
    /// <param name="line2">A y point on the query line.</param>
    /// <returns>true if "point" lies strictly to the right of the infinite directed line through line1 and line2;
    /// false otherwise (i.e. if the point lies on or to the left of the line).</return>
    bool IsPointRightOfLine(const sf::Vector2f& point, const sf::Vector2f& line1, const sf::Vector2f& line2);

    /// <summary>
    /// Checks and returns whether the points of a simple 2D polygon are given in clockwise order.
    /// </summary>
    /// <param name="polygon">A list of 2D points describing the boundary of a simple polygon (i.e. at least 3 points, nonzero area,
    /// not self-intersecting).</param> <returns>true if the points are given in clockwise order, false otherwise.</returns>
    bool IsClockwise(const Polygon& polygon);

    /// <summary>
    /// Checks and returns whether a given point lies inside a given 2D polygon.
    /// </summary>
    /// <param name="point">A query point.</param>
    /// <param name="polygon">A simple 2D polygon.</param>
    /// <returns>true if the point lies inside the polygon, false otherwise.</return>
    bool IsPointInsidePolygon(const sf::Vector2f& point, const Polygon& polygon);

    /// <summary>
    /// Computes and returns the centroid of a given polygon (= the average of its boundary points).
    /// </summary>
    sf::Vector2f ComputeCenterOfPolygon(const geometry2d::Polygon& polygon);

    /// <summary>
    /// Computes and returns the nearest point on a line segment segmentA-segmentB to another point p.
    /// </summary>
    /// <param name="p">A query point.</param>
    /// <param name="segmentA">The x endpoint of a line segment.</param>
    /// <param name="segmentB">The y endpoint of a line segment.</param>
    /// <returns>The point on the line segment segmentA-segmentB that is closest to p.</returns>
    sf::Vector2f GetNearestPointOnLineSegment(const sf::Vector2f& p, const sf::Vector2f& segmentA, const sf::Vector2f& segmentB);

    bool DoesSegmentIntersectDisk(sf::Vector2f start, sf::Vector2f end, sf::Vector2f center, float radius);

    sf::Vector2f GetLineIntersectionPoint(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D);


}
