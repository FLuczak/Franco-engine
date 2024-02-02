#pragma once

#include <any>
#include <optional>
#include <string>
#include <typeindex>
#include <vector>

#include "imgui.h"
#include "Engine/AI/BehaviorTrees/Behaviors.hpp"
#include "Engine/AI/BehaviorTrees/BehaviorTree.hpp"
#include "Engine/AI/FiniteStateMachines/Editor/FiniteStateMachineEditor.hpp"
#include "Engine/AI/Utils/EditorUtils.h"


namespace AI
{
    inline constexpr int maxAmountNodes = 1000;

    struct EditorVariableWrapper
    {
        std::string name;
        std::string value;
        std::type_index type;
    };

    struct BehaviorWrapper
    {
        int id = 1;
        ImVec2 position;

        std::string name = "root";
        std::string behaviorType;
        std::vector<BehaviorWrapper> children{};
        BehaviorsInBTEditor::BehaviorWrapperType type = BehaviorsInBTEditor::BehaviorWrapperType::DECORATOR;
        std::optional<std::string> action{};
        std::vector<EditorVariableWrapper> editorVariables{};

        bool justAdded = true;
        std::any additionalInfo;

        void DrawNode(size_t transitionIndex, const BehaviorTreeContext& context);
        void DrawNode(size_t transitionIndex);
        void AddChild(BehaviorWrapper child)
        {
            if (type == BehaviorsInBTEditor::BehaviorWrapperType::ACTION) return;
            if (type == BehaviorsInBTEditor::BehaviorWrapperType::DECORATOR && children.size() == 1)
            {
                return;
            }

            children.push_back(child);
        }
        void ClearChildren(std::vector<int>& ids);
    private:
        void DrawNodeContent();
    };

    class BehaviorTreeEditor
    {
    public:
        BehaviorTreeEditor();
        void Update();
        void PreviewContext(const BehaviorTree& bt, const BehaviorTreeContext& context);
    private:
        bool open = false;
        void DrawSelectedStateDetails();
        void DrawComparisonDetails(AI::BehaviorWrapper& selected) const;
        void DrawDecoratorChildMenu(BehaviorWrapper& selected);
        void DrawCompositeChildMenu(BehaviorWrapper& selected);
        void DrawActionContextMenu(BehaviorWrapper& selected) const;
        void DrawChildMenu(AI::BehaviorWrapper& selected);
        void CheckSelectedNode(BehaviorWrapper& behavior);
        void DrawNodes();
        void DrawNodesPreview(const BehaviorTreeContext& context);
        void DrawUpperMenu();

        void LoadBehavior(nlohmann::json& json, BehaviorWrapper& behavior, int& idToSet);
        void LoadBT();
        void SaveBT();

        void SaveBehavior(nlohmann::json& stream, const BehaviorWrapper& behavior);

        int GetBiggestId(const BehaviorWrapper& node);
        int GetIdToSet();

        BehaviorWrapper editorRoot;
        BehaviorWrapper previewRoot;

        const BehaviorTree* previewedTree = nullptr;

        ax::NodeEditor::EditorContext* m_context = nullptr;
        ax::NodeEditor::EditorContext* m_previewContext = nullptr;
        std::vector<std::reference_wrapper<BehaviorWrapper>> m_selectedNodes{};
        std::vector<int> m_removedIds;
    };

}
