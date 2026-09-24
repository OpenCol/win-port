/* hover_button_wndproc -- shipped at 1070:0cc0.
 *
 * Ported by tools/port.py from matched/platform/hover_button_wndproc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_hover_active;                           DGROUP:0x33c0
 *   extern int g_hover_pressed;                          DGROUP:0x33c2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
RECT * ptr_plus_8(void *c);
col_i16 ctl_id_1070_96c5(void *c);
col_i32 pascal control_message_handler(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
void * ctl_window_1070_96a8(void *c);
void * object_from_hwnd(col_u16 hwnd);
void call_handler_slot_26_1070_96de(void *c, col_i16 n);
void call_handler_slot_30_1070_971f(void *c, col_i16 n);
void call_handler_slot_34_1070_976b(void *c, col_i16 n);
void call_handler_slot_38(void *c, col_i16 n);
void call_handler_slot_42(void *c, col_i16 n);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal PtInRect(RECT *r, POINT pt);
extern col_u16 pascal SetCapture(col_u16 hwnd);
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal ReleaseCapture(void);
#endif

#include "w16const.h"

typedef struct { char pad[2]; void *ctl; } WinObj;








extern void set_current_window(void *w);

/* same object: 4-byte `push cs; call ` (rule 47) */











col_i32 pascal hover_button_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp)
{
    RECT r;
    POINT pt;
    WinObj *w;
    void *c;

    w = object_from_hwnd(hwnd);
    c = w->ctl;
    switch (msg) {
    case WM_MOUSEMOVE:
        pt.x = (col_i16)lp;
        pt.y = (col_i16)((col_u32)lp >> 16);
        if (g_hover_active == 0) {
            if (g_hover_pressed == 0) {
                g_hover_active = 1;
                SetCapture(hwnd);
                if (c != 0) {
                    set_current_window(ctl_window_1070_96a8(c));
                    call_handler_slot_26_1070_96de(c, ctl_id_1070_96c5(c));
                }
            }
        } else {
            r = *ptr_plus_8(c);
            OffsetRect(&r, -r.left, -r.top);
            if (PtInRect(&r, pt) == 0) {
                g_hover_active = 0;
                if (g_hover_pressed == 0) {
                    ReleaseCapture();
                    if (c != 0) {
                        set_current_window(ctl_window_1070_96a8(c));
                        call_handler_slot_30_1070_971f(c, ctl_id_1070_96c5(c));
                    }
                }
            }
        }
        break;
    case WM_LBUTTONDOWN:
        if (g_hover_active != 0) {
            g_hover_pressed = 1;
            set_current_window(ctl_window_1070_96a8(c));
            call_handler_slot_38(c, ctl_id_1070_96c5(c));
        }
        break;
    case WM_LBUTTONUP:
        if (g_hover_pressed != 0) {
            g_hover_pressed = 0;
            if (g_hover_active != 0) {
                if (c != 0) {
                    set_current_window(ctl_window_1070_96a8(c));
                    call_handler_slot_34_1070_976b(c, ctl_id_1070_96c5(c));
                }
            } else {
                ReleaseCapture();
                if (c != 0) {
                    set_current_window(ctl_window_1070_96a8(c));
                    call_handler_slot_30_1070_971f(c, ctl_id_1070_96c5(c));
                }
            }
        }
        break;
    case WM_LBUTTONDBLCLK:
        if (g_hover_active != 0) {
            set_current_window(ctl_window_1070_96a8(c));
            call_handler_slot_42(c, ctl_id_1070_96c5(c));
        }
        break;
    default:
        return control_message_handler(hwnd, msg, wp, lp);
    }
    return 0;
}
