/* RGBTree_insert: candidate for 1068:15a0 (367 bytes, 2 callers).  Verdict: EXACT.
 *
 * A k-d tree insert over 3-byte RGB keys, used for palette matching: both callers sit
 * beside nearest_palette_entry, build the key from three palette bytes and pass the
 * palette index as the value. Nodes are 10 bytes: three key bytes, a pad, a
 * value word, then lo and hi child indices. Node 0 is a header whose hi child is the
 * root; the tree carries its own nil index at +6. The axis cycles 0,1,2 with the depth.
 */

#include "colonization.h"

#include "records.h"

#include "classes.h"

void RGBTree_insert(RGBTree *tree, RGBTreeNode *key, short n)
{
    short parent = 0;
    short cur = tree->nodes[0].hi;
    short axis = -1;
    short went_hi = 1;

    tree->nodes[n].flags[1] = key->flags[1];
    tree->nodes[n].flags[0] = key->flags[0];
    tree->nodes[n].flags[2] = key->flags[2];
    tree->nodes[n].v = key->v;
    tree->nodes[n].lo = tree->nodes[n].hi = tree->nil;
    while (cur != tree->nil) {
        parent = cur;
        (axis == 2) ? (axis = 0) : axis++;
        went_hi = 0;
        if (tree->nodes[cur].flags[axis] <= key->flags[axis]) {
            cur = tree->nodes[cur].hi;
            went_hi++;
        } else
            cur = tree->nodes[cur].lo;
    }
    if (went_hi == 0) tree->nodes[parent].lo = n;
    else tree->nodes[parent].hi = n;
    return;
}
