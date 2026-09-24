/* opening_click_1010_1e79 -- shipped at 1010:1e79.
 *
 * Ported by tools/port.py from matched/game/opening_click_1010_1e79.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Opening far *g_opening;                       DGROUP:0x0964
 *   extern int g_stub_a;                                 
 *
 * The file's own Opening dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void opening_title_draw(Opening *w);

void opening_click_1010_1e79(void)
{
    Opening *w = g_opening;

    if (w->state == -1)
        return;
    else if (w->state == 4)
        w->done = 1;
    else {
        w->skip = 1;
        opening_title_draw(w);
    }
}
