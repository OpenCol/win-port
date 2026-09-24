/* dispatch_click_1030_0057 -- shipped at 1030:0057.
 *
 * Ported by tools/port.py from matched/game/dispatch_click_1030_0057.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern col_i16 panel_at(void *w, col_i16 x, col_i16 y);
extern void track_nation_choice(void *w, col_i16 panel);
void dispatch_click_1030_0057(col_i16 x, col_i16 y)
{
    char *c;
    char *w = current_window();

    c = w != 0 ? w - 0x26 : w;
    track_nation_choice(c, panel_at(c, x, y));
}
