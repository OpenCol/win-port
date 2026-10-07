/* ctl_hwnd_1070_9681: candidate for 1070:9681 (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ctl_hwnd_1070_9681
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x10 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_10_1070_9681: the control's window handle. MSCloseWindow
 * passes it to IsWindow and DestroyWindow, dialog_wndproc and dialog_mnemonic to MSPushButton and
 * checkbox_click, focus_at_or_after to prev_sibling_window, and list_find_by_id compares it to an HWND.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int ctl_hwnd_1070_9681(DlgControl *ctl)
{
    return ctl->hwnd;
}
