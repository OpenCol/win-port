/* win_client_width_1048_e585: candidate for 1048:e585 (37 bytes).  Verdict: RELOCATABLE.
 *
 * The client width of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern int win_client_width_1078_23db(void *win);

int win_client_width_1048_e585(Screen *o)
{
    return win_client_width_1078_23db((MSWin *)o->win);
}
