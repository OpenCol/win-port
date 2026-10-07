/* win_move_to_of: candidate for 1068:df2a (39 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `win_move_to_1078_1ff8` on the window the object holds at +6, with the caller's x and y passed
 * through -- the same one-line shape as `win_set_focus`, two arguments wider.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void win_move_to_1078_1ff8(void *win, int x, int y);

void win_move_to_of(DialogWin *o, int x, int y)
{
    win_move_to_1078_1ff8((MSWin *)o->win, x, y);
}
