#include <string>

#include "SFML/OpenGL.hpp"

namespace Dialogs
{
    static std::string OpenFileSaveDialog()
    {
        OPENFILENAMEA file;
        CHAR szFile[260] = { 0 };
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
        CHAR szFile[260] = { 0 };
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

    static std::string OpenFileLoadDialog(std::string fileFilter)
    {
        OPENFILENAMEA file;
        CHAR szFile[260] = { 0 };
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
        file.lpstrFilter = fileFilter.c_str();
        file.lpstrFile = szFile;
        file.nMaxFile = sizeof(szFile);
        file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_READONLY | OFN_EXPLORER;

        if (GetOpenFileNameA(&file) == TRUE)
        {
            return std::string(file.lpstrFile);
        }

        return std::string();
    }
}  // namespace Dialogs
