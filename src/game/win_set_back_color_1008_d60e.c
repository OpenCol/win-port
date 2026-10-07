/* PORT: this file's own declarations of MSSetBackground removed: the header declares it from the definition (W1 step 3). */
/* win_set_back_color_1008_d60e: candidate for 1008:d60e (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the background of the window the object holds at +6: `MSSetBackground` replaces the window's brush (+8) with a solid one of palette entry `index` and invalidates it.
 *
 * TRANSFERRED from `win_set_drag_height_1048_d714` (1048:d714), byte-identical except for the one relocated call:
 * this copy reaches `MSSetBackground` where that one reaches `win_set_drag_height_1078_2aef`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_back_color_1008_d60e(Screen *o, int index)
{
    MSSetBackground((MSWin *)o->win, index);
}
