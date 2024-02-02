#pragma once
#include <imgui_node_editor.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

#include "imgui.h"
#include "Engine/AI/FiniteStateMachines/FiniteStateMachine.hpp"
#include "Engine/AI/Utils/EditorUtils.h"
#include "Engine/EditorVariables.h"


namespace AI
{
struct StateMachineContext;

    struct NodeWrapper
    {
        NodeWrapper(){};
        ImVec2 position{0, 0};
        size_t id = 1;
        std::string name = "Test";    
        std::string stateType;
        std::unordered_map<std::string, std::string> editorVariables{};
        std::unique_ptr<State> underlyingState;
    };

    struct NodeTransition
    {
        int nodeTo = -1;
        std::vector<ComparatorWrapper> comparators{};
    };

    struct EditorFSMWrapper
    {
        std::vector<std::unique_ptr<NodeWrapper>> nodes;
        std::unordered_map<size_t, std::vector<NodeTransition>> transitions;
    };

    class FiniteStateMachineEditor
    {
    public:
        FiniteStateMachineEditor();
        void Update();
        void PreviewContext(const FiniteStateMachine& fsm, const StateMachineContext& context);
    private:

        std::vector<size_t> m_selectedNodes{};
        bool open = false;

        size_t m_hovered = -1;

        ImVec2 m_mousePositionOnMenu = {};
        std::vector<size_t> m_removedIds;
        ax::NodeEditor::EditorContext* m_previewContext = nullptr;

        const FiniteStateMachine* m_previewedFSM = nullptr;

        EditorFSMWrapper m_editorFSM;
        EditorFSMWrapper m_previewFSM;

        bool m_clicked = false;
        bool m_justOpenedMenu = false;
        bool m_reloadPositions = true;

        ax::NodeEditor::EditorContext* m_context = nullptr;
        size_t m_defaultState = 0;
        int m_maxNumStates = 1000;

        void DrawTransitions(EditorFSMWrapper& wrapper) const;
        void DeleteNode(size_t nodeIdToRemove);
        void ResetHoverInfo();
        void DrawNode(const AI::EditorFSMWrapper& wrapper, size_t i);

        void DrawNodes(const EditorFSMWrapper& wrapper);
        void DrawNodes(const EditorFSMWrapper& wrapper, const StateMachineContext& context);
        std::vector<std::string> GetStateNames() const;

        void DrawSelectedStateDetails();
        static void SelectedStateNameInputBox(const std::unique_ptr<AI::NodeWrapper>& focusedNode);
        void SelectedStateTypeSelection(const std::unique_ptr<AI::NodeWrapper>& focusedNode) const;
        void SelectedNodeTransitionButtons(const std::unique_ptr<AI::NodeWrapper>& focusedNode, int index, int stateToGoToChoice,AI::NodeTransition& transition) const;
        void SelectedNodeTransitionComparatorButtons(int index, std::vector<int>& transitionsToRemove,AI::NodeTransition& transition) const;
        void ComparatorValueInput(ComparatorWrapper& comparator, const char* label) const;
        void ChooseComparatorTypeInput(std::vector<AI::ComparatorWrapper>::value_type& comparator, const char* label) const;
        void ChooseComparisonTypeInput(std::vector<AI::ComparatorWrapper>::value_type& comparator, const char* label) const;
        void EditorVariableInput(std::unique_ptr<AI::NodeWrapper>& focusedNode, std::pair<const std::string, EditorVariable*>& editorVariable) const;

        void DrawUpperMenu();

        void SaveStates(nlohmann::json& states) const;
        void SaveTransitions(nlohmann::json& json) const;
        void SaveEditorData(nlohmann::json& json) const;
        void HandleClicks();
        void DrawPopupMenu();

        void SaveFSM() const;
        size_t GetIdToSet();
        void CreateNode();
        void LoadStates(nlohmann::json& states, EditorFSMWrapper& wrapper);
        static void DeserializeComparators(nlohmann::json& comparators, NodeTransition& node_transition);
        static void LoadTransitions(nlohmann::json& transitionData, EditorFSMWrapper& wrapper);
        static void LoadEditorData(const nlohmann::json& json, EditorFSMWrapper& wrapper);
        void LoadFSM();
    };
}
