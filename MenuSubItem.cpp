#include "pch.h"
#include "MenuSubItem.h"

MenuSubItem::MenuSubItem(LPCWSTR i_Icon, LPCWSTR i_Title, EXPCMDFLAGS i_Flags, EXPCMDSTATE i_CmdState, LPCWSTR i_ToolTip, Action i_Event, SubCommands i_SubCmds, BOOL i_BeSlow)
{
    CommonInit(i_Icon, i_Title, i_CmdState, i_ToolTip, i_BeSlow);
    Flags = i_Flags;
    Event = i_Event;
    SubCmds = i_SubCmds;
}

MenuSubItem::MenuSubItem(LPCWSTR i_Icon, LPCWSTR i_Title, Action i_Event, EXPCMDSTATE i_CmdState, LPCWSTR i_ToolTip, BOOL i_BeSlow)
{
    CommonInit(i_Icon, i_Title, i_CmdState, i_ToolTip, i_BeSlow);
    Flags = ECF_DEFAULT;
    Event = i_Event;
}

MenuSubItem::MenuSubItem(LPCWSTR i_Icon, LPCWSTR i_Title, SubCommands i_SubCmds, EXPCMDSTATE i_CmdState, BOOL i_BeSlow)
{
    CommonInit(i_Icon, i_Title, i_CmdState, nullptr, i_BeSlow);
    Flags = ECF_HASSUBCOMMANDS;
    SubCmds = i_SubCmds;
}
