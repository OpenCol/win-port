/* bitmap_button_proc -- shipped at 1070:1e81.
 *
 * Ported by tools/port.py from matched/platform/bitmap_button_proc.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char byte_at_98(void *o);
col_i16 ctl_id_1070_96c5(char *c);
col_i16 get_w_1a_1070_981e(char *c);
col_i16 rect_height_1070_9649(RECT *r);
col_i16 rect_width_1070_962a(RECT *r);
col_i32 pascal control_message_handler(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
void * ctl_window_1070_96a8(char *c);
void * object_from_hwnd(col_u16 hwnd);
void * ptr_plus_8(char *p);
void MSDoAllTasks_1070_9837(void);
void call_handler_slot_28_1070_9863(char *t, col_i16 a);
void repaint_hwnd(col_u16 hwnd);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal BitBlt(col_u16 dst, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_u16 src, col_i16 sx, col_i16 sy,
                             col_i32 rop);
extern col_i16 pascal DeleteDC(col_u16 hdc);
extern col_i16 pascal DeleteObject(col_u16 obj);
extern col_i16 pascal FrameRect(col_u16 hdc, RECT *r, col_u16 brush);
extern col_i16 pascal GetAsyncKeyState(col_i16 vk);
extern col_i16 pascal PtInRect(RECT *r, POINT pt);
extern col_i16 pascal ReleaseDC(col_u16 hwnd, col_u16 hdc);
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
extern col_u16 pascal BeginPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_u16 pascal CreateCompatibleDC(col_u16 hdc);
extern col_u16 pascal GetDC(col_u16 hwnd);
extern col_u16 pascal GetFocus(void);
extern col_u16 pascal GetParent(col_u16 hwnd);
extern col_u16 pascal GetStockObject(col_i16 i);
extern col_u16 pascal RealizePalette(col_u16 hdc);
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 obj);
extern col_u16 pascal SetCapture(col_u16 hwnd);
extern void pascal EndPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern void pascal GetClientRect(col_u16 hwnd, RECT *r);
extern void pascal InvalidateRect(col_u16 hwnd, RECT *r, col_i16 erase);
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal ReleaseCapture(void);
extern void pascal SetFocus(col_u16 hwnd);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
extern void pascal ValidateRect(col_u16 hwnd, RECT *r);
#endif

#include "w16const.h"
typedef struct { char pad[2]; char *ctl; char pad6[8 - 6]; col_u16 up, down, grey;
                 col_i16 hot; col_i16 pressed; } WinObj;


























extern void set_current_window(void *w);
extern col_i32 pascal bitmap_button_proc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);

/* same object: 4-byte `push cs; call ` (rule 47) */













col_i32 pascal bitmap_button_proc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp)
{
    col_u16 memdc;
    col_u16 old;
    col_u16 bmp;
    col_i16 hot;
    RECT r;
    POINT pt;
    WinObj *o;
    char *ctl;
    col_u16 brush;
    RECT cr;
    col_u16 key;
    PAINTSTRUCT ps;
    register col_u16 hwnd_;
    register col_u16 hdc;

    hwnd_ = hwnd;
    o = object_from_hwnd(hwnd_);
    if (o == 0)
        return 0L;
    ctl = o->ctl;
    switch (msg) {
    case WM_LBUTTONDOWN:
        if (get_w_1a_1070_981e(ctl) == 0)
            break;
        o->pressed = 1;
        SetFocus(hwnd_);
        SetCapture(hwnd_);
    case WM_MOUSEMOVE:
        if (o->pressed != 0) {
            pt.x = (col_i16)lp;
            pt.y = (col_i16)((col_u32)lp >> 16);
            r = *(RECT *)ptr_plus_8(ctl);
            OffsetRect(&r, -r.left, -r.top);
            if (PtInRect(&r, pt) == 0)
                hot = 0;
            else
                hot = 1;
            if (o->hot != hot) {
                o->hot = hot;
                InvalidateRect(hwnd_, &r, 0);
            }
        }
        break;
    case WM_LBUTTONUP:
        if (o->pressed != 0) {
            ReleaseCapture();
            o->pressed = 0;
            pt.x = (col_i16)lp;
            pt.y = (col_i16)((col_u32)lp >> 16);
            r = *(RECT *)ptr_plus_8(ctl);
            OffsetRect(&r, -r.left, -r.top);
            InvalidateRect(hwnd_, &r, 0);
            o->hot = 0;
            if (PtInRect(&r, pt)) {
                bitmap_button_proc(hwnd_, 0xf, 0, 0L);
                set_current_window(ctl_window_1070_96a8(ctl));
                call_handler_slot_28_1070_9863(ctl, ctl_id_1070_96c5(ctl));
            }
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd_, &ps);
        r = *(RECT *)ptr_plus_8(ctl);
        ValidateRect(GetParent(hwnd_), &r);
        if (get_w_1a_1070_981e(ctl) != 0) {
            if (o->hot == 0)
                bmp = o->up;
            if (o->hot == 1)
                bmp = o->down;
        } else {
            if (GetFocus() == hwnd_)
                SetFocus(0);
            bmp = o->grey;
        }
        memdc = CreateCompatibleDC(hdc);
        RealizePalette(memdc);
        old = SelectObject(memdc, bmp);
        BitBlt(hdc, 0, 0, rect_width_1070_962a(&r), rect_height_1070_9649(&r), memdc, 0, 0,
               0xcc0020L);
        SelectObject(memdc, old);
        DeleteDC(memdc);
        EndPaint(hwnd_, &ps);
        if (GetFocus() == hwnd_)
            bitmap_button_proc(hwnd_, 7, 0, 0L);
        break;
    case WM_KEYDOWN:
        if (wp == VK_TAB) {
            SendMessage(GetParent(hwnd_), msg, wp, lp);
        } else if (wp == VK_SPACE && get_w_1a_1070_981e(ctl) != 0
                   && byte_at_98(ctl_window_1070_96a8(ctl)) != 0) {
            o->hot = 1;
            repaint_hwnd(hwnd_);
            o->hot = 0;
            key = GetAsyncKeyState(0x20);
            MSDoAllTasks_1070_9837();
            while ((char)(key >> 8) != 0)
                key = GetAsyncKeyState(0x20);
            repaint_hwnd(hwnd_);
            set_current_window(ctl_window_1070_96a8(ctl));
            call_handler_slot_28_1070_9863(ctl, ctl_id_1070_96c5(ctl));
        } else {
            SendMessage(GetParent(hwnd_), msg, wp, lp);
        }
        break;
    case WM_SETFOCUS:
        if (byte_at_98(ctl_window_1070_96a8(ctl)) == 0)
            return control_message_handler(hwnd_, msg, wp, lp);
        hdc = GetDC(hwnd_);
        brush = GetStockObject(3);
        GetClientRect(hwnd_, &cr);
        SelectObject(hdc, brush);
        FrameRect(hdc, &cr, brush);
        SetRect(&cr, cr.left + 4, cr.top + 4, cr.right - 4, cr.bottom - 4);
        FrameRect(hdc, &cr, brush);
        DeleteObject(brush);
        ReleaseDC(hwnd_, hdc);
        break;
    case WM_KILLFOCUS:
        repaint_hwnd(hwnd_);
        break;
    default:
        return control_message_handler(hwnd_, msg, wp, lp);
    }
    return 0L;
}
