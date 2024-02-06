#include "Game/BoxCollider.h"
#include "Engine/Engine.hpp"

void BoxCollider::Start()
{
	BaseCollider::Start();
}

void BoxCollider::Update(float deltaTime)
{
	PolygonCollider::Update(deltaTime);

	if (oldExtents != extents)
	{
		polygon.clear();

        sf::Vector2f leftTop;
        leftTop.x = -1*extents.x / 2.0f;
        leftTop.y = extents.y / 2.0f;
        polygon.push_back(leftTop);

        // Right Top
        sf::Vector2f rightTop;
        rightTop.x = extents.x / 2.0f;
        rightTop.y = extents.y / 2.0f;
        polygon.push_back(rightTop);

        // Right Bottom
        sf::Vector2f rightBottom;
        rightBottom.x = extents.x / 2.0f;
        rightBottom.y = -1*extents.y / 2.0f;
        polygon.push_back(rightBottom);

        // Left Bottom
        sf::Vector2f leftBottom;
        leftBottom.x = -1*extents.x / 2.0f;
        leftBottom.y = -1*extents.y / 2.0f;
        polygon.push_back(leftBottom);
        oldExtents = extents;
	}
}

void BoxCollider::OnDestroy()
{
	BaseCollider::OnDestroy();
}
