#pragma once
#include "BaseCollider.h"
#include "SFML/SFMLMath.hpp"


class BoxCollider : public BaseCollider
{
public:
	SERIALIZE_FIELD(float, extents);
	BoxCollider(Entity& entityToSet): BaseCollider(entityToSet),extents(0)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
};
REGISTER_COMPONENT(BoxCollider);
