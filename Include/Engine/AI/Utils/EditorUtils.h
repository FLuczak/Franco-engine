#pragma once
#include <sstream>
#include <string>

#include "windows.h"

namespace AI
{
namespace ComparatorsInEditor
{
inline const std::array<std::string, 5> comparatorTypes = {"int", "float", "double", "bool", "string"};
inline const std::array<std::string, 6> comparisonTypes = {"equal","not equal", "less","less equal", "greater",   "greater equal"};
}  // namespace ComparatorsInEditor



struct ComparatorWrapper
{
    std::string key = "";
    std::string type = "int";
    std::string comparisonType = "equal";
    std::string value;

    int GetComparisonTypeNum() const;
    std::string ToString() const;
};

namespace BehaviorsInBTEditor
{
enum class BehaviorWrapperType
{
    DECORATOR,
    ACTION,
    COMPOSITE
};

inline std::vector<std::string> behaviorEditorType{"Action",   "Comparison", "Selector",       "Sequence","Repeater", "Inverter",   "Always Succeed", "Until Fail"};
inline std::unordered_map<std::string, BehaviorWrapperType> behaviorWrapperTypes
{
    {"Action", BehaviorWrapperType::ACTION},
    {"Comparison", BehaviorWrapperType::DECORATOR},
    {"Selector", BehaviorWrapperType::COMPOSITE},
    {"Sequence", BehaviorWrapperType::COMPOSITE},
    {"Repeater", BehaviorWrapperType::DECORATOR},
    {"Inverter", BehaviorWrapperType::DECORATOR},
    {"Always Succeed", BehaviorWrapperType::DECORATOR},
    {"Until Fail", BehaviorWrapperType::DECORATOR}
};

}  // namespace BehaviorsInBTEditor

namespace Dialogs
{
static std::string OpenFileSaveDialog()
{
    OPENFILENAMEA file;
    CHAR szFile[260] = {0};
    ZeroMemory(&file, sizeof(OPENFILENAMEA));
    file.lStructSize = sizeof(OPENFILENAMEA);

    wchar_t pBuf[256] = {};
    size_t len = sizeof(pBuf);
    GetModuleFileName(nullptr, pBuf, static_cast<DWORD>(len));

    std::wstring s = pBuf;
#pragma warning(push)
#pragma warning(disable : 4244)
    const std::string string(s.begin(), s.end());
#pragma warning(pop)

    file.lpstrInitialDir = string.c_str();
    file.lpstrFilter = ".txt";
    file.lpstrFile = szFile;
    file.nMaxFile = sizeof(szFile);
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileNameA(&file) == TRUE)
    {
        std::string filename = file.lpstrFile;

        return filename;
    }

    return std::string();
}

static std::string OpenFileLoadDialog()
{
    OPENFILENAMEA file;
    CHAR szFile[260] = {0};
    ZeroMemory(&file, sizeof(OPENFILENAMEA));
    file.lStructSize = sizeof(OPENFILENAMEA);

    wchar_t pBuf[256] = {};
    size_t len = sizeof(pBuf);
    GetModuleFileName(nullptr, pBuf, static_cast<DWORD>(len));

    std::wstring s = pBuf;

#pragma warning(push)
#pragma warning(disable : 4244)
    std::string string(s.begin(), s.end());
#pragma warning(pop)

    file.lpstrInitialDir = string.c_str();
    file.lpstrFilter = ".txt";
    file.lpstrFile = szFile;
    file.nMaxFile = sizeof(szFile);
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_READONLY;

    if (GetOpenFileNameA(&file) == TRUE)
    {
        return std::string(file.lpstrFile);
    }

    return std::string();
}
}  // namespace Dialogs

inline int AI::ComparatorWrapper::GetComparisonTypeNum() const
{
    for (int i = 0; i < ComparatorsInEditor::comparisonTypes.size(); i++)
    {
        if (ComparatorsInEditor::comparisonTypes[i] == comparisonType) return i;
    }
    return 0;
}

inline std::string AI::ComparatorWrapper::ToString() const
{
    std::stringstream toReturn;
    if (key.empty()) return {};
    if (type.empty()) return {};

    toReturn << key << " ";
    toReturn << type << " ";
    toReturn << GetComparisonTypeNum() << " ";
    toReturn << value << " ";
    return toReturn.str();
}
}  // namespace AI
