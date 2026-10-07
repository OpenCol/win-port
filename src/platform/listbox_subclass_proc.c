/* listbox_subclass_proc: candidate for 1070:530e (237 bytes).  Verdict: RELOCATABLE.
 *
 * Subclass procedure for MS controls: the previous window procedure and the handler table
 * live in the two window longs below the class's extra bytes. Tab, Return and Escape key
 * messages go to the parent, other keys to the old procedure; WM_DESTROY fires handler slot
 * 28; everything else is the old procedure. Handled messages return 1.
 *
 * RENAMED 2026-09-26 from subclass_proc_530e: the window procedure create_MSListBoxClass_window installs
 * (SetWindowLong GWL_WNDPROC) on every MSListBoxClass window, the only install site.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
#include "records.h"
typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void call_listbox_destroyed(void *listbox);
long listbox_subclass_proc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam)
{
    WNDPROC oldproc;
    void *t;
    int off;

    off = GetClassWord(hwnd, -18) - 8;
    oldproc = (WNDPROC)GetWindowLong(hwnd, off);
    t = (void *)GetWindowLong(hwnd, off + 4);
    switch (msg) {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
        if (wparam == 9 || wparam == 0xd || wparam == 0x1b)
            SendMessage(GetParent(hwnd), msg, wparam, lparam);
        else
            return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
        break;
    case WM_DESTROY:
        if (t != 0)
            call_listbox_destroyed((ListboxHandlers *)t);
        break;
    default:
        return CallWindowProc(oldproc, hwnd, msg, wparam, lparam);
    }
    return 1;
}
