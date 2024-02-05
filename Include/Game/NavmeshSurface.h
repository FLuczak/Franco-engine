#pragma once
#include "Engine/Component.hpp"
#include "Engine/EditorVariables.h"
#include "Engine/Physics/geometry2d.hpp"
#include "SFML/include/SFML/SFMLMath.hpp"

class NavmeshSurface : public Component
{
public:
	SERIALIZE_FIELD(sf::Vector2f, extents);
	SERIALIZE_FIELD(sf::Vector2f, center);

	explicit NavmeshSurface(Entity& entityToSet): Component(entityToSet)
	{
		tickInEditor = true;
	}

	geometry2d::Polygon GetPolygon();
	void Start() override;
	void Update(float deltaTime) override;
};
REGISTER_COMPONENT(NavmeshSurface);

