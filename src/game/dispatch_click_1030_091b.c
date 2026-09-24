/* dispatch_click_1030_091b -- shipped at 1030:091b.
 *
 * Ported by tools/port.py from matched/game/dispatch_click_1030_091b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern col_i16 hittest_region(void *w, col_i16 x, col_i16 y);
extern void track_difficulty(void *w, col_i16 panel);
void dispatch_click_1030_091b(col_i16 x, col_i16 y)
{
    char *c;
    char *w = current_window();

    c = w != 0 ? w - 0x26 : w;
    track_difficulty(c, hittest_region(c, x, y));
}
