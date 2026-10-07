/* CTable_UpdatePal: candidate for 1068:0000 (78 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_UpdatePal
 * name-source: sibling-build
 *
 * Installs count entries from start (clipped to 256) into the palette's own GDI handle
 * (+0x404), then reseeds the random generator from the palette.
 *
 * RENAMED 2026-09-26 from palette_install: the Mac build's CTable::UpdatePal(short, short): this plus a range, installed into the table's GDI palette (+0x404), after which the seed is refreshed. It sits in the CTable module (1068:0000..0d94) and the width matches.
 */

#include "colonization.h"

#include "records.h"
extern void MSUpdatePalette(Palette *pal, unsigned hpal, int start, int count);
#include "classes.h"
extern void CTable_Reseed(CTable *p);

void CTable_UpdatePal(CTable *p, int start, int count)
{
    if (start + count > 0x100)
        count = 0x100 - start;
    MSUpdatePalette((Palette *)p, p->hpal, start, count);
    CTable_Reseed(p);
}
