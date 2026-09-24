/* village_at -- shipped at 1038:0548.
 *
 * Ported by tools/port.py from matched/game/village_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { unsigned char x, y; char rest[16]; } Settlement_local; /* 18 bytes at SEG20:0xcb9a */


extern col_i16 square_native_nation(col_i16 x, col_i16 y);

col_i16 village_at(col_i16 x, col_i16 y)
{
    register col_i16 found = -1;
    register col_i16 i;

    if (square_native_nation(x, y) < 0)
        goto done;
    for (i = 0; found < 0 && i < g_settlement_count; i++) {
        if (g_settlements[i].x == (unsigned char)x && g_settlements[i].y == (unsigned char)y)
            found = i;
    }
done:
    return found;
}
