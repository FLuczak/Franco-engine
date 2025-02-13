#pragma once
#include "Engine/Core/Component.hpp"
#include "Engine/Editor/EditorVariables.h"

class Bullet : public Component
{
public:
	SERIALIZE_FIELD(float,lifeTime)

	explicit Bullet(Entity& entityToSet,float timeToSet) : Component(entityToSet), lifeTime(timeToSet)
	{
		timer = lifeTime;
	}

	explicit Bullet(Entity& entityToSet): Component(entityToSet)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	void OnCollisionEnter(CollisionEvent& event) override;
	void OnCollisionStay(CollisionEvent& event) override;
	void OnCollisionLeave(CollisionEvent& event) override;
private:
	float timer = 0.0f;
};
REGISTER_COMPONENT(Bullet);

