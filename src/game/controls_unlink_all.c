/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* controls_unlink_all: candidate for 1060:9aa9 (81 bytes).  Verdict: RELOCATABLE.
 * symbol: controls_unlink_all
 *
 * For every control on the window's list (head +0x8c, next +0x12) unlinks
 * its id (ctl_id_1060_abca) from the window's list.  Both helpers are
 * -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern int ctl_id_1060_abca(DlgControl *ctl);

void controls_unlink_all(DialogWin *w)
{
    DlgControl *c;

    for (c = w->first_ctl; c != 0; c = c->next)
        DialogWin_RemoveControl(w, ctl_id_1060_abca(c));
}
