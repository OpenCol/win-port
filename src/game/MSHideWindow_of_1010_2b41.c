/* MSHideWindow_of_1010_2b41 -- shipped at 1010:2b41.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1010_2b41.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1010_2b41(GameWinBase *o)
{
    MSHideWindow(o->win);
}
