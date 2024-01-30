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
        leftTop.x =GetTransform().position.x -extents.x / 2.0f;
        leftTop.y =GetTransform().position.y +extents.y / 2.0f;
        polygon.push_back(leftTop);

        // Right Top
        sf::Vector2f rightTop;
        rightTop.x =GetTransform().position.x +extents.x / 2.0f;
        rightTop.y =GetTransform().position.y +extents.y / 2.0f;
        polygon.push_back(rightTop);

        // Right Bottom
        sf::Vector2f rightBottom;
        rightBottom.x =GetTransform().position.x +extents.x / 2.0f;
        rightBottom.y =GetTransform().position.y -extents.y / 2.0f;
        polygon.push_back(rightBottom);

        // Left Bottom
        sf::Vector2f leftBottom;
        leftBottom.x =GetTransform().position.x -extents.x / 2.0f;
        leftBottom.y =GetTransform().position.y -extents.y / 2.0f;
        polygon.push_back(leftBottom);
	}
}

void BoxCollider::OnDestroy()
{
	BaseCollider::OnDestroy();
}
