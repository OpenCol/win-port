/* GRPort_FillColor: candidate for 1068:505c (248 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_FillColor
 * name-source: sibling-build
 *
 * Fills the part of `r` inside the port's rectangle (+8) with `color` through the assembly
 * filler asm_fill_rect (buffer at +0x18, colour, left, top, width, height, signed row pitch,
 * plane count). An unlocked port is reported ("Warning: Port not locked in FillColor") and locked first.
 *
 * RENAMED 2026-09-26 from FillColor: the Mac build's GRPort::FillColor(Rect*, short): this plus a rectangle and a colour, the part inside the port filled; its own warning string calls it FillColor, and GRPort_FillColor_all is the (short) overload over the whole port. The class prefix now says whose method it is. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" char GRPort_IsLocked(GRPort *p);
extern "C" void MSLockGraf(GRPort *port);
extern "C" void report_error_1068_963d(void *ctx, char *msg);

extern "C" int port_height_1068_9886(GRPort *p);
extern "C" int port_stride_1068_986d(GRPort *p);
extern "C" void *GRPort_GetWin_1068_968c(GRPort *p);
#include "records.h"
extern int MSGrafOrient(GrafRec *graf);

void GRPort_FillColor(GRPort *p, RECT *r, int color)
{
    int w;
    int h;
    RECT clip;
    int locked = 1;

    if (!GRPort_IsLocked(p)) {
        report_error_1068_963d(&g_error_log, "Warning: Port not locked in FillColor");
        locked = 0;
        MSLockGraf(p);
    }
    IntersectRect(&clip, r, &p->rect);
    if ((w = rect_width_1068_96a9(&clip)) == 0 || (h = rect_height_1068_96c8(&clip)) == 0)
        return;
    asm_fill_rect((unsigned long)p->bits, color, clip.left, clip.top, w, h,
                  port_stride_1068_986d(p) * (MSGrafOrient((GrafRec *)GRPort_GetWin_1068_968c(p)) ? 1 : -1), port_height_1068_9886(p));
}
