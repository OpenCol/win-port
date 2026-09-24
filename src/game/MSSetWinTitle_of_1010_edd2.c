/* MSSetWinTitle_of_1010_edd2 -- shipped at 1010:edd2.
 *
 * Ported by tools/port.py from matched/game/MSSetWinTitle_of_1010_edd2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetWinTitle(void *w, void *p);

void MSSetWinTitle_of_1010_edd2(GameWinBase *o, void *p)
{
    MSSetWinTitle(o->win, p);
}
