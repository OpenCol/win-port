/* MSTagNonAnimate: candidate for 1070:0208 (114 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * MSSetCTEntry for entries first .. first+n-1, clipped at 256, each taking the colour
 * of entry n -- the clipped count doubles as the source index, which is what the bytes
 * say (SI never moves; DI runs). Odd, but the original's.
 *
 * RENAMED 2026-09-27 from palette_fill_range_from: the Mac build's MSTagNonAnimate(MSCTable*, short, short), the partner of MSTagAnimate one function before it; CTable_ResetAnimate calls it and then CTable_UpdatePal, as the Mac ResetAnimate does.
 */

#include "colonization.h"

#include "w16types.h"

struct logpal { unsigned short version; unsigned short count; PALETTEENTRY entries[1]; };
extern void MSSetCTEntry(struct logpal *pal, int i, unsigned char r, unsigned char g, unsigned char b);

void MSTagNonAnimate(struct logpal *pal, int first, int count)
{
    int n = count;
    int i;

    if (first + n > 0x100)
        n = 0x100 - first;
    for (i = first; first + n > i; i++)
        MSSetCTEntry(pal, i, pal->entries[n].peRed, pal->entries[n].peGreen, pal->entries[n].peBlue);
}
