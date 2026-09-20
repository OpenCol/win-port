/* count_fathers_owned -- shipped at 1020:9923.
 *
 * Ported by tools/port.py from matched/game/count_fathers_owned.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Father far g_fathers[];                       SEG20:0xd6fa
 *
 * The file's own Father dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "fathers.h"

extern col_i16 nation_flag_test(col_i16 n, col_i16 bit);

col_i16 count_fathers_owned(col_i16 nation, col_i16 cat)
{
    col_i16 n = 0;
    register col_i16 i;

    for (i = 0; i < FATHER_COUNT; i++) {
        if (nation_flag_test(nation, i) == 0)
            continue;
        if (g_fathers[i].cat != (unsigned char)cat)
            continue;
        n++;
    }
    return n;
}
