/* MSHideWindow_of_1068_d995 -- shipped at 1068:d995.
 *
 * Ported by tools/port.py from matched/gfx/MSHideWindow_of_1068_d995.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1068_d995(GameWinBase *o)
{
    MSHideWindow(o->win);
}
