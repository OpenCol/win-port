/* palette_nearest_index: candidate for 1068:1176 (227 bytes), 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 *
 * Builds a temporary RGBTree over all 256 entries of a LOGPALETTE and returns the index
 * of the entry nearest (r, g, b) within a tolerance of 0x20. The tree is a local object,
 * so the function carries Borland's exception block (__InitExceptBlock) for its destructor.
 */

#include "colonization.h"

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
#include "records.h"

class RGBTree {
public:
    short handle;
    RGBTreeNode *nodes;
    short nil;
    short count;
    short base;
    short stack[(0x606 - 0xc) / 2];
    short sp;
    short built;
    short visited;
    RGBTree();
    ~RGBTree();
    void build(unsigned char *pal, short base, short count);
    void insert(RGBTreeNode *key, short n);
    short lookup(unsigned char r, unsigned char g, unsigned char b, short tol);
};

struct logpal;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern "C" short palette_nearest_index(struct logpal *pal, unsigned char r, unsigned char g, unsigned char b)
{
    RGBTree tree;
    short h = MSAllocateHandle(0x300L);
    unsigned char *p = (unsigned char *)MSHLock(h);
    short result;

    CTable_CopyRawColors((CTable *)pal, p, 0, 0x100);
    tree.build(p, 0, 0x100);
    result = tree.lookup(r, g, b, 0x20);
    p = (unsigned char *)MSHUnlock(h);
    h = MSDisposeHandle(h);
    return result;
}
