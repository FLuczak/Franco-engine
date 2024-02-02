#pragma once
#include "Engine/Entity.hpp"

class EntityInspector
{
public:
	void InspectEntity(Entity* entity);
	void DisplayEntityNameChangeBox() const;
	void DisplayComponents() const;
	void DisplayTemplateSaveButton();
	void Update();
private:
	Entity* inspectedEntity = nullptr;
};

