/* colony_window_draw_status: candidate for 1010:6d1d (28 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `ColonyWin_DrawTitle(&g_colony_window)`: redraws the colony window's status line, on the
 * window object at SEG20:0x25be. `message_timer_expired` refreshes the colony window the same
 * way when its message timer fires.
 *
 * ColonyWin_DrawTitle is in segment 1010 but reached here by a FAR call, so it is extern.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct { char pad[1]; } ColonyWin;

extern void ColonyWin_DrawTitle(ColonyWin *w);

void colony_window_draw_status(void)
{
    ColonyWin_DrawTitle(&g_colony_window);
}
