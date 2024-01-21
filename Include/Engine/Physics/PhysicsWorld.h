#pragma once
#include <vector>

#include "PhysicsBody.hpp"
#include "SFML/System/Vector2.hpp"

class PhysicsWorld
{
public:
	sf::Vector2f gravity = { 0,0 };
	void HandlePhysicsBodyAdded(PhysicsBody& toAdd);
	void HandlePhysicsBodyRemove(PhysicsBody& toRemove);
	PhysicsWorld();
	void Simulate(float dt);
	void DetectCollisions();
private:
	std::vector<std::reference_wrapper<PhysicsBody>> physicsBodies{};
};

