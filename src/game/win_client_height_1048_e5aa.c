/* win_client_height_1048_e5aa: candidate for 1048:e5aa (37 bytes).  Verdict: RELOCATABLE.
 *
 * The client height of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern int win_client_height_1078_241a(void *win);

int win_client_height_1048_e5aa(Screen *o)
{
    return win_client_height_1078_241a((MSWin *)o->win);
}
