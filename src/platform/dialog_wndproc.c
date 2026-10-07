/* dialog_wndproc: candidate for 1070:9331 (699 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: dialog_wndproc
 * size-source: 699, not the census 690. The trailing 9 bytes are the dead last epilogue.
 *
 * The window procedure of a game dialog, which keeps its object in window long 8 and hands
 * everything it does not finish to the subclass procedure at 1070:69ba (what
 * `subclass_control_1070_67dd` installs). Before the object is attached, every message goes
 * straight there.
 *
 *   WM_HSCROLL / WM_VSCROLL (0x114, 0x115): to `scrollbar_message` with the scroll bar's
 *       handle from HIWORD(lParam), unless the dialog's style word has 0x80 or 0x100 set.
 *   WM_KEYDOWN (0x100): Tab walks the focus forward, Shift+Tab backward, when byte +0x98 allows;
 *       Enter presses the default button at +0x90 and Esc the cancel button at +0x94 (unshifted);
 *       any other key is lower-cased and tried as a mnemonic. The key is passed on either way.
 *   WM_DESTROY (2): clears the control list head at +0x8c and passes the message on.
 *   WM_COMMAND (0x111): notification 2 is re-sent to the control that raised it as message 2
 *       with the same parameters, and 0x300 (EN_CHANGE) runs the edit control's own handler,
 *       whose object is the long in the last four of its window extra bytes. Other
 *       notifications are passed on.
 *
 * The switch bodies are laid out in the source's order -- scroll, key, destroy, command, default --
 * not the order of the tests (rule 36).
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern void dialog_mnemonic(DialogWin *w, char key);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void *get_win_1070_95f0(DialogWin *r);

extern "C" char get_keyboard_nav(DialogWin *r);
extern "C" void focus_walk_siblings(DialogWin *o, unsigned hwnd, int dir);
extern "C" int ctl_hwnd_1070_9681(void *ctl);
extern "C" void MSPushButton(unsigned hwnd);

extern "C" void call_edit_changed(void *edit);

extern "C" long dialog_wndproc(unsigned hwnd, unsigned msg, unsigned wparam,
                                         unsigned long lparam)
{
    DialogWin *d;
    unsigned hi;
    unsigned hctl;
    unsigned hedit;
    void *obj;
    int keys;
    int idx;

    d = (DialogWin *)GetWindowLong(hwnd, 8);
    if (d == 0)
        return object_message_handler(hwnd, msg, wparam, lparam);
    switch (msg) {
    case WM_HSCROLL:
    case WM_VSCROLL:
        if ((((Scroll *)get_win_1070_95f0(d))->style & 0x80) != 0
            || (((Scroll *)get_win_1070_95f0(d))->style & 0x100) != 0) {
            return object_message_handler(hwnd, msg, wparam, lparam);
        } else {
            hi = lparam >> 16;
            scrollbar_message(hi, msg, wparam, lparam);
        }
        break;
    case WM_KEYDOWN:
        keys = GetAsyncKeyState(VK_SHIFT);
        if (wparam == VK_TAB && (unsigned char)((unsigned)keys >> 8) == 0) {
            if (get_keyboard_nav(d))
                focus_walk_siblings(d, 0, 2);
        } else if (wparam == VK_TAB && (unsigned char)((unsigned)keys >> 8) != 0) {
            if (get_keyboard_nav(d))
                focus_walk_siblings(d, 0, 3);
        } else if (wparam == VK_RETURN && (unsigned char)((unsigned)keys >> 8) == 0) {
            if (d->ok_btn != 0)
                MSPushButton(ctl_hwnd_1070_9681(d->ok_btn));
        } else if (wparam == VK_ESCAPE && (unsigned char)((unsigned)keys >> 8) == 0) {
            if (d->cancel_btn != 0)
                MSPushButton(ctl_hwnd_1070_9681(d->cancel_btn));
        } else {
            dialog_mnemonic(d, translate_key(wparam));
        }
        return object_message_handler(hwnd, msg, wparam, lparam);
    case WM_DESTROY:
        d->first_ctl = 0;
        return object_message_handler(hwnd, msg, wparam, lparam);
    case WM_COMMAND:
        switch ((unsigned)(lparam >> 16)) {
        case WM_DESTROY:
            hctl = (unsigned)lparam;
            SendMessage(hctl, (unsigned)(lparam >> 16), wparam, lparam);
            break;
        case EN_CHANGE:
            hedit = (unsigned)lparam;
            idx = GetClassWord(hedit, -18) - 4;
            obj = (void *)GetWindowLong(hedit, idx);
            call_edit_changed((SEditBox *)obj);
            break;
        default:
            return object_message_handler(hwnd, msg, wparam, lparam);
        }
        break;
    default:
        return object_message_handler(hwnd, msg, wparam, lparam);
    }
    return 0;
}
