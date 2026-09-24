/* window_of_1068_d72a -- shipped at 1068:d72a.
 *
 * Ported by tools/port.py from matched/gfx/window_of_1068_d72a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;

void * window_of_1068_d72a(GameWinBase *o)
{
    return o->win;
}
