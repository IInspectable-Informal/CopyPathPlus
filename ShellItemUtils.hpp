#pragma once
#include "pch.h"

std::vector<std::wstring> GetSelectedItemPathList(IShellItemArray* itemArray)
{
    DWORD count{ 0 };
    if (itemArray && SUCCEEDED(itemArray->GetCount(&count)))
    {
        std::vector<std::wstring> buffer{  };
        buffer.reserve(count);
        for (DWORD i = 0; i < count; ++i)
        {
            IShellItem* item{ nullptr };
            if (SUCCEEDED(itemArray->GetItemAt(i, &item)))
            {
                wchar_t* wstrBuffer{ nullptr };
                if (SUCCEEDED(item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wstrBuffer)))
                {
                    if (*wstrBuffer != L':')
                    { buffer.emplace_back(wstrBuffer); }
                    CoTaskMemFree(wstrBuffer);
                }
                item->Release();
            }
        }
        return buffer;
    }
    else
    { return {}; }
}
