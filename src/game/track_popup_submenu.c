/* track_popup_submenu -- shipped at 1048:e1fd.
 *
 * Ported by tools/port.py from matched/game/track_popup_submenu.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_u16 submenu_of_1048_e8c1(void *o, col_i16 pos);
void * window_of_1048_e6d0(void *o);

extern void MSTrackPopupMenu(col_u16 hmenu, void *win, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */



void track_popup_submenu(void *w, void *m, col_i16 x, col_i16 y)
{
    MSTrackPopupMenu(submenu_of_1048_e8c1(m, 0), window_of_1048_e6d0(w), x, y);
}
