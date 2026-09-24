/* pick_unit_at -- shipped at 1008:3ca7.
 *
 * Ported by tools/port.py from matched/game/pick_unit_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_4b00;                                   DGROUP:0x4b00
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 pick_unit_in_stack(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */



void pick_unit_at(void *ctx, col_i16 x, col_i16 y)
{
    g_4b00 = 1;
    pick_unit_in_stack(x, y);
}
