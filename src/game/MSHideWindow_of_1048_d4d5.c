/* MSHideWindow_of_1048_d4d5 -- shipped at 1048:d4d5.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1048_d4d5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1048_d4d5(GameWinBase *o)
{
    MSHideWindow(o->win);
}
