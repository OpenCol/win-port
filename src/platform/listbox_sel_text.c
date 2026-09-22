/* listbox_sel_text -- shipped at 1070:54d4.
 *
 * Ported by tools/port.py from matched/platform/listbox_sel_text.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void listbox_get_text_1070_54b4(col_i16 h, col_i16 i, col_i16 a, col_i16 b);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
#endif

void listbox_sel_text(col_i16 h, col_i16 a, col_i16 b)
{
    col_i16 i;

    i = SendMessage(h, 0x409, 0, 0L);
    listbox_get_text_1070_54b4(h, i, a, b);
}
