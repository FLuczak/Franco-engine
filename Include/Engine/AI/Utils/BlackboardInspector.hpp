#pragma once
#include "Engine/AI/Blackboards/Blackboard.hpp"

namespace AI
{
    class BlackboardInspector
    {
    public:
        BlackboardInspector() = default;
        void Inspect(AI::Blackboard& blackboardToInspect);
        bool open = false;
    };
}
