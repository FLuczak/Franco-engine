#pragma once
#include "Engine/Component.hpp"
#include "Engine/EditorVariables.h"
#include "Engine/Physics/geometry2d.hpp"

class NavMeshObstacle : public Component
{
public:
	SERIALIZE_FIELD(geometry2d::Polygon, polygon);

	explicit NavMeshObstacle(Entity& entityToSet): Component(entityToSet)
	{
		tickInEditor = true;
	}

	void Start() override;
	void Update(float deltaTime) override;
	geometry2d::Polygon GetPolygon();
};
REGISTER_COMPONENT(NavMeshObstacle);
