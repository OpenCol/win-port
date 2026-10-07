/* PORT: this file's own declarations of win_move_to_1078_1ff8 removed: the header declares it from the definition (W1 step 3). */
/* win_move_to_1048_d7e4: candidate for 1048:d7e4 (39 bytes).  Verdict: RELOCATABLE.
 *
 * Moves the window the object holds at +6 to a point, leaving its size alone.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_move_to_1048_d7e4(DialogWin *o, int left, int top)
{
    win_move_to_1078_1ff8((MSWin *)o->win, left, top);
}
