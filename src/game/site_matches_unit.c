/* site_matches_unit -- shipped at 1028:b1f8.
 *
 * Ported by tools/port.py from matched/game/site_matches_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern struct unit far g_units[];                    SEG20:0xa208
 *   extern struct colony far g_colonies[];               SEG20:0x7c28
 *
 * The file's own struct unit, struct colony dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f0 -> x, f1 -> y, f3 -> flags
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes */
/* 0xca bytes */




col_i16 site_matches_unit(col_i16 a, col_i16 b)
{
    col_i16 r = 0;

    if (b == 999) {
        if (g_units[a].x == (char)((g_units[a].flags & 0xf) - 0x14))
            r = 1;
    }
    else if (g_units[a].x == g_colonies[b].x && g_units[a].y == g_colonies[b].y)
        r = 1;
    return r;
}
