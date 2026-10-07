/* ctl_hwnd_1060_ac4a: candidate for 1060:ac4a (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x10 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_10_1060_ac4a: the control's window handle. MSCloseWindow
 * passes it to IsWindow and DestroyWindow, dialog_wndproc and dialog_mnemonic to MSPushButton and
 * checkbox_click, focus_at_or_after to prev_sibling_window, and list_find_by_id compares it to an HWND.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int ctl_hwnd_1060_ac4a(DlgControl *ctl)
{
    return ctl->hwnd;
}
