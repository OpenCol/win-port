/* show_text_dialog_1020_e73b -- shipped at 1020:e73b.
 *
 * Ported by tools/port.py from matched/game/show_text_dialog_1020_e73b.c.
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
col_i16 text_dialog_1020_e760(void *file, void *name);

col_i16 show_text_dialog_1020_e73b(void *name)
{
    return text_dialog_1020_e760(g_dialog_ctx, name);
}
