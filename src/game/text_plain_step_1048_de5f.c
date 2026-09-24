/* text_plain_step_1048_de5f -- shipped at 1048:de5f.
 *
 * Ported by tools/port.py from matched/game/text_plain_step_1048_de5f.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_plain_1048_e7f3(void *p, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);

col_i16 text_plain_step_1048_de5f(void *p, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e)
{
    return text_dialog_plain_1048_e7f3(p, a, b, c, d, 0, e);
}
