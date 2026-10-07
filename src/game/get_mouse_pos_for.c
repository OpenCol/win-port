/* get_mouse_pos_for: candidate for 1030:e668 (49 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * MSGetMousePos against the window derived from a, into b and c. The outer call's second
 * and third arguments are pushed before the inner call is made.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *get_win_1030_ed1f(void *a);
#include "classes.h"
extern void MSGetMousePos(void *win, void *x, void *y);

void get_mouse_pos_for(Screen *screen, int *x, int *y)
{
    MSGetMousePos((MSWin *)(get_win_1030_ed1f(screen)), x, y);
}
