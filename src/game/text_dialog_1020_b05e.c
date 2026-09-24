/* text_dialog_1020_b05e -- shipped at 1020:b05e.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1020_b05e.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1020_b169(void *file, void *name, col_i16 a, col_i16 b);

col_i16 text_dialog_1020_b05e(void *file, void *name, col_i16 a)
{
    return text_dialog_1020_b169(file, name, 0, a);
}
