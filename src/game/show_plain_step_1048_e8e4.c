/* show_plain_step_1048_e8e4 -- shipped at 1048:e8e4.
 *
 * Ported by tools/port.py from matched/game/show_plain_step_1048_e8e4.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_plain_step_1048_de5f(void *p, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e);

col_i16 show_plain_step_1048_e8e4(void *p, col_i16 a, col_i16 b, col_i16 c, col_i16 d)
{
    return text_plain_step_1048_de5f(p, a, b, c, d, 0);
}
