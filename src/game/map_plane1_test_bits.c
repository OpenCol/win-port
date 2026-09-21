/* map_plane1_test_bits -- shipped at 1018:6417.
 *
 * Ported by tools/port.py from matched/game/map_plane1_test_bits.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_plane1_at(col_i16 x, col_i16 y);

col_i16 map_plane1_test_bits(col_i16 x, col_i16 y, unsigned char mask)
{
    col_i16 r = 0;

    if (!on_map(x, y))
        goto done;
    if (map_plane1_at(x, y) & mask)
        r = 1;
done:
    return r;
}
