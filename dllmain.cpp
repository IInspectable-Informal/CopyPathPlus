#include "pch.h"
#include "ClassFactory.hpp"
#include "MenuItem.h"
#include "ShellItemUtils.hpp"
#include "StringUtils.hpp"
#include "ClipboardUtils.hpp"

HMODULE g_DllHandle{ nullptr };
wchar_t g_DllPath[MAX_PATH]{ 0 };

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_DllHandle = hModule;
        GetModuleFileNameW(hModule, g_DllPath, MAX_PATH);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

bool GetTheme()
{
    HKEY key{ nullptr };
    DWORD value{ 1 };
    if (RegOpenKeyExW(HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)", 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS)
    {
        static DWORD rtype{ REG_DWORD };
        static DWORD size{ sizeof(value) };
        if (RegQueryValueExW(key, L"AppUseLightTheme", nullptr, &rtype, reinterpret_cast<LPBYTE>(&value), &size) != ERROR_SUCCESS)
        { value = 1; }
        RegCloseKey(key);
    }
    return value;
}

class __declspec(uuid("D07AC24F-A624-4009-ABC4-7C298F79C5AC")) RootMenu : public MenuItem
{
public:
    RootMenu()
    {
        m_Flags = ECF_HASSUBCOMMANDS;
        m_Icon = std::wstring{ g_DllPath } + (GetTheme() ? L",0" : L",1");
        m_Title = L"复制路径+";
        m_SubCommands = std::move(SubCommands
        {
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,107", L"对象绝对路径",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                SortPaths(paths);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,107", L"对象绝对路径（自动添加双引号）",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                SortPaths(paths);
                AddQuot(paths, false);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,107", L"对象绝对路径（统一添加双引号）",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                SortPaths(paths);
                AddQuot(paths, true);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,3", L"父文件夹路径",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                GetParents(paths);
                MergeRepeatedItems(paths);
                SortPaths(paths);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,3", L"父文件夹路径（自动添加双引号）",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                GetParents(paths);
                MergeRepeatedItems(paths);
                SortPaths(paths);
                AddQuot(paths, false);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,3", L"父文件夹路径（统一添加双引号）",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                GetParents(paths);
                MergeRepeatedItems(paths);
                SortPaths(paths);
                AddQuot(paths, true);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,27", L"所在驱动器路径",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                GetDrivePathOrLetter(paths, false);
                MergeRepeatedItems(paths);
                SortPaths(paths);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,27", L"所在驱动器路径（统一添加双引号）",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                GetDrivePathOrLetter(paths, false);
                MergeRepeatedItems(paths);
                SortPaths(paths);
                AddQuot(paths, true);
                CopyToClipboard(JoinStrings(paths));
            }),
            winrt::make<MenuItem>(L"%windir%\\System32\\imageres.dll,27", L"所在驱动器号",
            [](IShellItemArray* itemArray)
            {
                auto paths{ GetSelectedItemPathList(itemArray) };
                GetDrivePathOrLetter(paths, true);
                MergeRepeatedItems(paths);
                SortPaths(paths);
                CopyToClipboard(JoinStrings(paths));
            }),
        });
    }
};

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    try
    {
        if (rclsid == __uuidof(RootMenu))
        { return winrt::make<ClassFactory<RootMenu>>().as(riid, ppv); }
        else
        { return CLASS_E_CLASSNOTAVAILABLE; }
    }
    catch (...)
    { return winrt::to_hresult(); }
}

STDAPI DllCanUnloadNow()
{
    if (winrt::get_module_lock())
    { return S_FALSE; }
    else
    {
        winrt::clear_factory_cache();
        return S_OK;
    }
}
