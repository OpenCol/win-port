/* MSGetWinRect_of -- shipped at 1068:df03.
 *
 * Ported by tools/port.py from matched/gfx/MSGetWinRect_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSGetWinRect(void *w, void *p);

void MSGetWinRect_of(GameWinBase *o, void *p)
{
    MSGetWinRect(o->win, p);
}
