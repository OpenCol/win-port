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

extern void *ctl_window_1070_96a8(void *ctl);
extern MSWin *get_win_1070_95f0(void *o);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern CtrlBlock *InitCtrlBlock(unsigned hwnd, void *obj);
extern long region_wndproc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam);

unsigned MSInitRegion(RECT *r, void *ctl)
{
    CtrlBlock *c;
    unsigned hwnd;

    hwnd = CreateWindow("MSControlClass", "", 0x50000000L, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
    c = InitCtrlBlock(hwnd, ctl);
    c->kind = 1;
    SetWindowLong(hwnd, -4, (long)(void *)region_wndproc);
    return hwnd;
}
