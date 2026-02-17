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
    MenuItem(std::wstring, std::wstring, EXPCMDFLAGS, EXPCMDSTATE, std::wstring, Action, SubCommands = {});
    MenuItem(std::wstring, std::wstring, Action, EXPCMDSTATE = ECS_ENABLED, std::wstring = L"");
    MenuItem(std::wstring, std::wstring, SubCommands, EXPCMDSTATE = ECS_ENABLED);

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

protected:
    //Properties
    std::wstring Icon{ L"" };
    std::wstring Title{ L"" };
    EXPCMDFLAGS Flags{ ECF_ISSEPARATOR };

    EXPCMDSTATE CmdState{ ECS_ENABLED };

    std::wstring ToolTip{ L"" };

    Action Event;

    SubCommands SubCmds = {  };

private:
    void CommonInit(std::wstring, std::wstring, EXPCMDSTATE, std::wstring);

    ULONG i = 0;
};
