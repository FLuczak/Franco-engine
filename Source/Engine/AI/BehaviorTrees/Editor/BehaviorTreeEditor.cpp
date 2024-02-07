#include "Engine/AI/BehaviorTrees/Editor/BehaviorTreeEditor.hpp"
#include <filesystem>
#include <fstream>
#include <imgui_node_editor.h>
#include <iostream>
#include "GenericFactory.hpp"
#include "Engine/AI/BehaviorTrees/Behaviors.hpp"


void AI::BehaviorWrapper::DrawNode(size_t transitionIndex)
{
    ax::NodeEditor::BeginNode(id);
    
    if (justAdded)
    {
        ax::NodeEditor::SetNodePosition(id, position);
        justAdded = false;
    }

        DrawNodeContent();

    size_t childIndex = 0;
    for (auto& child : children)
    {
        child.DrawNode(childIndex + transitionIndex + children.size());
        childIndex++;
    }

    size_t index = 0;
    for (auto& child : children)
    {
        ax::NodeEditor::Link(transitionIndex + index * 100 +id + child.id, maxAmountNodes + id * 2, maxAmountNodes + child.id * 2 - 1);
        index++;
    }

}

void AI::BehaviorWrapper::DrawNodeContent()
{
    ImGui::Dummy(ImVec2(ax::NodeEditor::GetNodeSize(id).x / 3, 0));
    ImGui::SameLine();
    ax::NodeEditor::BeginPin(2 * id - 1 + maxAmountNodes, ax::NodeEditor::PinKind::Output);
    ImGui::Dummy(ImVec2(1, 1));
    ax::NodeEditor::EndPin();
    if (action.has_value())
    {
        ImGui::Text(std::string(name + "<" + action.value() + ">" + "(" + std::to_string(id) + ")").c_str());
    }
    else
    {
        ImGui::Text(std::string(name + "(" + std::to_string(id) + ")").c_str());
    }
    ImGui::Dummy(ImVec2(ax::NodeEditor::GetNodeSize(id).x / 3, 0));
    ImGui::SameLine();
    ax::NodeEditor::BeginPin(2 * id + maxAmountNodes, ax::NodeEditor::PinKind::Input);
    ImGui::Dummy(ImVec2(1, 1));
    ax::NodeEditor::EndPin();
    ax::NodeEditor::EndNode();
}

void AI::BehaviorWrapper::DrawNode(size_t transitionIndex, const BehaviorTreeContext& context)
{
    ax::NodeEditor::BeginNode(id);

    if (justAdded)
    {
        ax::NodeEditor::SetNodePosition(id, position);
        justAdded = false;
    }

    DrawNodeContent();

    int childIndex = 0;
    for (auto& child : children)
    {
        child.DrawNode(childIndex + transitionIndex + children.size(),context);
        childIndex++;
    }

    int index = 0;
    for (auto& child : children)
    {
        ax::NodeEditor::Link(transitionIndex + index * 100 + id + child.id, maxAmountNodes + id * 2,maxAmountNodes + child.id * 2 - 1);

        if (context.statuses.contains(child.id-2))
        {
            if ( context.statuses.at(child.id - 2) == Status::RUNNING)
            {
                ax::NodeEditor::Flow(transitionIndex + index * 100 + id + child.id);
            }
        }

        index++;
    }
}

void AI::BehaviorWrapper::ClearChildren(std::vector<int>& ids)
{
    for (auto& child : children)
    {
        ids.push_back(child.id);
        child.ClearChildren(ids);
    }

    children.clear();
}

AI::BehaviorTreeEditor::BehaviorTreeEditor()
{
    ax::NodeEditor::Config config;
    config.SettingsFile = "SimpleBT.json";
    m_context = ax::NodeEditor::CreateEditor(&config);
    m_previewContext = ax::NodeEditor::CreateEditor(&config);
}


