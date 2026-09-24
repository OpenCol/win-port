/* colony_flag_get -- shipped at 1018:3da3.
 *
 * Ported by tools/port.py from matched/game/colony_flag_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 colony_flag_get(col_i16 colony, col_i16 bit)
{
    if (bit < 0)
        return 0;
    return ((signed char *)((unsigned char *)&(g_colonies[colony]) + 0x84))[bit >> 3] & (1 << (bit & 7));
}
