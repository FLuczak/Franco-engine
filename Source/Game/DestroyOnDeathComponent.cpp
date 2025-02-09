#include "Game/DestroyOnDeathComponent.h"

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Entity.hpp"
#include "Game/Health.h"

void DestroyOnDeathComponent::Start()
{
	Component::Start();
	health = GetEntity().GetComponent<Health>();
	health->OnDied.bind(this, &DestroyOnDeathComponent::Destroy);
}

void DestroyOnDeathComponent::Destroy()
{
	Engine.world.Destroy(GetEntity());
}
