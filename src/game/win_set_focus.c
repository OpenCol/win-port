/* win_set_focus -- shipped at 1060:67c3.
 *
 * Ported by tools/port.py from matched/game/win_set_focus.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *hwnd);

void win_set_focus(GameWinBase *w)
{
    MSSetFocus(w->win);
}
