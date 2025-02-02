#pragma once
#include "MenuItem.h"

class MenuSubItem : public MenuItem
{
public:
	MenuSubItem() = default;
	MenuSubItem(LPCWSTR, LPCWSTR, EXPCMDFLAGS, EXPCMDSTATE, LPCWSTR, Action, SubCommands = {}, BOOL = true);
	MenuSubItem(LPCWSTR, LPCWSTR, Action, EXPCMDSTATE = ECS_ENABLED, LPCWSTR = nullptr, BOOL = true);
	MenuSubItem(LPCWSTR, LPCWSTR, SubCommands, EXPCMDSTATE = ECS_ENABLED, BOOL = true);

	HRESULT __stdcall GetState(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)
	{
		fOkToBeSlow = BeSlow;
		*pCmdState = CmdState;
		return S_OK;
	}
};
