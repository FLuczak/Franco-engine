#include "Engine/AI/BehaviorTrees/BehaviorTree.hpp"
#include "Engine/AI/BehaviorTrees/BehaviorTreeBuilder.hpp"

void AI::BehaviorTree::Execute(BehaviorTreeContext& context) const
{
    m_root->Execute(context);
}

std::stringstream AI::BehaviorTree::Serialize() const
{
    BehaviorTreeBuilder builder{};
    return std::stringstream(builder.SerializeToJson(m_root.get()).dump());
}

std::unique_ptr<AI::BehaviorTree> AI::BehaviorTree::Deserialize(std::stringstream& stringstream)
{
    BehaviorTreeBuilder builder{};
    if (stringstream.str().empty()) return {};
    nlohmann::json json = nlohmann::json::parse(stringstream.str());
    auto temp = builder.DeserializeFromJson(json);
    return temp;
}

AI::BehaviorTree::BehaviorTree(std::unique_ptr<Behavior>& rootToSet)
{
    m_root.swap(rootToSet);
}
