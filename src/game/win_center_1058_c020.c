/* win_center_1058_c020 -- shipped at 1058:c020.
 *
 * Ported by tools/port.py from matched/game/win_center_1058_c020.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSCenterWindow(void *w, col_i16 a, col_i16 b);

void win_center_1058_c020(GameWinBase *o)
{
    MSCenterWindow(o->win, 0, 0);
}
