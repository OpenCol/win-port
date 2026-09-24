/* text_dialog_1048_e6ed -- shipped at 1048:e6ed.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1048_e6ed.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1048_e911(void *p, col_i16 a, col_i16 b, col_i16 c);

col_i16 text_dialog_1048_e6ed(void *p, col_i16 a, col_i16 b)
{
    return text_dialog_1048_e911(p, a, b, 0);
}
