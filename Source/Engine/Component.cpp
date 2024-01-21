#include "Engine/Component.hpp"
#include "Engine/Entity.hpp"

Transform& Component::GetTransform() const
{
	return entity.get().GetTransform(); 
}

void Component::Start()
{
}

void Component::Update()
{
}

void Component::OnDestroy()
{
}

Component::~Component()
{
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
