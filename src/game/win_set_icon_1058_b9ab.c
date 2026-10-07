/* PORT: this file's own declarations of win_set_icon_1078_2832 removed: the header declares it from the definition (W1 step 3). */
/* win_set_icon_1058_b9ab: candidate for 1058:b9ab (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the window-class icon of the window the object holds at +6: win_set_icon_1078_2832 loads
 * icon resource `icon` and stores it with SetClassWord(GCW_HICON).
 *
 * TRANSFERRED from `win_set_drag_height_1048_d714` (1048:d714), byte-identical except for the one relocated call:
 * this copy reaches `win_set_icon_1078_2832` where that one reaches `win_set_drag_height_1078_2aef`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_icon_1058_b9ab(DialogWin *o, int icon)
{
    win_set_icon_1078_2832((MSWin *)o->win, icon);
}
