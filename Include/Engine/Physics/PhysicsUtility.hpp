#pragma once
#include "geometry2d.hpp"
#include "Engine/Core/Entity.hpp"

namespace Physics
{
class PhysicsUtility
{
	public:
	    static sf::Vector2f FindClosestPolygonVertexToPoint(sf::Vector2f point, const geometry2d::Polygon& polygon);
	};

}  // namespace Physics
