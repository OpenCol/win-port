/* win_show: candidate for 1008:d0dd (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line forwarder shape as
 * `win_set_focus`. Its twin at 1008:d0bc does the opposite, and the two sit back to back.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void win_show(Screen *o)
{
    MSShowWindow((MSWin *)o->win);
}