void AI::BehaviorTreeEditor::DrawComparisonDetails(AI::BehaviorWrapper& selected) const
{
    ComparatorWrapper comparator;

    if (!selected.additionalInfo.has_value())
    {
        selected.additionalInfo = comparator;
    }
    else
    {
        comparator = std::any_cast<ComparatorWrapper>(selected.additionalInfo);
    }

    std::string typeToChoose = comparator.type;
    ImGui::Text("Type");
    ImGui::SameLine();
    if (ImGui::BeginCombo(std::string(std::string("##Type")).c_str(),comparator.type.empty() ? ComparatorsInEditor::comparatorTypes[0].c_str() : comparator.type.c_str()))
    {
        for (auto& value : ComparatorsInEditor::comparatorTypes)
        {
            const bool is_selected = (typeToChoose == value);
            if (ImGui::Selectable(std::string(value).c_str(), is_selected))
            {
                comparator.type = value;
            }
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    char* buffer = const_cast<char*>(comparator.key.c_str());

    ImGui::Text("Key");
    ImGui::SameLine();
    ImGui::InputText(std::string(std::string("##Key")).c_str(), buffer, 31);
    comparator.key = buffer;

    ImGui::Text("Comparison");
    ImGui::SameLine();
    if (ImGui::BeginCombo(std::string(std::string("##Comparison")).c_str(),comparator.comparisonType.empty() ? ComparatorsInEditor::comparisonTypes[0].c_str(): comparator.comparisonType.c_str()))
    {
        for (auto& value : ComparatorsInEditor::comparisonTypes)
        {
            const bool is_selected = (typeToChoose == value);
            if (ImGui::Selectable(std::string(value).c_str(), is_selected))
            {
                comparator.comparisonType = value;
            }
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }


    buffer = comparator.value.data();
    const char* label = "##CompValue";

    ImGui::Text("Value");
    ImGui::SameLine();
    std::string toString;
    if (comparator.type == "int")
    {
        int val = std::string(buffer).empty() ? 0 : std::stoi(buffer);
        if (ImGui::InputInt(label, &val))
        {
            toString = std::to_string(val);
            buffer = toString.data();
        }
    }

    if (comparator.type == "float" || comparator.type == "double")
    {
        float val = std::string(buffer).empty() ? 0.0f : std::stof(buffer);
        if (ImGui::InputFloat(label, &val))
        {
            toString = std::to_string(val);
            buffer = toString.data();
        }
    }

    if (comparator.type == "bool")
    {
        int intVal = std::string(buffer).empty() ? 0 : std::stoi(buffer);
        bool val = intVal == 1;
        ImGui::Checkbox(label, &val);
        intVal = val ? 1 : 0;
        toString = std::to_string(intVal);
        buffer = toString.data();
    }

    if (comparator.type == "string")
    {
        ImGui::InputText(label, buffer, 31);
    }

    comparator.value = buffer;

    selected.additionalInfo = comparator;
}

void AI::BehaviorTreeEditor::DrawDecoratorChildMenu(BehaviorWrapper& selected)
{
    std::string choice = selected.children.empty() ? "Select child" : selected.children[0].name;
    if (ImGui::BeginCombo("##stateSelection", choice.c_str()))
    {
        for (auto value : BehaviorsInBTEditor::behaviorEditorType)
        {
            bool is_selected = (choice == value);
            if (ImGui::Selectable(value.c_str(), is_selected)) choice = value;
            is_selected = choice == value;
            if (is_selected)
            {
                if (!selected.children.empty())
                {
                    selected.ClearChildren(m_removedIds);
                }
                selected.AddChild({GetIdToSet(),ImVec2(selected.position.x, 90 + selected.position.y),choice,choice,{},BehaviorsInBTEditor::behaviorWrapperTypes[choice]});
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (selected.behaviorType == "Repeater")
    {
        int repeats = selected.additionalInfo.has_value() ? std::any_cast<int>(selected.additionalInfo) : 0;
        ImGui::InputInt("repeater repeats", &repeats);
        selected.additionalInfo = repeats;
    }

    if (selected.behaviorType == "Comparison")
    {
        DrawComparisonDetails(selected);
    }
}


void AI::BehaviorTreeEditor::DrawCompositeChildMenu(BehaviorWrapper& selected)
{
    std::string choice =  "Add child";
    if (ImGui::BeginCombo("##stateSelection", choice.c_str()))
    {
        for (auto value : BehaviorsInBTEditor::behaviorEditorType)
        {
            bool is_selected = (choice == value);
            if (ImGui::Selectable(value.c_str(), is_selected)) choice = value;
            is_selected = choice == value;
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
                selected.AddChild({GetIdToSet(),ImVec2(selected.position.x + 90.0f * selected.children.size(), 90 + selected.position.y),choice,choice,{}, BehaviorsInBTEditor::behaviorWrapperTypes[choice],{}});
            }
        }

        ImGui::EndCombo();
    }
    int toRemove = -1;
    int index = 0;

    for (auto& value : selected.children)
    {
        ImGui::Text((value.name + " (" + std::to_string(value.id) + ") ").c_str());
        ImGui::SameLine();
        if (ImGui::Button(std::string("- ##" + std::to_string(index)).c_str(), ImVec2(30, 30)))
        {
            toRemove = index;
        }
        index++;
    }

    if (toRemove != -1)
    {
        m_removedIds.push_back(selected.children[toRemove].id);
        selected.children[toRemove].ClearChildren(m_removedIds);
        selected.children.erase(selected.children.begin() + toRemove);
    }
}

void AI::BehaviorTreeEditor::DrawActionContextMenu(BehaviorWrapper& selected) const
{
    std::vector<std::string> types = GenericFactory<AI::BehaviorTreeAction>::Instance().GetKeys();
    std::string choice = !selected.action.has_value() ? types[0] : selected.action.value();

    if (ImGui::BeginCombo("##stateSelection", choice.c_str()))
    {
        for (auto value : types)
        {
            const bool is_selected = (choice == value);
            if (ImGui::Selectable(value.c_str(), is_selected)) choice = value;
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (!selected.action.has_value() || choice != selected.action.value())
    {
        selected.action = std::optional(choice);
        auto behavior = GenericFactory<BehaviorTreeAction>::Instance().CreateProduct(selected.action.value());
        selected.editorVariables.clear();

        for (auto pair : behavior->editorVariables)
        {
            selected.editorVariables.emplace_back(pair.first, pair.second->ToString(),pair.second->GetTypeInfo());
        }
    }

    ImGui::Text("Editor variables:");

    for (auto& editorVariable : selected.editorVariables)
    {
        ImGui::Text(editorVariable.name.c_str());
        ImGui::SameLine();
        std::string variableValue = editorVariable.value;
        char* buf = variableValue.data();
        ImGui::SameLine();

        if (editorVariable.type == typeid(std::string))
        {
            char* buf = variableValue.data();
            if (ImGui::InputText(std::string("##" + editorVariable.name).c_str(), buf, 100))
            {
                editorVariable.value = buf;
            }
        }

        if (editorVariable.type == typeid(float) || editorVariable.type == typeid(double))
        {
            float value = std::stof(variableValue);
            if (ImGui::InputFloat(std::string("##" + editorVariable.name).c_str(), &value))
            {
                auto str = std::to_string(value);
                editorVariable.value = str;
            }
        }

        if (editorVariable.type == typeid(int))
        {
            int value = std::stoi(variableValue);
            if (ImGui::InputInt(std::string("##" + editorVariable.name).c_str(), &value))
            {
                auto str = std::to_string(value);
                editorVariable.value = str;
            }
        }

        if (editorVariable.type == typeid(bool))
        {
            bool value = std::stoi(variableValue) == 1;
            if (ImGui::Checkbox(std::string("##" + editorVariable.name).c_str(), &value))
            {
                auto str = std::to_string(value == true ? 1 : 0);
                editorVariable.value = str;
            }
        }
    }
}

void AI::BehaviorTreeEditor::DrawChildMenu(AI::BehaviorWrapper& selected)
{
    std::string choice = selected.children.empty() ? BehaviorsInBTEditor::behaviorEditorType[0] : selected.children[0].behaviorType;

    switch (selected.type)
    {
        case BehaviorsInBTEditor::BehaviorWrapperType::DECORATOR:
            DrawDecoratorChildMenu(selected);
            break;
        case BehaviorsInBTEditor::BehaviorWrapperType::ACTION:
            DrawActionContextMenu(selected);
            break;
        case BehaviorsInBTEditor::BehaviorWrapperType::COMPOSITE:
            DrawCompositeChildMenu(selected);
            break;
        default: ;
    }
}

void AI::BehaviorTreeEditor::DrawSelectedStateDetails()
{
    ImGui::BeginChild("FSM Details", ImVec2(300, ImGui::GetWindowSize().y * 0.90f));
    
    if (m_selectedNodes.size() != 1)
    {
        ImGui::EndChild();
        return;
    }
    
    auto& selected = m_selectedNodes[0].get();
    ImGui::Text(std::string(selected.name + "(" + std::to_string(selected.id) + ")").c_str());

    ImGui::Text("Child");
    ImGui::SameLine();

    DrawChildMenu(selected);
    
    ImGui::EndChild();
}

void AI::BehaviorTreeEditor::Update()
{
    auto& io = ImGui::GetIO();

    if(ImGui::Begin("BT editor", nullptr))
    {
        open = true;
    }
	else
    {
        open = false;
    }

    if(open)
    {
        DrawUpperMenu();
        ImGui::Separator();

        DrawSelectedStateDetails();
        m_selectedNodes.clear();
        ImGui::SameLine();

        ax::NodeEditor::SetCurrentEditor(m_context);
        ax::NodeEditor::Begin("BT editor", ImVec2(0.0, 0.0f));

        CheckSelectedNode(editorRoot);
        DrawNodes();

        ax::NodeEditor::End();
        ax::NodeEditor::SetCurrentEditor(nullptr);

    }

    ImGui::End();
}

void AI::BehaviorTreeEditor::PreviewContext(const BehaviorTree& bt,const BehaviorTreeContext& context)
{
    auto& io = ImGui::GetIO();
	open = true;

    if (&bt != previewedTree)
    {
        previewedTree = &bt;
        nlohmann::json serialized = nlohmann::json::parse(bt.Serialize().str());
        int id = editorRoot.id;
        previewRoot.ClearChildren(m_removedIds);
        LoadBehavior(serialized["children"][0], previewRoot, id);
    }

    ImGui::Begin("BT context", &open);
    ax::NodeEditor::SetCurrentEditor(m_previewContext);
    ax::NodeEditor::Begin("BT context", ImVec2(0.0, 0.0f));

    DrawNodesPreview(context);

    ax::NodeEditor::NavigateToContent(0);
    ax::NodeEditor::End();
    ax::NodeEditor::SetCurrentEditor(nullptr);

    ImGui::End();
}

void AI::BehaviorTreeEditor::CheckSelectedNode(BehaviorWrapper& behavior)
{
    if (ax::NodeEditor::IsNodeSelected(behavior.id))
    {
        m_selectedNodes.emplace_back(behavior);
    }

    for (auto& child : behavior.children)
    {
        CheckSelectedNode(child);
    }
}

void AI::BehaviorTreeEditor::DrawNodes()
{
    editorRoot.DrawNode(0);
}

void AI::BehaviorTreeEditor::DrawNodesPreview(const BehaviorTreeContext& context)
{
    previewRoot.DrawNode(0,context);
}

void AI::BehaviorTreeEditor::DrawUpperMenu()
{
    if (ImGui::Button("Save", ImVec2(50, 30)))
    {
        SaveBT();
    }

    ImGui::SameLine(0);

    if (ImGui::Button("Load", ImVec2(50, 30)))
    {
        LoadBT();
    }

        ImGui::SameLine(0);

    if (ImGui::Button("Clear", ImVec2(50, 30)))
    {
        editorRoot.ClearChildren(m_removedIds);
    }

    ImGui::SameLine(ImGui::GetWindowSize().x * 0.99f - 30);
}

void AI::BehaviorTreeEditor::LoadBehavior(nlohmann::json& json, BehaviorWrapper& behavior,int& idToSet)
{
    std::string name = json["name"];
    std::erase_if(name, isspace);

    size_t numChildren = json["children"].size();

    auto temp = BehaviorWrapper({++idToSet,ImVec2(behavior.position.x + 100.0f * behavior.children.size(), 90 + behavior.position.y),name,name,{},BehaviorsInBTEditor::behaviorWrapperTypes[name]});

    if (name == "Repeater")
    {
        int numRepeats = json["numRepeats"].get<int>();
        temp.additionalInfo = numRepeats;
    }

    if (name.find("Comparison") != std::string::npos)
    {
        std::stringstream stream = std::stringstream(json["comparator"].get<std::string>());
        std::string comparisonKey;
        std::string typeName;
        int comparisonType;
        std::string value;

        stream >> comparisonKey;
        stream >> typeName;
        stream >> comparisonType;
        stream >> value;

        temp.additionalInfo =ComparatorWrapper{comparisonKey, typeName, ComparatorsInEditor::comparisonTypes[comparisonType], value};
    }
    if (name == "Action")
    {
        temp.name = "Action";
        temp.behaviorType = "Action";

        std::string actionName = json["type"];
        temp.action = actionName;
        temp.type = BehaviorsInBTEditor::BehaviorWrapperType::ACTION;

        erase_if(actionName, isspace);
        auto tempProduct = GenericFactory<BehaviorTreeAction>::Instance().CreateProduct(actionName);

        for (auto element : json["editor-variables"])
        {
            auto name = element["name"].get<std::string>();
            auto value = element["value"].get<std::string>();
            if (!tempProduct->editorVariables.contains(name)) continue;
            temp.editorVariables.emplace_back(name, value, tempProduct->editorVariables[name]->GetTypeInfo());
        }

        for (auto variable : tempProduct->editorVariables)
        {
            auto itr = std::ranges::find(temp.editorVariables, variable.first, &EditorVariableWrapper::name);
            if (itr == temp.editorVariables.end())
            {
                temp.editorVariables.emplace_back(variable.first, variable.second->ToString(), variable.second->GetTypeInfo());
            }
        }
    }

    for (int i = 0; i < numChildren; i++)
    {
        LoadBehavior(json["children"][i], temp, idToSet);
    }
    behavior.AddChild(temp);
}

void AI::BehaviorTreeEditor::LoadBT()
{
    auto path = Dialogs::OpenFileLoadDialog();
    std::ifstream inputFile(path);
    nlohmann::json json;

    if (inputFile.is_open())
    {
        inputFile >> json;
        inputFile.close();
        std::cout << "JSON data has been loaded and deserialized." << std::endl;
    }
    else
    {
        std::cerr << "Error opening the file." << std::endl;
    }

	if (json.empty()) return;
    int id = editorRoot.id;
    if (json["behavior-structure-type"].get<std::string>() != "BehaviorTree") return;
    editorRoot.ClearChildren(m_removedIds);
    LoadBehavior(json["children"][0], editorRoot, id);
}

void AI::BehaviorTreeEditor::SaveBT()
{
    nlohmann::json toSave;

    toSave["version"] = "1.0";
    toSave["children"] = nlohmann::json::array();
    toSave["behavior-structure-type"] = "BehaviorTree";

    SaveBehavior(toSave, editorRoot.children[0]);
    std::string directory = Dialogs::OpenFileSaveDialog();
    std::ofstream outputFile(directory);

    if (outputFile.is_open())
    {
        outputFile << std::setw(toSave.size()) << toSave << std::endl;
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file." << std::endl;
    }
}

void AI::BehaviorTreeEditor::SaveBehavior(nlohmann::json& stream, const BehaviorWrapper& behavior)
{
    nlohmann::json node;
    node["children"] = nlohmann::json::array({});
    auto name = behavior.behaviorType;

    switch (behavior.type)
    {
        case BehaviorsInBTEditor::BehaviorWrapperType::DECORATOR:
            node["name"] = name;

            if (behavior.behaviorType == "Repeater")
            {
                node["num-repeats"] = std::any_cast<int>(behavior.additionalInfo);
            }
            else if (behavior.behaviorType == "Comparison")
            {
                std::stringstream ss;
                const auto comparatorWrapper = std::any_cast<ComparatorWrapper>(behavior.additionalInfo);
                ss << comparatorWrapper.key << " ";
                ss << comparatorWrapper.type << " ";
                ss << comparatorWrapper.GetComparisonTypeNum() << " ";
                ss << comparatorWrapper.value << " ";

                node["comparator"] = ss.str();
            }

            if (!behavior.children.empty())
            {
                SaveBehavior(node, behavior.children[0]);
            }
            break;
        case BehaviorsInBTEditor::BehaviorWrapperType::ACTION:
            node["name"] = name;
            node["type"] = behavior.action.value();
            node["children"] = nlohmann::json::array();
            node["editor-variables"] = nlohmann::json();

            for (auto& variable : behavior.editorVariables)
            {
                auto toAdd = nlohmann::json();
                toAdd["name"] = variable.name;
                toAdd["value"] = variable.value;
                node["editor-variables"].push_back(toAdd);
            }
        break;
        case BehaviorsInBTEditor::BehaviorWrapperType::COMPOSITE:
            node["name"] = name;
        
            for (const auto& element : behavior.children)
            {
                SaveBehavior(node, element);
            }
            break;
    }

    nlohmann::json children = stream.at("children");
    children.push_back(node);
    stream["children"] = children;
}

int AI::BehaviorTreeEditor::GetBiggestId(const BehaviorWrapper& node)
{
    int highestId = node.id;

    for (const auto& child : node.children)
    {
        int childHighestId = GetBiggestId(child);
        highestId = max(highestId, childHighestId);
    }

    return highestId;
}

int AI::BehaviorTreeEditor::GetIdToSet()
{
    if (m_removedIds.empty())
    {
        int max = GetBiggestId(editorRoot);
        return max +1;
    }
    else
    {
        std::sort(m_removedIds.begin(), m_removedIds.end());
        int toReturn = m_removedIds[0];
        m_removedIds.erase(m_removedIds.begin());
        return toReturn;
    }
}
