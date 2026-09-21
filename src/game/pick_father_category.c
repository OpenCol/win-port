/* pick_father_category -- shipped at 1020:99cd.
 *
 * Ported by tools/port.py from matched/game/pick_father_category.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 count_fathers_available(col_i16 nation, col_i16 cat);

/* same object: 4-byte `push cs; call ` */


col_i16 pick_father_category(col_i16 nation)
{
    col_i16 best = -1;
    col_i16 bestn = -1;
    register col_i16 i;
    register col_i16 n;

    if (g_nations[nation].fathers >= 0x19)
        return 5;
    for (i = 0; i < 5; i++) {
        n = count_fathers_available(nation, i);
        if (n >= bestn) {
            bestn = n;
            best = i;
        }
    }
    return best;
}
