/* unit_cargo_type -- shipped at 1018:7be8.
 *
 * Ported by tools/port.py from matched/game/unit_cargo_type.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


col_i16 unit_cargo_type(col_i16 u, col_i16 slot)
{
    col_i16 v;
    col_i16 idx;

    if (g_units[u].cargo_count <= slot) {
        v = -1;
    } else {
        idx = slot >> 1;
        v = ((unsigned char *)((unsigned char *)&(g_units[u]) + 0xd))[idx];
        if (slot & 1)
            v = v >> 4;
        else
            v = v & 0xf;
    }
    return v;
}
