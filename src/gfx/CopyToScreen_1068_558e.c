/* PORT: this file's own declarations of get_ctable_1068_966f, get_win_1068_96e8, wing_stretch removed: the header declares them from the definition (W1 step 3). */
/* CopyToScreen_1068_558e: candidate for 1068:558e (230 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CopyToScreen_1068_558e
 *
 * The STRETCHING CopyToScreen. `CopyToScreen` at `1068:54a3` and this one print the same
 * warning -- "Warning: Port not locked in CopyToScreen", DGROUP:0x2f84 -- and 210 of their
 * 230 bytes differ, which finding 72 had already established. The difference is now
 * readable: `1068:54a3` calls `wing_blit` with EIGHT arguments and copies pixels at
 * their own size, while this one calls `wing_stretch` with TEN and passes a
 * width and a height for BOTH rectangles. It is the scaling overload, and the extra pair of
 * arguments is the whole of the difference in intent.
 *
 * `1068:54a3` keeps the plain name by finding 72's decision -- it has 35 call sites and
 * this one has none -- so the suffix here is not a confidence marker but a tie-break that
 * was already made.
 *
 * Otherwise the shape is the sibling's: warn and lock if the caller did not,
 * `GRPort_ColorSync` with the source's page, blit, and unlock again only if this
 * function was the one that locked. The one thing it does NOT do is the sibling's opening
 * "Error: NULL Port in CopyToScreen" check, which is 20 of the 230 bytes.
 *
 * The field at +0x22 is the WINDOW, not the surface. `MSLockGraf` passes it to
 * `wing_dib_pointer` and `MSGrafOrient` and puts the surface it gets back at +0x18, so a
 * GRPort holds both and they are four bytes apart plus padding. `matched/CopyToScreen.c`
 * calls +0x22 `surface`, which is the one field name in that file the bytes contradict.
 *
 * Eight of the callees arrive as the compiler's own 4-byte PUSH CS / CALL NEAR and are
 * defined here as stubs; `GRPort_ColorSync` is TLINK's 5-byte form and
 * `wing_stretch` is a genuine call, so both stay `extern`.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
/* PORT: this file's own Screen, GRPort, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"


   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */
extern "C" char GRPort_IsLocked(GRPort *p);
extern "C" void report_error_1068_963d(void *ctx, char *msg);
extern "C" void MSLockGraf(GRPort *port);
extern "C" void MSUnlockGraf(GRPort *port);
    /* the rectangle's width */
    /* the rectangle's height */

void CopyToScreen_1068_558e(GRPort *port, POINT *dst_win, POINT *src_rect, POINT *at)
{
    int locked = 1;

    if (!GRPort_IsLocked(port)) {
        report_error_1068_963d(&g_error_log, "Warning: Port not locked in CopyToScreen");
        locked = 0;
        MSLockGraf(port);
    }
    GRPort_ColorSync(port, (CTable *)get_ctable_1068_966f((Screen *)dst_win));
    wing_stretch((GrafRec *)port->win, src_rect->x, src_rect->y, rect_width_1068_96a9((RECT *)src_rect), rect_height_1068_96c8((RECT *)src_rect),
                           (MSWin *)(get_win_1068_96e8((Screen *)dst_win)), at->x, at->y, rect_width_1068_96a9((RECT *)at),
                           rect_height_1068_96c8((RECT *)at));
    if (!locked)
        MSUnlockGraf(port);
}
