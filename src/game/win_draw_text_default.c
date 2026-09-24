/* win_draw_text_default -- shipped at 1048:e326.
 *
 * Ported by tools/port.py from matched/game/win_draw_text_default.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_dialog_ctx;                        DGROUP:0x24dc
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: flags -> flags_86
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void text_dialog_plain_1048_e7f3(Win *w, void *style, col_i16 a, col_i16 b, col_i16 c, col_i16 flags);

/* same object: 4-byte `push cs; call ` (rule 47) */

void win_draw_text_default(Win *w, col_i16 a, col_i16 b, col_i16 c, col_i16 unused1)
{
    text_dialog_plain_1048_e7f3(w, &(*g_dialog_ctx), a, b, c, w->flags_86);
    return;
}
