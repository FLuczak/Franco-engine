#include "Engine/NavMeshAgent.h"
#include "Engine/Engine.hpp"
#include "Engine/AI/NavMesh/Navigation.hpp"

void NavMeshAgent::Start()
{
	Component::Start();
}

void NavMeshAgent::Update(float deltaTime)
{
	Component::Update(deltaTime);

    AI::Navigation::DebugDrawPath(Engine.debugRenderer, path.GetPoints());

    const sf::Vector2f agentPos2D = GetTransform().position;
    const sf::Vector2f referencePoint = path.GetClosestPointOnPath(agentPos2D);
    const float referencePointT =  path.GetPercentageAlongPath(referencePoint);
	attractionPoint = path.FindPointOnPath(referencePointT + 0.125f);

    Engine.debugRenderer.AddCircle(DebugCategory::AINavigation, attractionPoint, 30.0f, sf::Color::Red);

    if (!path.IsEmpty())
    {
        preferredVelocity = attractionPoint - agentPos2D;
        if(sf::distance2(attractionPoint,agentPos2D) >= 0.01f)
        {
            preferredVelocity = sf::normalize(preferredVelocity);
        }
    }

    if (referencePointT >= 0.995f)
    {
        path = AI::NavMeshPath();
        preferredVelocity = { 0, 0 };
    }

    GetTransform().position += preferredVelocity * deltaTime * movementSpeed;
}

void NavMeshAgent::SetDestination(sf::Vector2f dest)
{
	path = Engine.navMesh.FindPath(GetTransform().position, dest);
	path = Engine.navMesh.FunnelPath(path);
}
