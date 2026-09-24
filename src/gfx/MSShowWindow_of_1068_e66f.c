/* MSShowWindow_of_1068_e66f -- shipped at 1068:e66f.
 *
 * Ported by tools/port.py from matched/gfx/MSShowWindow_of_1068_e66f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1068_e66f(GameWinBase *o)
{
    MSShowWindow(o->win);
}
