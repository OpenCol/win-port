/* show_text_dialog_1048_e35c -- shipped at 1048:e35c.
 *
 * Ported by tools/port.py from matched/game/show_text_dialog_1048_e35c.c.
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
col_i16 show_plain_step_1048_e8e4(void *w, void *p, col_i16 a, col_i16 b);

col_i16 show_text_dialog_1048_e35c(void *w, col_i16 a, col_i16 b)
{
    return show_plain_step_1048_e8e4(w, g_dialog_ctx, a, b);
}
