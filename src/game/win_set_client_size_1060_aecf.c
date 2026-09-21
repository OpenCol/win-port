/* win_set_client_size_1060_aecf -- shipped at 1060:aecf.
 *
 * Ported by tools/port.py from matched/game/win_set_client_size_1060_aecf.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_client_size_1078_2059(void *w, col_i16 a, col_i16 b);

void win_set_client_size_1060_aecf(GameWinBase *o, col_i16 a, col_i16 b)
{
    win_set_client_size_1078_2059(o->win, a, b);
}
