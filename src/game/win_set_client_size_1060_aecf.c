/* win_set_client_size_1060_aecf: candidate for 1060:aecf (39 bytes).  Verdict: RELOCATABLE.
 *
 * Resizes the window the object holds at +6 so its client area is the given size.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void win_set_client_size_1078_2059(void *win, int cw, int ch);

void win_set_client_size_1060_aecf(DialogWin *o, int cw, int ch)
{
    win_set_client_size_1078_2059((MSWin *)o->win, cw, ch);
}
