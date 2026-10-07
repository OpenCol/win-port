/* CopyToScreen: candidate for 1068:54a3 (235 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CopyToScreen
 *
 * Blits the port's pixels -- its WinG surface at +0x22 -- to the window passed second (the 
 * pointer at +6 of that object), after syncing the port to that window's colour table (+2):
 * wing_blit copies a block the size of the source rectangle from its origin to the given point. A port with no window is reported ("Error: NULL Port in CopyToScreen"); an unlocked one is
 * reported ("Warning: Port not locked in CopyToScreen"), locked, and unlocked again at the end.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Screen, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

extern "C" void report_error_1068_963d(void *ctx, char *msg);
extern "C" char GRPort_IsLocked(GRPort *p);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSLockGraf(GRPort *port);
extern "C" void MSUnlockGraf(GRPort *port);
extern "C" void *get_ctable_1068_966f(void *r);

   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *table);

extern "C" void *get_win_1068_96e8(void *r);
#include "records.h"
extern void wing_blit(GrafRec *src, int sx, int sy, int w, int h, MSWin *dst, int dx, int dy);

void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst)
{
    int locked = 1;

    if (port->win == 0) {
        report_error_1068_963d(&g_error_log, "Error: NULL Port in CopyToScreen");
        return;
    }
    if (!GRPort_IsLocked(port)) {
        report_error_1068_963d(&g_error_log, "Warning: Port not locked in CopyToScreen");
        locked = 0;
        MSLockGraf(port);
    }
    GRPort_ColorSync(port, (CTable *)get_ctable_1068_966f(screen));
    wing_blit((GrafRec *)port->win, src->left, src->top, rect_width_1068_96a9(src), rect_height_1068_96c8(src), (MSWin *)get_win_1068_96e8(screen), dst->left, dst->top);
    if (!locked)
        MSUnlockGraf(port);
}
