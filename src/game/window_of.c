/* window_of -- shipped at 1060:ab6e.
 *
 * Ported by tools/port.py from matched/game/window_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;

void * window_of(GameWinBase *o)
{
    return o->win;
}
