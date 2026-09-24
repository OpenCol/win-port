/* MSHideWindow_of_1050_8cd8 -- shipped at 1050:8cd8.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1050_8cd8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void MSHideWindow_of_1050_8cd8(GameWinBase *o)
{
    MSHideWindow(o->win);
}
