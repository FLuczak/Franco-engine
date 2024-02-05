
#include "Engine/AI/NavMesh/Navigation.hpp"
#include <queue>
#include <unordered_map>
#include "Engine/AI/NavMesh/Graph.hpp"

/**
 \brief A method that while provided a start node from the graph, end node from the graph and a heuristic function (To chose from ZeroHeuristic- mostly used for non euclidean space graphs and DistanceHeuristic- mostly used for graphs representing euclidean spaces)
 * \param start - A Start node from the graph
 * \param goal - An end node from the graph
 * \param heuristicFunction - A provided heuristic function (ZeroHeuristic or DistanceHeuristic)
 * \return - A list of node pointers. If the path doesn't exist- the list is empty
 */
std::vector<const AI::Node*> AI::Navigation::FindPath(const AI::Node& start, const Node& goal,float (*heuristicFunction)(const Node&, const Node&))
{
    std::priority_queue<AStarNode, std::vector<AStarNode>, CompareNodes> openSet;
    std::unordered_map<const Node*,float> scoresMap = {};
    std::unordered_map<const Node*, const Node*> predecessors = {};

    AStarNode startAStarNode = AStarNode(&start);
    startAStarNode.SetHeuristic(heuristicFunction(start,goal));

    predecessors[&start] = nullptr;
    openSet.push(startAStarNode);
    scoresMap.insert(std::pair(&start, startAStarNode.g));

    while (!openSet.empty())
    {
        // Get the node with the lowest F score from the open set
        const AStarNode current = openSet.top();
        openSet.pop();

        if (current.graphNode->position == goal.position)
        {
            std::vector<const AI::Node*> path = RetracePath(current, predecessors);
            return path;
        }

        for (const Edge& edge : current.graphNode->edges)
        {
            Node* neighbor = edge.b;
            const float potentialG = current.g + edge.GetCost();

            if (scoresMap.find(neighbor) != scoresMap.end() && potentialG > scoresMap[neighbor]) continue;

            AStarNode aStarNode (neighbor);
            aStarNode.SetHeuristic(heuristicFunction(neighbor, goal));
            aStarNode.g = (potentialG);

            predecessors[aStarNode.graphNode] = current.graphNode;
            scoresMap[neighbor]=aStarNode.g;
            openSet.push(aStarNode);
        }
    }
    
    return {};
}

/**
 * \brief Used for the FindPath method. Returns an actual path from Astar search result
 * \param end - goal of the astar search
 * \param predecessors - map of predecessors created by the Astar search
 * \return - A valid path of nodes ready for use
 */
std::vector<const AI::Node*> AI::Navigation::RetracePath(const AStarNode& end,const std::unordered_map<const Node*, const Node*>& predecessors)
{
    std::vector<const AI::Node*> path;
    const Node* currentNode = end.graphNode;

    while (currentNode != nullptr)
    {
        path.push_back(currentNode);
        currentNode  = predecessors.find(currentNode)->second;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * \brief A method for drawing the provided path of nodes path inside the game world
 * \param renderer - a debug renderer instance destined to create the debug requests
 * \param path - a list of graph nodes
 */
void AI::Navigation::DebugDrawPath(DebugRenderer& renderer, const std::vector<const Node*>& path)
{
    if (path.empty())
    {
        return;
    }

    for (size_t i = 0; i < path.size() - 1; i++)
    {
        renderer.AddLine(DebugCategory::AINavigation, sf::Vector2f(path[i]->position.x, path[i]->position.y),
        sf::Vector2f(path[i+1]->position.x, path[i+1]->position.y),sf::Color::Blue);   
    }
}

void AI::Navigation::DebugDrawPath(DebugRenderer& renderer, const std::vector<sf::Vector2f>& path)
{
    if (path.empty())
    {
        return;
    }

    for (size_t i = 0; i < path.size() - 1; i++)
    {
        renderer.AddLine(DebugCategory::Gameplay, sf::Vector2f(path[i].x, path[i].y),sf::Vector2f(path[i + 1].x, path[i + 1].y), sf::Color::Cyan);
    }
}
