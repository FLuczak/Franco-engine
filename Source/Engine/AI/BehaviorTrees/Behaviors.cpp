#include "Engine/AI/BehaviorTrees/Behaviors.hpp"

void AI::Behavior::Reset(BehaviorTreeContext& context)
{
    context.statuses[m_id] = Status::INVALID;
}

AI::Status AI::Behavior::Execute(BehaviorTreeContext& context)
{
    Status& status = context.statuses[m_id];
    if (status != Status::RUNNING)
    {
        Initialize(context);
    }

    status = Tick(context);

    if (status != Status::RUNNING)
    {
        End(context, status);
    }

    context.statuses[m_id] = status;
    return status;
}

void AI::Composite::Reset(BehaviorTreeContext& context)
{
    Behavior::Reset(context);
    for (auto& child : m_children)
    {
        child->Reset(context);
    }
}

void AI::Composite::RemoveChild(Behavior* child)
{
    m_children.erase(std::remove_if(m_children.begin(), m_children.end(),[child](const std::unique_ptr<Behavior>& behavior)
    {
        return behavior.get() == child;
    }), m_children.end());
}

void AI::Decorator::Reset(BehaviorTreeContext& context)
{
    Behavior::Reset(context);
    m_child->Reset(context);
}

AI::Status AI::Condition::Tick(BehaviorTreeContext& context)
{
    if (m_function())
    {
        if (!m_isNegation)
        {
            m_child->Execute(context);
        }
        return m_isNegation ? Status::FAILURE : Status::SUCCESS;
    }

    if (m_isNegation)
    {
        m_child->Execute(context);
    }
    return m_isNegation ? Status::SUCCESS : Status::FAILURE;
}

AI::Status AI::Sequence::Tick(BehaviorTreeContext& context)
{
    bool allSuccess = true;
    for (const auto& child : m_children)
    {
        if (context.statuses[child->GetId()] == Status::SUCCESS) continue;
        allSuccess = false;
        const Status childStatus = child->Execute(context);
        if (childStatus != Status::SUCCESS)
        {
            return childStatus;
        }
    }

    if (allSuccess)
    {
        for (const auto& child : m_children)
        {
            child->Reset(context);
        }
    }

    return Status::SUCCESS;
}

AI::Status AI::Selector::Tick(BehaviorTreeContext& context)
{
    for (const auto& child : m_children)
    {
        const Status childStatus = child->Execute(context);
        if (childStatus != Status::FAILURE)
        {
            for (auto& element : m_children)
            {
                if (element == child)continue;
                element->Reset(context);
            }
            return childStatus;
        }
    }

    return Status::FAILURE;
}

AI::Status AI::Repeater::Tick(BehaviorTreeContext& context)
{
    for (int i = 0 ; i < m_numRepeats;i++)
    {
        m_child->Execute(context);
        if (context.statuses[m_id] == Status::RUNNING) return Status::SUCCESS;
        if (context.statuses[m_id] == Status::FAILURE) return Status::FAILURE;
        m_child->Reset(context);
    }

    return Status::SUCCESS;
}

AI::Status AI::Inverter::Tick(BehaviorTreeContext& context)
{
    const Status status = m_child->Execute(context);
    if(status == Status::FAILURE) return Status::SUCCESS;
    if(status == Status::SUCCESS) return Status::FAILURE;
    return Status::INVALID;
}

AI::Status AI::AlwaysSucceed::Tick(BehaviorTreeContext& context)
{
    m_child->Execute(context);
    return Status::SUCCESS;
}

AI::Status AI::UntilFail::Tick(BehaviorTreeContext& context)
{
    auto status = m_child->Execute(context);
    if ( status != Status::FAILURE)
    {
        return Status::SUCCESS;
    }

    return Status::FAILURE;
}
