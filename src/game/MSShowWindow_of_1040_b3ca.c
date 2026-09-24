/* MSShowWindow_of_1040_b3ca -- shipped at 1040:b3ca.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1040_b3ca.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1040_b3ca(GameWinBase *o)
{
    MSShowWindow(o->win);
}
