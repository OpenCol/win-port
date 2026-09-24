/* MSShowWindow_of_1050_8ff2 -- shipped at 1050:8ff2.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1050_8ff2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1050_8ff2(GameWinBase *o)
{
    MSShowWindow(o->win);
}
