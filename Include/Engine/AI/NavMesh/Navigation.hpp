#pragma once
#include <unordered_map>
#include "Graph.hpp"
#include "SFML/SFMLMath.hpp"

class DebugRenderer;

namespace AI
{

    class Navigation
    {
    public:
        //Heuristic for traversing graphs without euclidean distances between nodes
        static float ZeroHeuristic(const AI::Node& a, const AI::Node& b) { return 0.0f; };
        //Heuristic for traversing graphs with euclidean distances between nodes
        static float DistanceHeuristic(const AI::Node& a, const AI::Node& b) { return sf::getLength(a.position - b.position); };
        static std::vector<const AI::Node*> FindPath(const AI::Node& start, const AI::Node& goal,float (*heuristicFunction)(const AI::Node&, const AI::Node&));
        static void DebugDrawPath(DebugRenderer& renderer, const std::vector<const AI::Node*>& path);
        static void DebugDrawPath(DebugRenderer& renderer, const std::vector<sf::Vector2f>& path);
    private:
        /**
         * \brief A struct that is created during A* search. It contains
         * methods and variables that make A* search possible.
         */
        struct AStarNode
        {
            AStarNode(AI::Node* node) : graphNode(node) {}
            AStarNode(const AI::Node* node) : graphNode(node) {}

            float GetF() const { return g + h; }
            void SetG(float newG) { g = newG; }
            void SetHeuristic(const float newH) { h = newH; }

            const AI::Node* graphNode = nullptr;
            float g = 0;  // Cost from start node to this node
            float h = 0;  // Heuristic (estimated cost from this node to goal node)
        };

        /**
         * \brief A struct used in priority queue for comparison methods
         */
        struct CompareNodes
        {
            bool operator()(const AStarNode& one, const AStarNode& two) const { return one.GetF() > two.GetF(); }
        };

        static std::vector<const AI::Node*> RetracePath(const AStarNode& end,const std::unordered_map<const AI::Node*, const AI::Node*>&predecessors);
    };
}  // namespace AI