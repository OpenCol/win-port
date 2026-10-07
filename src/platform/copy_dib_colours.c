/* copy_dib_colours: candidate for 1070:a8c0 (124 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Reads `n` entries of the WinG surface's DIB colour table starting at `first` into tbl[0..n-1],
 * then hands tbl[first..first+n-1] to `CTable_Set` as an index and three separate components --
 * the entries just read only when `first` is 0.
 *
 * The RGBQUAD order is the giveaway and it is backwards from the name: the table entry is
 * blue, green, red, reserved, and the three bytes are pushed +0, +1, +2 -- so the LAST push,
 * which is the first argument after the index, is the byte at +2, the RED. The helper takes
 * (palette, index, r, g, b).
 *
 * The table is a 0x400-byte local -- 256 entries -- and the loop indexes it with `SHL BX,2`
 * and a separate `LEA` per component, which is three address computations per entry where
 * one would have done.
 *
 * Nothing calls this in the disassembly, so the argument list is the body's evidence alone.
 */

#include "colonization.h"

#include "classes.h"
typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;
#include "records.h"
extern unsigned WinGGetDIBColorTable(unsigned hdc, unsigned first, unsigned n,
                                            RGBQUAD *tbl);
extern void CTable_Set(void *pal, int i, unsigned char r, unsigned char g,
                              unsigned char b);

void copy_dib_colours(GrafRec *graf, void *pal, int first, int n)
{
    RGBQUAD tbl[0x100];
    register int i;

    WinGGetDIBColorTable(graf->hdc, first, n, tbl);
    for (i = first; first + n > i; i++)
        CTable_Set((CTable *)pal, i, tbl[i].rgbRed, tbl[i].rgbGreen, tbl[i].rgbBlue);
}
