#include "Engine/Core/AnimationController.h"
#include "Engine/Core/AssetManager.hpp"
#include "Engine/Core/Entity.hpp"

void AnimationController::Start()
{
	Component::Start();
	fsm = &AssetManager::GetAnimationFSM(path);
	sprite = entity.get().GetComponent<SpriteRenderComponent>();
	context.blackboard->SetData("Sprite", sprite);
}

void AnimationController::Update(float deltaTime)
{
	Component::Update(deltaTime);
	context.deltaTime = deltaTime;
	fsm->Execute(context);
}

void AnimationController::OnDestroy()
{
	Component::OnDestroy();
}



