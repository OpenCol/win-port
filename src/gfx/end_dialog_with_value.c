/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of EventWin_EndModal removed: the header declares it from the definition (W1 step 3). */
/* end_dialog_with_value: candidate for 1068:cbcc (39 bytes).  Verdict: RELOCATABLE.
 *
 * Stores its second argument at DGROUP:0x56b4 and calls EventWin_EndModal on the
 * DialogWin part (+0x26) of the window the pointer at DGROUP:0x56b6 holds.
 * The first argument is not used.
 *
 * RENAMED 2026-09-26 from store_and_call_9fad: stores its value argument in g_dialog_result and ends the
 * owner dialog's modal loop; ask_choice installs it as a radio set's double-click callback, whose
 * (id, item) arguments make the double-clicked item the result.
 */

#include "colonization.h"

#include "classes.h"


void end_dialog_with_value(int unused, int value)
{
    g_dialog_result = value;
    EventWin_EndModal((EventWin *)(g_dialog_owner + PORT_DIALOG_BASE));
}
