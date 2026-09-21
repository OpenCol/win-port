/* colony_flag2_set -- shipped at 1018:3d48.
 *
 * Ported by tools/port.py from matched/game/colony_flag2_set.c.
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


void colony_flag2_set(col_i16 bit, col_i16 on)
{
    col_i16 idx = bit >> 3;
    col_i16 mask = 1 << (bit & 7);
    unsigned char *p = &((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x8a))[idx];

    if (on != 0)
        *p |= mask;
    else
        *p &= ~mask;
}
