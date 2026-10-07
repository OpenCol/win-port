/* PORT: this file's own declarations of CTable_SetAnimate removed: the header declares it from the definition (W1 step 3). */
/* CTable_SetScale: candidate for 1068:08da (164 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_SetScale
 * name-source: sibling-build
 *
 * Re-derives a colour table's packed RGB triples from its LOGPALETTE (the object's first
 * 0x404 bytes) for `count` entries from `first`, then marks and installs them. Records the
 * three arguments in the object and keeps the handle at +0x41c.
 *
 * RENAMED 2026-09-26 from palette_reinstall: the Mac build's CTable::SetScale(short, short, short): this plus first, count and mode, recorded in the table with the packed source CTable_Scale then fades from. It sits in the CTable module (1068:0000..0d94) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
struct logpal;

extern void MSCopyRawColors(struct logpal *pal, unsigned char *dst, int first, int count);
/* same object: 4-byte `push cs; call ` (rule 47) */

void CTable_SetScale(CTable *table, short first, short count, short step_count)
{
    short f = first;
    short c = count;
    unsigned char *p;

    table->steps = step_count;
    table->first = f;
    table->count = c;
    table->hsrc = MSAllocateHandle((long)(c * 3));
    p = (unsigned char *)MSHLock(table->hsrc);
    MSCopyRawColors((struct logpal *)table, p, f, c);
    p = (unsigned char *)MSHUnlock(table->hsrc);
    CTable_SetAnimate(table, f, c);
}
