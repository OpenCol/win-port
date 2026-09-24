/* show_plain_dialog_1008_cd53 -- shipped at 1008:cd53.
 *
 * Ported by tools/port.py from matched/game/show_plain_dialog_1008_cd53.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_dialog_ctx[];                      DGROUP:0x24dc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_plain_1008_db4e(void *p, void *ctx, void *name, col_i16 a, col_i16 g);

/* same object: 4-byte `push cs; call ` */


col_i16 show_plain_dialog_1008_cd53(void *p, void *name, col_i16 g)
{
    return text_dialog_plain_1008_db4e(p, g_dialog_ctx, name, 0, g);
}
