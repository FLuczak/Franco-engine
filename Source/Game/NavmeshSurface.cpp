#include "Game/NavmeshSurface.h"

#include "Engine/Engine.hpp"

geometry2d::Polygon NavmeshSurface::GetPolygon()
{
	sf::Vector2 topLeft = sf::Vector2(center.x - extents.x / 2, center.y + extents.y / 2);
	sf::Vector2 topRight = sf::Vector2(center.x + extents.x / 2, center.y + extents.y / 2);
	sf::Vector2 bottomLeft = sf::Vector2(center.x - extents.x / 2, center.y - extents.y / 2);
	sf::Vector2 bottomRight = sf::Vector2(center.x + extents.x / 2, center.y - extents.y / 2);

	return geometry2d::Polygon({ topLeft,bottomLeft,bottomRight,topRight,topLeft });
}

void NavmeshSurface::Start()
{
	Component::Start();
	geometry2d::Polygon floor = GetPolygon();
	Engine.navMesh = AI::NavMesh(0.75f, { floor }, {});
}

void NavmeshSurface::Update(float deltaTime)
{
	Component::Update(deltaTime);
	sf::Vector2 topLeft =  sf::Vector2(center.x - extents.x / 2, center.y + extents.y / 2);
	sf::Vector2 topRight =  sf::Vector2(center.x + extents.x / 2, center.y + extents.y / 2);
	sf::Vector2 bottomLeft =  sf::Vector2(center.x - extents.x / 2, center.y - extents.y / 2);
	sf::Vector2 bottomRight =  sf::Vector2(center.x + extents.x / 2, center.y - extents.y / 2);

	Engine.debugRenderer.AddLine(DebugCategory::AINavigation,topLeft, topRight, sf::Color::Magenta);
	Engine.debugRenderer.AddLine(DebugCategory::AINavigation, topRight, bottomRight, sf::Color::Magenta);
	Engine.debugRenderer.AddLine(DebugCategory::AINavigation, bottomRight, bottomLeft, sf::Color::Magenta);
	Engine.debugRenderer.AddLine(DebugCategory::AINavigation, bottomLeft, topLeft, sf::Color::Magenta);
}
