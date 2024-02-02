#include "Engine/AI/Blackboards/Blackboard.hpp"

#include <string>
#include <vector>

std::vector<std::pair<std::string, std::string>> AI::Blackboard::PreviewToString()
{
    std::vector<std::pair<std::string, std::string>> toReturn = {};

    for (auto& pair :  m_Map)
    {
        toReturn.push_back({pair.first, pair.second->ToString()});
    }

    return toReturn;
}
