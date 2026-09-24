/* MSShowWindow_of_1048_ecce -- shipped at 1048:ecce.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1048_ecce.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSShowWindow(void *w);

void MSShowWindow_of_1048_ecce(GameWinBase *o)
{
    MSShowWindow(o->win);
}
