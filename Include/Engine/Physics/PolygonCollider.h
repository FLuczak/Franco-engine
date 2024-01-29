#pragma once
#include "Game/BaseCollider.h"

class PolygonCollider : public BaseCollider
{
public:
	explicit PolygonCollider(Entity& entityToSet): BaseCollider(entityToSet)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	geometry2d::Polygon GetTransformedPolygon()const;

	geometry2d::Polygon polygon;
	geometry2d::AABB AABB() override;
};
REGISTER_COMPONENT(PolygonCollider);

