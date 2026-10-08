/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* PORT: this file's own declarations of ctl_window_1070_96a8 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* MSInitRegion: candidate for 1070:0be3 (182 bytes).  Verdict: RELOCATABLE.
 *
 * Creates an MSControlClass child window for a region control: position from the object's
 * first two words, size from rect_width_1070_962a/9649, parent from the owner chain
 * (get_win_1070_95f0(ctl_window_1070_96a8(ctl))), a control block (kind 1 at +0x1c), and the region
 * window procedure installed. Returns the window handle.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern CtrlBlock *InitCtrlBlock(unsigned hwnd, void *obj);
extern long region_wndproc(unsigned hwnd, unsigned msg, unsigned wparam, intptr_t lparam);

unsigned MSInitRegion(RECT *r, void *ctl)
{
    CtrlBlock *c;
    unsigned hwnd;

    hwnd = CreateWindow("MSControlClass", "", 0x50000000L, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
    c = InitCtrlBlock(hwnd, ctl);
    c->kind = 1;
    SetWindowLong(hwnd, -4, (intptr_t)(void *)region_wndproc);
    return hwnd;
}
