/* unit_set_cargo_type -- shipped at 1018:7ca5.
 *
 * Ported by tools/port.py from matched/game/unit_set_cargo_type.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes */

col_i16 unit_set_cargo_type(col_i16 unit, col_i16 slot, col_i16 type)
{
    col_i16 mask = 0xf0;
    col_i16 off = slot >> 1;
    col_i16 cur = ((unsigned char *)((unsigned char *)&(g_units[unit]) + 0xd))[off];
    if (slot & 1) {
        mask = mask >> 4;
        type = type << 4;
    }
    cur = (cur & mask) | type;
    ((unsigned char *)((unsigned char *)&(g_units[unit]) + 0xd))[off] = cur;
    return cur;
}
