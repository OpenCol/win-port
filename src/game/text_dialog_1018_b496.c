/* text_dialog_1018_b496 -- shipped at 1018:b496.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1018_b496.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1018_b4fb(void *file, void *name, col_i16 a);

col_i16 text_dialog_1018_b496(void *file, void *name)
{
    return text_dialog_1018_b4fb(file, name, 0);
}
