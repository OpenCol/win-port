/* editbox_subclass_proc: candidate for 1070:57ce (681 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: editbox_subclass_proc
 *
 * Subclass procedure of MSEditBoxClass. The owner is window long 10, the old procedure the
 * long below the class's extra bytes. Key handling: Tab and Return (and Escape in a
 * single-line box) go to the parent. WM_KEYDOWN asks the owner's slot-30 handler whether
 * the key is accepted (DGROUP:0x33ce remembers the answer, 0x33cf that a key-down was
 * seen, 0x56d4 the key); an accepted key goes to the parent or the old procedure. WM_CHAR
 * and WM_KEYUP follow that verdict; WM_SETFOCUS selects the whole text first. Handled
 * messages return 1.
 *
 * RENAMED 2026-09-26 from subclass_proc_57ce: the window procedure create_MSEditBoxClass_window installs
 * (SetWindowLong GWL_WNDPROC) on every MSEditBoxClass window, the only install site.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
typedef struct { char pad[1]; } EditHandlers;
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int edit_key_allowed(SEditBox *table, int key);

extern "C" long editbox_subclass_proc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam)
{
    WNDPROC oldproc;
    SEditBox *edit;
    int off;

    edit = (SEditBox *)GetWindowLong(hwnd, 10);
    off = GetClassWord(hwnd, -18) - 8;
    oldproc = (WNDPROC)GetWindowLong(hwnd, off);
    switch (msg) {
    case WM_KEYUP:
        if (g_key_accepted != 0) {
            if (wparam == 9 || wparam == 0xd || (wparam == 0x1b && edit->multiline == 0))
                SendMessage(GetParent(hwnd), msg, wparam, lparam);
            else
                return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
        }
        break;
    case WM_KEYDOWN:
        g_key_down_seen = 1;
        if (edit_key_allowed(edit, translate_key(wparam)) != 0) {
            if (wparam == 9 || wparam == 0xd || (wparam == 0x1b && edit->multiline == 0))
                SendMessage(GetParent(hwnd), msg, wparam, lparam);
            else
                return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
            g_key_accepted = 1;
        } else
            g_key_accepted = 0;
        g_last_key = wparam;
        break;
    case WM_CHAR:
        if (g_key_accepted != 0) {
            if (edit->multiline == 0) {
                if (wparam == 9 || wparam == 0xd || wparam == 0x1b)
                    SendMessage(GetParent(hwnd), msg, wparam, lparam);
                else if (g_key_down_seen == 0) {
                    if (edit_key_allowed(edit, translate_key(g_last_key)) != 0)
                        return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
                } else
                    return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
            } else if (g_key_down_seen == 0) {
                if (edit_key_allowed(edit, translate_key(g_last_key)) != 0)
                    return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
            } else
                return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
        }
        g_key_accepted = 1;
        g_key_down_seen = 0;
        break;
    case WM_SETFOCUS:
        send_makelong(hwnd, 0, -1);
        return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
    default:
        return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
    }
    return 1;
}
