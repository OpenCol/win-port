/* find_control: candidate for 1060:9be6 (102 bytes).  Verdict: RELOCATABLE.
 * symbol: find_control
 *
 * The first control on the window's list that get_initial_focus accepts,
 * stepping with ctl_next_1060_abe3; null when none.  Both helpers are
 * -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern char get_initial_focus(DlgControl *ctl);
extern DlgControl *ctl_next_1060_abe3(DlgControl *ctl);

DlgControl *find_control(DialogWin *w)
{
    DlgControl *c;

    for (c = w->first_ctl; c != 0; c = ctl_next_1060_abe3(c)) {
        if (get_initial_focus(c))
            return c;
    }
    return 0;
}
