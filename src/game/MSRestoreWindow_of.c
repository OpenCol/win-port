/* MSRestoreWindow_of -- shipped at 1060:6809.
 *
 * Ported by tools/port.py from matched/game/MSRestoreWindow_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSRestoreWindow(void *w);

void MSRestoreWindow_of(GameWinBase *o)
{
    MSRestoreWindow(o->win);
}
