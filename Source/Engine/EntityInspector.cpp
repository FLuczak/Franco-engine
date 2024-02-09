#include "Engine/EntityInspector.hpp"

#include <fstream>
#include <iostream>

#include "GenericFactory.hpp"
#include "imgui.h"
#include "Engine/EditorUtility.h"
#include "Engine/EditorVariables.h"
#include "Engine/AI/Utils/EditorUtils.h"
#include "magic_enum/magic_enum_utility.hpp"
#include "Visit struct/visit_struct.hpp"

void EntityInspector::InspectEntity(Entity* entity)
{
	if(entity != inspectedEntity)
	{
		inspectedEntity = entity;
	}
}

void EntityInspector::DisplayEntityNameChangeBox() const
{
	ImGui::Checkbox(std::string("##enabled" + std::to_string(inspectedEntity->GetId())).c_str(), &inspectedEntity->active);
	ImGui::SameLine();
	ImGui::InputText(std::string("Name##" + std::to_string(inspectedEntity->GetId())).c_str(), inspectedEntity->name.data(), inspectedEntity->name.size() + inspectedEntity->name.capacity());

	constexpr auto tags = magic_enum::enum_names<Tag>();
	if (ImGui::BeginCombo("Tag:", magic_enum::enum_name(inspectedEntity->tag).data()))
	{
		for (auto value : tags)
		{
			bool isSelected = false;
			if (ImGui::Selectable(std::string(value).c_str(), isSelected))
			{
				isSelected = true;
			}

			if (isSelected)
			{
				inspectedEntity->tag = magic_enum::enum_cast<Tag>(std::string(value)).value();
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
}


void EntityInspector::DisplayComponents() const
{
	ImGui::Text("Components");
	auto components = GenericFactory<Component>::Instance().GetKeys();
	
	if (ImGui::TreeNode("transform"))
	{
		ImGui::Text("Position");
		auto& transform = inspectedEntity->GetTransform();
		ImGui::InputFloat("x ##Position", &transform.position.x, 0.01f);
		ImGui::InputFloat("y ##Position", &transform.position.y, 0.01f);

		ImGui::Text("Scale");
		ImGui::InputFloat("x ##Scale", &transform.scale.x, 0.01f);
		ImGui::InputFloat("y ##Scale", &transform.scale.y, 0.01f);

		ImGui::Text("Rotation");
		ImGui::InputFloat("##Rotation", &transform.rotation, 0.1f);
		ImGui::TreePop();
	}


	int index = 0;
	int componentIndex = 0;
	int removedId = -1;
	for (auto& component : inspectedEntity->components)
	{
		std::string componentName = typeid(*component).name();
		componentName.erase(componentName.find("class "),6);

		if(ImGui::Button((std::string("- ##") + std::to_string(componentIndex)).c_str()))
		{
			removedId = componentIndex;
		}

		ImGui::SameLine();

		if (ImGui::TreeNode((componentName + " ##"+std::to_string(index)).c_str()))
		{
			for (std::pair<const std::string, EditorVariable*>& editorVariable : component->GetSerializedFields())
			{
				AI::DrawSerializedField(editorVariable);
			}
			index++;
			ImGui::TreePop();
		}

		componentIndex++;
	}

	if(removedId != -1)
	{
		inspectedEntity->components.erase(inspectedEntity->components.begin()+removedId);
	}

	ImGui::Separator();

	if (ImGui::TreeNode("AddComponent"))
	{
		for(auto& component: components)
		{
			if(ImGui::Button(component.c_str()))
			{
				auto temp = GenericFactory<Component>::Instance().CreateProduct(component,inspectedEntity);
				inspectedEntity->components.push_back(std::move(temp));
				inspectedEntity->components.back()->SetOwner(*inspectedEntity);
			}
		}
		ImGui::TreePop();
	}
}

void EntityInspector::DisplayTemplateSaveButton()
{
	if(ImGui::Button("Save template"))
	{
		std::string path = Dialogs::OpenFileSaveDialog();

		if (path.empty())return;
		auto json = inspectedEntity->Serialize();

		std::ofstream outputFile(path);

		if (outputFile.is_open()) {
			// Write the JSON data to the file
			outputFile << std::setw(json.size()) << json << std::endl;

			// Close the file stream
			outputFile.close(); std::cout << "JSON data has been written to 'output.json'." << std::endl;
		}
		else
		{
			std::cerr << "Error opening the file." << std::endl;
		}
	}
}

void EntityInspector::Update()
{
	ImGui::Begin("Entity inspector");
	if (inspectedEntity == nullptr)
	{
		ImGui::End();
		return;
	}

	DisplayEntityNameChangeBox();
	DisplayTemplateSaveButton();
	DisplayComponents();

	ImGui::End();
}
