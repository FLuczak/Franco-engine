#pragma once

#pragma warning(push)

#include "Graph.hpp"
#include "NavMeshPath.hpp"
#include "cdt/Triangulation.h"
#include "clipper/include/clipper2/clipper.core.h"
#include "Engine/Physics/geometry2d.hpp"


namespace AI
{
    /**
     * \brief A class for handling navmesh sampling- it contains information if a sample was successful and
     * a potential node if it was. IMPORTANT: node is a nullptr if the sample has not hit a place on a navmesh
     */
    class NavMeshSample
    {
    public:
        NavMeshSample(bool didHit, const AI::Node* hitNode) : hit(didHit), node(hitNode){};
        bool DidHit() const { return hit; }
        const AI::Node* GetHitNode() const { return node; }
    private:
        bool hit = false;
        const AI::Node* node{};
    };

    /**
     * \brief Main nav mesh class. After it is initialized from given geometry and baked
     * path finding and navigation. 
     */
    class NavMesh
    {
    public:
    	NavMesh(float agentRadius);
        NavMesh(float agentRadiusToSet, const std::vector<geometry2d::Polygon>& floor,const std::vector<geometry2d::Polygon>& obstacles);

        //Getter function for the underlying graph
        const AI::Graph& GetGraph() { return graph; }
        //Getter function for if the navmesh was baked properly.
        bool IsBaked() const { return isBaked; }

        //A function that takes a path to the graph and creates paths for obstacles and walkable floors
        //Main initialization function- triangulates the mesh and generates the graph
        void Bake();
        //Draw the navmesh as well as the underlying graph
        void DebugDraw(DebugRenderer& debugRenderer) const;
        //Check if a given position is on navmesh- returned object is a navmeshsample
        NavMeshSample SamplePosition(const sf::Vector2f& position) const;

        //Find a path from a node to another node
        NavMeshPath FindPath(const AI::Node& start, const AI::Node& goal)const;
        //Find a path from a world position to another position
        NavMeshPath FindPath(sf::Vector2f startPos, sf::Vector2f endPos) const;
        //Returns a path after applying a funnel algorithm to it
        std::vector<sf::Vector2f> FunnelPath(NavMeshPath path) const;
        // Given a point returns the closest navmesh node point
        sf::Vector2f FindClosestPointOnNavMesh(sf::Vector2f point) const;
    private:
        /**
         * \brief a class representing a triangle in the navmesh: it has some extra data
         * for handling the graph and creates a bridge between CDT triangulation and Navigation api
         */
        struct Triangle
        {
            Triangle(const CDT::Triangle& triangle) : cdtTriangle(triangle){};
            std::array<sf::Vector2f, 3> vertices{};
            CDT::Triangle cdtTriangle;
            int nodeIndex = 0;
        };

        /**
         * \brief A struct representing a portal used for the funnel algorithm. It consists of the
         * left and right points of the portal.
         */
        struct Portal
        {
            Portal(sf::Vector2f right, sf::Vector2f left) : rightPoint(right), leftPoint(left){};
            sf::Vector2f rightPoint;
            sf::Vector2f leftPoint;
        };

        // BAKING:

        void RemoveHolesFromPaths(Clipper2Lib::PathsD& levelPaths);
        void BakeFloorIntoSummarizedPath(Clipper2Lib::PathsD& levelPaths);
        // Given a summarized level path- differentiate obstacles path from the floors
        void BakeObstaclesIntoSummarizedPath(std::vector<Clipper2Lib::PathsD>& levelPaths, float offset) const;
        // Separate floors divided by obstacles
        void HandleSeparatedFloorsByObstacles(Clipper2Lib::PathsD& path, std::vector<Clipper2Lib::PathsD>& levelPaths) const;

        // Find all portals along a path- used for he funnel algorithm
        std::vector<Portal> FindPortalsAlongPath(NavMeshPath& path) const;
        // Check if a point is on the left from a line
        static bool IsLeft(sf::Vector2f startPoint, sf::Vector2f endPoint, sf::Vector2f point);
        // TRIANGULATION:
        // collective triangulation method that calls all the methods below
        void TriangulateMesh(std::vector<Clipper2Lib::PathsD>& paths);
        // Add vertices to the triangulation from previously created paths for both obstacles and floors
        static void AddTriangulationVertices(CDT::Triangulation<double>& cdt, const Clipper2Lib::PathsD& levelPath);
        // Add edges to the triangulation from previously created paths for both obstacles and floors
        static void AddTriangulationEdges(CDT::Triangulation<double>& cdt, const Clipper2Lib::PathsD& levelPath);
        // Initialize the triangles vector from the mesh triangulation result
        void InitializeTriangles(const CDT::Triangulation<double>& cdt);
        // Initialize graph from the triangles vector
        void InitializeGraph();
        // Make a mesh from previously created triangles- used in debugging
        void CreateTrianglesMesh();

        //Get a centroid of a triangle
        static sf::Vector2f GetTriangleCentroid(const Triangle& triangle);
        //Method that returns a glm vector2 from a Cdt vector2
        static sf::Vector2f GlmFromCDT(const CDT::V2d<double>& cdtVec);

        static float SignedArea(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3);
        static bool PointInTriangle(const sf::Vector2f& pt, const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3);
        // Final mesh representing triangulated navmesh- final effect of baking
        Clipper2Lib::PathsD trianglesPath;
        // An underlying navmesh graph used for finding paths and navigation
        AI::Graph graph = AI::Graph();
        // All floors in the navmesh
        Clipper2Lib::PathsD floorPaths;
        // All obstacles in the navmesh
        Clipper2Lib::PathsD obstaclesPath;
        // Triangles
        std::vector<Triangle> triangles;
        // Is navmesh baked? If not, the pathfinding will not be performed
        bool isBaked = false;
        //How big is the agent- used for offseting the navmesh obstacles
        float agentRadius = 0.0f;
    };
}  // namespace Navigation
