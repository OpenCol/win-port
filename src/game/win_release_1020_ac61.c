/* win_release_1020_ac61: candidate for 1020:ac61 (44 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_release_1020_ac61
 * name-source: descriptive
 *
 * Disposes the window's DialogWin window record and then initialises its port at no size:
 * `dialog_dispose_window_1020_af04` on the member at +0x26, then `GRPort_Init_wh_1020_af30` on the
 * object itself. One of SEVEN copies (1010, 1020, 1040, 1048, 1050, 1058, 1060), one per module, and both callees are this module's
 * own copies of shared routines.
 *
 * Both are -called and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from win_init_menu_port_1020_ac61: releases a game window's resources: dialog_dispose_window on its DialogWin at +0x26, then
 * GRPort_Init_wh re-initialises its port at no size.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void dialog_dispose_window_1020_af04(DialogWin *dialog);

void win_release_1020_ac61(GameWin *o)
{
    dialog_dispose_window_1020_af04(&(DialogWin &)*o);
    GRPort_Init_wh_1020_af30(o);
}
