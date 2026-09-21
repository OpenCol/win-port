/* RGBTree_insert -- shipped at 1068:15a0.
 *
 * Ported by tools/port.py from matched/gfx/RGBTree_insert.c.
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

void RGBTree_insert(RGBTree *t, Node *key, col_i16 n)
{
    col_i16 parent = 0;
    col_i16 cur = t->nodes[0].hi;
    col_i16 axis = -1;
    col_i16 went_hi = 1;

    ((unsigned char *)((unsigned char *)&(t->nodes[n]) + 0x0))[1] = ((unsigned char *)((unsigned char *)(key) + 0x0))[1];
    ((unsigned char *)((unsigned char *)&(t->nodes[n]) + 0x0))[0] = ((unsigned char *)((unsigned char *)(key) + 0x0))[0];
    ((unsigned char *)((unsigned char *)&(t->nodes[n]) + 0x0))[2] = ((unsigned char *)((unsigned char *)(key) + 0x0))[2];
    t->nodes[n].v = key->v;
    t->nodes[n].lo = t->nodes[n].hi = t->nil;
    while (cur != t->nil) {
        parent = cur;
        (axis == 2) ? (axis = 0) : axis++;
        went_hi = 0;
        if (((unsigned char *)((unsigned char *)&(t->nodes[cur]) + 0x0))[axis] <= ((unsigned char *)((unsigned char *)(key) + 0x0))[axis]) {
            cur = t->nodes[cur].hi;
            went_hi++;
        } else
            cur = t->nodes[cur].lo;
    }
    if (went_hi == 0) t->nodes[parent].lo = n;
    else t->nodes[parent].hi = n;
    return;
}
