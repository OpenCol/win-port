/* MSSetFocus_of_1010_eb8f -- shipped at 1010:eb8f.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1010_eb8f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1010_eb8f(GameWinBase *o)
{
    MSSetFocus(o->win);
}
