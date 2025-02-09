#pragma once
#include "Engine/Core/Entity.hpp"

class EntityInspector
{
public:
	void InspectEntity(Entity* entity);
	void DisplayEntityNameChangeBox() const;
	void DisplayComponents() const;
	void DisplayTemplateSaveButton() const;
	void Update();
	Entity* GetInspectedEntity() const { return inspectedEntity; }
private:
	void DisplayGizmo();
	Entity* inspectedEntity = nullptr;
};

