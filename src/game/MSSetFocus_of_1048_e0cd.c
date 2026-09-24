/* MSSetFocus_of_1048_e0cd -- shipped at 1048:e0cd.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1048_e0cd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1048_e0cd(GameWinBase *o)
{
    MSSetFocus(o->win);
}
