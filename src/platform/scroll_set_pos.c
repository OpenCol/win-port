/* scroll_set_pos -- shipped at 1070:5fe3.
 *
 * Ported by tools/port.py from matched/platform/scroll_set_pos.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void scrollbar_message(col_i16 h, col_i16 msg, col_i16 w, col_i32 l);

void scroll_set_pos(col_i16 h, col_i16 v)
{
    scrollbar_message(h, 0x111, 0x7f, (col_i32)v);
}
