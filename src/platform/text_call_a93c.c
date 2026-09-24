/* text_call_a93c -- shipped at 1070:a9c0.
 *
 * Ported by tools/port.py from matched/platform/text_call_a93c.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void wing_set_dib_colours(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);

void text_call_a93c(col_i16 a, col_i16 b, col_i16 c, col_i16 d)
{
    wing_set_dib_colours(a, b, c, d, 0, 0x100);
}
