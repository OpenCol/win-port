/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of EventWin_EndModal removed: the header declares it from the definition (W1 step 3). */
/* popup_done: candidate for 1050:2c32 (48 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets bit 0x400 in the current popup's flags and ends its owner window's modal session.
 *
 * RENAMED 2026-09-26 from ctl_flag_and_refresh: the old name misread the body: nothing is refreshed. It sets bit 0x400 in the current popup's flags and ends the popup's modal session (EventWin_EndModal on its owner's window); a list selection, a dialog menu command and the Enter key in a list reach it.
 *
 * RENAMED 2026-09-27 from popup_end_modal: BEYOND THE PROPOSALS: the CODE 19 popup-control block maps one to one onto 1050:2c32..3319 (see chains). Mac popup_done(void), CODE 19 0x34b6, 56 bytes, calls EventWin::EndModal and is called by button_click, key_click and radio_double_click. 2c32 (void), 48 bytes, calls EventWin_EndModal and is called by dialog_menu_command, list_key and ctl_select_item_index (= those three, below).
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */

void popup_done(void)
{
    g_cur_dialog->flags |= 0x400;
    EventWin_EndModal((EventWin *)((char *)g_cur_dialog->win + PORT_DIALOG_BASE));
}
