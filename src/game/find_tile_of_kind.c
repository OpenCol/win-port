/* find_tile_of_kind -- shipped at 1038:c71b.
 *
 * Ported by tools/port.py from matched/game/find_tile_of_kind.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char map_plane2_nibble(col_i16 x, col_i16 y);

extern col_i16 ocean(col_i16 x, col_i16 y);



col_i16 find_tile_of_kind(col_i16 x0, col_i16 y0, col_i16 *px, col_i16 *py, col_i16 kind)
{
    register col_i16 x;
    register col_i16 y;
    col_i16 found = 0;
    col_i16 nib = -1;

    for (x = x0; found == 0 && x0 + 1 >= x; x++) {
        for (y = y0; found == 0 && y0 + 1 >= y; y++) {
            if (ocean(x, y) == kind) {
                nib = map_plane2_nibble(x, y);
                if (kind != 0 && nib != 1)
                    continue;
                *px = x;
                *py = y;
                found = 1;
            }
        }
    }
    return nib;
}
