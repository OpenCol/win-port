/* cur_win_ret1 -- shipped at 1060:2bc3.
 *
 * Ported by tools/port.py from matched/game/cur_win_ret1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);

col_i16 cur_win_ret1(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;
    return 1;
}
