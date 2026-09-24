/* MSSetWinTitle_of_1038_e517 -- shipped at 1038:e517.
 *
 * Ported by tools/port.py from matched/game/MSSetWinTitle_of_1038_e517.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetWinTitle(void *w, void *p);

void MSSetWinTitle_of_1038_e517(GameWinBase *o, void *p)
{
    MSSetWinTitle(o->win, p);
}
