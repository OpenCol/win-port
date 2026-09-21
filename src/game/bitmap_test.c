/* bitmap_test -- shipped at 1018:3d17.
 *
 * Ported by tools/port.py from matched/game/bitmap_test.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 bitmap_test(col_i16 n)
{
    return ((char *)((unsigned char *)(g_cur_colony_rec) + 0x8a))[n >> 3] & (1 << (n & 7));
}
