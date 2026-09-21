/* win_redraw_menu_bar_1008_d7b3 -- shipped at 1008:d7b3.
 *
 * Ported by tools/port.py from matched/game/win_redraw_menu_bar_1008_d7b3.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * get_p_6_1008_e4e5(Win *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSDrawMenuBar(void *hwnd);

void win_redraw_menu_bar_1008_d7b3(Win *w)
{
    if ((*((void * *)((unsigned char *)(w) + 0x6a))) != 0)
        MSDrawMenuBar(get_p_6_1008_e4e5(w));
}
