#include "Engine/AI/Utils/BlackboardInspector.hpp"
#include <imgui.h>
#include "Engine/AI/Blackboards/Blackboard.hpp"

void AI::BlackboardInspector::Inspect(AI::Blackboard& blackboardToInspect)
{
    ImGui::Begin("Blackboard", &open);
    for (auto& preview : blackboardToInspect.PreviewToString())
    {
        if (preview.second.find('\n') != std ::string::npos)
        {
            if(ImGui::CollapsingHeader(preview.first.c_str()))
            {
                ImGui::Indent(20);
                ImGui::Text(preview.second.c_str());
                ImGui::Indent(-20);
            }
        }
        else
        {
            ImGui::Text(std::string(preview.first + ": ").c_str());
            ImGui::SameLine();
            ImGui::Text(preview.second.c_str());
        }
    }
    ImGui::End();
}
