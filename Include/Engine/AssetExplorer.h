#pragma once
#include <vector>
#include <filesystem>
#include <initializer_list>

struct ProjectFile
{
    std::filesystem::directory_entry m_fileDirectory{};
    std::string m_Icon = "";
    bool m_isOpen = false;
};

class AssetExplorer 
{
public:
    AssetExplorer();

    void Update(float dt);

#ifdef BEE_INSPECTOR
    void Inspect() override;
#endif

    /// <summary>
    /// Set the ImGui widget above this line as a DragDrop source.
    /// </summary>
    /// <param name="file">The file you want to drag to somewhere else.</param>
    /// <returns>TRUE if the resource is being dragged in the current frame.</returns>
    static bool SetDragDropSource(ProjectFile file);

    /// <summary>
    /// Set the ImGui widget above this line as a DragDrop target.
    /// </summary>
    /// <param name="payloadData">A return argument in which the asset path will be written so you can use it.</param>
    /// <param name="dataType">What data type should the DragDropTarget look for. Currently, it works with the file's extensions. Use something like {".gltf", ".glb", etc.}</param>
    /// <returns>TRUE if the payloadData was updated with the asset's path.</returns>
    static bool SetDragDropTarget(std::filesystem::path& payloadData, const std::initializer_list<std::string>& dataTypes);

private:  // private functions
    void PrintAllFolders();
    void ExtractFilesFromFolder(const std::filesystem::path& path);
    void DisplayFolderContents(const std::filesystem::path& root);
    void DisplayFilteredContents();

    void DisplayOnlyFilesInFolder(std::filesystem::path directory) const;
    static const std::string DetermineIcon(const std::string& extension);

    /// <summary>
    /// Says whether the entry's filename matches the current filter.
    /// </summary>
    /// <param name="entry">The entry you want to check the filter against.</param>
    /// <returns>TRUE if it should be displayed.</returns>
    bool IsEntryFiltered(std::filesystem::directory_entry entry) const;


private:  // private parameters
    std::vector<std::filesystem::directory_entry> m_folders{};

    std::vector<ProjectFile> m_files{};

    std::filesystem::path directory = "";
    std::string filter = "";

    int m_numberOfFiles = 0;
    int m_numberOfDirectories = 0;
};
