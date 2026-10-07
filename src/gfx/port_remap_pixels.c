/* port_remap_pixels: candidate for 1068:62fe (195 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * Walks every pixel of a port's bitmap and replaces each value v >= d with table[v - d];
 * values below d are left alone. The row pointer is and steps by stride - width at
 * the end of each row.
 *
 * THE FOURTH ARGUMENT IS NEVER READ. Both callers push one -- `build_opening_art` passes
 * `(&port, remap, 10, 0xec)` and `MagicWin_remap_to_palette` passes the palette's own count --
 * so the signature is four wide and that is not in doubt. The body simply does not use it:
 * every pixel at or above `d` is remapped whether or not the table has that many entries,
 * so a pixel above `d + count` reads past the end of it. The shipped code is what it is;
 * this is here so the next reader does not go looking for the bounds check.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void port_remap_pixels(Port *p, unsigned char *table, short base, short count)
{
    unsigned char *row;
    short y;
    short x;

    if (p->bits != 0) {
        row = p->bits;
        for (y = 0; p->height > y; y++, row += p->stride - p->width) {
            for (x = 0; p->width > x; x++, row++) {
                if (*row - base >= 0)
                    *row = table[*row - base];
            }
        }
    }
}
