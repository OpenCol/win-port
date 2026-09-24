/* win_set_focus_1068_e278 -- shipped at 1068:e278.
 *
 * Ported by tools/port.py from matched/gfx/win_set_focus_1068_e278.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *hwnd);

void win_set_focus_1068_e278(GameWinBase *w)
{
    MSSetFocus(w->win);
}
