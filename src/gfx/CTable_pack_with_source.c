/* PORT: this file's own declarations of CTable_SetAnimate removed: the header declares it from the definition (W1 step 3). */
/* CTable_pack_with_source: candidate for 1068:0c8b (265 bytes, 0 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_pack_with_source
 *
 * Like CTable_SetScale, but packs two RGB tables: the colour table's own LOGPALETTE
 * into the handle at +0x418 and a source object's palette (reached through a getter)
 * into the one at +0x41a, then marks and installs. Records mode, first and count at
 * +0x40e..+0x412. No caller in the shipped binary.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
struct logpal;

extern void MSCopyRawColors(struct logpal *pal, unsigned char *dst, int first, int count);
/* same object: 4-byte `push cs; call ` (rule 47) */

void CTable_pack_with_source(CTable *ctable, short first, short count, short mode, void *src)
{
    short f = first;
    short c = count;
    unsigned char *p;

    ctable->steps2 = mode;
    ctable->first2 = f;
    ctable->count2 = c;
    ctable->hsrc_a = MSAllocateHandle((long)(c * 3));
    ctable->hsrc_b = MSAllocateHandle((long)(c * 3));
    p = (unsigned char *)MSHLock(ctable->hsrc_a);
    MSCopyRawColors((struct logpal *)ctable, p, f, c);
    p = (unsigned char *)MSHUnlock(ctable->hsrc_a);
    p = (unsigned char *)MSHLock(ctable->hsrc_b);
    MSCopyRawColors((struct logpal *)(identity_ptr_1068_9625(src)), p, f, c);
    p = (unsigned char *)MSHUnlock(ctable->hsrc_b);
    CTable_SetAnimate(ctable, f, c);
}
