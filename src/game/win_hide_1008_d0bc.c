/* win_hide_1008_d0bc -- shipped at 1008:d0bc.
 *
 * Ported by tools/port.py from matched/game/win_hide_1008_d0bc.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSHideWindow(void *w);

void win_hide_1008_d0bc(GameWinBase *o)
{
    MSHideWindow(o->win);
}
