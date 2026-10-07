/* win_move_to_rect: candidate for 1048:d6da (39 bytes).  Verdict: RELOCATABLE.
 *
 * Moves and resizes the window the object holds at +6 to a rectangle.
 *
 * RENAMED 2026-09-27 from win_move_to_rect_1048_d6da: the address suffix went with the family: its window-level sibling is now MSFitWindow.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void MSFitWindow(MSWin *win, RECT *r);

void win_move_to_rect(Screen *o, RECT *r)
{
    MSFitWindow((MSWin *)o->win, r);
}
