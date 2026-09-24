/* MSHideWindow_of_1030_e647 -- shipped at 1030:e647.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1030_e647.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1030_e647(GameWinBase *o)
{
    MSHideWindow(o->win);
}
