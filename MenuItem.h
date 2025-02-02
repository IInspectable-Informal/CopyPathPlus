#pragma once
#include "pch.h"

//Windows
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//STL
#include <functional>

typedef std::function<void(IShellItemArray*)> Action;
typedef std::vector<winrt::com_ptr<IExplorerCommand>> SubCommands;

class MenuItem : public winrt::implements<MenuItem, IExplorerCommand, IEnumExplorerCommand>
{
public:
    MenuItem() = default;
    MenuItem(LPCWSTR, LPCWSTR, EXPCMDFLAGS, EXPCMDSTATE, LPCWSTR, Action, SubCommands = {}, BOOL = true);
    MenuItem(LPCWSTR, LPCWSTR, Action, EXPCMDSTATE = ECS_ENABLED, LPCWSTR = nullptr, BOOL = true);
    MenuItem(LPCWSTR, LPCWSTR, SubCommands, EXPCMDSTATE = ECS_ENABLED, BOOL = true);

    IExplorerCommand* GetPtr() { return this; }

    //IExplorerCommand
    HRESULT __stdcall GetIcon(IShellItemArray*, LPWSTR*);
    HRESULT __stdcall GetTitle(IShellItemArray*, LPWSTR*);
    HRESULT __stdcall GetFlags(EXPCMDFLAGS*);
    HRESULT __stdcall GetState(IShellItemArray*, BOOL, EXPCMDSTATE*);
    HRESULT __stdcall GetToolTip(IShellItemArray*, LPWSTR*);
    HRESULT __stdcall Invoke(IShellItemArray*, IBindCtx*);
    HRESULT __stdcall EnumSubCommands(IEnumExplorerCommand**);
    HRESULT __stdcall GetCanonicalName(GUID*);

    //IEnumExplorerCommand
    HRESULT __stdcall Next(ULONG, IExplorerCommand**, ULONG*);
    HRESULT __stdcall Reset();
    HRESULT __stdcall Clone(IEnumExplorerCommand**);
    HRESULT __stdcall Skip(ULONG);

    //Properties
    LPCWSTR Icon{ L"" };
    LPCWSTR Title{ L"" };
    EXPCMDFLAGS Flags{ ECF_ISSEPARATOR };

    BOOL BeSlow = true;
    EXPCMDSTATE CmdState{ ECS_ENABLED };

    LPCWSTR ToolTip{ L"" };

    Action Event;

    SubCommands SubCmds = {  };

    void CommonInit(LPCWSTR, LPCWSTR, EXPCMDSTATE, LPCWSTR, BOOL);

private:
    ULONG i = 0;
};
