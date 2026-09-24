/* win_show -- shipped at 1008:d0dd.
 *
 * Ported by tools/port.py from matched/game/win_show.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void win_show(GameWinBase *o)
{
    MSShowWindow(o->win);
}
