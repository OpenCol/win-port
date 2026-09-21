/* win_disable_1010_e459 -- shipped at 1010:e459.
 *
 * Ported by tools/port.py from matched/game/win_disable_1010_e459.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; char pad2[2]; char disabled; } GameWinBase;
extern void MSEnableWindow(void *w, col_i16 on);

void win_disable_1010_e459(GameWinBase *o)
{
    o->disabled = 1;
    MSEnableWindow(o->win, 0);
}
