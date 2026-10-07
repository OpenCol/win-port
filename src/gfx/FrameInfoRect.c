/* FrameInfoRect: candidate for 1068:c41f (284 bytes).  Verdict: RELOCATABLE.
 * symbol: FrameInfoRect
 * name-source: sibling-build
 *
 * Draws a bevelled frame `depth` pixels thick around a rectangle: the right and bottom edges in
 * one colour, the left and top in the other, and the rectangle shrunk by one all round before
 * the next ring.
 *
 * THE SIGN OF `depth` SWAPS THE TWO COLOURS. Positive gives the fifth argument to the right and
 * bottom edges; negative gives it to the left and top instead, and the magnitude is the
 * thickness -- one function for a raised button and a sunken one, told apart by a minus sign.
 *
 * The rectangle is modified THROUGH THE CALLER'S POINTER: `InflateRect(r, -1, -1)` shrinks the
 * caller's own rectangle once per ring, so a caller that wants it back has to keep a copy.
 *
 * `while (n-- != 0)` is a post-decrement -- the counter is read into AX, decremented in SI and
 * the value that was read is what is tested -- so `depth` of zero draws nothing and the loop
 * cannot run 65535 times on it.
 *
 * RENAMED 2026-09-27 from draw_bevel: Mac FrameInfoRect(GRPort&, Rect*, short, short, short), 256 bytes, against 284. Callers: 13 of 13 match (DoExtendedLaborReport, DoTerrainArticle, SetNations/SetDifficulty DrawIt and Init, DrawListArea, ColChild_DrawNonClient, Fame, HandleLeftClick, PostLowMemReport, TradeRoutes, get_game_options). Callees: TextColor x2 and DrawLine x4 on both. Layout: after DoFizzle(8) (wipe_rows) on both. Arity 5 = (port, r, depth, light, dark).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_DrawLine(GRPort *p, short x1, short y1, short x2, short y2);

void FrameInfoRect(GRPort *port, RECT *r, int depth, int light, int dark)
{
    int lo;
    register int n;
    register int hi;

    n = depth;
    if (n > 0) {
        hi = dark;
        lo = light;
    } else {
        hi = light;
        lo = dark;
        n = -n;
    }
    while (n-- != 0) {
        TextColor(hi);
        GRPort_DrawLine(port, r->right - 1, r->top, r->right - 1, r->bottom - 1);
        GRPort_DrawLine(port, r->right - 1, r->bottom - 1, r->left, r->bottom - 1);
        TextColor(lo);
        GRPort_DrawLine(port, r->left, r->bottom - 1, r->left, r->top);
        GRPort_DrawLine(port, r->left, r->top, r->right - 1, r->top);
        InflateRect(r, -1, -1);
    }
}
