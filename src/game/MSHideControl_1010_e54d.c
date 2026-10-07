/* MSHideControl_1010_e54d: candidate for 1010:e54d (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Hides a control's window (the handle at +0x10) when it has one.
 *
 * TRANSFERRED: byte-identical to `MSHideControl_1060_669f` (1060:669f), one of 2 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * RENAMED 2026-09-27 from control_hide_1010_e54d: Mac MSHideControl. Callers: DrawMiscellaneous, MiscButtonClick, TimerProc and colony_hammer_change, the Mac ColonyWin callers. FAMILY: control_hide_* are 2 byte-identical per-module copies (hide_hwnd on control+0x10). They take the control record, as Mac MSHideControl(ControlRecord**) does. Per-module suffix convention as for MSShowControl_*. Mac MSHideControl's 8 callers split exactly across the two copies. Arity 1. Mac MiscButtonClick's 4 MSHideControl calls are the 4 control_hide_1010_e54d calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void MSHideControl_1010_e54d(DlgControl *control)
{
    if (control->hwnd != 0)
        hide_hwnd(control->hwnd);
}
