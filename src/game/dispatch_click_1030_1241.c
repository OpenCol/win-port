/* dispatch_click_1030_1241 -- shipped at 1030:1241.
 *
 * Ported by tools/port.py from matched/game/dispatch_click_1030_1241.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern col_i16 rect12_hit(void *w, col_i16 x, col_i16 y);
extern void track_panel_choice(void *w, col_i16 panel);
void dispatch_click_1030_1241(col_i16 x, col_i16 y)
{
    char *c;
    char *w = current_window();

    c = w != 0 ? w - 0x26 : w;
    track_panel_choice(c, rect12_hit(c, x, y));
}
