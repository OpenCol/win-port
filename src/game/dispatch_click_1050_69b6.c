/* dispatch_click_1050_69b6 -- shipped at 1050:69b6.
 *
 * Ported by tools/port.py from matched/game/dispatch_click_1050_69b6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_label[];                           SEG20:0x6ee6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void drag_sprite(void *w, char *s, col_i16 x, col_i16 y, col_i16 *a, col_i16 *b);

extern char * current_window(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

void dispatch_click_1050_69b6(col_i16 x, col_i16 y)
{
    char *c;
    col_i16 a;
    col_i16 b;
    char *w;

    w = current_window();
    c = w != 0 ? w - 0x26 : w;
    drag_sprite(c, g_label, x, y, &a, &b);
}
