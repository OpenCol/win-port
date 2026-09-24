/* MSShowWindow_of_1038_8c1b -- shipped at 1038:8c1b.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1038_8c1b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1038_8c1b(GameWinBase *o)
{
    MSShowWindow(o->win);
}
