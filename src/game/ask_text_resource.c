/* ask_text_resource -- shipped at 1048:dbb1.
 *
 * Ported by tools/port.py from matched/game/ask_text_resource.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_dialog_ctx[];                      DGROUP:0x24dc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1048_e6ed(void *ctx, char *name);

extern col_i16 text_dialog_1048_e911(void *ctx, char *name, col_i16 a);



col_i16 ask_text_resource(char *name)
{
    return text_dialog_1048_e6ed(g_dialog_ctx, name);
}
