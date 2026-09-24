/* MSShowWindow_of_1060_6ce9 -- shipped at 1060:6ce9.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1060_6ce9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1060_6ce9(GameWinBase *o)
{
    MSShowWindow(o->win);
}
