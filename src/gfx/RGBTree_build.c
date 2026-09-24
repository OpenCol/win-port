/* RGBTree_build -- shipped at 1068:1351.
 *
 * Ported by tools/port.py from matched/gfx/RGBTree_build.c.
 *
 * The file's own Node dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct {
    col_i16 handle;
    Node *nodes;
    col_i16 nil;
    col_i16 count;
    col_i16 base;
    col_i16 stack[(0x606 - 0xc) / 2];
    col_i16 sp;
    col_i16 built;
    col_i16 visited;
} RGBTree;

extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern col_i16 RGBTree_nearest_grey(RGBTree *t, unsigned char *pal);
extern void RGBTree_insert(RGBTree *t, Node *key, col_i16 n);

void RGBTree_build(RGBTree *t, unsigned char *pal, col_i16 base, col_i16 count)
{
    Node key;
    col_i16 mid;
    col_i16 up;
    col_i16 next = 1;
    col_i16 j;

    t->base = base;
    t->count = count;
    t->handle = MSAllocateHandle((t->count + 2) * sizeof(Node));
    t->nodes = MSHLock(t->handle);
    t->nil = t->count + 1;
    t->nodes[0].v = -1;
    t->nodes[0].lo = t->nil;
    t->nodes[0].hi = t->nil;
    t->nodes[t->nil].v = -1;
    t->nodes[t->nil].lo = t->nodes[t->nil].hi = t->nil;
    key.lo = 0;
    key.hi = 0;
    mid = RGBTree_nearest_grey(t, pal);
    up = mid + 1;
    for (; mid > -1; mid--) {
        j = mid * 3;
        ((unsigned char *)((unsigned char *)&(key) + 0x0))[1] = pal[j];
        j++;
        ((unsigned char *)((unsigned char *)&(key) + 0x0))[0] = pal[j];
        j++;
        ((unsigned char *)((unsigned char *)&(key) + 0x0))[2] = pal[j];
        key.v = mid;
        RGBTree_insert(t, &key, next++);
    }
    for (; t->count > up; up++) {
        j = up * 3;
        ((unsigned char *)((unsigned char *)&(key) + 0x0))[1] = pal[j];
        j++;
        ((unsigned char *)((unsigned char *)&(key) + 0x0))[0] = pal[j];
        j++;
        ((unsigned char *)((unsigned char *)&(key) + 0x0))[2] = pal[j];
        key.v = up;
        RGBTree_insert(t, &key, next++);
    }
    t->built = 1;
}
