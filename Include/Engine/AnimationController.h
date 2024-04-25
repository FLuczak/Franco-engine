#pragma once
#include "SpriteRenderComponent.hpp"
#include "Engine/Component.hpp"
#include "Engine/EditorVariables.h"

VISITABLE_STRUCT(sf::Vector2i, x, y);
class AnimationState : public AI::State
{
public:
	SERIALIZE_FIELD(float, width)
	SERIALIZE_FIELD(float, height)
	SERIALIZE_FIELD(sf::Vector2i, startIndex)
	SERIALIZE_FIELD(sf::Vector2i, endIndex)
	SERIALIZE_FIELD(float, animationTime)
	SERIALIZE_FIELD(bool, looping)

	void Initialize(AI::StateMachineContext& context) override
	{
		context.blackboard->SetData("CurrentFrame", startIndex.x);
		context.blackboard->SetData("TimeInFrame", 0.0f);
		context.blackboard->SetData<bool>("AnimationEnded", false);
	}

	void Update(AI::StateMachineContext& context) override
	{
		bool ended = context.blackboard->GetData<bool>("AnimationEnded");

		if (ended)return;

		float frameTime = static_cast<float>(endIndex.x - startIndex.x) * animationTime;
		float& timeInFrame = context.blackboard->GetData<float>("TimeInFrame");
		if (timeInFrame <= frameTime)
		{
			timeInFrame += context.deltaTime;
		}
		else
		{
			timeInFrame = 0.0f;
			int& currentFrame = context.blackboard->GetData<int>("CurrentFrame");
			currentFrame++;
			if(currentFrame >= endIndex.x)
			{
				if (!looping)
				{
					context.blackboard->SetData<bool>("AnimationEnded", true);
				}
				currentFrame = startIndex.x;
			}
			sf::IntRect rectangle = sf::IntRect(currentFrame * width, startIndex.y*height,width,height);
			context.blackboard->GetData<SpriteRenderComponent*>("Sprite")->sprite->setTextureRect(rectangle);
		}
	}

	void End(AI::StateMachineContext& context) override
	{
		context.blackboard->SetData<bool>("AnimationEnded", false);
	}
};
REGISTER_STATE(AnimationState);

class AnimationController : public Component 
{
public:
	SERIALIZE_FIELD(std::string, path)

	explicit AnimationController(Entity& entityToSet): Component(entityToSet)
	{
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;

private:
	SpriteRenderComponent* sprite;
	AI::StateMachineContext context;
	AI::FiniteStateMachine* fsm;
};
REGISTER_COMPONENT(AnimationController);