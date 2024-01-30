#pragma once
#include "Engine/Entity.hpp"

class EntityInspector
{
public:
	void InspectEntity(Entity* entity);
	void DisplayEntityNameChangeBox() const;
	void DrawSerializedField(const std::unique_ptr<Component>& component, const std::pair<std::string, EditorVariable*>& serializedField) const;
	void DisplayComponents() const;
	void DisplayTemplateSaveButton();
	void Update();
private:
	Entity* inspectedEntity = nullptr;
};

