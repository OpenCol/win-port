/* dialog_wndproc -- shipped at 1070:9331.
 *
 * Ported by tools/port.py from matched/platform/dialog_wndproc.c.
 *
 * The file's own Scroll dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char byte_at_98(void *r);
col_i16 get_w_10_1070_9681(void *r);
col_i16 translate_key(col_i16 key);
col_i32 pascal object_message_handler(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);
void * get_p_6_1070_95f0(void *r);
void button_click(col_u16 hwnd);
void call_handler_slot_26_self(void *t);
void focus_walk_siblings(void *o, col_u16 hwnd, col_i16 dir);
void scrollbar_message(col_u16 hwnd, col_u16 msg, col_u16 code, col_i32 value);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetAsyncKeyState(col_i16 vk);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 idx);
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);
extern col_u16 pascal GetClassWord(col_u16 hwnd, col_i16 idx);
#endif

#include "w16const.h"
typedef struct { char pad[0x8c]; void *obj; void *ok_btn; void *cancel_btn; } Dlg_local;




extern void dialog_mnemonic(Dlg_local *w, char key);

/* same object: 4-byte `push cs; call ` (rule 47) */










col_i32 pascal dialog_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wparam,
                                         col_u32 lparam)
{
    Dlg_local *d;
    col_u16 hi;
    col_u16 hctl;
    col_u16 hedit;
    void *obj;
    col_i16 keys;
    col_i16 idx;

    d = (Dlg_local *)GetWindowLong(hwnd, 8);
    if (d == 0)
        return object_message_handler(hwnd, msg, wparam, lparam);
    switch (msg) {
    case WM_HSCROLL:
    case WM_VSCROLL:
        if ((((Scroll *)get_p_6_1070_95f0(d))->style & 0x80) != 0
            || (((Scroll *)get_p_6_1070_95f0(d))->style & 0x100) != 0) {
            return object_message_handler(hwnd, msg, wparam, lparam);
        } else {
            hi = lparam >> 16;
            scrollbar_message(hi, msg, wparam, lparam);
        }
        break;
    case WM_KEYDOWN:
        keys = GetAsyncKeyState(VK_SHIFT);
        if (wparam == VK_TAB && (unsigned char)((col_u16)keys >> 8) == 0) {
            if (byte_at_98(d))
                focus_walk_siblings(d, 0, 2);
        } else if (wparam == VK_TAB && (unsigned char)((col_u16)keys >> 8) != 0) {
            if (byte_at_98(d))
                focus_walk_siblings(d, 0, 3);
        } else if (wparam == VK_RETURN && (unsigned char)((col_u16)keys >> 8) == 0) {
            if (d->ok_btn != 0)
                button_click(get_w_10_1070_9681(d->ok_btn));
        } else if (wparam == VK_ESCAPE && (unsigned char)((col_u16)keys >> 8) == 0) {
            if (d->cancel_btn != 0)
                button_click(get_w_10_1070_9681(d->cancel_btn));
        } else {
            dialog_mnemonic(d, translate_key(wparam));
        }
        return object_message_handler(hwnd, msg, wparam, lparam);
    case WM_DESTROY:
        d->obj = 0;
        return object_message_handler(hwnd, msg, wparam, lparam);
    case WM_COMMAND:
        switch ((col_u16)(lparam >> 16)) {
        case WM_DESTROY:
            hctl = (col_u16)lparam;
            SendMessage(hctl, (col_u16)(lparam >> 16), wparam, lparam);
            break;
        case WM_CUT:
            hedit = (col_u16)lparam;
            idx = GetClassWord(hedit, -18) - 4;
            obj = (void *)GetWindowLong(hedit, idx);
            call_handler_slot_26_self(obj);
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
