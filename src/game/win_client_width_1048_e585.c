/* win_client_width_1048_e585 -- shipped at 1048:e585.
 *
 * Ported by tools/port.py from matched/game/win_client_width_1048_e585.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_client_width_1078_23db(void *w);

col_i16 win_client_width_1048_e585(GameWinBase *o)
{
    return win_client_width_1078_23db(o->win);
}
