#include "Engine/AI/NavMeshObstacle.h"

#include "Engine/Core/Component.hpp"
#include "Engine/Core/Engine.hpp"
#include "SFML/Graphics/Color.hpp"

void NavMeshObstacle::Start()
{
	Component::Start();
}

void NavMeshObstacle::Update(float deltaTime)
{
	Component::Update(deltaTime);
    if (polygon.size() <= 1)return;

	const auto transformed = GetPolygon();
    for (size_t i = 0; i < transformed.size(); i++)
    {
        Engine.debugRenderer.AddLine(DebugCategory::AINavigation, transformed[i], transformed[(i + 1) % transformed.size()], sf::Color::Red);
    }
}

geometry2d::Polygon NavMeshObstacle::GetPolygon()
{
    geometry2d::Polygon toReturn = polygon;

    Transform& transform = GetTransform();
    for (auto& pos : toReturn)
    {
        pos += transform.position;
        pos.x *= transform.scale.x;
        pos.y *= transform.scale.y;
    }

    return toReturn;
}
