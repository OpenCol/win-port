/* pedia_set_page -- shipped at 1060:31fc.
 *
 * Ported by tools/port.py from matched/game/pedia_set_page.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_pedia_page;                             DGROUP:0x28a6
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_all_to_screen_1060_610c(PediaWin *p);
void pedia_redraw(PediaWin *p);

extern void * current_window(void); /* 1060:9fcd */

/* same object: 4-byte `push cs; call ` */



void pedia_set_page(col_i16 page)
{
    PediaWin *p;
    void *w;

    w = current_window();
    p = (PediaWin *)(w != 0 ? (char *)w - 0x26 : (char *)w);
    if (page != g_pedia_page) {
        p->page = g_pedia_page = page;
        pedia_redraw(p);
        copy_all_to_screen_1060_610c(p);
    }
}
