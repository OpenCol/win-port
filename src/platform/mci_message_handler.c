/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* mci_message_handler: candidate for 1070:73d5 (2669 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: mci_message_handler
 * name-source: descriptive
 * size-source: 2669, not the census 1846. The census stops at the first scroll jump table; the
 * function runs to 1070:7e42 and its last 35 bytes are a dead epilogue and the two six-entry tables.
 *
 * THE MEDIA WINDOW'S MESSAGE HANDLER, and the twin of `object_message_handler`. Message for
 * message the two are the same -- the object in window long 4, the handler slots, the two scroll jump
 * tables in the same 4, 5, 1, 0, 3, 2 order -- and the differences are entirely about owning an MCI
 * device.
 *
 * WM_ACTIVATE, WM_QUERYNEWPALETTE AND WM_PALETTECHANGED share one body, and it is not GDI at all: the
 * object's MCI record at +0x8c is fetched, and if there is one the device is sent MCI_REALIZE (0x840)
 * so the media driver can claim the palette. A window with no MCI record passes the message on. Where
 * the sibling selects and realises a palette itself, this one hands the whole problem to MMSYSTEM.
 *
 * MM_MCINOTIFY (0x3b9) IS THE ODD ONE. A successful notification (wParam 1) calls handler slot 144 and
 * answers zero. Anything else -- superseded, aborted, failure -- FALLS THROUGH INTO WM_SIZE, so a
 * failed media command is treated as a resize: the object is published, and if the window is not
 * iconic its size handler runs with the notify's wParam as the size code. That is a shipped bug, and
 * a fall-through between two cases that share nothing.
 *
 * WM_PARENTNOTIFY (0x210) forwards a child window's left or right button down to the same slots 4 and
 * 16 the window's own clicks use, so a click on a child control reaches the parent's handler. And
 * WM_COMMAND does nothing here but publish the object at DGROUP:0x2ae0 -- the sibling at least calls
 * `wm_command`.
 *
 * WHAT THE BYTES SETTLED: MCI_REALIZE's parameter block goes through as an `unsigned long` with a 
 * pointer CAST, not as a typed pointer parameter. The two spellings differ by where `PUSH SS`
 * lands (rule 56), and this is that rule's third form. The device id is at +4 of the MCI record and
 * the flags dword at +8, with two bytes between them.
 */

#include "colonization.h"

#include "w16mci.h"
#include "w16types.h"
#include "w16const.h"
#include "records.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

typedef struct { POINT ptReserved, ptMaxSize, ptMaxPosition, ptMinTrackSize,
                       ptMaxTrackSize; } MINMAXINFO;

extern int nonclient_height(MSWin *win);
extern int nonclient_width(MSWin *win);

/* 5-byte form, so extern (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" Mci *get_mci(Audio *o);
extern "C" void call_on_mci_done(Audio *table);
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

extern "C" long mci_message_handler(unsigned hwnd, unsigned msg, unsigned wp, unsigned long lp)
{
    register int pos;
    register unsigned w_;
    EventWin *obj;
    MSWin *win;
    Mci *mci;
    int cw;
    int ch;
    int cx;
    int cy;
    int ncw;
    int nch;
    MINMAXINFO *mmi;
    MCI_GENERIC_PARMS parm;

    w_ = hwnd;
    obj = (EventWin *)GetWindowLong(w_, 4);
    if (obj == 0)
        return main_window_message_handler(w_, msg, wp, lp);
    switch (msg) {
    case WM_PARENTNOTIFY:
        switch (wp) {
        case WM_LBUTTONDOWN:
            g_current_window = obj;
            call_on_lbutton_down(&(EventTable &)*obj, (unsigned)lp,
                                (unsigned)(lp >> 16));
            break;
        case WM_RBUTTONDOWN:
            g_current_window = obj;
            call_on_rbutton_down(&(EventTable &)*obj, (unsigned)lp,
                                 (unsigned)(lp >> 16));
            break;
        }
        break;
    case WM_GETMINMAXINFO:
        mmi = (MINMAXINFO *)lp;
        nch = nonclient_height(get_win_1070_95f0(obj));
        ncw = nonclient_width(get_win_1070_95f0(obj));
        get_min_track_size_1070_9c10(obj, &cw, &ch);
        get_max_track_size_1070_9c38(obj, &cx, &cy);
        mmi->ptMaxTrackSize.x = cx + ncw;
        mmi->ptMaxTrackSize.y = cy + nch;
        mmi->ptMinTrackSize.x = cw + ncw;
        mmi->ptMinTrackSize.y = ch + nch;
        break;
    case WM_LBUTTONDOWN:
        win = get_win_1070_95f0((EventWin *)GetWindowLong(w_, 0));
        g_current_window = obj;
        if (win->flags & 0x200) {
            SetFocus(win->hwnd);
            BringWindowToTop(w_);
        }
        obj->ldown = 1;
        call_on_lbutton_down(&(EventTable &)*obj, (unsigned)lp,
                            (unsigned)(lp >> 16));
        break;
    case WM_LBUTTONUP:
        g_current_window = obj;
        if (obj->ldown == 1) {
            obj->ldown = 0;
            call_on_lclick(&(EventTable &)*obj, (unsigned)lp,
                                 (unsigned)(lp >> 16));
        }
        call_on_lbutton_up(&(EventTable &)*obj, (unsigned)lp,
                            (unsigned)(lp >> 16));
        break;
    case WM_RBUTTONDOWN:
        win = get_win_1070_95f0((EventWin *)GetWindowLong(w_, 0));
        if (win->flags & 0x200) {
            SetFocus(win->hwnd);
            BringWindowToTop(w_);
        }
        g_current_window = obj;
        obj->rdown = 1;
        call_on_rbutton_down(&(EventTable &)*obj, (unsigned)lp,
                             (unsigned)(lp >> 16));
        break;
    case WM_RBUTTONUP:
        g_current_window = obj;
        if (obj->rdown == 1) {
            g_current_window = obj;
            obj->rdown = 0;
            call_on_rclick(&(EventTable &)*obj, (unsigned)lp,
                                 (unsigned)(lp >> 16));
        }
        call_on_rbutton_up(&(EventTable &)*obj, (unsigned)lp,
                             (unsigned)(lp >> 16));
        break;
    case WM_MOUSEMOVE:
        g_current_window = obj;
        call_far_slot((Handler *)&obj->on_mousemove, (unsigned)lp,
                                (unsigned)(lp >> 16));
        break;
    case WM_LBUTTONDBLCLK:
        g_current_window = obj;
        call_on_ldblclk(&(EventTable &)*obj, (unsigned)lp,
                             (unsigned)(lp >> 16));
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
    case WM_ACTIVATE:
    case WM_QUERYNEWPALETTE:
    case WM_PALETTECHANGED:
        mci = get_mci((Audio *)obj);
        if (mci == 0)
            return main_window_message_handler(w_, msg, wp, lp);
        mciSendCommand(mci->devid, 0x840, mci->parm,
                       (unsigned long)(MCI_GENERIC_PARMS *)&parm);
        break;
    case MM_MCINOTIFY:
        switch (wp) {
        case MCI_NOTIFY_SUCCESSFUL:
            call_on_mci_done((Audio *)obj);
            return 0;
        }
        /* falls through */
    case WM_SIZE:
        g_current_window = obj;
        if (!IsIconic(w_)) {
            empty_1070_9e5e(obj, (unsigned)lp, (unsigned)(lp >> 16));
            call_on_size(&(EventTable &)*obj, wp);
        }
        break;
    case WM_MOVE:
        g_current_window = obj;
        call_on_move(&(EventTable &)*obj, (unsigned)lp,
                             (unsigned)(lp >> 16));
        break;
    case WM_COMMAND:
        g_current_window = obj;
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
        call_on_commnotify(&(EventTable &)*obj, (unsigned)lp);
        return 1;
    case WM_HSCROLL:
        pos = get_hpos(obj);
        switch (wp) {
        case SB_THUMBPOSITION:
            pos = (unsigned)lp;
            SetScrollPos(w_, SB_HORZ, pos, 1);
            g_current_window = obj;
            obj->hpos = pos;
            call_on_hscroll(&(EventTable &)*obj, obj->hpos);
            break;
        case SB_THUMBTRACK:
            g_current_window = obj;
            call_on_hthumb(&(EventTable &)*obj, (unsigned)lp);
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
            pos = (unsigned)lp;
            SetScrollPos(w_, SB_VERT, pos, 1);
            g_current_window = obj;
            obj->vpos = pos;
            call_on_vscroll(&(EventTable &)*obj, obj->vpos);
            break;
        case SB_THUMBTRACK:
            g_current_window = obj;
            call_on_vthumb(&(EventTable &)*obj, (unsigned)lp);
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
