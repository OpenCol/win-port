/* MSHideWindow_of_1010_eb6e -- shipped at 1010:eb6e.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1010_eb6e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1010_eb6e(GameWinBase *o)
{
    MSHideWindow(o->win);
}
