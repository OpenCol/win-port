/* MSMaximizeWindow_of -- shipped at 1048:e1b5.
 *
 * Ported by tools/port.py from matched/game/MSMaximizeWindow_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSMaximizeWindow(void *w);

void MSMaximizeWindow_of(GameWinBase *o)
{
    MSMaximizeWindow(o->win);
}
