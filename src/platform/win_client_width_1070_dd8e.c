/* win_client_width_1070_dd8e -- shipped at 1070:dd8e.
 *
 * Ported by tools/port.py from matched/platform/win_client_width_1070_dd8e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_client_width_1078_23db(void *w);

col_i16 win_client_width_1070_dd8e(GameWinBase *o)
{
    return win_client_width_1078_23db(o->win);
}
