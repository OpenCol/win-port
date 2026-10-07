/* show_window_1048_dd42: candidate for 1048:dd42 (31 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_window_1048_dd42
 * name-source: descriptive
 *
 * `win_show_and_focus_1048_e7ca` on the member 0x26 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from close_dialog_at_26_1048_dd42: closes a game window: close_dialog on its DialogWin, embedded at
 * +0x26; callers pass the map, info, overlay and other game windows.
 *
 * RENAMED 2026-09-27 from close_window_1048_dd42: it runs win_show_and_focus on the DialogWin at +0x26, and its callers (declaration_of_independence, show_ship, CDControlWin_Init) call it right after building a window, before MSDoAllTasks: it opens the window. The identical +0x26 wrapper is already show_window.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void win_show_and_focus_1048_e7ca(DialogWin *o);

void show_window_1048_dd42(GameWin *o)
{
    win_show_and_focus_1048_e7ca(&(DialogWin &)*o);
}
