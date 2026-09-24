/* win_minimize_of -- shipped at 1048:d920.
 *
 * Ported by tools/port.py from matched/game/win_minimize_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_minimize(void *w);

void win_minimize_of(GameWinBase *o)
{
    win_minimize(o->win);
}
