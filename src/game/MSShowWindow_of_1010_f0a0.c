/* MSShowWindow_of_1010_f0a0 -- shipped at 1010:f0a0.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1010_f0a0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1010_f0a0(GameWinBase *o)
{
    MSShowWindow(o->win);
}
