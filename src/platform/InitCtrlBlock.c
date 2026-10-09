/* PORT: a record holding pointers is allocated by its sizeof (the game's size on i386, larger on 64-bit Windows) (W9). */
/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* InitCtrlBlock: candidate for 1070:08e5 (201 bytes).  Verdict: RELOCATABLE.
 *
 * Allocates the 0x1e-byte control block for a window: handle at +0, the owning object at
 * +2, the rest zeroed, and stores the block pointer in window long 0. A null window is
 * reported ("Error: Tried to initialize NULL control in InitCtrlBlock") and 0 returned.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_969a(void *ctx, char *msg);

CtrlBlock *InitCtrlBlock(unsigned hwnd, void *obj)
{
    CtrlBlock *c = 0;
    unsigned h;

    if (hwnd != 0) {
        h = MSAllocateHandle((long)sizeof(CtrlBlock));
        c = (CtrlBlock *)MSHLock(h);
        c->shared_bitmaps = 0;
        c->bmp0 = c->bmp1 = c->bmp2 = 0;
        c->handle = h;
        c->ctl = obj;
        c->hot = 0;
        c->pressed = 0;
        c->f12 = 0;
        c->kind = 0;
        SetWindowLong(hwnd, 0, (intptr_t)c);
    } else {
        report_error_1070_969a(&g_error_log, "Error: Tried to initialize NULL control in InitCtrlBlock");
    }
    return c;
}
