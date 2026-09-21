/* colonist_move_kind -- shipped at 1018:4e69.
 *
 * Ported by tools/port.py from matched/game/colonist_move_kind.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0xca bytes */


col_i16 colonist_move_kind(col_i16 c, col_i16 job)
{
    col_i16 r;

    if (g_cur_colony_rec->pop > c) {
        if (job < 0x13)
            r = 0;
        else
            r = 2;
    } else {
        if (job >= 0x13)
            r = 1;
        else
            r = 3;
    }
    return r;
}
