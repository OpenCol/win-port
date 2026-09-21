/* win_client_height_1048_e5aa -- shipped at 1048:e5aa.
 *
 * Ported by tools/port.py from matched/game/win_client_height_1048_e5aa.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_client_height_1078_241a(void *w);

col_i16 win_client_height_1048_e5aa(GameWinBase *o)
{
    return win_client_height_1078_241a(o->win);
}
