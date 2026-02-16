#pragma once
#include "StringUtils.h"

//Windows
#include <ShObjIdl_core.h>

//STL
#include <algorithm>
#include <functional>
#include <vector>

constexpr wchar_t g_FailedWstr[] = L"Cannot get path.";

enum GetPathOptions
{
    Object,
    Parent,
    Drive,
    DriveLetter
};

template<typename TElem>
bool HasElement(std::vector<TElem> const& vec, TElem const& specelem)
{
    for (TElem const& elem : vec)
    { if (elem == specelem) { return true; } }
    return false;
}

std::vector<std::wstring> GetPaths(IShellItemArray* psiItemArray, GetPathOptions Option = Object)
{
    std::vector<std::wstring> wstrs;
    unsigned long c = 0;
    psiItemArray->GetCount(&c);
    std::vector<wchar_t*> strs;
    strs.resize(c, nullptr);
    for (unsigned long i = 0; i < c; i++)
    {
        IShellItem* Item = nullptr;
        if (SUCCEEDED(psiItemArray->GetItemAt(i, &Item)))
        {
            if (FAILED(Item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &strs[i])))
            { strs[i] = const_cast<wchar_t*>(g_FailedWstr); }
            Item->Release();
        }
        else
        { strs[i] = const_cast<wchar_t*>(g_FailedWstr); }
    }
    for (wchar_t* const& str : strs)
    {
        if (HasSpecWchar(str, L'\\'))
        {
            switch (Option)
            {
                case Object:
                { wstrs.push_back(str); break; }
                case Parent:
                {
                    std::wstring twstr = str;
                    twstr = twstr.substr(0, twstr.rfind(L'\\'));
                    if (!HasElement<std::wstring>(wstrs, twstr))
                    { wstrs.push_back(twstr); } break;
                }
                default:
                {
                    std::wstring twstr = std::wstring(str).substr(0, Option == Drive ? 3 : 1);
                    if (!HasElement<std::wstring>(wstrs, twstr))
                    { wstrs.push_back(twstr); } break;
                }
            }
        }
    }
    std::sort(wstrs.begin(), wstrs.end());
    return wstrs;
}

void CopyToClipboard(std::wstring const& wstr)
{
    if (OpenClipboard(NULL))
    {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, wstr.size() * sizeof(wchar_t));
        void* pv = GlobalLock(hMem);
        wcscpy_s((wchar_t*)pv, wstr.size() * sizeof(wchar_t), wstr.c_str());
        GlobalUnlock(hMem);
        SetClipboardData(CF_UNICODETEXT, hMem);
        CloseClipboard();
    }
}

void CopyString(IShellItemArray* psiItemArray, GetPathOptions Option, bool AutoAddQuots = false)
{
    std::wstring wstr;
    for (auto const& swstr : GetPaths(psiItemArray, Option))
    {
        if (swstr != g_FailedWstr && AutoAddQuots && HasSpaceWchar(swstr))
        { wstr += L"\"" + swstr + L"\"\n"; }
        else { wstr += swstr + L"\n"; }
    } CopyToClipboard(wstr);
}

