/* MSInitRegion -- shipped at 1070:0be3.
 *
 * Ported by tools/port.py from matched/platform/MSInitRegion.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *
 * The file's own Region, MSWin, CtrlBlock dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
CtrlBlock * InitCtrlBlock(col_u16 hwnd, void *obj);
MSWin * get_p_6_1070_95f0(void *o);
col_i16 rect_height_1070_9649(Region *r);
col_i16 rect_width_1070_962a(Region *r);
void * ctl_window_1070_96a8(void *rect);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
extern col_u16 pascal CreateWindow(const char *cls, const char *title, col_u32 style, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_u16 parent, col_u16 menu, col_u16 hinst, void *param);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i32 pascal hover_button_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);

col_u16 MSInitRegion(Region *r, void *rect)
{
    CtrlBlock *c;
    col_u16 hwnd;

    hwnd = CreateWindow("MSControlClass", "", 0x50000000L, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_p_6_1070_95f0(ctl_window_1070_96a8(rect))->hwnd, 0, g_hinstance, 0);
    c = InitCtrlBlock(hwnd, rect);
    c->kind = 1;
    SetWindowLong(hwnd, -4, (col_i32)(void *)hover_button_wndproc);
    return hwnd;
}
