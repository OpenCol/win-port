/* GRPort_InitStruct: candidate for 1068:367b (236 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_InitStruct
 *
 * Sets a GRPort's geometry from a source rectangle. The surface at +0x18, the window at
 * +0x22, the word at +0x20 and the word at +6 are cleared first; then the port's width and
 * height come from the rectangle (+4 minus +0, +6 minus +2) and both of the port's own
 * rectangles -- at +8 and +0x10 -- are set to (0, 0, width, height). A null rectangle takes
 * the same path with zeros throughout, which is why the `SetRect` calls appear twice.
 *
 * The rectangle is tested with one `CMP DWORD` and so is ``, not `` (rule 38).
 *
 * Both 38-byte layouts this branch has recorded for GRPort agree with it: the surface at
 * +0x18 and the window at +0x22 are what matched/CopyToScreen.c and MSLockGraf already
 * assert, and this function is the writer that zeroes both.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void GRPort_InitStruct(GRPort *p, RECT *rect)
{
    p->bits = 0L;
    p->win = 0L;
    p->current = 0;
    p->stride = 0;
    if (rect != 0) {
        p->h = rect->bottom - rect->top;
        p->w = rect->right - rect->left;
        SetRect(&p->bounds, 0, 0, p->w, p->h);
        SetRect(&p->rect, 0, 0, p->w, p->h);
    } else {
        p->h = 0;
        p->w = 0;
        SetRect(&p->bounds, 0, 0, 0, 0);
        SetRect(&p->rect, 0, 0, 0, 0);
    }
}
