/* win_redraw_menu_bar -- shipped at 1048:e28b.
 *
 * Ported by tools/port.py from matched/game/win_redraw_menu_bar.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of_1048_e6d0(Win *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSDrawMenuBar(void *hwnd);

void win_redraw_menu_bar(Win *w)
{
    if ((*((void * *)((unsigned char *)(w) + 0x6a))) != 0)
        MSDrawMenuBar(window_of_1048_e6d0(w));
}
