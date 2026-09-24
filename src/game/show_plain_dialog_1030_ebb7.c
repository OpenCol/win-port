/* show_plain_dialog_1030_ebb7 -- shipped at 1030:ebb7.
 *
 * Ported by tools/port.py from matched/game/show_plain_dialog_1030_ebb7.c.
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
col_i16 text_dialog_plain_1030_ef1a(void *p, void *file, void *name,
                                    col_i16 c, col_i16 d);

col_i16 show_plain_dialog_1030_ebb7(void *p, void *name, col_i16 c)
{
    return text_dialog_plain_1030_ef1a(p, g_dialog_ctx, name, 0, c);
}
