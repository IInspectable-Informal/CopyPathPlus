#pragma once
#include "pch.h"

void CopyToClipboard(const std::wstring& str)
{
    if (OpenClipboard(nullptr))
    {
        if (EmptyClipboard())
        {
            size_t chars{ str.size() + 1 };
            HGLOBAL hMem{ GlobalAlloc(GMEM_MOVEABLE, chars * sizeof(wchar_t)) };
            if (hMem)
            {
                wchar_t* pv{ reinterpret_cast<wchar_t*>(GlobalLock(hMem)) };
                if (pv)
                {
                    wcsncpy_s(pv, chars, str.c_str(), chars);
                    GlobalUnlock(hMem);
                    if (!SetClipboardData(CF_UNICODETEXT, hMem))
                    { GlobalFree(hMem); }
                }
                else { GlobalFree(hMem); }
            }
        }
        CloseClipboard();
    }
}
