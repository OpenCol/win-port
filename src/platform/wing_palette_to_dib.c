/* wing_palette_to_dib: candidate for 1070:a9e6 (175 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Copies a whole GDI palette into a WinG surface's colour table: 256 `PALETTEENTRY`s out with
 * `GetPaletteEntries`, converted one at a time into 256 `RGBQUAD`s, and in with
 * `WinGSetDIBColorTable`.
 *
 * THE CONVERSION IS THE RGB-TO-BGR SWAP AND THE BYTES SPELL IT OUT. Red at +0 of the source
 * goes to +2 of the destination, green at +1 to +1, blue at +2 to +0, and the fourth byte is
 * zeroed. Three `LEA`s per field on each side, computed from scratch every time, because
 * `-Od` keeps nothing.
 *
 * Both tables are 0x400 bytes of frame, which is what says 256 entries of four bytes each --
 * the loop bound of 0x100 agrees, and the `SHL BX,2` says the stride.
 *
 * This is `wing_set_dib_colours`'s neighbour and does the same job from the other
 * end: that one asks a helper for each entry's three components and is parked on a scheduling
 * difference; this one takes them all in one `GetPaletteEntries` and has no calls in the loop
 * at all, which is why it comes out.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;
#include "records.h"

extern int WinGSetDIBColorTable(unsigned hdc, unsigned start, unsigned n,
                                           RGBQUAD *t);

void wing_palette_to_dib(GrafRec *graf, unsigned hpal)
{
    PALETTEENTRY pe[0x100];
    RGBQUAD tbl[0x100];
    register int i;

    GetPaletteEntries(hpal, 0, 0x100, pe);
    for (i = 0; i < 0x100; i++) {
        tbl[i].rgbRed = pe[i].peRed;
        tbl[i].rgbGreen = pe[i].peGreen;
        tbl[i].rgbBlue = pe[i].peBlue;
        tbl[i].rgbReserved = 0;
    }
    WinGSetDIBColorTable(graf->hdc, 0, 0x100, tbl);
}
