#include "Game/AnimationController.h"

#include "Engine/AssetManager.hpp"
#include "Engine/Entity.hpp"
#include "Game/BaseCollider.h"
#include "Game/Bullet.h"

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



