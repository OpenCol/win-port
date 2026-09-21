/* current_colony_flag_get -- shipped at 1018:3de8.
 *
 * Ported by tools/port.py from matched/game/current_colony_flag_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cur_colony_idx;                         DGROUP:0x4ba2
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_flag_get(col_i16 colony, col_i16 bit);

col_i16 current_colony_flag_get(col_i16 bit)
{
    return colony_flag_get(g_cur_colony_idx, bit);
}
