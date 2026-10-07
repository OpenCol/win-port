/* static_text_wndproc: candidate for 1070:6426 (315 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The window procedure of a static-text control: WM_PAINT draws the window's own title text,
 * vertically centred and single-line (left-aligned), in the font at DGROUP:0x2ace, over whatever `wing_child_paint`
 * paints behind it. Everything else goes to `control_message_handler`.
 *
 * The font is locked, selected, and UNLOCKED IMMEDIATELY -- `MSHLock`, `SelectObject`,
 * `MSHUnlock`, all before any drawing -- so the handle is held only long enough to read the
 * `HFONT` out of it. The lock is not protecting the font during the draw; it is how a
 * movable global is read at all.
 *
 * `SetBkMode(hdc, 1)` is TRANSPARENT, which is what lets the text sit on top of the
 * background `wing_child_paint` has just drawn rather than over a filled box.
 *
 * 315 bytes, not the census 304: the `return` in the else branch costs a second epilogue, and
 * a `RETF 0x0A` frame's epilogue is eleven bytes (rule 44).
 *
 * `hwnd` reaches SI by being COPIED into the first of two register locals (rule 53): the
 * window handle is used eight times and the device context four, and both are registers.
 *
 * IT IS A ONE-CASE `switch`, NOT AN `if`. The shipped code loads the message into AX
 * before comparing -- `MOV AX,[BP+0xc] / CMP AX,0xF` where an `if` folds to
 * `CMP WORD [BP+0xc],0xF` -- and then jumps with the conditional-over-unconditional
 * pair a compare chain uses. Four spellings of the `if` were tried, including the goto
 * form and a written-out `(int)` cast, and none of them loads the value first.
 *
 * The result is a `long` and the message a `WPARAM`-sized word, so `GetWindowLong`'s
 * return goes through EAX (`SHL EAX,0x10 / SHRD EAX,EDX,0x10`), which is rule 48's
 * signed/unsigned conversion: the control value is `unsigned long` here.
 *
 * RENAMED 2026-09-26 from text_out: This is the window procedure (hwnd, msg, wp, lp) of the MSStaticClass control. register_MSStaticClass installs it with SetWindowLong(hwnd, -4, ...). It handles WM_PAINT by calling wing_child_paint, then DrawText of the window title, then draw_ctl_border, and sends every other message to control_message_handler. 'text_out' reads like a TextOut drawing wrapper, not a wndproc. Its siblings are named checkbox_wndproc, radio_wndproc and region_wndproc.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#include "classes.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */

extern int font_handle_1070_9806(void *ref);

long static_text_wndproc(unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    register unsigned w;
    register unsigned hdc;
    FontRec *font;
    RECT r;
    unsigned saved;
    unsigned long ctl;
    PAINTSTRUCT ps;
    char buf[0x100];

    w = hwnd;
    ctl = GetWindowLong(w, 6);
    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(w, &ps);
        GetClientRect(w, &r);
        wing_child_paint(&ps, w);
        font = (FontRec *)MSHLock(font_handle_1070_9806((Font *)g_control_font));
        saved = SelectObject(hdc, font->hfont);
        MSHUnlock(font_handle_1070_9806((Font *)g_control_font));
        SetBkMode(hdc, TRANSPARENT);
        GetWindowText(w, buf, 0xff);
        DrawText(hdc, buf, STRLEN(buf), &r, DT_VCENTER | DT_SINGLELINE);
        draw_ctl_border(w, hdc, ctl);
        SelectObject(hdc, saved);
        EndPaint(w, &ps);
        break;
    default:
        return control_message_handler(w, msg, wp, lp);
    }
    return 0;
}
