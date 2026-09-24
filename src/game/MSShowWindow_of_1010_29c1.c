/* MSShowWindow_of_1010_29c1 -- shipped at 1010:29c1.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1010_29c1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1010_29c1(GameWinBase *o)
{
    MSShowWindow(o->win);
}
