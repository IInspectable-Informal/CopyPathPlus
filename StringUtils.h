#pragma once
#include <string>

uint64_t CountSpecWchar(std::wstring const& wstr, wchar_t const& specwch)
{
    uint64_t i = 0;
    for (wchar_t const& wch : wstr)
    { if (wch == specwch) { i++; } }
    return i;
}

bool HasSpecWchar(std::wstring const& wstr, wchar_t const& specwch)
{
    for (wchar_t const& wch : wstr)
    { if (wch == specwch) { return true; } }
    return false;
}

bool HasSpaceWchar(std::wstring const& wstr)
{ return HasSpecWchar(wstr, L' '); }
