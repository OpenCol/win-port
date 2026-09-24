/* show_text_dialog_1048_e3ae -- shipped at 1048:e3ae.
 *
 * Ported by tools/port.py from matched/game/show_text_dialog_1048_e3ae.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_dialog_ctx[];                      DGROUP:0x24dc
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1048_e911(void *p, col_i16 a, col_i16 b, col_i16 c);

col_i16 show_text_dialog_1048_e3ae(col_i16 a, col_i16 b, col_i16 c)
{
    return text_dialog_1048_e911(g_dialog_ctx, a, b, c);
}
