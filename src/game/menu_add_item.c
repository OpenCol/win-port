/* menu_add_item -- shipped at 1038:e2b5.
 *
 * Ported by tools/port.py from matched/game/menu_add_item.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 submenu_of_1038_e55f(col_i16 b, col_i16 c, col_i16 d);
void * get_p_6_1038_e372(void *w);

extern void MSTrackPopupMenu(col_i16 id, void *menu, col_i16 e, col_i16 f);




void menu_add_item(void *w, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f)
{
    MSTrackPopupMenu(submenu_of_1038_e55f(b, c, d), get_p_6_1038_e372(w), e, f);
}
