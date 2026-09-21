/* route_entry_init -- shipped at 1028:c5ca.
 *
 * Ported by tools/port.py from matched/game/route_entry_init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Route far *g_cur_route_stop;                  DGROUP:0x4c1c
 *
 * The file's own Route dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void route_stop_flag_nibble_set(col_i16 which, col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

void route_entry_init(col_i16 v)
{
    g_cur_route_stop->stop = v;
    route_stop_flag_nibble_set(1, 0);
    route_stop_flag_nibble_set(0, 0);
}
