/* win_release_1048_e0a1: candidate for 1048:e0a1 (44 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_release_1048_e0a1
 *
 * dialog_dispose_window_1048_e895 on the GameWin's DialogWin part (+0x26), then GRPort_Init_wh_1048_e613
 * on the window.  Both are -called and defined in-file.
 *
 * RENAMED 2026-09-26 from dialog_attach_menu_1048_e0a1: releases a game window's resources: dialog_dispose_window on its DialogWin at +0x26, then
 * GRPort_Init_wh re-initialises its port at no size.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void dialog_dispose_window_1048_e895(DialogWin *dialog);

void win_release_1048_e0a1(GameWin *o)
{
    dialog_dispose_window_1048_e895(&(DialogWin &)*o);
    GRPort_Init_wh_1048_e613(o);
}
