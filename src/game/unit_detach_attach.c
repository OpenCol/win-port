/* unit_detach_attach -- shipped at 1038:1d92.
 *
 * Ported by tools/port.py from matched/game/unit_detach_attach.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_place_at(col_i16 u, col_i16 a, col_i16 b);
void unit_unlink(col_i16 u);

void unit_detach_attach(col_i16 u, col_i16 a, col_i16 b)
{
    unit_unlink(u);
    unit_place_at(u, a, b);
}
