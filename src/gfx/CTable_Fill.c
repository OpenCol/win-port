/* CTable_Fill: candidate for 1068:083b (159 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_Fill
 * name-source: sibling-build
 *
 * Sets count entries from start (clipped to 256) from a packed 3-byte-per-entry RGB
 * buffer, then installs them.
 *
 * RENAMED 2026-09-26 from palette_set_range: the Mac build's CTable::Fill(short, short, unsigned char*): this plus first, count and an RGB block, in that order. It sits in the CTable module (1068:0000..0d94) and the width matches.
 */

#include "colonization.h"

struct logpal;
#include "classes.h"
extern void MSSetCTEntry(struct logpal *pal, int i, unsigned char r, unsigned char g, unsigned char b);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_UpdatePal(CTable *pal, int start, int count);
void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb)
{
    int i;

    if (start + count > 0x100)
        count = 0x100 - start;
    for (i = start; start + count > i; i++) {
        MSSetCTEntry((struct logpal *)pal, i, rgb[0], rgb[1], rgb[2]);
        rgb += 3;
    }
    CTable_UpdatePal(pal, start, count);
}
