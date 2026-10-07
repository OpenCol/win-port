/* win_release_1040_acac: candidate for 1040:acac (44 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_release_1040_acac
 *
 * dialog_dispose_window_1040_b09c on the GameWin's DialogWin part (+0x26), then GRPort_Init_wh_1040_b0c8
 * on the window.  Both are -called and defined in-file.
 *
 * RENAMED 2026-09-26 from dialog_attach_menu_1040_acac: releases a game window's resources: dialog_dispose_window on its DialogWin at +0x26, then
 * GRPort_Init_wh re-initialises its port at no size.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void dialog_dispose_window_1040_b09c(DialogWin *dialog);

void win_release_1040_acac(GameWin *o)
{
    dialog_dispose_window_1040_b09c(&(DialogWin &)*o);
    GRPort_Init_wh_1040_b0c8(o);
}
