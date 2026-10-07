/* win_minimize_of: candidate for 1048:d920 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `win_minimize` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void win_minimize(void *win);

void win_minimize_of(DialogWin *o)
{
    win_minimize((MSWin *)o->win);
}
