/* colonist_set_spec_nib -- shipped at 1018:4a39.
 *
 * Ported by tools/port.py from matched/game/colonist_set_spec_nib.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);

/* 0xca bytes */




void colonist_set_spec_nib(col_i16 c, col_i16 v)
{
    col_i16 mask;

    if (g_cur_colony_rec->pop > c) {
        mask = 0xf0;
        v = min_int_1018_b329(v, 15);
        if (c & 1) {
            mask = mask >> 4;
            v = v << 4;
        }
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x60))[c >> 1] &= mask;
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x60))[c >> 1] |= v;
    }
}
