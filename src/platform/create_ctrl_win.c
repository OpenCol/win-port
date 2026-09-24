/* create_ctrl_win -- shipped at 1070:2688.
 *
 * Ported by tools/port.py from matched/platform/create_ctrl_win.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *
 * The file's own Win, CtrlBlock dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
CtrlBlock * InitCtrlBlock(col_u16 hwnd, void *obj);
Win * get_p_6_1070_95f0(void *o);
col_i16 rect_height_1070_9649(RECT *r);
col_i16 rect_width_1070_962a(RECT *r);
void * get_p_4_1070_96a8(void *o);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
extern col_u16 pascal CreateWindow(const char *cls, const char *title,
                                        col_u32 style, col_i16 x, col_i16 y, col_i16 w, col_i16 h,
                                        col_u16 parent, col_u16 menu, col_u16 hinst,
                                        void *param);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern col_i32 pascal image_button_proc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);

/* same object: 4-byte `push cs; call ` */






col_u16 create_ctrl_win(col_i16 x, col_i16 y, col_i16 w, col_i16 h, void *obj, col_i16 visible)
{
    col_u16 hwnd;
    void *o;
    CtrlBlock *c;
    RECT r;
    col_u32 style;

    style = 0x40810000L;
    if (visible != 0)
        style |= 0x10000000L;
    o = obj;
    SetRect(&r, x, y, x + w, y + h);
    hwnd = CreateWindow("MSControlClass", "", style, x, y,
                        rect_width_1070_962a(&r), rect_height_1070_9649(&r),
                        get_p_6_1070_95f0(get_p_4_1070_96a8(obj))->hdc,
                        0, g_hinstance, 0);
    c = InitCtrlBlock(hwnd, obj);
    c->kind = 5;
    SetWindowLong(hwnd, -4, (col_i32)(void *)image_button_proc);
    return hwnd;
}
