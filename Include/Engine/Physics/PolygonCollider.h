#pragma once
#include "Game/BaseCollider.h"

class PolygonCollider : public BaseCollider
{
public:
	explicit PolygonCollider(Entity& entityToSet): BaseCollider(entityToSet)
	{
		tickInEditor = true;
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	geometry2d::Polygon GetTransformedPolygon()const;

	SERIALIZE_FIELD(geometry2d::Polygon, polygon);

	geometry2d::Polygon transformed;
	geometry2d::Polygon oldTransformed;
	geometry2d::AABB AABB() override;
};
REGISTER_COMPONENT(PolygonCollider);

