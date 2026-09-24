/* MSHideWindow_of_1040_a80c -- shipped at 1040:a80c.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1040_a80c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1040_a80c(GameWinBase *o)
{
    MSHideWindow(o->win);
}
