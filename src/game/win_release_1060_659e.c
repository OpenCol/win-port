/* win_release_1060_659e: candidate for 1060:659e (44 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_release_1060_659e
 *
 * dialog_dispose_window_1060_6c08 on the GameWin's DialogWin part (+0x26), then GRPort_Init_wh_1060_65fc
 * on the window.  Both are -called and defined in-file.
 *
 * RENAMED 2026-09-26 from dialog_attach_menu_1060_659e: releases a game window's resources: dialog_dispose_window on its DialogWin at +0x26, then
 * GRPort_Init_wh re-initialises its port at no size.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" void dialog_dispose_window_1060_6c08(DialogWin *dialog);

void win_release_1060_659e(GameWin *o)
{
    dialog_dispose_window_1060_6c08(&(DialogWin &)*o);
    GRPort_Init_wh_1060_65fc(o);
}
