#pragma once
#include <sstream>
#include <string>

#include "windows.h"
#include "Engine/Editor/EditorVariables.h"
#include "imgui.h"

namespace AI
{
namespace ComparatorsInEditor
{
inline const std::array<std::string, 5> comparatorTypes = {"int", "float", "double", "bool", "string"};
inline const std::array<std::string, 6> comparisonTypes = {"equal","not equal", "less","less equal", "greater",   "greater equal"};
}  // namespace ComparatorsInEditor



struct ComparatorWrapper
{
    std::string key = "";
    std::string type = "int";
    std::string comparisonType = "equal";
    std::string value;

    int GetComparisonTypeNum() const;
    std::string ToString() const;
};

namespace BehaviorsInBTEditor
{
enum class BehaviorWrapperType
{
    DECORATOR,
    ACTION,
    COMPOSITE
};

inline std::vector<std::string> behaviorEditorType{"Action",   "Comparison", "Selector",       "Sequence","Repeater", "Inverter",   "Always Succeed", "Until Fail"};
inline std::unordered_map<std::string, BehaviorWrapperType> behaviorWrapperTypes
{
    {"Action", BehaviorWrapperType::ACTION},
    {"Comparison", BehaviorWrapperType::DECORATOR},
    {"Selector", BehaviorWrapperType::COMPOSITE},
    {"Sequence", BehaviorWrapperType::COMPOSITE},
    {"Repeater", BehaviorWrapperType::DECORATOR},
    {"Inverter", BehaviorWrapperType::DECORATOR},
    {"Always Succeed", BehaviorWrapperType::DECORATOR},
    {"Until Fail", BehaviorWrapperType::DECORATOR}
};

}  // namespace BehaviorsInBTEditor

inline bool SetDragDropTarget(std::filesystem::path& payloadData, const std::initializer_list<std::string>& dataTypes)
{
	if (ImGui::BeginDragDropTarget())
	{
		for (const auto& dataType : dataTypes)
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dataType.c_str()))
			{
				payloadData = std::filesystem::path(static_cast<const char*>(payload->Data));

				ImGui::EndDragDropTarget();
				return true;
			}
		}
	}
	ImGui::SameLine();
	ImGui::Text(payloadData.string().c_str());
	return false;
}

namespace Dialogs
{
 static inline std::string OpenFileSaveDialog()
{
    OPENFILENAMEA file;
    CHAR szFile[260] = {0};
    ZeroMemory(&file, sizeof(OPENFILENAMEA));
    file.lStructSize = sizeof(OPENFILENAMEA);

    wchar_t pBuf[256] = {};
    size_t len = sizeof(pBuf);
    GetModuleFileName(nullptr, pBuf, static_cast<DWORD>(len));

    std::wstring s = pBuf;
#pragma warning(push)
#pragma warning(disable : 4244)
    const std::string string(s.begin(), s.end());
#pragma warning(pop)

    file.lpstrInitialDir = string.c_str();
    file.lpstrFilter = ".txt";
    file.lpstrFile = szFile;
    file.nMaxFile = sizeof(szFile);
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&file) == TRUE)
    {
        std::string filename = file.lpstrFile;

        return filename;
    }

    return std::string();
}

static std::string OpenFileLoadDialog()
{
    OPENFILENAMEA file;
    CHAR szFile[260] = {0};
    ZeroMemory(&file, sizeof(OPENFILENAMEA));
    file.lStructSize = sizeof(OPENFILENAMEA);

    wchar_t pBuf[256] = {};
    size_t len = sizeof(pBuf);
    GetModuleFileName(nullptr, pBuf, static_cast<DWORD>(len));

    std::wstring s = pBuf;

#pragma warning(push)
#pragma warning(disable : 4244)
    std::string string(s.begin(), s.end());
#pragma warning(pop)

    file.lpstrInitialDir = string.c_str();
    file.lpstrFilter = ".txt";
    file.lpstrFile = szFile;
    file.nMaxFile = sizeof(szFile);
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_READONLY;

    if (GetOpenFileNameA(&file) == TRUE)
    {
        return std::string(file.lpstrFile);
    }

    return std::string();
}
}  // namespace Dialogs

inline int AI::ComparatorWrapper::GetComparisonTypeNum() const
{
    for (int i = 0; i < ComparatorsInEditor::comparisonTypes.size(); i++)
    {
        if (ComparatorsInEditor::comparisonTypes[i] == comparisonType) return i;
    }
    return 0;
}

inline void DrawSerializedField(const std::pair<std::string, EditorVariable*>& serializedField) 
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
		if (ImGui::Button(std::string("+##" + serializedField.first).c_str(), ImVec2(20, 20)))
		{
			addElement = true;
		}

		if (std::string(serializedField.second->GetUnderlyingType().name()).find("class") != std::string::npos && serializedField.second->GetUnderlyingType() != typeid(std::string))
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

		if (addElement)
		{
			deserializeStream << "{}";
		}

		serializedField.second->Deserialize(deserializeStream.str());
		return;
	}

	if (variableType == typeid(PathHelper))
	{
		nlohmann::json j = nlohmann::json::parse(serializedValue);
		PathHelper helper;
		helper.format = j["format"].get<std::string>();
		helper.path = std::filesystem::path(j["path"].get<std::string>());

		SetDragDropTarget(helper.path, { helper.format });

		std::string pathToString = helper.path.string();
		size_t pos = pathToString.find("Assets");
		if (pos != std::string::npos) 
		{
			pathToString = pathToString.substr(pos);
		}

		helper.path = pathToString;

		j["format"] = helper.format;
		j["path"] = helper.path.string();

		serializedField.second->Deserialize(j.dump());
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
				ImGui::InputText(std::string("##" + serializedField.first + name + std::to_string(index)).data(), &buf[0], 100);

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

inline std::string AI::ComparatorWrapper::ToString() const
{
    std::stringstream toReturn;
    if (key.empty()) return {};
    if (type.empty()) return {};

    toReturn << key << " ";
    toReturn << type << " ";
    toReturn << GetComparisonTypeNum() << " ";
    toReturn << value << " ";
    return toReturn.str();
}


}  // namespace AI
