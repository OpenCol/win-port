/* settle_initial_size -- shipped at 1030:8184.
 *
 * Ported by tools/port.py from matched/game/settle_initial_size.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern ARec far g_villages[];                        SEG20:0xcb9c
 *   extern BRec far g_tribe_rec[];                       SEG20:0xd184
 *
 * The file's own ARec, BRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 settle_initial_size(col_i16 i)
{
    col_i16 k = g_villages[i].kind - 4;
    col_i16 r = (g_tribe_rec[k].n + 1) * 2 + 1;
    if (g_villages[i].flags & 4)
        r += g_tribe_rec[k].n + 1;
    return r;
}
