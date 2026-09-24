/* window_of_1048_e6d0 -- shipped at 1048:e6d0.
 *
 * Ported by tools/port.py from matched/game/window_of_1048_e6d0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;

void * window_of_1048_e6d0(GameWinBase *o)
{
    return o->win;
}
