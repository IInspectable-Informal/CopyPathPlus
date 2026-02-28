#pragma once
#include "pch.h"

void GetParents(std::vector<std::wstring>& paths)
{
    for (auto& path : paths)
    {
        size_t pos = path.rfind(L'\\');
        path.resize(pos != std::wstring::npos && path.rfind(L'\\', pos - 1) != std::wstring::npos ? pos : 0);
    }
}

void GetDrivePathOrLetter(std::vector<std::wstring>& paths, bool reserveLetterOnly)
{
    for (auto& path : paths)
    { path.resize(reserveLetterOnly ? 1 : 3); }
}

void MergeRepeatedItems(std::vector<std::wstring>& paths)
{
    std::unordered_set<std::wstring> seen;
    std::vector<std::wstring> result;
    
    for (const auto& path : paths)
    {
        if (path.size() != 0 && seen.find(path) == seen.end())
        {
            seen.insert(path);
            result.push_back(path);
        }
    }
    
    paths = std::move(result);
}

void AddQuot(std::vector<std::wstring>& paths, bool alwaysAdd)
{
    for (auto& path : paths)
    {
        if (alwaysAdd || path.find(L' ') != std::wstring::npos)
        {
            path.insert(0, 1, L'"');
            path.append(1, L'"');
        }
    }
}

void SortPaths(std::vector<std::wstring>& paths)
{
    if (paths.size() > 1)
    {
        std::sort(
            paths.begin(), paths.end(),
            [](const std::wstring& a, const std::wstring& b) -> bool
            { return _wcsicmp(a.c_str(), b.c_str()) < 0; }
        );
    }
}

std::wstring JoinStrings(const std::vector<std::wstring>& paths)
{
    size_t totalSize{ 0 };
    for (const auto& path : paths)
    { totalSize += path.size(); }
    if (paths.size())
    { totalSize += paths.size() - 1; }

    if (totalSize)
    {
        std::wstring result{};
        result.reserve(totalSize);
        for (size_t i = 0; i < paths.size(); ++i)
        {
            if (i > 0) { result += L'\n'; }
            result += paths.at(i);
        }
        return result;
    }
    else { return {}; }
}
