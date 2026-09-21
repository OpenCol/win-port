/* win_enable_1068_d96a -- shipped at 1068:d96a.
 *
 * Ported by tools/port.py from matched/gfx/win_enable_1068_d96a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; char pad2[2]; char disabled; } GameWinBase;
extern void MSEnableWindow(void *w, col_i16 on);

void win_enable_1068_d96a(GameWinBase *o)
{
    o->disabled = 0;
    MSEnableWindow(o->win, 1);
}
