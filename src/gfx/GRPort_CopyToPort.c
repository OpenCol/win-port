/* PORT: this file's own declarations of GRPort_GetWin_1068_968c removed: the header declares it from the definition (W1 step 3). */
/* GRPort_CopyToPort: candidate for 1068:52d0 (467 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_CopyToPort
 * name-source: sibling-build
 *
 * Copies the overlap of two rectangles from one port to another through the assembly
 * blitter asm_copy_bits (pixel pointers, source and destination origins, size, plane counts,
 * signed pitches). A port without a locked buffer (+0x18) is reported
 * ("Warning: Source Port not locked in CopyToPort" / "Warning: Destination Port not locked in CopyToPort"), locked for the copy and unlocked again.
 *
 * RENAMED 2026-09-27 from CopyToPort: Mac GRPort::CopyToPort(GRPort&, Rect*, Rect*), 344 bytes, against 467. Callers: DragIconMUp, DragIconMove, ColonyWin_DrawColonyMap, Customize DoMessage/DrawIt, DrawSpriteSquare, Fame, MSInitImageButton, MapWin RevealHiddenTerrain/SlideIcon... match the Mac's. Seq: Lock x2, PixelMap x2, NextScanLine x2, Unlock x2 on Mac; MSLockGraf x2, MSGrafOrient x2 (surface), MSUnlockGraf x2 and the blitter here. The strings say 'Warning: Source/Destination Port not locked in CopyToPort'. Layout: Mac FillColor x2, CopyToPort, CopyToScreen, ColorSync; Windows GRPort_FillColor, _all, ChangeColor_all, ChangeColor, 52d0, CopyToScreen, CopyToScreen_1068_558e... Arity: this+3. The rename adds the class prefix the corpus gives GRPort methods (GRPort_FillColor, GRPort_String).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" void report_error_1068_963d(void *ctx, char *msg);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSLockGraf(GRPort *port);
extern "C" void MSUnlockGraf(GRPort *port);

extern "C" int port_height_1068_9886(GRPort *p);
extern "C" int port_stride_1068_986d(GRPort *p);
#include "records.h"
extern int MSGrafOrient(GrafRec *graf);
extern "C" void *port_surface_1068_989f(GRPort *port);

void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr)
{
    int w;
    int h;
    RECT a;
    RECT b;
    int src_locked = 1;
    int dst_locked = 1;

    if (src->bits == 0) {
        report_error_1068_963d(&g_error_log, "Warning: Source Port not locked in CopyToPort");
        MSLockGraf(src);
        src_locked = 0;
    }
    if (dst->bits == 0) {
        report_error_1068_963d(&g_error_log, "Warning: Destination Port not locked in CopyToPort");
        MSLockGraf(dst);
        dst_locked = 0;
    }
    IntersectRect(&a, sr, &src->rect);
    IntersectRect(&b, dr, &dst->rect);
    w = min_int_1068_9847(b.right - b.left, a.right - a.left);
    h = min_int_1068_9847(b.bottom - b.top, a.bottom - a.top);
    if (w == 0 || h == 0)
        goto done;
    asm_copy_bits(port_surface_1068_989f(src), port_surface_1068_989f(dst), a.left, a.top, b.left, b.top, w, h,
                  port_height_1068_9886(src), port_height_1068_9886(dst),
                  port_stride_1068_986d(src) * (MSGrafOrient((GrafRec *)GRPort_GetWin_1068_968c(src)) ? 1 : -1),
                  port_stride_1068_986d(dst) * (MSGrafOrient((GrafRec *)GRPort_GetWin_1068_968c(dst)) ? 1 : -1));
done:
    if (!src_locked)
        MSUnlockGraf(src);
    if (!dst_locked)
        MSUnlockGraf(dst);
}
