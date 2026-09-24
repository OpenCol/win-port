/* MSHideWindow_of_1060_87fe -- shipped at 1060:87fe.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1060_87fe.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1060_87fe(GameWinBase *o)
{
    MSHideWindow(o->win);
}
