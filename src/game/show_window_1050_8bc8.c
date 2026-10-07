/* show_window_1050_8bc8: candidate for 1050:8bc8 (31 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_window_1050_8bc8
 * name-source: descriptive
 *
 * `win_show_and_focus_1050_94ab` on the member 0x26 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from close_dialog_at_26_1050_8bc8: closes a game window: close_dialog on its DialogWin, embedded at
 * +0x26; callers pass the map, info, overlay and other game windows.
 *
 * RENAMED 2026-09-27 from close_window_1050_8bc8: it runs win_show_and_focus on the DialogWin at +0x26, and its callers (declaration_of_independence, show_ship, CDControlWin_Init) call it right after building a window, before MSDoAllTasks: it opens the window. The identical +0x26 wrapper is already show_window.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void win_show_and_focus_1050_94ab(DialogWin *o);

void show_window_1050_8bc8(GameWin *o)
{
    win_show_and_focus_1050_94ab(&(DialogWin &)*o);
}
