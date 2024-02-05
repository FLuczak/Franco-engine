#pragma once
#include <vector>

#include "Engine/DebugRenderer.hpp"
#include "SFML/System/Vector2.hpp"

namespace AI
{
class Edge;
class Graph;
    class Node
    {
    public:
        Node(Node* node);
        Node(const sf::Vector2f& pos);
        sf::Vector2f position = sf::Vector2f(0, 0);
        std::vector<Edge> edges{};
    };

    class Edge
    {
    public:
        Edge(Edge* edge);
        Edge(Node& nodeA, Node& nodeB, float newCost = 1.0f);
        float GetCost() const { return cost; }
        void SetCost(float newCost) { cost = newCost; }

        Node* a;
        Node* b;
    private:
        float cost = 1.0f;
    };

    /**
     * \brief A struct for creating edges from node indices from the graph
     */
    struct EdgeData
    {
        EdgeData(int nodeIndOne, int nodeIndTwo, float newCost):nodeIndexOne(nodeIndOne),nodeIndexTwo(nodeIndTwo),cost(newCost){}
        int nodeIndexOne = 0;
        int nodeIndexTwo = 0;
        float cost = 1.0f;
    };

    class Graph
    {
    public:
        Graph();
        Graph(const std::vector<sf::Vector2f>& nodeData, const std::vector<EdgeData>& edgeData);
        Graph(const std::vector<Node>& nodesToSet, const std::vector<Edge>& edgesToSet);
        Graph(const std::vector<Node>& nodesToSet, const std::vector<EdgeData>& edgeData);

        void InitializeEuclideanWeights();
        void DebugDrawGraph(DebugRenderer& debugRenderer) const;

        void AddNodes(const std::vector<Node>& nodesToAdd);
        void AddNodesAndEdges(const std::vector<Node>& nodesToAdd, const std::vector<Edge>& edgesToSet);
        void AddNodesAndEdges(const std::vector<Node>& nodesToAdd, const std::vector<EdgeData>& edgesToSet);
        
        const std::vector<Node>& GetNodes()const  { return nodes; }
    private:
        std::vector<Node> nodes;
    };

}  // namespace AI
