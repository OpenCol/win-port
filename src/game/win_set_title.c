/* win_set_title -- shipped at 1048:e5cf.
 *
 * Ported by tools/port.py from matched/game/win_set_title.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetWinTitle(void *w, char *s);

void win_set_title(GameWinBase *o, char *s)
{
    MSSetWinTitle(o->win, s);
}
