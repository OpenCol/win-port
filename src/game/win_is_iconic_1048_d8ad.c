/* win_is_iconic_1048_d8ad -- shipped at 1048:d8ad.
 *
 * Ported by tools/port.py from matched/game/win_is_iconic_1048_d8ad.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_is_iconic_1078_217b(void *w);

col_i16 win_is_iconic_1048_d8ad(GameWinBase *o)
{
    return win_is_iconic_1078_217b(o->win);
}
