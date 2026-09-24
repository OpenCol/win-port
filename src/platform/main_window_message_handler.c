/* main_window_message_handler -- shipped at 1070:b344.
 *
 * Ported by tools/port.py from matched/platform/main_window_message_handler.c.
 *
 * The file's own Win, Obj dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: brush -> f8, cursor -> hcursor, f25 -> flags, focus -> f6, hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Win * get_p_6_1070_dd3f(Obj *o);
col_i16 get_w_2_1070_dd5c(void *p);
col_i16 get_w_4_1070_dd75(void *p);
col_i16 rect_height(RECT *r);
col_i16 rect_width(RECT *r);
col_i16 win_client_height_1070_ddb3(Obj *o);
col_i16 win_client_width_1070_dd8e(Obj *o);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal BringWindowToTop(col_u16 hwnd);
extern col_i16 pascal EndPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_i16 pascal FillRect(col_u16 hdc, RECT *r, col_u16 brush);
extern col_i16 pascal IntersectRect(RECT *d, RECT *a, RECT *b);
extern col_i16 pascal SetMenu(col_u16 hwnd, col_u16 menu);
extern col_i32 pascal DefWindowProc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
extern col_u16 pascal BeginPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_u16 pascal GetParent(col_u16 hwnd);
extern col_u16 pascal SetCursor(col_u16 cur);
extern col_u16 pascal SetFocus(col_u16 hwnd);
extern void pascal GetClientRect(col_u16 hwnd, RECT *r);
extern void pascal GetWindowRect(col_u16 hwnd, RECT *r);
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "w16const.h"
extern void CopyToScreen(void *port, void *win, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */








