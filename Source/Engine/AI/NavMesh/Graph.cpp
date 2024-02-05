#include "Engine/AI/NavMesh/Graph.hpp"

#include "Engine/DebugRenderer.hpp"
#include "SFML/SFMLMath.hpp"

AI::Node::Node(Node* node)
{
    position = node->position;
}

AI::Node::Node(const sf::Vector2f& pos)
{
    position = pos;
}

AI::Edge::Edge(Edge* edge)
{
    a = edge->a;
    b = edge->b;
    cost = edge->GetCost();
}

AI::Edge::Edge(Node& nodeA, Node& nodeB, float newCost)
{
    a = &nodeA;
    b = &nodeB;
    cost = newCost;
}

AI::Graph::Graph()
{

}

/**
 * \brief Constructor for creating a graph out of only values- takes in a vector of node positions
 *  and a tuple of node indices in the nodeData array + float
 * \param nodeData - vector of node positions- each separate entry will be a separate node of the graph
 * \param edgeData - a tuple of 3 entries- index of A node index of the edge, B node index of the edge and a weight
 */
AI::Graph::Graph(const std::vector<sf::Vector2f>& nodeData, const std::vector<EdgeData>& edgeData)
{
    for (sf::Vector2f potentialNode : nodeData)
    {
        nodes.emplace_back(potentialNode);
    }

    for (const EdgeData& potentialEdge : edgeData)
    {
        nodes[potentialEdge.nodeIndexOne].edges.emplace_back(nodes[potentialEdge.nodeIndexOne],nodes[potentialEdge.nodeIndexTwo], potentialEdge.cost);
    }
}

/**
 * \brief Constructor for preconstructed nodes and edges
 * \param nodesToSet - a vector of nodes that is going to be assinged to graph nodes
 * \param edgesToSet - a vector of pre-constructed edges that is going to be assigned to graph edges
 */
AI::Graph::Graph(const std::vector<Node>& nodesToSet, const std::vector<Edge>& edgesToSet)
{
    for (Node node : nodesToSet)
    {
        nodes.emplace_back(&node);
        for (Edge const& edge : edgesToSet)
        {
            if (edge.a != &node) continue;
            node.edges.emplace_back(edge);
        }
    }
}

/**
 * \brief A graph constructor where a vector of preconstructed nodes is provided and and a tuple of node indices in the
 * \nodeData array + float for edge weight
 * \param nodesToSet - a vector of nodes that is going to be assinged to graph nodes
 * \param edgeData -  A vector of edge data structures with 2 indices and a cost
 */
AI::Graph::Graph(const std::vector<Node>& nodesToSet, const std::vector<EdgeData>& edgeData)
{
    for (const Node& node : nodesToSet)
    {
        nodes.push_back(node);
    }

    if (nodesToSet.empty()) return;
    for (const EdgeData& potentialEdge : edgeData)
    {
        nodes[potentialEdge.nodeIndexOne].edges.emplace_back(nodes[potentialEdge.nodeIndexOne],nodes[potentialEdge.nodeIndexTwo],potentialEdge.cost);
    }
}

/**
 * \brief A method for initializing weights in accordance to the distance between node vector2 positions
 * It can be used during initialization of euclidean space graphs without provided edge weights 
 */
void AI::Graph::InitializeEuclideanWeights() 
{
    if (nodes.empty()) return;
    for (Node& node : nodes)
    {
        for (Edge& edge :node.edges)
        {
            edge.SetCost(sf::getLength(edge.a->position - edge.b->position));
        }
    }
}

/**
 * \brief A method for drawing the graph inside the game world
 * \param debugRenderer - a debug renderer instance destined to create the
 * debug requests
 */
void AI::Graph::DebugDrawGraph(DebugRenderer& debugRenderer) const
{
    for (const Node& node : nodes)
    {
        debugRenderer.AddCircle(DebugCategory::AINavigation, sf::Vector2f(node.position.x, node.position.y), 0.25f,sf::Color::Red);

        for (const Edge& edge : node.edges)
        {
            sf::Vector2f aPos = sf::Vector2f(edge.a->position.x, edge.a->position.y);
            sf::Vector2f bPos = sf::Vector2f(edge.b->position.x, edge.b->position.y);
            debugRenderer.AddLine(DebugCategory::AINavigation, aPos, bPos,sf::Color::Green);
        }
    }
}

/**
 * \brief A function that pushes nodes to the graph after it was constructed
 * \param nodesToAdd - a vector of nodes that are going to be added to the graph
 */
void AI::Graph::AddNodes(const std::vector<Node>& nodesToAdd)
{
    for (const Node& node : nodesToAdd)
    {
        nodes.push_back(node);
    }
}

/**
 * \brief A function that adds nodes and assigns edges to them
 * \param nodesToAdd - a vector of nodes to add to the graph
 * \param edgesToSet - Edges that are going to be assigned to the nodes
 */
void AI::Graph::AddNodesAndEdges(const std::vector<Node>& nodesToAdd, const std::vector<Edge>& edgesToSet)
{
    for (Node node : nodesToAdd)
    {
        nodes.emplace_back(&node);
        for (Edge const& edge : edgesToSet)
        {
            if (edge.a != &node) continue;
            node.edges.emplace_back(edge);
        }
    }
}

/**
 * \brief A function that adds nodes and assigns edges to them
 * \param nodesToAdd - A vector of nodes to add
 * \param edgesToSet - A vector of edge data that will be later turned into edges and assigned to nodes
 */
void AI::Graph::AddNodesAndEdges(const std::vector<Node>& nodesToAdd, const std::vector<EdgeData>& edgesToSet)
{
    for (const Node& node : nodesToAdd)
    {
        nodes.push_back(node);
    }

    if (nodesToAdd.empty()) return;
    for (const EdgeData& potentialEdge : edgesToSet)
    {
        nodes[potentialEdge.nodeIndexOne].edges.emplace_back(nodes[potentialEdge.nodeIndexOne],nodes[potentialEdge.nodeIndexTwo], potentialEdge.cost);
    }
}
