/* get_mouse_pos_for_1050_8f2b: candidate for 1050:8f2b (49 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * The module-1050 copy of get_mouse_pos_for. MSGetMousePos against the window derived from a, into b and c. The outer call's second
 * and third arguments are pushed before the inner call is made.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *get_win_1050_94fb(Screen *a);
extern void MSGetMousePos(void *win, void *x, void *y);

void get_mouse_pos_for_1050_8f2b(void *screen, void *x, void *y)
{
    MSGetMousePos((MSWin *)(get_win_1050_94fb((Screen *)screen)), (int *)x, (int *)y);
}
