#include "pch.h"
#include "MenuItem.h"

MenuItem::MenuItem(std::wstring i_Icon, std::wstring i_Title,
                   EXPCMDFLAGS i_Flags, EXPCMDSTATE i_CmdState, std::wstring i_ToolTip,
                   const Action& i_Event, const SubCommands& i_SubCmds) :
    Event(std::move(i_Event)) ,
    SubCmds(std::move(i_SubCmds))
{
    CommonInit(i_Icon, i_Title, i_CmdState);
    Flags = i_Flags;
    ToolTip = i_ToolTip;
}

MenuItem::MenuItem(std::wstring i_Icon, std::wstring i_Title,
                   const Action& i_Event,
                   EXPCMDSTATE i_CmdState, std::wstring i_ToolTip) :
    Event(std::move(i_Event))
{
    CommonInit(i_Icon, i_Title, i_CmdState);
    Flags = ECF_DEFAULT;
    Event = i_Event;
    ToolTip = i_ToolTip;
}

MenuItem::MenuItem(std::wstring i_Icon, std::wstring i_Title,
                   const SubCommands& i_SubCmds,
                   EXPCMDSTATE i_CmdState) :
    SubCmds(std::move(i_SubCmds))
{
    CommonInit(i_Icon, i_Title, i_CmdState);
    Flags = ECF_HASSUBCOMMANDS;
}

void MenuItem::CommonInit(std::wstring i_Icon, std::wstring i_Title, EXPCMDSTATE i_CmdState)
{
    Icon = i_Icon;
    Title = i_Title;
    CmdState = i_CmdState;
}

//IExplorerCommand
HRESULT MenuItem::GetIcon(IShellItemArray*, LPWSTR* p_Icon)
{
    if (Flags == ECF_ISSEPARATOR)
    {
        *p_Icon = nullptr;
        return S_FALSE;
    }
    return SHStrDup(Icon.c_str(), p_Icon);
}

HRESULT MenuItem::GetTitle(IShellItemArray*, LPWSTR* p_Title)
{
    if (Flags == ECF_ISSEPARATOR)
    {
        *p_Title = nullptr;
        return S_FALSE;
    }
    return SHStrDup(Title.c_str(), p_Title);
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
    return SHStrDup(ToolTip.c_str(), p_InfoTip);
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

HRESULT MenuItem::GetCanonicalName([[maybe_unused]] GUID* pguidCommandName)
{
    return E_NOTIMPL;
}

//IEnumExplorerCommand
HRESULT MenuItem::Next(ULONG celt, __out_ecount_part(celt, *pceltFetched) IExplorerCommand** pp_Command, ULONG* pceltFetched)
{
    IExplorerCommand** commands = pp_Command;
    ULONG fetched = 0;
    while (i < SubCmds.size() && fetched < celt)
    {
        *commands = SubCmds[i].get();
        (*commands)->AddRef();
        ++commands;
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
