#include "pch.h"
#include "MenuItem.h"

MenuItem::MenuItem()
{

}

MenuItem::MenuItem(EXPCMDFLAGS flags, std::wstring const& icon, std::wstring const& title, EXPCMDSTATE state,
                   Action const& action, SubCommands const& subcmds) :
    m_Flags(flags), m_Icon(icon), m_Title(title), m_State(state),
    m_Action(std::move(action)), m_SubCommands(std::move(subcmds))
{
    
}

MenuItem::MenuItem(std::wstring const& icon, std::wstring const& title,
                   Action const& action, EXPCMDSTATE state) :
    MenuItem(ECF_DEFAULT, icon, title, state, action, {})
{
    
}

MenuItem::MenuItem(std::wstring const& icon, std::wstring const& title,
                   SubCommands const& subcmds, EXPCMDSTATE state) :
    MenuItem(ECF_HASSUBCOMMANDS, icon, title, ECS_ENABLED, nullptr, subcmds)
{

}

//IExplorerCommand
HRESULT STDMETHODCALLTYPE MenuItem::GetTitle(IShellItemArray* psiItemArray, LPWSTR* ppszName)
{
    if ((m_Flags & ECF_ISSEPARATOR) != ECF_ISSEPARATOR)
    { return SHStrDupW(m_Title.c_str(), ppszName); }
    else
    { return S_FALSE; }
}

HRESULT STDMETHODCALLTYPE MenuItem::GetIcon(IShellItemArray* psiItemArray, LPWSTR* ppszIcon)
{
    if ((m_Flags & ECF_ISSEPARATOR) != ECF_ISSEPARATOR)
    { return SHStrDupW(m_Icon.c_str(), ppszIcon); }
    else
    {
        *ppszIcon = nullptr;
        return E_NOTIMPL;
    }
}

HRESULT STDMETHODCALLTYPE MenuItem::GetToolTip(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MenuItem::GetCanonicalName(GUID* pguidCommandName)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MenuItem::GetState(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)
{
    *pCmdState = m_State;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE MenuItem::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc)
{
    m_Action(psiItemArray);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE MenuItem::GetFlags(EXPCMDFLAGS* pFlags)
{
    *pFlags = m_Flags;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE MenuItem::EnumSubCommands(IEnumExplorerCommand** ppEnum)
{
    if ((m_Flags & ECF_HASSUBCOMMANDS) == ECF_HASSUBCOMMANDS && m_SubCommands.size() != 0)
    {
        *ppEnum = this;
        AddRef();
        return S_OK;
    }
    else
    { return E_NOTIMPL; }
}

//IEnumExplorerCommand
HRESULT STDMETHODCALLTYPE MenuItem::Next(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched)
{
    IExplorerCommand** commands = pUICommand;
    ULONG fetched = 0;
    while (m_Index < m_SubCommands.size() && fetched < celt)
    {
        *commands = m_SubCommands[m_Index].get();
        (*commands)->AddRef();
        ++commands;
        ++fetched;
        ++m_Index;
    }
    if (pceltFetched) { *pceltFetched = fetched; }
    return (fetched == celt) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE MenuItem::Skip(ULONG celt)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MenuItem::Reset()
{
    m_Index = 0;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE MenuItem::Clone(IEnumExplorerCommand** ppenum)
{
    return E_NOTIMPL;
}
