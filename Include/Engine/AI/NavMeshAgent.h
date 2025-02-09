#pragma once
#include "Engine/AI/NavMesh/NavMeshPath.hpp"
#include "Engine/Editor/EditorVariables.h"

class NavMeshAgent : public Component
{
public:
	SERIALIZE_FIELD(float, movementSpeed);

	explicit NavMeshAgent(Entity& entityToSet): Component(entityToSet), movementSpeed(0)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void SetDestination(sf::Vector2f dest);
private:
	AI::NavMeshPath path;
	sf::Vector2f attractionPoint;
	sf::Vector2f preferredVelocity;
};
REGISTER_COMPONENT(NavMeshAgent);