col_i32 pascal main_window_message_handler(col_u16 hwnd, col_u16 msg, col_u16 wp,
                                            col_u32 lp)
{
    register col_i16 col;
    register col_i16 tw;
    Win *w1;
    Win *w2;
    Obj *o;
    col_u16 hdc;
    col_i32 res;
    col_i16 th;
    col_i16 row;
    col_i16 mx;
    col_i16 my;
    RECT box;
    RECT rc;
    RECT rc2;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_SYSCOMMAND:
        if (wp == 0xf100 || wp == 0xf090) {
            w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
            if (((*((unsigned char *)((unsigned char *)(w1) + 0x24))) & 0x20) == 0)
                return 0;
            else
                break;
        }
        break;
    case WM_KILLFOCUS:
        w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
        if (w1->f6 != 0)
            SendMessage(w1->f6, 0x86, 0, 0L);
        break;
    case WM_SETFOCUS:
        w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
        if (w1->f6 != 0)
            SendMessage(w1->f6, 0x86, 1, 0L);
        break;
    case WM_CHILDACTIVATE:
        w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
        if (w1->flags & 2) {
            w2 = get_p_6_1070_dd3f((Obj *)GetWindowLong(GetParent(hwnd), 0));
            if (w2->f6 != 0)
                SendMessage(w2->f6, 0x86, 0, 0L);
            SendMessage(hwnd, 0x86, 1, 0L);
            w2->f6 = hwnd;
        }
        break;
    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
        if (w1->flags & 2) {
            SetFocus(w1->hdc);
            BringWindowToTop(hwnd);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        o = (Obj *)GetWindowLong(hwnd, 0);
        if ((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0xe))) != 0) {
            tw = get_w_2_1070_dd5c((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0xe))));
            th = get_w_4_1070_dd75((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0xe))));
            if (tw != 0 && th != 0) {
                box = ps.rcPaint;
                SetRect(&rc, 0, 0, tw, th);
                for (row = box.top / th; box.bottom / th + 1 > row; row++) {
                    for (col = box.left / tw; box.right / tw + 1 > col; col++) {
                        SetRect(&rc2, col * tw, row * th, col * tw + tw,
                                row * th + th);
                        CopyToScreen((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0xe))), o, &rc, &rc2);
                    }
                }
            }
        } else if (get_p_6_1070_dd3f(o)->f8 != 0)
            FillRect(hdc, &ps.rcPaint, get_p_6_1070_dd3f(o)->f8);
        if ((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0x16))) != 0) {
            tw = get_w_2_1070_dd5c((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0x16))));
            th = get_w_4_1070_dd75((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0x16))));
            box = ps.rcPaint;
            SetRect(&rc, 0, 0, tw, th);
            OffsetRect(&rc, (win_client_width_1070_dd8e(o) - tw) / 2,
                       (win_client_height_1070_ddb3(o) - th) / 2);
            if (IntersectRect(&rc2, &rc, &box)) {
                rc = rc2;
                OffsetRect(&rc, -(win_client_width_1070_dd8e(o) - tw) / 2,
                           -(win_client_height_1070_ddb3(o) - th) / 2);
                CopyToScreen((*((void * *)((unsigned char *)(get_p_6_1070_dd3f(o)) + 0x16))), o, &rc, &rc2);
            }
        }
        EndPaint(hwnd, &ps);
        return 0;
    case WM_DESTROY:
        SetMenu(hwnd, 0);
        w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
        (*((col_i16 *)((unsigned char *)(w1) + 0x22))) = 1;
        if (w1->flags & 2) {
            w2 = get_p_6_1070_dd3f((Obj *)GetWindowLong(GetParent(hwnd), 0));
            if (w2->f6 == hwnd)
                w2->f6 = 0;
        }
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    res = DefWindowProc(hwnd, msg, wp, lp);
    switch (msg) {
    case WM_NCHITTEST:
        w1 = get_p_6_1070_dd3f((Obj *)GetWindowLong(hwnd, 0));
        if ((*((col_i16 *)((unsigned char *)(w1) + 0x1c))) != 0 && res == 1) {
            GetWindowRect(hwnd, &rc);
            mx = (col_u16)lp - rc.left;
            my = (col_u16)(lp >> 16) - rc.top;
            tw = (*((col_i16 *)((unsigned char *)(w1) + 0x20))) == 0 ? 5 : (*((col_i16 *)((unsigned char *)(w1) + 0x20)));
            GetClientRect(hwnd, &rc2);
            col = row = 0;
            if (my < tw)
                row = -1;
            else if (rect_height(&rc2) - tw < my)
                row = 1;
            if (mx < tw)
                col = -1;
            else if (rect_width(&rc2) - tw < mx)
                col = 1;
            if (row < 0) {
                if (tw * 4 > mx)
                    col = -1;
                else if (rect_width(&rc2) - tw * 4 < mx)
                    col = 1;
                if (col < 0)
                    res = 13;
                else if (col > 0)
                    res = 14;
                else
                    res = 12;
            } else if (row > 0) {
                if (tw * 4 > mx)
                    col = -1;
                else if (rect_width(&rc2) - tw * 4 < mx)
                    col = 1;
                if (col < 0)
                    res = 16;
                else if (col > 0)
                    res = 17;
                else
                    res = 15;
            } else if (col < 0) {
                if (tw * 4 > my)
                    row = -1;
                else if (rect_height(&rc2) - tw * 4 < my)
                    row = 1;
                if (row < 0)
                    res = 13;
                else if (row > 0)
                    res = 16;
                else
                    res = 10;
            } else if (col > 0) {
                if (tw * 4 > my)
                    row = -1;
                else if (rect_height(&rc2) - tw * 4 < my)
                    row = 1;
                if (row < 0)
                    res = 14;
                else if (row > 0)
                    res = 17;
                else
                    res = 11;
            }
        }
        if ((*((col_i16 *)((unsigned char *)(w1) + 0x1a))) != 0 && res == 1) {
            GetWindowRect(hwnd, &rc);
            my = (col_u16)(lp >> 16) - rc.top;
            if ((*((col_i16 *)((unsigned char *)(w1) + 0x1e))) == 0 || (*((col_i16 *)((unsigned char *)(w1) + 0x1e))) > my)
                res = 2;
        }
        if (res == 1)
            SetCursor(w1->hcursor);
        break;
    }
    return res;
}
