#pragma once
#include "Game/BaseCollider.h"

class DiskCollider : public BaseCollider
{
public:
	SERIALIZE_FIELD(float, radius);
	explicit DiskCollider(Entity& entityToSet): BaseCollider(entityToSet), radius(0)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	geometry2d::AABB AABB() override;
};
REGISTER_COMPONENT(DiskCollider);

