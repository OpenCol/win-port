/* show_window_1010_e404: candidate for 1010:e404 (31 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_window_1010_e404
 * name-source: descriptive
 *
 * `win_show_and_focus_1010_ecec` on the member 0x26 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from win_show_focus_at_26_1010_e404: shows a game window and gives it the focus: win_show_and_focus on its
 * DialogWin, embedded at +0x26.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

void show_window_1010_e404(GameWin *o)
{
    win_show_and_focus_1010_ecec(&(DialogWin &)*o);
}
