/* RGBTree_build: candidate for 1068:1351 (591 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: RGBTree::build
 *
 * Builds the palette k-d tree that RGBTree_insert (1068:15a0) fills: allocates and
 * locks a handle for count+2 nodes, makes node 0 the header and node count+1 the nil
 * sentinel, then inserts every palette entry starting from the one nearest mid-grey
 * (RGBTree_nearest_grey) and walking outward -- downward first, then upward -- so the
 * tree is roughly balanced around the middle of the colour cube. The palette is a 
 * pointer to RGB triples (the index arithmetic goes through __AHSHIFT).
 *
 * A METHOD since 2026-10-02, RGBTree::build, as its callers call it (`tree.build(...)`); it was a
 * function over the record, `tree` now `this`.
 */

#include "colonization.h"

#include "records.h"

#include "classes.h"

extern short RGBTree_nearest_grey(RGBTree *tree, unsigned char *pal);
extern void RGBTree_insert(RGBTree *tree, RGBTreeNode *key, short n);

void RGBTree::build(unsigned char *pal, short base, short count)
{
    RGBTreeNode key;
    short mid;
    short up;
    short next = 1;
    short j;

    this->base = base;
    this->count = count;
    this->handle = MSAllocateHandle((this->count + 2) * sizeof(RGBTreeNode));
    this->nodes = (RGBTreeNode *)MSHLock(this->handle);
    this->nil = this->count + 1;
    this->nodes[0].v = -1;
    this->nodes[0].lo = this->nil;
    this->nodes[0].hi = this->nil;
    this->nodes[this->nil].v = -1;
    this->nodes[this->nil].lo = this->nodes[this->nil].hi = this->nil;
    key.lo = 0;
    key.hi = 0;
    mid = RGBTree_nearest_grey(this, pal);
    up = mid + 1;
    for (; mid > -1; mid--) {
        j = mid * 3;
        key.flags[1] = pal[j];
        j++;
        key.flags[0] = pal[j];
        j++;
        key.flags[2] = pal[j];
        key.v = mid;
        RGBTree_insert(this, &key, next++);
    }
    for (; this->count > up; up++) {
        j = up * 3;
        key.flags[1] = pal[j];
        j++;
        key.flags[0] = pal[j];
        j++;
        key.flags[2] = pal[j];
        key.v = up;
        RGBTree_insert(this, &key, next++);
    }
    this->built = 1;
}
