/* win_disable_1050_8c93 -- shipped at 1050:8c93.
 *
 * Ported by tools/port.py from matched/game/win_disable_1050_8c93.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; char pad2[2]; char disabled; } GameWinBase;
extern void MSEnableWindow(void *w, col_i16 on);

void win_disable_1050_8c93(GameWinBase *o)
{
    o->disabled = 1;
    MSEnableWindow(o->win, 0);
}
