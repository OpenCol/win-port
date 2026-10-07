/* MSHideControl_1060_669f: candidate for 1060:669f (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Hides a control's window (the handle at +0x10) when it has one.
 *
 * RENAMED 2026-09-27 from control_hide_1060_669f: Mac MSHideControl. Callers: PediaWin_DrawArticle, DrawListArea, Show and open_colonopedia_ex (ShowModal), the Mac PediaWin callers. ShowModal's seq has MSHideControl x2, and open_colonopedia_ex calls this x2. FAMILY: control_hide_* are 2 byte-identical per-module copies (hide_hwnd on control+0x10). They take the control record, as Mac MSHideControl(ControlRecord**) does. Per-module suffix convention as for MSShowControl_*. Mac MSHideControl's 8 callers split exactly across the two copies. Arity 1. Mac MiscButtonClick's 4 MSHideControl calls are the 4 control_hide_1010_e54d calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void MSHideControl_1060_669f(DlgControl *control)
{
    if (control->hwnd != 0)
        hide_hwnd(control->hwnd);
}
