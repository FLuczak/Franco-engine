#include "Engine/Entity.hpp"

#include <iostream>

#include "Engine/Component.hpp"


void Entity::OnDestroy() const
{
	for (auto& element : components)
	{
		element->OnDestroy();
	}
}

void Entity::RemoveComponent(Component& component)
{
	const auto find = std::find_if(components.begin(), components.end(), [&component](const std::unique_ptr<Component>& temp)
	{
		return &component == temp.get();
	});

	if(find == components.end())
	{
		return;
	}

	component.OnDestroy();
	componentsToRemove.push(find - components.begin());
}

void Entity::Update(float deltaTime,bool isGame)
{
	int removed = 0;

	while (!componentsToRemove.empty())
	{
		components.erase(components.begin() + componentsToRemove.front() - removed);
		componentsToRemove.pop();
		removed++;
	}

	for (const auto& component : components)
	{
		if (!isGame && !component->tickInEditor)continue;
		component->Update(deltaTime);
	}
}

void Entity::OnCollisionEnter(CollisionEvent& event) const
{
	for (const auto& element : components)
	{
		element->OnCollisionEnter(event);
	}
}

void Entity::OnCollisionStay(CollisionEvent& event) const
{
	for (const auto& element : components)
	{
		element->OnCollisionStay(event);
	}
}

void Entity::OnCollisionLeave(CollisionEvent& event) const
{
	for (const auto& element : components)
	{
		element->OnCollisionLeave(event);
	}
}

nlohmann::json Entity::Serialize()
{
	nlohmann::json toReturn;
	toReturn["Name"] = name;
	toReturn["Components"] = {};
	toReturn["Transform"] = {};

	for (auto& component : components)
	{
		toReturn["Components"].push_back(component->Serialize());
	}

	toReturn["Transform"]["Translation"] = { transform.position.x,transform.position.y };
	toReturn["Transform"]["Rotation"] = { transform.rotation };
	toReturn["Transform"]["Scale"] = { transform.scale.x,transform.scale.y };

	return toReturn;
}

void Entity::Deserialize(nlohmann::json& json)
{
	name = json["Name"].get<std::string>();
	sf::Vector2f translation = { json["Transform"]["Translation"][0],json["Transform"]["Translation"][1]};
	sf::Vector2f scale = { json["Transform"]["Scale"][0],json["Transform"]["Scale"][1]};
	float rotation = json["Transform"]["Rotation"][0];

	transform.position = translation;
	transform.rotation = rotation;
	transform.scale = scale;

	for (auto& component : json["Components"])
	{
		std::string name = component["ComponentName"].get<std::string>();
		const auto classSize = name.find("class ");
		name.erase(classSize, 6);

		auto temp = GenericFactory<Component>::Instance().CreateProduct(name, this);
		components.push_back(std::move(temp));
		components.back()->SetOwner(*this);
		components.back()->Deserialize(component);
	}
}
