#include "pch.h"
#include "MenuItem.h"

MenuItem::MenuItem(LPCWSTR i_Icon, LPCWSTR i_Title, EXPCMDFLAGS i_Flags, EXPCMDSTATE i_CmdState, LPCWSTR i_ToolTip, Action i_Event, SubCommands i_SubCmds)
{
    CommonInit(i_Icon, i_Title, i_CmdState, i_ToolTip);
    Flags = i_Flags;
    Event = i_Event;
    SubCmds = i_SubCmds;
}

MenuItem::MenuItem(LPCWSTR i_Icon, LPCWSTR i_Title, Action i_Event, EXPCMDSTATE i_CmdState, LPCWSTR i_ToolTip)
{
    CommonInit(i_Icon, i_Title, i_CmdState, i_ToolTip);
    Flags = ECF_DEFAULT;
    Event = i_Event;
}

MenuItem::MenuItem(LPCWSTR i_Icon, LPCWSTR i_Title, SubCommands i_SubCmds, EXPCMDSTATE i_CmdState)
{
    CommonInit(i_Icon, i_Title, i_CmdState, nullptr);
    Flags = ECF_HASSUBCOMMANDS;
    SubCmds = i_SubCmds;
}

void MenuItem::CommonInit(LPCWSTR i_Icon, LPCWSTR i_Title, EXPCMDSTATE i_CmdState, LPCWSTR i_ToolTip)
{
    Icon = i_Icon;
    Title = i_Title;
    CmdState = i_CmdState;
    ToolTip = i_ToolTip;
}

//IExplorerCommand
HRESULT MenuItem::GetIcon(IShellItemArray*, LPWSTR* p_Icon)
{
    return SHStrDup(Icon, p_Icon);
}

HRESULT MenuItem::GetTitle(IShellItemArray*, LPWSTR* p_Title)
{
    if (Flags == ECF_ISSEPARATOR)
    {
        p_Title = nullptr;
        return S_FALSE;
    }
    return SHStrDup(Title, p_Title);
}

HRESULT MenuItem::GetFlags(EXPCMDFLAGS* pFlags)
{
    *pFlags = Flags;
    return S_OK;
}

HRESULT MenuItem::GetState(IShellItemArray*, [[maybe_unused]] BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)
{
    *pCmdState = CmdState;
    return S_OK;
}

HRESULT MenuItem::GetToolTip(IShellItemArray*, LPWSTR* p_InfoTip)
{
    return SHStrDup(ToolTip, p_InfoTip);
}

HRESULT MenuItem::Invoke(IShellItemArray* psiItemArray, IBindCtx*)
{
    if (Event != nullptr)
    {
        Event(psiItemArray);
        psiItemArray->Release();
    }
    return S_OK;
}

HRESULT MenuItem::EnumSubCommands(IEnumExplorerCommand** pp_Enum)
{
    if (Flags == ECF_HASSUBCOMMANDS && CmdState == ECS_ENABLED && SubCmds.size() != 0)
    {
        *pp_Enum = this;
        AddRef();
        return S_OK;
    }
    else
    {
        *pp_Enum = nullptr;
        return E_NOTIMPL;
    }
}

HRESULT MenuItem::GetCanonicalName(GUID* pguidCommandName)
{
    pguidCommandName = nullptr;
    return E_NOTIMPL;
}

//IEnumExplorerCommand
HRESULT MenuItem::Next(ULONG celt, __out_ecount_part(celt, *pceltFetched) IExplorerCommand** pp_Command, ULONG* pceltFetched)
{
    ULONG fetched = 0;
    while (i < SubCmds.size() && fetched < celt)
    {
        pp_Command[0] = SubCmds[i].get();
        pp_Command[0]->AddRef();
        ++fetched;
        ++i;
    }
    if (pceltFetched) { *pceltFetched = fetched; }
    return (fetched == celt) ? S_OK : S_FALSE;
}

HRESULT MenuItem::Reset()
{
    i = 0;
    return S_OK;
}

HRESULT MenuItem::Clone([[maybe_unused]] IEnumExplorerCommand** ppEnum)
{
    return E_NOTIMPL;
}

HRESULT MenuItem::Skip([[maybe_unused]] ULONG celt)
{
    return E_NOTIMPL;
}
