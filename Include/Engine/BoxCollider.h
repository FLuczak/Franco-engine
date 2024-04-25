#pragma once
#include "BaseCollider.h"
#include "Engine/Physics/PolygonCollider.h"
#include "SFML/SFMLMath.hpp"


class BoxCollider : public PolygonCollider
{
public:
	SERIALIZE_FIELD(sf::Vector2f, extents);
	BoxCollider(Entity& entityToSet): PolygonCollider(entityToSet),extents({0,0})
	{
		colliderShape = ColliderShape::POLYGON;
		tickInEditor = true;
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;

private:
	sf::Vector2f oldExtents = {0,0};
};
REGISTER_COMPONENT(BoxCollider);
