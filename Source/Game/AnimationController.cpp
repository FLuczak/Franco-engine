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

void AnimationController::OnCollisionEnter(CollisionEvent& event)
{
	Component::OnCollisionEnter(event);
	if (event.otherCollider.GetEntity().GetComponent<Bullet>() == nullptr)return;
	context.blackboard->SetData("IsHit", true);
}

void AnimationController::OnCollisionLeave(CollisionEvent& event)
{
	Component::OnCollisionLeave(event);
	context.blackboard->SetData("IsHit", false);
}


