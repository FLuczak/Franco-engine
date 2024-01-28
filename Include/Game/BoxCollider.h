#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
public:
	BoxCollider(Entity& entityToSet): BaseCollider(entityToSet)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
};
REGISTER_COMPONENT(BoxCollider);
