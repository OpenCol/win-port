/* show_window_1020_ac0e: candidate for 1020:ac0e (31 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_window_1020_ac0e
 * name-source: descriptive
 *
 * `win_show_and_focus_1020_aedb` on the member 0x26 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from win_show_focus_at_26_1020_ac0e: shows a game window and gives it the focus: win_show_and_focus on its
 * DialogWin, embedded at +0x26.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

void show_window_1020_ac0e(GameWin *o)
{
    win_show_and_focus_1020_aedb(&(DialogWin &)*o);
}
