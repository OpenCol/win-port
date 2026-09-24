/* text_dialog_1048_e911 -- shipped at 1048:e911.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1048_e911.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1048_ecef(void *p, col_i16 a, col_i16 b, col_i16 z, col_i16 c);

col_i16 text_dialog_1048_e911(void *p, col_i16 a, col_i16 b, col_i16 c)
{
    return text_dialog_1048_ecef(p, a, b, 0, c);
}
