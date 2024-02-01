#include "Engine/EntityInspector.hpp"

#include <fstream>
#include <iostream>

#include "GenericFactory.hpp"
#include "imgui.h"
#include "Engine/EditorUtility.h"
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
	ImGui::InputText(std::string("Name##" + std::to_string(inspectedEntity->GetId())).c_str(), inspectedEntity->name.data(), inspectedEntity->name.size() + inspectedEntity->name.capacity());
}

void EntityInspector::DrawSerializedField(const std::unique_ptr<Component>& component, const std::pair<std::string, EditorVariable*>& serializedField) const
{
	auto serializedValue = serializedField.second->ToString();
	std::stringstream stream = std::stringstream(serializedValue);

	ImGui::Text(serializedField.first.c_str());

	auto variableType = serializedField.second->GetTypeInfo();
	auto typeName = std::string(variableType.name());

	if (variableType == typeid(std::string))
	{
		ImGui::SameLine();
		const char* buf = serializedValue.c_str();
		ImGui::InputText(std::string("##" + serializedField.first).c_str(), const_cast<char*>(buf), 100);
		serializedField.second->Deserialize(buf);
		return;
	}

	if (variableType == typeid(float) || variableType == typeid(double))
	{
		ImGui::SameLine();
		float value = std::stof(serializedValue);
		if (ImGui::InputFloat(std::string("##" + serializedField.first).c_str(), &value))
		{
			auto str = std::to_string(value);
			serializedField.second->Deserialize(str);
			return;
		}
	}

	if (variableType == typeid(int))
	{
		ImGui::SameLine();
		int value = std::stoi(serializedValue);
		if (ImGui::InputInt(std::string("##" + serializedField.first).c_str(), &value))
		{
			auto str = std::to_string(value);
			serializedField.second->Deserialize(str);
			return;
		}
	}

	if (variableType == typeid(bool))
	{
		ImGui::SameLine();
		bool value = std::stoi(serializedValue) == 1;
		ImGui::Checkbox(std::string("##" + serializedField.first).c_str(), &value);
		auto str = std::to_string(value == true ? 1 : 0);
		serializedField.second->Deserialize(str);
		return;
	}

	if (typeName.find("std::vector") != std::string::npos)
	{
		std::stringstream deserializeStream;

		bool addElement = false;
		ImGui::SameLine();
		if (ImGui::Button(std::string("+##"+serializedField.first).c_str(), ImVec2(20, 20)))
		{
			addElement = true;
		}

		if(std::string(serializedField.second->GetUnderlyingType().name()).find("class") != std::string::npos)
		{
			std::istringstream stream(serializedValue);
			std::vector<std::string> pairs;
			std::string tempPair;

			while (std::getline(stream, tempPair, '}'))
			{
				pairs.push_back(tempPair);
			}

			int index = 0;
			for (const auto& pair : pairs) 
			{
				std::istringstream pairStream(pair);
				std::string keyValue;
				deserializeStream << "{";
				while (pairStream >> keyValue)
				{
					size_t colonPos = keyValue.find(':');
					if (colonPos != std::string::npos) 
					{
						std::string name = keyValue.substr(0, colonPos);

						if (name[0] == '{')
						{
							name = name.substr(1); 
						}

						std::string value = keyValue.substr(colonPos + 1);

						std::string buf = value;
						ImGui::Text(name.c_str());
						ImGui::SameLine();
						ImGui::InputText(std::string("##" + serializedField.first + name + std::to_string(index)).data(), &buf[0], 100);

						deserializeStream << name << ":" << buf.c_str() << " ";
					}
				}

				deserializeStream << "}";
				index++;
			}
		}
		else
		{
			std::istringstream stream(serializedValue);

			if (!stream.str().empty())
			{
				std::vector<std::string> pairs;
				std::string tempPair;
				while (std::getline(stream, tempPair, '}'))
				{
					pairs.push_back(tempPair);
				}

				int index = 0;
				for (const auto& pair : pairs)
				{
					std::istringstream pairStream(pair);
					std::string keyValue;
					deserializeStream << "{";
					while (pairStream >> keyValue)
					{

						if (keyValue[0] == '{')
						{
							keyValue = keyValue.substr(1);
						}

						std::string buf = keyValue;
						ImGui::Text(std::to_string(index).c_str());
						ImGui::SameLine();
						ImGui::InputText(std::string("##" + serializedField.first + std::to_string(index) + std::to_string(index)).data(), &buf[0], 100);

						deserializeStream << buf.c_str() << " }";
						index++;
					}
				}
			}
		}

		if(addElement)
		{
			deserializeStream << "{}";
		}

		serializedField.second->Deserialize(deserializeStream.str());
		return;
	}

	if (typeName.find("class") != std::string::npos)
	{
		std::stringstream deserializeStream;
		std::string pair;

		int index = 0;
		while (stream >> pair)
		{
			std::stringstream valueStream;
			size_t colonPos = pair.find(':');

			if (colonPos != std::string::npos)
			{
				std::string name = pair.substr(0, colonPos);
				std::string value = pair.substr(colonPos + 1);

				std::string buf = value;

				ImGui::Text(name.c_str());
				ImGui::SameLine();
				ImGui::InputText(std::string("##" + serializedField.first + name +std::to_string(index)).data(), &buf[0], 100);

				deserializeStream << name << ":" << buf.c_str() << " ";
			}
			index++;
		}

		serializedField.second->Deserialize(deserializeStream.str());
		return;
	}

	if (typeName.find("enum") != std::string::npos)
	{
		ImGui::SameLine();
		if (ImGui::BeginCombo("EnumDropdown", serializedValue.c_str()))
		{
			for (const auto& value : serializedField.second->GetEnumNames())
			{
				bool isSelected = false;
				if (ImGui::Selectable(value.c_str(), isSelected))
				{
					 isSelected = true;
				}

				if (isSelected)
				{
					serializedField.second->Deserialize(value);
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
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
	for (auto& component : inspectedEntity->components)
	{
		std::string componentName = typeid(*component).name();
		componentName.erase(componentName.find("class "),6);
		if (ImGui::TreeNode((componentName + " ##"+std::to_string(index)).c_str()))
		{
			for (std::pair<const std::string, EditorVariable*>& editorVariable : component->GetSerializedFields())
			{
				DrawSerializedField(component, editorVariable);
			}
			index++;
			ImGui::TreePop();
		}
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
