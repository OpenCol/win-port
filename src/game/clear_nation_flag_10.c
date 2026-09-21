/* clear_nation_flag_10 -- shipped at 1020:82c7.
 *
 * Ported by tools/port.py from matched/game/clear_nation_flag_10.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far * far g_cur_nation_rec;            DGROUP:0x4cc4
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void clear_nation_flag_10(void)
{
    col_i16 nation;
    col_i16 b = 0;
    col_i16 c;

    nation = g_turn_nation;
    c = g_cur_nation_rec->flags & 0x10;
    g_cur_nation_rec->flags &= 0xef;
}
