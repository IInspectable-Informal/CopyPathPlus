#pragma once

using Action = std::function<void(IShellItemArray*)>;
using SubCommands = std::vector<winrt::com_ptr<IExplorerCommand>>;

class MenuItem : public winrt::implements<MenuItem, IExplorerCommand, IEnumExplorerCommand>
{
public:
	MenuItem();
    MenuItem(EXPCMDFLAGS, std::wstring const&, std::wstring const&, EXPCMDSTATE, Action const&, SubCommands const&);
    MenuItem(std::wstring const&, std::wstring const&, Action const&, EXPCMDSTATE = ECS_ENABLED);
    MenuItem(std::wstring const&, std::wstring const&, SubCommands const&, EXPCMDSTATE = ECS_ENABLED);

	//IExplorerCommand
    HRESULT STDMETHODCALLTYPE GetTitle(IShellItemArray* psiItemArray, LPWSTR* ppszName);
    HRESULT STDMETHODCALLTYPE GetIcon(IShellItemArray* psiItemArray, LPWSTR* ppszIcon);
    HRESULT STDMETHODCALLTYPE GetToolTip(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip);
    HRESULT STDMETHODCALLTYPE GetCanonicalName(GUID* pguidCommandName);
    HRESULT STDMETHODCALLTYPE GetState(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState);
    HRESULT STDMETHODCALLTYPE Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc);
    HRESULT STDMETHODCALLTYPE GetFlags(EXPCMDFLAGS* pFlags);
    HRESULT STDMETHODCALLTYPE EnumSubCommands(IEnumExplorerCommand** ppEnum);

    //IEnumExplorerCommand
    HRESULT STDMETHODCALLTYPE Next(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched);
    HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
    HRESULT STDMETHODCALLTYPE Reset();
    HRESULT STDMETHODCALLTYPE Clone(IEnumExplorerCommand** ppenum);


    std::wstring m_Icon{ L"" };
    std::wstring m_Title{ L"" };

    EXPCMDFLAGS m_Flags{ ECF_ISSEPARATOR };
    EXPCMDSTATE m_State{ ECS_ENABLED };
    
    Action m_Action{ nullptr };
    SubCommands m_SubCommands{};

private:
    ULONG m_Index{ 0 };
};
