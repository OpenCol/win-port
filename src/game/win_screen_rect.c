/* win_screen_rect -- shipped at 1048:e1d6.
 *
 * Ported by tools/port.py from matched/game/win_screen_rect.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSGetWinRect(void *w, void *r);

void win_screen_rect(GameWinBase *o, void *r)
{
    MSGetWinRect(o->win, r);
}
