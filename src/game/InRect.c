/* InRect: candidate for 1048:174f (78 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Whether (x, y) is inside the inclusive rectangle the record starts with.
 * Two separate `return 0` blocks in the shipped code say the source nests the
 * y test inside the x test rather than running one four-operand chain.

 * The stated size was 74 and the function is 78: `tools/re_agent/check_sizes.py`
 * found the extra bytes decoding as an unreachable epilogue, and the reconstruction
 * already grades over the whole extent. Only the number moved.
 *
 * RENAMED 2026-09-26 from rect_contains_1048_174f: the Mac build's InRect(Rect*, short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between ColChild_PackQueue and ColChild_DragIconMove, where the Mac gap holds 1 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 0.92x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
int InRect(RECT *r, int x, int y)
{
    if (r->left <= x && r->right >= x) {
        if (r->top <= y && r->bottom >= y)
            return 1;
        else
            return 0;
    } else {
        return 0;
    }
}
