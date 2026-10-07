/* win_hide_1008_d0bc: candidate for 1008:d0bc (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSHideWindow` on the window the object holds at +6 -- the same one-line forwarder shape as
 * `win_set_focus`. Its twin at 1008:d0dd does the opposite, and the two sit back to back.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSHideWindow(void *win);

void win_hide_1008_d0bc(Screen *o)
{
    MSHideWindow((MSWin *)o->win);
}
