#include "Engine/Component.hpp"

#include "Engine/EditorVariables.h"
#include "Engine/Entity.hpp"

Transform& Component::GetTransform() const
{
	return entity.get().GetTransform(); 
}

void Component::Start()
{
}

void Component::Update(float deltaTime)
{
}

void Component::OnDestroy()
{
}

Component::~Component()
{
}

void Component::RegisterSerializedField(const std::string& name, EditorVariable* variable)
{
	editorVariables[name] = variable;
}

nlohmann::json Component::Serialize()
{
	nlohmann::json toReturn{};
	toReturn["ComponentName"] = typeid(*this).name();
	for (auto& pair : GetSerializedFields())
	{
		toReturn["SerializedValues"][pair.first] =pair.second->ToString();
	}

	return toReturn;
}

void Component::Deserialize(nlohmann::json& json)
{
	for (auto& pair : GetSerializedFields())
	{
		if(json["SerializedValues"].contains(pair.first))
		{
			pair.second->Deserialize( json["SerializedValues"][pair.first]);
		}
	}
}
