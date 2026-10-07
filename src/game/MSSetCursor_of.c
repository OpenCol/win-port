/* MSSetCursor_of: candidate for 1038:e219 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetCursor` on the window the object holds at +6, with the caller's cursor `cur` and
 * `now` flag passed through -- the same one-line shape as `win_set_focus`, two words wider.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetCursor(void *win, int cur, int now);

void MSSetCursor_of(DialogWin *o, int cur, int now)
{
    MSSetCursor((MSWin *)o->win, cur, now);
}
