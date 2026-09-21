/* current_nation_boycotts -- shipped at 1048:497c.
 *
 * Ported by tools/port.py from matched/game/current_nation_boycotts.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 current_nation_boycotts(col_i16 g)
{
    return g_cur_nation_rec->boycott & (1 << g);
}
