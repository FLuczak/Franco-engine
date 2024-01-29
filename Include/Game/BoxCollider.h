#pragma once
#include "BaseCollider.h"
#include "SFML/SFMLMath.hpp"

VISITABLE_STRUCT(sf::Vector2f, x, y);

class BoxCollider : public BaseCollider
{
public:
	SERIALIZE_FIELD(float, extents);
	SERIALIZE_FIELD(sf::Vector2f, offset);
	BoxCollider(Entity& entityToSet): BaseCollider(entityToSet),extents(0),offset({})
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
};
REGISTER_COMPONENT(BoxCollider);
