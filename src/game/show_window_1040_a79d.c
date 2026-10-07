/* show_window_1040_a79d: candidate for 1040:a79d (31 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_window_1040_a79d
 * name-source: descriptive
 *
 * `win_show_and_focus_1040_b052` on the member 0x26 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from close_dialog_at_26_1040_a79d: closes a game window: close_dialog on its DialogWin, embedded at
 * +0x26; callers pass the map, info, overlay and other game windows.
 *
 * RENAMED 2026-09-27 from close_window_1040_a79d: it runs win_show_and_focus on the DialogWin at +0x26, and its callers (declaration_of_independence, show_ship, CDControlWin_Init) call it right after building a window, before MSDoAllTasks: it opens the window. The identical +0x26 wrapper is already show_window.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void win_show_and_focus_1040_b052(DialogWin *o);

void show_window_1040_a79d(GameWin *o)
{
    win_show_and_focus_1040_b052(&(DialogWin &)*o);
}
