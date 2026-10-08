/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* PORT: lParam's words as Borland's 16-bit casts took them -- i16 for coordinates, sizes and positions, u16 for handles, ids and codes; (unsigned)/(int) keep 32 bits here (W4). */
/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* object_message_handler: candidate for 1070:69ba (2587 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: object_message_handler
 * name-source: descriptive
 * size-source: 2587, not the census 1764. The census stops at the first scroll jump table; the
 * function runs to 1070:73d5 and its last 35 bytes are a dead epilogue and the two six-entry tables.
 *
 * EVERY WINDOWS MESSAGE, TURNED INTO A HANDLER SLOT. `dialog_wndproc` hands this the raw
 * message; it finds the window's object in window long 4, and if there is none the message goes
 * straight to `main_window_message_handler`. Twenty-one messages are handled and the rest are passed
 * on, so this is the whole bridge between the Windows message loop and the game's window objects.
 *
 * EVERY ARM PUBLISHES THE OBJECT at DGROUP:0x2ae0 before it calls anything. That is how a handler
 * knows which window it is being called for without being passed it, and it is done separately in each
 * arm rather than once at the top -- WM_GETMINMAXINFO and the shared WM_PALETTECHANGED/WM_QUERYNEWPALETTE
 * body skip it, WM_RBUTTONDOWN publishes only after SetFocus/BringWindowToTop, and every scroll case
 * but thumb track publishes only after SetScrollPos.
 *
 * THE OBJECT IS AT WINDOW LONG 4 AND THE WINDOW RECORD AT WINDOW LONG 0, and the two button-down arms
 * use both: the record's +0x25 bit 1 decides whether the click takes the focus and raises the window,
 * while the object carries the pressed flags at +0x84 and +0x86. Each button-up fires two slots -- the
 * "released" one only if the matching down was seen, then the plain one -- so a drag that began outside
 * the window still reports the release.
 *
 * WM_KEYDOWN FIRES TWO SLOTS, 32 and 40, and calls `translate_key` TWICE with the same key
 * rather than keeping the answer. WM_CHAR masks the character to a byte at byte width. WM_COMMAND is
 * only forwarded when the low half of lParam is zero, which is Windows' own test for a menu command
 * rather than a control notification.
 *
 * WM_PALETTECHANGED FALLS THROUGH into WM_QUERYNEWPALETTE unless the change came from this window, in
 * which case the message is passed on untouched -- the standard way of not answering your own
 * broadcast. The shared body selects and realises the port's palette and invalidates the window.
 *
 * A SHIPPED BUG lives in that body: the value it returns is `realized`, set only inside the
 * `if (palette != 0)`. A window whose port has no palette answers with whatever was in that frame slot.
 *
 * THE TWO SCROLL ARMS are identical apart from the bar, the position field and the slot numbers. Each
 * is a six-entry jump table, and the CASES ARE NOT IN VALUE ORDER in the source: thumb position and
 * thumb track come first, then line down, line up, page down and page up -- 4, 5, 1, 0, 3, 2, which the
 * table's entries settle exactly. Thumb position calls the slot with the field it has just written
 * rather than with the value, and the four stepping cases ask Windows for the position back after
 * setting it, so the clamping is Windows' and not the game's.
 *
 * WHAT THE BYTES SETTLED: WM_CLOSE's test is an `if`/`else`, not an `if` with a trailing `return`. The
 * else costs a dead `JMP SHORT` over itself, two bytes that jump to the next case and can never run.
 * And the palette arm's answer is `realized == 0 ? 0 : 1`, not `realized != 0`: the two spellings put
 * the constants in the opposite order.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

typedef struct { POINT ptReserved, ptMaxSize, ptMaxPosition, ptMinTrackSize,
                       ptMaxTrackSize; } MINMAXINFO;
#include "records.h"

extern int nonclient_height(MSWin *win);
extern int nonclient_width(MSWin *win);

/* 5-byte form, so extern (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void get_min_track_size_1070_9c10(EventWin *obj, int *pa, int *pb);
extern "C" void get_max_track_size_1070_9c38(EventWin *obj, int *x, int *y);
extern "C" int get_hpos(EventWin *obj);
extern "C" int get_hpage(EventWin *obj);
extern "C" int get_vpos(EventWin *obj);
extern "C" int get_vpage(EventWin *obj);

typedef void (*Handler)(int a, int b);

extern "C" void call_on_lbutton_down(EventTable *table, int x, int y);
extern "C" void call_on_lbutton_up(EventTable *table, int x, int y);
extern "C" void call_on_lclick(EventTable *table, int x, int y);
extern "C" void call_on_rbutton_down(EventTable *table, int x, int y);
extern "C" void call_on_rbutton_up(EventTable *table, int x, int y);
extern "C" void call_on_rclick(EventTable *table, int x, int y);
extern "C" void call_on_ldblclk(EventTable *table, int x, int y);
extern "C" void call_on_keydown(EventTable *table, int key);
extern "C" void call_on_keyup(EventTable *table, int key);
extern "C" void call_on_keydown_after(EventTable *table, int key);
extern "C" void call_on_char(EventTable *table, int ch);
extern "C" int call_on_close(EventTable *table);
extern "C" void call_on_minimize(EventTable *table);
extern "C" void call_on_restore(EventTable *table);
extern "C" void call_on_setfocus(EventTable *handlers);
extern "C" void call_on_size(EventTable *table, int size_type);
extern "C" void call_on_move(EventTable *t, int x, int y);
extern "C" void call_on_commnotify(EventTable *table, int notify_status);
extern "C" void call_on_hscroll(EventTable *table, int pos);
extern "C" void call_on_hthumb(EventTable *handlers, int pos);
extern "C" void call_on_vscroll(EventTable *table, int pos);
extern "C" void call_on_vthumb(EventTable *table, int pos);

extern "C" long object_message_handler(unsigned hwnd, unsigned msg, unsigned wp, uintptr_t lp)
{
    register int pos;
    register unsigned w_;
    EventWin *obj;
    MSWin *win;
    unsigned hdc;
    int realized;
    int min_w;
    int min_h;
    int max_w;
    int max_h;
    int ncw;
    int nch;
    MINMAXINFO *mmi;

    w_ = hwnd;
    obj = (EventWin *)GetWindowLong(w_, 4);
    if (obj == 0)
        return main_window_message_handler(w_, msg, wp, lp);
    switch (msg) {
    case WM_GETMINMAXINFO:
        mmi = (MINMAXINFO *)lp;
        nch = nonclient_height(get_win_1070_95f0(obj));
        ncw = nonclient_width(get_win_1070_95f0(obj));
        get_min_track_size_1070_9c10(obj, &min_w, &min_h);
        get_max_track_size_1070_9c38(obj, &max_w, &max_h);
        mmi->ptMaxTrackSize.x = max_w + ncw;
        mmi->ptMaxTrackSize.y = max_h + nch;
        mmi->ptMinTrackSize.x = min_w + ncw;
        mmi->ptMinTrackSize.y = min_h + nch;
        break;
    case WM_LBUTTONDOWN:
        win = get_win_1070_95f0((EventWin *)GetWindowLong(w_, 0));
        g_current_window = obj;
        if (win->flags & 0x200) {
            SetFocus(win->hwnd);
            BringWindowToTop(w_);
        }
        obj->ldown = 1;
        call_on_lbutton_down(&(EventTable &)*obj, (i16)lp,
                            (i16)(lp >> 16));
        break;
    case WM_LBUTTONUP:
        g_current_window = obj;
        if (obj->ldown == 1) {
            obj->ldown = 0;
            call_on_lclick(&(EventTable &)*obj, (i16)lp,
                                 (i16)(lp >> 16));
        }
        call_on_lbutton_up(&(EventTable &)*obj, (i16)lp,
                            (i16)(lp >> 16));
        break;
    case WM_RBUTTONDOWN:
        win = get_win_1070_95f0((EventWin *)GetWindowLong(w_, 0));
        if (win->flags & 0x200) {
            SetFocus(win->hwnd);
            BringWindowToTop(w_);
        }
        g_current_window = obj;
        obj->rdown = 1;
        call_on_rbutton_down(&(EventTable &)*obj, (i16)lp,
                             (i16)(lp >> 16));
        break;
    case WM_RBUTTONUP:
        g_current_window = obj;
        if (obj->rdown == 1) {
            g_current_window = obj;
            obj->rdown = 0;
            call_on_rclick(&(EventTable &)*obj, (i16)lp,
                                 (i16)(lp >> 16));
        }
        call_on_rbutton_up(&(EventTable &)*obj, (i16)lp,
                             (i16)(lp >> 16));
        break;
    case WM_MOUSEMOVE:
        g_current_window = obj;
        call_far_slot((Handler *)&obj->on_mousemove, (i16)lp,
                                (i16)(lp >> 16));
        break;
    case WM_LBUTTONDBLCLK:
        g_current_window = obj;
        call_on_ldblclk(&(EventTable &)*obj, (i16)lp,
                             (i16)(lp >> 16));
        break;
    case WM_KEYDOWN:
        g_current_window = obj;
        call_on_keydown(&(EventTable &)*obj, translate_key(wp));
        call_on_keydown_after(&(EventTable &)*obj, translate_key(wp));
        break;
    case WM_CHAR:
        g_current_window = obj;
        call_on_char(&(EventTable &)*obj, (unsigned char)(wp & 0xff));
        break;
    case WM_KEYUP:
        g_current_window = obj;
        call_on_keyup(&(EventTable &)*obj, translate_key(wp));
        break;
    case WM_SETFOCUS:
        g_current_window = obj;
        call_on_setfocus(&(EventTable &)*obj);
        break;
    case WM_PALETTECHANGED:
        if (wp == w_)
            return main_window_message_handler(w_, msg, wp, lp);
        /* falls through */
    case WM_QUERYNEWPALETTE:
        hdc = GetDC(w_);
        if (get_win_1070_95f0(obj)->hpal != 0) {
            SelectPalette(hdc, get_win_1070_95f0(obj)->hpal, 0);
            realized = RealizePalette(hdc);
            InvalidateRect(w_, 0, 1);
        }
        ReleaseDC(w_, hdc);
        return realized == 0 ? 0 : 1;
    case WM_SIZE:
        g_current_window = obj;
        if (!IsIconic(w_)) {
            empty_1070_9e5e(obj, (i16)lp, (i16)(lp >> 16));
            call_on_size(&(EventTable &)*obj, wp);
        }
        break;
    case WM_MOVE:
        g_current_window = obj;
        call_on_move(&(EventTable &)*obj, (i16)lp,
                             (i16)(lp >> 16));
        break;
    case WM_COMMAND:
        g_current_window = obj;
        if ((u16)lp == 0)
            wm_command(w_, wp);
        break;
    case WM_SYSCOMMAND:
        g_current_window = obj;
        switch (wp) {
        case SC_MINIMIZE:
            call_on_minimize(&(EventTable &)*obj);
            break;
        case SC_RESTORE:
            call_on_restore(&(EventTable &)*obj);
            break;
        default:
            break;
        }
        return main_window_message_handler(w_, msg, wp, lp);
    case WM_CLOSE:
        g_current_window = obj;
        if (call_on_close(&(EventTable &)*obj) == 0)
            break;
        else
            return main_window_message_handler(w_, msg, wp, lp);
    case WM_COMMNOTIFY:
        g_current_window = obj;
        call_on_commnotify(&(EventTable &)*obj, (u16)lp);
        return 1;
    case WM_HSCROLL:
        pos = get_hpos(obj);
        switch (wp) {
        case SB_THUMBPOSITION:
            pos = (i16)lp;
            SetScrollPos(w_, SB_HORZ, pos, 1);
            g_current_window = obj;
            obj->hpos = pos;
            call_on_hscroll(&(EventTable &)*obj, obj->hpos);
            break;
        case SB_THUMBTRACK:
            g_current_window = obj;
            call_on_hthumb(&(EventTable &)*obj, (i16)lp);
            break;
        case SB_LINEDOWN:
            pos++;
            SetScrollPos(w_, SB_HORZ, pos, 1);
            pos = GetScrollPos(w_, SB_HORZ);
            g_current_window = obj;
            obj->hpos = pos;
            call_on_hscroll(&(EventTable &)*obj, pos);
            break;
        case SB_LINEUP:
            pos--;
            SetScrollPos(w_, SB_HORZ, pos, 1);
            pos = GetScrollPos(w_, SB_HORZ);
            g_current_window = obj;
            obj->hpos = pos;
            call_on_hscroll(&(EventTable &)*obj, pos);
            break;
        case SB_PAGEDOWN:
            pos += get_hpage(obj);
            SetScrollPos(w_, SB_HORZ, pos, 1);
            pos = GetScrollPos(w_, SB_HORZ);
            g_current_window = obj;
            obj->hpos = pos;
            call_on_hscroll(&(EventTable &)*obj, pos);
            break;
        case SB_PAGEUP:
            pos -= get_hpage(obj);
            SetScrollPos(w_, SB_HORZ, pos, 1);
            pos = GetScrollPos(w_, SB_HORZ);
            g_current_window = obj;
            obj->hpos = pos;
            call_on_hscroll(&(EventTable &)*obj, pos);
            break;
        }
        break;
    case WM_VSCROLL:
        pos = get_vpos(obj);
        switch (wp) {
        case SB_THUMBPOSITION:
            pos = (i16)lp;
            SetScrollPos(w_, SB_VERT, pos, 1);
            g_current_window = obj;
            obj->vpos = pos;
            call_on_vscroll(&(EventTable &)*obj, obj->vpos);
            break;
        case SB_THUMBTRACK:
            g_current_window = obj;
            call_on_vthumb(&(EventTable &)*obj, (i16)lp);
            break;
        case SB_LINEDOWN:
            pos++;
            SetScrollPos(w_, SB_VERT, pos, 1);
            pos = GetScrollPos(w_, SB_VERT);
            g_current_window = obj;
            obj->vpos = pos;
            call_on_vscroll(&(EventTable &)*obj, pos);
            break;
        case SB_LINEUP:
            pos--;
            SetScrollPos(w_, SB_VERT, pos, 1);
            pos = GetScrollPos(w_, SB_VERT);
            g_current_window = obj;
            obj->vpos = pos;
            call_on_vscroll(&(EventTable &)*obj, pos);
            break;
        case SB_PAGEDOWN:
            pos += get_vpage(obj);
            SetScrollPos(w_, SB_VERT, pos, 1);
            pos = GetScrollPos(w_, SB_VERT);
            g_current_window = obj;
            obj->vpos = pos;
            call_on_vscroll(&(EventTable &)*obj, pos);
            break;
        case SB_PAGEUP:
            pos -= get_vpage(obj);
            SetScrollPos(w_, SB_VERT, pos, 1);
            pos = GetScrollPos(w_, SB_VERT);
            g_current_window = obj;
            obj->vpos = pos;
            call_on_vscroll(&(EventTable &)*obj, pos);
            break;
        }
        break;
    default:
        return main_window_message_handler(w_, msg, wp, lp);
    }
    return 0;
}
