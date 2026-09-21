/* win_is_zoomed_1048_d8d2 -- shipped at 1048:d8d2.
 *
 * Ported by tools/port.py from matched/game/win_is_zoomed_1048_d8d2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_is_zoomed_1078_212b(void *w);

col_i16 win_is_zoomed_1048_d8d2(GameWinBase *o)
{
    return win_is_zoomed_1078_212b(o->win);
}
