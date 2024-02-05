#pragma once
#include "Graph.hpp"

namespace AI
{
class NavMeshPath
{
public:
    NavMeshPath() = default;
    NavMeshPath(std::vector<const AI::Node*>& nodesToSet);
    NavMeshPath(const std::vector<sf::Vector2f>& pointsToSet);

    //Given a point return a t value of how far along the path the given point is
    float GetPercentageAlongPath(const sf::Vector2f& point) const;
    //get an arbitrary closest point on the path to a given point 
    sf::Vector2f GetClosestPointOnPath(sf::Vector2f point) const;
    //Given a t value- return a point in this percentage along the path
    sf::Vector2f FindPointOnPath(float t) const;

    std::vector<sf::Vector2f>& GetPoints() { return points; }
    std::vector<const AI::Node*>& GetGraphNodes() { return aiNodes; }
    bool IsEmpty() const { return points.empty(); }

private:
    std::vector<sf::Vector2f> points;
    std::vector<const AI::Node*> aiNodes;
};
}

