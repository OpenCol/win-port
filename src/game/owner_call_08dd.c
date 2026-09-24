/* owner_call_08dd -- shipped at 1060:0721.
 *
 * Ported by tools/port.py from matched/game/owner_call_08dd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern void redraw_map_1060_08dd(void *o);

void owner_call_08dd(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;
    redraw_map_1060_08dd(o);
}
