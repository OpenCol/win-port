/* win_is_iconic_1060_67e4 -- shipped at 1060:67e4.
 *
 * Ported by tools/port.py from matched/game/win_is_iconic_1060_67e4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern col_i16 win_is_iconic_1078_217b(void *w);

col_i16 win_is_iconic_1060_67e4(GameWinBase *o)
{
    return win_is_iconic_1078_217b(o->win);
}
