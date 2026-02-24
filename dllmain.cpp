#include "pch.h"
#include "ClassFactory.h"
#include "MenuItem.h"
#include "ClipboardUtils.h"
#include "StringUtils.h"

//Windows
#include <winreg.h>

HMODULE g_HMD = nullptr;
wchar_t g_ModulePath[MAX_PATH];

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_HMD = hModule;
        GetModuleFileNameW(hModule, g_ModulePath, MAX_PATH);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

unsigned long GetTheme()
{
    HKEY key = nullptr;
    unsigned long value = 1;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)", 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS)
    {
        unsigned long rtype = REG_DWORD;
        unsigned long size = sizeof(value);
        if (RegQueryValueExW(key, L"AppsUseLightTheme", nullptr, &rtype, (LPBYTE)&value, &size) != ERROR_SUCCESS)
        { value = 1; }
        RegCloseKey(key);
    } return value;
}

class __declspec(uuid("D07AC24F-A624-4009-ABC4-7C298F79C5AC")) MenuRoot : public MenuItem
{
public:
    MenuRoot()
    {
        Icon = std::wstring(g_ModulePath) + (GetTheme() ? L",0" : L",1");
        Title = L"复制路径+";
        Flags = ECF_HASSUBCOMMANDS;
        SubCmds = std::move(SubCommands
        {
            winrt::make<MenuItem>
            (
                L"imageres.dll,107", L"对象绝对路径",
                Action([this](IShellItemArray* psiItemArray)
                { CopyString(psiItemArray, Object); })
            ),
            winrt::make<MenuItem>
            (
                L"imageres.dll,107", L"对象绝对路径（自动添加双引号）",
                Action([this](IShellItemArray* psiItemArray)
                { CopyString(psiItemArray, Object, true); })
            ),
            winrt::make<MenuItem>(),
            winrt::make<MenuItem>
            (
                L"imageres.dll,3", L"父文件夹路径",
                Action([this](IShellItemArray* psiItemArray)
                { CopyString(psiItemArray, Parent); })
            ),
            winrt::make<MenuItem>
            (
                L"imageres.dll,3", L"父文件夹路径（自动添加双引号）",
                Action([this](IShellItemArray* psiItemArray)
                { CopyString(psiItemArray, Parent, true); })
            ),
            winrt::make<MenuItem>(),
            winrt::make<MenuItem>
            (
                L"imageres.dll,27", L"所在驱动器路径",
                Action([this](IShellItemArray* psiItemArray)
                { CopyString(psiItemArray, Drive); })
            ),
            winrt::make<MenuItem>
            (
                L"imageres.dll,27", L"所在驱动器号",
                Action([this](IShellItemArray* psiItemArray)
                { CopyString(psiItemArray, DriveLetter); })
            )
        });
    }
};

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    try
    {
        *ppv = nullptr;
        if (rclsid == __uuidof(MenuRoot))
        { return winrt::make_self<ClassFactory<MenuRoot>>().as(riid, ppv); }
        return CLASS_E_CLASSNOTAVAILABLE;
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
