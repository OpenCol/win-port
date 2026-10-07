/* RGBTree_lookup: candidate for 1068:170f (764 bytes, 2 callers).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: RGBTree::lookup
 *
 * Nearest-colour search over the k-d tree RGBTree_build makes and
 * RGBTree_insert fills. Iterative, with an explicit stack of (axis, node) pairs
 * kept inside the tree structure itself (+0xc, pointer at +0x606). A per-axis window of
 * +/- tol around the query prunes subtrees; the squared distance is 32-bit; the search
 * stops early when an exact hit drives the best distance to zero. Axis 0 is green,
 * 1 is red, 2 is blue. Returns the node's palette index plus the tree's base index,
 * 0 when nothing fell inside the window, -1 when the tree is not built.
 *
 * A METHOD since 2026-10-02, RGBTree::lookup, as its callers call it (`tree.lookup(...)`); it was a
 * function over the record, `tree` now `this`.
 */

#include "colonization.h"

#include "records.h"

#include "classes.h"

short RGBTree::lookup(unsigned char r, unsigned char g, unsigned char b, short tol)
{
    long best;
    short best_node = this->nil;
    long dist;
    short k1, k0, k2;
    short axis = -1;
    short keyval;
    Bounds bounds[3];
    short cur;

    if (this->built == 0)
        return -1;
    best = 0x30000L;
    this->visited = 0;
    bounds[0].lo = g - tol;
    bounds[0].hi = g + tol;
    bounds[1].lo = r - tol;
    bounds[1].hi = r + tol;
    bounds[2].lo = b - tol;
    bounds[2].hi = b + tol;
    this->stack[this->sp++] = axis;
    this->stack[this->sp++] = 0;
    while (this->sp != 0 && best != 0) {
        cur = this->nodes[this->stack[--this->sp]].hi;
        axis = this->stack[--this->sp];
        while (cur != this->nil && best != 0) {
            this->visited++;
            (axis == 2) ? (axis = 0) : axis++;
            keyval = this->nodes[cur].flags[axis];
            if (bounds[axis].lo < keyval) {
                if (bounds[axis].hi >= keyval) {
                    this->stack[this->sp++] = axis;
                    this->stack[this->sp++] = cur;
                    k1 = this->nodes[cur].flags[1];
                    k0 = this->nodes[cur].flags[0];
                    k2 = this->nodes[cur].flags[2];
                    dist = (long)(k1 - r) * (k1 - r) + (long)(k0 - g) * (k0 - g)
                         + (long)(k2 - b) * (k2 - b);
                    if (dist < best) {
                        best = dist;
                        best_node = cur;
                    }
                }
                cur = this->nodes[cur].lo;
            } else
                cur = this->nodes[cur].hi;
        }
    }
    if (this->nodes[best_node].v != -1)
        return this->nodes[best_node].v + this->base;
    else
        return 0;
}
