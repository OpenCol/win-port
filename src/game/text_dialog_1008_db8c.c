/* text_dialog_1008_db8c -- shipped at 1008:db8c.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1008_db8c.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 text_dialog_1008_e651(void *file, void *name, col_i16 a);

col_i16 text_dialog_1008_db8c(void *file, void *name)
{
    return text_dialog_1008_e651(file, name, 0);
}
