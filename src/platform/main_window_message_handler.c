/* PORT: this file's own declarations of port_height_1070_dd75, port_width_1070_dd5c, win_client_height_1070_ddb3, win_client_width_1070_dd8e removed: the header declares them from the definition (W1 step 3). */
/* main_window_message_handler: candidate for 1070:b344 (2066 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: main_window_message_handler
 * name-source: original
 *
 * THE BASE WINDOW PROCEDURE, and the last thing both `object_message_handler` and
 * `mci_message_handler` fall back to. Ten messages have a case (the four mouse-downs share one arm), everything else goes
 * to `DefWindowProc` -- and then, whatever the message was, WM_NCHITTEST's answer is rewritten.
 *
 * THE FOCUS CHAIN is the first half. WM_SETFOCUS and WM_KILLFOCUS forward message 0x86 to the window
 * remembered at +6 of the window record, with 1 and 0; WM_CHILDACTIVATE tells the PARENT'S remembered
 * child it has lost the focus, tells this window it has gained it, and writes itself into the parent's
 * +6. WM_DESTROY drops the menu, marks the record dead at +0x22, and clears the parent's +6 if it
 * pointed here. Every mouse-down message -- client or non-client, left or right -- shares one arm that
 * takes the focus and raises the window, but only for a record whose +0x25 bit 1 is set.
 *
 * WM_PAINT TILES A BACKGROUND. If the record has a pattern port at +0xe, the damaged rectangle is
 * divided by the pattern's own width and height and the pattern is blitted into every cell that
 * touches it -- a double loop over `rc.top / th` to `rc.bottom / th` and the same across. If there is
 * no pattern but there is a brush at +8, the damage is simply filled. Either way, an icon port at
 * +0x16 is then centred over the client area, intersected with the damage, and blitted -- the second
 * `OffsetRect` moves the rectangle BACK by the same amount so the source coordinates come out right.
 *
 * WM_NCHITTEST IS REWRITTEN AFTER THE FACT. `DefWindowProc` runs first, and only an answer of HTCLIENT
 * is reconsidered: the cursor's offset inside the window rectangle is measured against a border width
 * from +0x20 (five if it is zero), and an offset inside that border turns HTCLIENT into one of the
 * eight sizing codes. The corners use FOUR TIMES the border width along the other axis, which is what
 * makes the corner grips wider than the edges. A record with +0x1a set then turns a surviving HTCLIENT
 * into HTCAPTION -- unless +0x1e is set and the cursor is below it -- which is how a window with no
 * title bar is still dragged by its top strip. Anything still HTCLIENT sets the record's cursor.
 *
 * WHAT THE BYTES SETTLED: the WM_SYSCOMMAND guard is `(w1->flags & 0x20) == 0`, not `!(w1->flags & 0x20)`.
 * That is rule 59, and it is the only difference between a one-instruction `TEST BYTE [ES:BX+0x24],0x20`
 * and a load, a widen and a register test. The WM_NCHITTEST block is a ONE-CASE `switch`, not an `if`:
 * the message is loaded into AX before the compare and the case's `break` costs rule 54's `EB 00`.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" MSWin *get_win_1070_dd3f(Screen *o);
extern "C" int rect_width(RECT *r);
extern "C" int rect_height(RECT *r);

extern "C" long main_window_message_handler(unsigned hwnd, unsigned msg, unsigned wp,
                                            unsigned long lp)
{
    register int col;
    register int tw;
    MSWin *w1;
    MSWin *w2;
    Screen *o;
    unsigned hdc;
    long res;
    int th;
    int row;
    int mx;
    int my;
    RECT box;
    RECT rc;
    RECT rc2;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_SYSCOMMAND:
        if (wp == 0xf100 || wp == 0xf090) {
            w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
            if ((w1->flags & 0x20) == 0)
                return 0;
            else
                break;
        }
        break;
    case WM_KILLFOCUS:
        w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
        if (w1->focus != 0)
            SendMessage(w1->focus, 0x86, 0, 0L);
        break;
    case WM_SETFOCUS:
        w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
        if (w1->focus != 0)
            SendMessage(w1->focus, 0x86, 1, 0L);
        break;
    case WM_CHILDACTIVATE:
        w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
        if (w1->flags & 0x200) {
            w2 = get_win_1070_dd3f((Screen *)GetWindowLong(GetParent(hwnd), 0));
            if (w2->focus != 0)
                SendMessage(w2->focus, 0x86, 0, 0L);
            SendMessage(hwnd, 0x86, 1, 0L);
            w2->focus = hwnd;
        }
        break;
    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
        if (w1->flags & 0x200) {
            SetFocus(w1->hwnd);
            BringWindowToTop(hwnd);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        o = (Screen *)GetWindowLong(hwnd, 0);
        if (get_win_1070_dd3f(o)->pattern != 0) {
            tw = port_width_1070_dd5c((GRPort *)(get_win_1070_dd3f(o)->pattern));
            th = port_height_1070_dd75((GRPort *)(get_win_1070_dd3f(o)->pattern));
            if (tw != 0 && th != 0) {
                box = ps.rcPaint;
                SetRect(&rc, 0, 0, tw, th);
                for (row = box.top / th; box.bottom / th + 1 > row; row++) {
                    for (col = box.left / tw; box.right / tw + 1 > col; col++) {
                        SetRect(&rc2, col * tw, row * th, col * tw + tw,
                                row * th + th);
                        CopyToScreen((GRPort *)get_win_1070_dd3f(o)->pattern, (Screen *)o, &rc, &rc2);
                    }
                }
            }
        } else if (get_win_1070_dd3f(o)->hbrush != 0)
            FillRect(hdc, &ps.rcPaint, get_win_1070_dd3f(o)->hbrush);
        if (get_win_1070_dd3f(o)->icon != 0) {
            tw = port_width_1070_dd5c((GRPort *)(get_win_1070_dd3f(o)->icon));
            th = port_height_1070_dd75((GRPort *)(get_win_1070_dd3f(o)->icon));
            box = ps.rcPaint;
            SetRect(&rc, 0, 0, tw, th);
            OffsetRect(&rc, (win_client_width_1070_dd8e((DialogWin *)o) - tw) / 2,
                       (win_client_height_1070_ddb3((DialogWin *)o) - th) / 2);
            if (IntersectRect(&rc2, &rc, &box)) {
                rc = rc2;
                OffsetRect(&rc, -(win_client_width_1070_dd8e((DialogWin *)o) - tw) / 2,
                           -(win_client_height_1070_ddb3((DialogWin *)o) - th) / 2);
                CopyToScreen((GRPort *)get_win_1070_dd3f(o)->icon, (Screen *)o, &rc, &rc2);
            }
        }
        EndPaint(hwnd, &ps);
        return 0;
    case WM_DESTROY:
        SetMenu(hwnd, 0);
        w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
        w1->destroyed = 1;
        if (w1->flags & 0x200) {
            w2 = get_win_1070_dd3f((Screen *)GetWindowLong(GetParent(hwnd), 0));
            if (w2->focus == hwnd)
                w2->focus = 0;
        }
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    res = DefWindowProc(hwnd, msg, wp, lp);
    switch (msg) {
    case WM_NCHITTEST:
        w1 = get_win_1070_dd3f((Screen *)GetWindowLong(hwnd, 0));
        if (w1->size_by_edges != 0 && res == HTCLIENT) {
            GetWindowRect(hwnd, &rc);
            mx = (unsigned)lp - rc.left;
            my = (unsigned)(lp >> 16) - rc.top;
            tw = w1->edge_width == 0 ? 5 : w1->edge_width;
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
                    res = HTTOPLEFT;
                else if (col > 0)
                    res = HTTOPRIGHT;
                else
                    res = HTTOP;
            } else if (row > 0) {
                if (tw * 4 > mx)
                    col = -1;
                else if (rect_width(&rc2) - tw * 4 < mx)
                    col = 1;
                if (col < 0)
                    res = HTBOTTOMLEFT;
                else if (col > 0)
                    res = HTBOTTOMRIGHT;
                else
                    res = HTBOTTOM;
            } else if (col < 0) {
                if (tw * 4 > my)
                    row = -1;
                else if (rect_height(&rc2) - tw * 4 < my)
                    row = 1;
                if (row < 0)
                    res = HTTOPLEFT;
                else if (row > 0)
                    res = HTBOTTOMLEFT;
                else
                    res = HTLEFT;
            } else if (col > 0) {
                if (tw * 4 > my)
                    row = -1;
                else if (rect_height(&rc2) - tw * 4 < my)
                    row = 1;
                if (row < 0)
                    res = HTTOPRIGHT;
                else if (row > 0)
                    res = HTBOTTOMRIGHT;
                else
                    res = HTRIGHT;
            }
        }
        if (w1->drag_by_client != 0 && res == HTCLIENT) {
            GetWindowRect(hwnd, &rc);
            my = (unsigned)(lp >> 16) - rc.top;
            if (w1->drag_height == 0 || w1->drag_height > my)
                res = HTCAPTION;
        }
        if (res == HTCLIENT)
            SetCursor(w1->hcursor);
        break;
    }
    return res;
}
