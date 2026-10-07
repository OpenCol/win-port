/* palette_remap_table: candidate for 1068:0fd6 (388 bytes), 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 *
 * Builds the table that maps every entry of palette B to its nearest entry in palette A:
 * packs A into RGB triples, builds an RGBTree over them, repacks B into the same buffer,
 * then looks each B triple up (tolerance 0x20) and writes the index into out[].
 */

#include "colonization.h"

/* PORT: this file's own CTable, RGBTree removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */
#include "records.h"



struct logpal;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern "C" void palette_remap_table(struct logpal *pal_a, unsigned char *out, struct logpal *pal_b, short first, short n)
{
    RGBTree tree;
    short h;
    unsigned char *p;
    short i;

    h = MSAllocateHandle((long)(n * 3));
    p = (unsigned char *)MSHLock(h);
    CTable_CopyRawColors((CTable *)pal_a, p, first, n);
    tree.build(p, first, n);
    CTable_CopyRawColors((CTable *)pal_b, p, first, n);
    for (i = 0; n * 3 > i; i += 3)
        out[i / 3] = tree.lookup(p[i], p[i + 1], p[i + 2], 0x20);
    p = (unsigned char *)MSHUnlock(h);
    h = MSDisposeHandle(h);
}
