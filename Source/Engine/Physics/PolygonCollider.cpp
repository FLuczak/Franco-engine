#include "Engine/Physics/PolygonCollider.h"

#include "Engine/Engine.hpp"
#include "Engine/Entity.hpp"

void PolygonCollider::Start()
{
	BaseCollider::Start();
}

void PolygonCollider::Update(float deltaTime)
{
	BaseCollider::Update(deltaTime);

    auto transformed = GetTransformedPolygon();
    if (transformed.size() <= 1)return;
    for(int i= 0 ; i < transformed.size();i++)
    {
        Engine.debugRenderer.AddLine(DebugCategory::Physics, transformed[i] , transformed[(i + 1)%transformed.size()], sf::Color::Red);
    }
}

void PolygonCollider::OnDestroy()
{
	BaseCollider::OnDestroy();
}

geometry2d::Polygon PolygonCollider::GetTransformedPolygon()const
{
    geometry2d::Polygon transformed{};

    sf::Vector2f translation = GetTransform().position;
    float rotation = GetTransform().rotation;
    sf::Vector2f scale = GetTransform().scale;

    for (const auto& vertex : polygon) 
    {
	    const sf::Vector2f translatedVertex = vertex + translation + sf::Vector2f(offset.x,offset.y);

	    const float rotatedX = translatedVertex.x * cos(rotation) - translatedVertex.y * sin(rotation);
	    const float rotatedY = translatedVertex.x * sin(rotation) + translatedVertex.y * cos(rotation);
	    const sf::Vector2f rotatedVertex(rotatedX, rotatedY);

        sf::Vector2f scaledVertex = sf::Vector2f(rotatedVertex.x * scale.x, rotatedVertex.y * scale.y);

        transformed.push_back(scaledVertex);
    }

    return transformed;
}

geometry2d::AABB PolygonCollider::AABB()
{
    sf::Vector2 min = sf::Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    sf::Vector2 max = sf::Vector2f(-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max());

    for (sf::Vector2 vec : GetTransformedPolygon())
    {
        min = sf::min(min, vec);
        max = sf::max(max, vec);
    }

    return geometry2d::AABB(min, max);
}
