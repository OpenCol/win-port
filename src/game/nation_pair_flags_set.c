/* nation_pair_flags_set -- shipped at 1030:7e86.
 *
 * Ported by tools/port.py from matched/game/nation_pair_flags_set.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_nation_pair[][0x13c];     SEG20:0xc328
 *   extern Tribe far g_tribes[];                         SEG20:0xd182
 *
 * The file's own Tribe dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 nation_pair_flags_set(col_i16 a, col_i16 b, col_i16 v)
{
    if (a >= 4)
        ((unsigned char *)&g_tribes[a] - 0xfe)[b] = v;
    else
        g_nation_pair[a][b] = v;
    return v;
}
