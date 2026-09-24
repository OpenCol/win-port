/* text_dialog_1038_8b3c -- shipped at 1038:8b3c.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1038_8b3c.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1038_8c3c(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);

col_i16 text_dialog_1038_8b3c(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e)
{
    return text_dialog_1038_8c3c(a, b, c, d, 0, e);
}
