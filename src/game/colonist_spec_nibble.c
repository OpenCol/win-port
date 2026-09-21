/* colonist_spec_nibble -- shipped at 1018:49ec.
 *
 * Ported by tools/port.py from matched/game/colonist_spec_nibble.c.
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


col_i16 colonist_spec_nibble(col_i16 c)
{
    col_i16 r;

    if (g_cur_colony_rec->pop > c) {
        r = ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x60))[c >> 1];
        if (c & 1)
            r = r >> 4;
        r = r & 0xf;
    } else {
        r = 0;
    }
    return r;
}
