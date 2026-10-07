/* PORT: this file's own declarations of SetEventWindow, ctl_window_1070_96a8 removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of call_bar_changed, call_bar_tracking, get_bar_page, get_bar_pos removed: the header declares them from the definition (W1 step 3). */
/* scrollbar_message: candidate for 1070:5d32 (580 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: scrollbar_message
 * name-source: descriptive
 * size-source: 580, not the census 568. The trailing 12 bytes are the six-entry jump table of
 * the scroll-code switch; `enable_window` starts immediately after it.
 *
 * The message handler of the MSScrollBarClass control, called with a window procedure's four
 * arguments. The bar's record is the pointer at `GetClassWord(GCW_CBWNDEXTRA) - 6` -- the
 * six bytes `create_MSScrollBarClass_window` added to the stock SCROLLBAR class -- and its position
 * is kept at +0x26.
 *
 * WM_HSCROLL and WM_VSCROLL are one arm. Line up/down move the position by one, page up/down
 * by the page size at +0x22; each of the four sets the thumb, READS THE POSITION BACK with
 * `GetScrollPos` so Windows clamps it to the range, stores that, and calls the change handler
 * at +0x1a with it. SB_THUMBPOSITION sets and stores the position it was given without reading
 * it back, and passes the change handler the stored copy. SB_THUMBTRACK moves nothing and stores
 * nothing: it hands the tracking position to a SECOND handler at +0x1e. Codes 6 and up
 * (top, bottom, end-scroll) fall outside the table and are ignored.
 *
 * Before every handler call `SetEventWindow` is pointed at the bar's owner, because the
 * handlers are called with the position alone and find their dialog through that global.
 *
 * WM_COMMAND with code 0x7f is the programmatic setter `scroll_set_pos` sends: it sets,
 * reads back and stores the position and CALLS NO HANDLER, so moving the bar from code never
 * looks like the user moving it.
 *
 * The case bodies are in the order 4, 5, 1, 0, 3, 2 -- the jump table's targets ascend in that
 * order. Both callers push the last argument as a long (one sign-extends a word into it, the
 * other passes a window procedure's lParam through), and only its low word is ever read; `int`
 * compiles to the same bytes. The WM_COMMAND arm is a one-case `switch`: it loads the code into
 * AX before comparing.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

/* same object: 4-byte `push cs; call ` (rule 47) */

void scrollbar_message(unsigned hwnd, unsigned msg, unsigned code, long value)
{
    SScrollBar *bar;
    int idx;
    register int pos;
    register unsigned h;

    h = hwnd;
    idx = GetClassWord(h, GCW_CBWNDEXTRA) - 6;
    bar = (SScrollBar *)GetWindowLong(h, idx);
    pos = get_bar_pos(bar);
    switch (msg) {
    case WM_HSCROLL:
    case WM_VSCROLL:
        switch (code) {
        case SB_THUMBPOSITION:
            pos = (int)value;
            SetScrollPos(h, SB_CTL, pos, 1);
            SetEventWindow(ctl_window_1070_96a8(bar));
            bar->pos = pos;
            call_bar_changed(bar, bar->pos);
            break;
        case SB_THUMBTRACK:
            SetEventWindow(ctl_window_1070_96a8(bar));
            call_bar_tracking(bar, (int)value);
            break;
        case SB_LINEDOWN:
            pos++;
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            SetEventWindow(ctl_window_1070_96a8(bar));
            bar->pos = pos;
            call_bar_changed(bar, pos);
            break;
        case SB_LINEUP:
            pos--;
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            SetEventWindow(ctl_window_1070_96a8(bar));
            bar->pos = pos;
            call_bar_changed(bar, pos);
            break;
        case SB_PAGEDOWN:
            pos += get_bar_page(bar);
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            SetEventWindow(ctl_window_1070_96a8(bar));
            bar->pos = pos;
            call_bar_changed(bar, pos);
            break;
        case SB_PAGEUP:
            pos -= get_bar_page(bar);
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            SetEventWindow(ctl_window_1070_96a8(bar));
            bar->pos = pos;
            call_bar_changed(bar, pos);
            break;
        }
        break;
    case WM_COMMAND:
        switch (code) {
        case 0x7f:
            SetScrollPos(h, SB_CTL, (int)value, 1);
            pos = GetScrollPos(h, SB_CTL);
            bar->pos = pos;
            break;
        }
        break;
    }
}
