/* PORT: this file's own declarations of MSCenterWindow removed: the header declares it from the definition (W1 step 3). */
/* win_center_1040_a778: candidate for 1040:a778 (37 bytes).  Verdict: RELOCATABLE.
 *
 * MSCenterWindow on the window's handle (the pointer at +6), offsets 0, 0.
 *
 * TRANSFERRED: byte-identical to `win_center_1048_d80b` (1048:d80b), one of 12 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_center_1040_a778(DialogWin *o)
{
    MSCenterWindow((MSWin *)o->win, 0, 0);
}
