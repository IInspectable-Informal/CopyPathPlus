#pragma once
//Windows
#if defined(_M_IX86) || defined(__i386__)
#define _X86_
#define WIN32
#elif defined(_M_X64) || defined(__x86_64__)
#define _AMD64_
#elif defined(_M_ARM64) || defined(__aarch64__)
#define _ARM64_
#elif defined(_M_ARM64EC)
#define _ARM64EC_
#elif defined(_M_ARM) || defined(__arm__)
#define _ARM_
#else
#error "Unsupported architecture"
#endif
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windef.h>
#include <combaseapi.h>
#include <unknwn.h>
#include <shobjidl_core.h>
#include <shlwapi.h>

//WinRT
#define WINRT_LEAN_AND_MEAN
#include <winrt/base.h>

//Standard Libraries
#include <functional>
#include <unordered_set>
