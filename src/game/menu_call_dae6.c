/* menu_call_dae6 -- shipped at 1060:8f99.
 *
 * Ported by tools/port.py from matched/game/menu_call_dae6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 submenu_of_1060_a938(void *w, col_i16 a);

extern void MSAppendMenuItem(col_u16 h, col_i16 pos, char *text, col_u16 id);



void menu_call_dae6(void *w, col_i16 a, col_i16 b, char *text, col_u16 id)
{
    col_i16 h;

    h = submenu_of_1060_a938(w, a);
    MSAppendMenuItem(h, b, text, id);
}
