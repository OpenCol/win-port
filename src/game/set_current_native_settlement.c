/* set_current_native_settlement -- shipped at 1038:004a.
 *
 * Ported by tools/port.py from matched/game/set_current_native_settlement.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern int far g_cur_settlement;                     DGROUP:0x4c34
 *   extern Settlement far * far g_cur_settlement_rec;    DGROUP:0x4c2c
 *
 * The file's own Settlement dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_current_tribe(col_i16 t);

/* same object: 4-byte `push cs; call ` (rule 47) */

void set_current_native_settlement(col_i16 s)
{
    g_cur_settlement = s;
    if (s < 0)
        goto done;
    if (s < 0 || g_settlement_count <= s)
        s = 0;
    g_cur_settlement_rec = &g_settlements[s];
    set_current_tribe(g_cur_settlement_rec->owner - 4);
done:
    ;
}
