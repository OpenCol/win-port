/* text_plain_step_1010_ed15 -- shipped at 1010:ed15.
 *
 * Ported by tools/port.py from matched/game/text_plain_step_1010_ed15.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_plain_1010_ef3c(void *p, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);

col_i16 text_plain_step_1010_ed15(void *p, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e)
{
    return text_dialog_plain_1010_ef3c(p, a, b, c, d, 0, e);
}
