/* cur_win_set_9e0 -- shipped at 1040:9d9f.
 *
 * Ported by tools/port.py from matched/game/cur_win_set_9e0.c.
 *
 * The file's own Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);

void cur_win_set_9e0(void)
{
    Owner *o;
    char *w = current_window();

    o = (Owner *)(w != 0 ? w - 0x26 : w);
    o->flag = 1;
}
