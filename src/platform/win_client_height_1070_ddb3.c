/* win_client_height_1070_ddb3 -- shipped at 1070:ddb3.
 *
 * Ported by tools/port.py from matched/platform/win_client_height_1070_ddb3.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_client_height_1078_241a(void *w);

col_i16 win_client_height_1070_ddb3(GameWinBase *o)
{
    return win_client_height_1078_241a(o->win);
}
