/* PORT: this file's own declarations of win_set_client_size_1078_2059 removed: the header declares it from the definition (W1 step 3). */
/* win_set_client_size_1060_aecf: candidate for 1060:aecf (39 bytes).  Verdict: RELOCATABLE.
 *
 * Resizes the window the object holds at +6 so its client area is the given size.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_client_size_1060_aecf(DialogWin *o, int cw, int ch)
{
    win_set_client_size_1078_2059((MSWin *)o->win, cw, ch);
}
