/* scrollbar_init -- shipped at 1060:661c.
 *
 * Ported by tools/port.py from matched/game/scrollbar_init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own ScrollBar dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1060_6b92(void *o, void *win, col_i16 kind, col_i16 id, RECT *r);

extern col_i16 register_MSScrollBarClass(RECT *r, void *o, col_i32 l, col_i16 k);
extern void scroll_set_range(col_i16 h, col_i16 lo, col_i16 hi);



void scrollbar_init(ScrollBar *o, void *win, col_i16 id, RECT *r, col_i32 l)
{
    dlg_control_init_1060_6b92(o, win, 8, id, r);
    o->hwnd = register_MSScrollBarClass(r, o, l, 1);
    scroll_set_range(o->hwnd, 1, 100);
    o->f24 = 1;
}
