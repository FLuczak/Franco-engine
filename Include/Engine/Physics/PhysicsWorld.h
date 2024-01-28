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
	void IntegrateForces(float dt) const;
	void Simulate(float dt);
	void DetectCollisions();
private:
	std::vector<std::reference_wrapper<PhysicsBody>> physicsBodies{};
	float m_timesPerSecond = 50.0f;
	float m_tick_timer = 0.0f;
	float m_timeThreshold = 0.0f;
	bool m_executedFrame = false;
};

