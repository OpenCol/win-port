/* PORT: the surface's stride read as GrafRec.stride, not through the DlgControl accessor at its offset (W9). */
/* PORT: this file's own declarations of GRPort_InitStruct, MSInitGraf, control_hwnd, dispose_wing_surface removed: the header declares them from the definition (W1 step 3). */
/* GRPort_Init: candidate for 1068:352f (272 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_Init
 *
 * (Re)initialises a port for rectangle r: an existing surface (+0x22) is unlocked and
 * released through dispose_wing_surface; a null or empty rectangle just resets the struct; else the
 * struct is filled, a WinG surface created by MSInitGraf on the header at +0x10, its handle
 * stored at +6 and the port locked. Returns 0 when the surface could not be made (error 2).
 */

#include "colonization.h"

#include "grerror.h"
#include "w16types.h"
/* PORT: this file's own DlgControl, GRPort removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" char GRPort_IsLocked(GRPort *p);
extern void MSUnlockGraf(GRPort *port);
extern void MSLockGraf(GRPort *port);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void error_code_set(int code);

int GRPort_Init(GRPort *p, void *r)
{
    int ok = 1;

    if (p->win != 0) {
        if (GRPort_IsLocked(p))
            MSUnlockGraf(p);
        p->win = dispose_wing_surface((WinGRec *)p->win);
    }
    if (r == 0 || rect_width_1068_96a9((RECT *)r) == 0 || rect_height_1068_96c8((RECT *)r) == 0) {
        GRPort_InitStruct(p, 0);
    } else {
        GRPort_InitStruct(p, (RECT *)r);
        p->win = MSInitGraf(&p->bounds);
        if (p->win != 0) {
            p->stride = ((GrafRec *)p->win)->stride;   /* PORT: the game read it through control_hwnd -- a DlgControl's hwnd is at GrafRec.stride's offset 0x10 on 16 and 32 bits, not on 64 (W9) */
            MSLockGraf(p);
        } else {
            error_code_set(GRERR_NO_SURFACE);
            ok = 0;
        }
    }
    return ok;
}
