/* win_enable_1060_6798 -- shipped at 1060:6798.
 *
 * Ported by tools/port.py from matched/game/win_enable_1060_6798.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; char pad2[2]; char disabled; } GameWinBase;
extern void MSEnableWindow(void *w, col_i16 on);

void win_enable_1060_6798(GameWinBase *o)
{
    o->disabled = 0;
    MSEnableWindow(o->win, 1);
}
