#include "Engine/Editor/AssetExplorer.h"

#include <iostream>

#include "IconsFontAwesome.h"
#include"imgui_stdlib.h"
#include "imgui.h"

AssetExplorer::AssetExplorer()
{
    directory = std::filesystem::current_path() / "Assets";
    ExtractFilesFromFolder(directory);
}

void AssetExplorer::Update(float dt)
{
    if (ImGui::Begin("Asset Explorer"))
    {
        ImGui::InputText("Filter", &filter);
        ImGui::SameLine();
        if (ImGui::Button(std::string(reinterpret_cast<const char*>(u8"\uf021")).c_str()))
        {
            m_files.clear();
            m_folders.clear();
            ExtractFilesFromFolder(directory);
        }
        ImGui::Separator();

        if(ImGui::TreeNodeEx("root", ImGuiTreeNodeFlags_None))
        {
            if (filter == "")
            {
                DisplayFolderContents(directory);
                DisplayOnlyFilesInFolder(directory);
            }
            else
            {
                DisplayFilteredContents();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}


void AssetExplorer::PrintAllFolders() {}

void AssetExplorer::ExtractFilesFromFolder(const std::filesystem::path &path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            m_folders.push_back(entry);
            ExtractFilesFromFolder(entry.path().string());
            std::cout << entry.path().string() << "\n";
        }
        else
        {
            ProjectFile myFile;
            myFile.m_fileDirectory = entry;
            myFile.m_Icon = DetermineIcon(myFile.m_fileDirectory.path().extension().string());
            m_files.push_back(myFile);
        }
    }
}

void AssetExplorer::DisplayFolderContents(const std::filesystem::path& root)
{
    for (const auto& folder : m_folders)
    {
        auto folderRoot = folder.path().parent_path();
        if (folderRoot == root)
        {
            ImGuiTreeNodeFlags folderNodeFlags = ImGuiTreeNodeFlags_None;
            if (!std::filesystem::exists(folder)) continue;
            if (std::filesystem::is_empty(folder)) folderNodeFlags |= ImGuiTreeNodeFlags_Leaf;

            std::string folder_name = std::string(reinterpret_cast<const char*>(u8"\uf07b ")) + folder.path().filename().string();
            if (ImGui::TreeNodeEx((folder_name + "##" + folder.path().string()).c_str(), folderNodeFlags))
            {
                DisplayFolderContents(folder.path());
                DisplayOnlyFilesInFolder(folder.path());

                ImGui::TreePop();
            }
        }
    }
}

void AssetExplorer::DisplayFilteredContents()
{
    for (const auto& folder : m_folders)
    {
        if (IsEntryFiltered(folder))
        {
            ImGuiTreeNodeFlags folderNodeFlags = ImGuiTreeNodeFlags_None;
            if (std::filesystem::is_empty(folder)) folderNodeFlags |= ImGuiTreeNodeFlags_Leaf;
            std::string folderName = std::string(reinterpret_cast<const char*>(u8"\uf07b ")) + folder.path().filename().string();
            if (ImGui::TreeNodeEx((folderName + "##" + folder.path().string()).c_str(), folderNodeFlags))
            {
                DisplayFolderContents(folder.path());
                DisplayOnlyFilesInFolder(folder.path());

                ImGui::TreePop();
            }
        }
    }
    for (const auto& file : m_files)
    {
        if (IsEntryFiltered(file.m_fileDirectory))
        {
            ImGuiTreeNodeFlags fileNodeFlags = ImGuiTreeNodeFlags_Leaf;
            std::string fileName = file.m_Icon + file.m_fileDirectory.path().filename().string();
            if (ImGui::TreeNodeEx((fileName + "##" + file.m_fileDirectory.path().string()).c_str(), fileNodeFlags))
            {
                ImGui::TreePop();
            }
            SetDragDropSource(file);
        }
    }
}

void AssetExplorer::DisplayOnlyFilesInFolder(std::filesystem::path directory) const
{
    ImGuiTreeNodeFlags file_node_flags = ImGuiTreeNodeFlags_Leaf;
    for (const auto& file : m_files)
    {
        const auto& fileRoot = file.m_fileDirectory.path().parent_path();
        if (fileRoot == directory)
        {
            std::string file_name = file.m_Icon + file.m_fileDirectory.path().filename().string();
            if (ImGui::TreeNodeEx((file_name + "##" + file.m_fileDirectory.path().string()).c_str(), file_node_flags))
            {
                ImGui::TreePop();
            }
            SetDragDropSource(file);
        }
    }
}

bool AssetExplorer::IsEntryFiltered(std::filesystem::directory_entry entry) const
{
    std::string mainStringLowercase = entry.path().filename().string();
    std::transform(mainStringLowercase.begin(), mainStringLowercase.end(), mainStringLowercase.begin(), ::towlower);
    std::string subStringLowercase = filter;
    std::transform(subStringLowercase.begin(), subStringLowercase.end(), subStringLowercase.begin(), ::towlower);
    size_t foundPosition = mainStringLowercase.find(subStringLowercase);
    if (foundPosition != std::string::npos)
    {
        return true;
    }
    return false;
}

bool AssetExplorer::SetDragDropSource(ProjectFile file)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        std::string extension = file.m_fileDirectory.path().extension().string();
        std::string path = file.m_fileDirectory.path().string();

    	ImGui::SetDragDropPayload(extension.c_str(), path.c_str(), sizeof(char) * (path.size() + 1));
        ImGui::Text("%s", (file.m_Icon + path).c_str());
        ImGui::EndDragDropSource();
        std::string keyword = "Assets/";

        return true;
    }
    return false;
}

bool AssetExplorer::SetDragDropTarget(std::filesystem::path& payloadData, const std::initializer_list<std::string>& dataTypes)
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

    return false;
}

const std::string AssetExplorer::DetermineIcon(const std::string& extension)
{
    if (extension == ".txt")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf15b "));
    }
    else if (extension == ".gltf" || extension == ".glb")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf1b3 "));
    }
    else if (extension == ".obj")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf1b2 "));
    }
    else if (extension == ".png")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf03e "));
    }
    else if (extension == ".ttf")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf658 "));
    }
    else if (extension == ".json")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf15c "));
    }
    else if (extension == ".wav")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf1c7 "));
    }
    else if (extension == ".ent")
    {
        return std::string(reinterpret_cast<const char*>(u8"\uf1b2 "));
    }

    return "";
}