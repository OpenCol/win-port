/* get_valid_square_2: candidate for 1038:c71b (146 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_valid_square_2
 *
 * Scans the 2x2 block at (x0, y0) for a tile whose terrain_is_ocean() flag equals
 * `kind`; for land (kind 0) any such tile is taken, for ocean only one whose
 * plane-2 nibble is 1.  The tile goes to *px/*py; the last nibble read (or
 * -1) is returned.  continent_at is -called and defined in-file
 * as a stub.
 *
 * RENAMED 2026-09-26 from find_tile_of_kind: the Mac build's get_valid_square_2(short, short, short*, short*, short), bound by call-sequence alignment (1.5 aligned gaps in compute_moves leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.10x this one.
 */

#include "colonization.h"

extern int g_stub_a;
extern int terrain_is_ocean(int x, int y);

extern unsigned char continent_at(int x, int y);

int get_valid_square_2(int x0, int y0, int *px, int *py, int kind)
{
    register int x;
    register int y;
    int found = 0;
    int nib = -1;

    for (x = x0; found == 0 && x0 + 1 >= x; x++) {
        for (y = y0; found == 0 && y0 + 1 >= y; y++) {
            if (terrain_is_ocean(x, y) == kind) {
                nib = continent_at(x, y);
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
