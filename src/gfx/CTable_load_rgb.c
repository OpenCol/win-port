/* CTable_load_rgb: candidate for 1068:0a5b (99 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_load_rgb
 *
 * Copies count RGB triples from src into the colour table's packed-RGB handle at +0x41c.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void CTable_load_rgb(CTable *table, int count, void *src)
{
    int n = count;
    void *p;

    p = MSHLock(table->hsrc);
    MSCopyMem(src, p, (long)(n * 3));
    p = MSHUnlock(table->hsrc);
}
