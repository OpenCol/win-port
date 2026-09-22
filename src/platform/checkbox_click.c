/* checkbox_click -- shipped at 1070:3113.
 *
 * Ported by tools/port.py from matched/platform/checkbox_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_current_window;              DGROUP:0x2ae0
 *
 * The file's own Win, Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Win * object_from_hwnd(col_u16 hwnd);
col_i16 get_w_1e_1070_9921(Ctl *c);
col_i16 get_w_2_1070_96c5(Ctl *c);
void * get_p_4_1070_96a8(Ctl *c);
void call_handler_slot_26_1070_9952(Ctl *c, col_i16 a, col_i16 b);
void set_w_1e_1070_993a(Ctl *c, col_i16 v);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetCapture(col_u16 hwnd);
extern col_u16 pascal SetFocus(col_u16 hwnd);
extern void pascal InvalidateRect(col_u16 hwnd, RECT *r, col_i16 erase);
#endif

/* same object: 4-byte `push cs; call ` */







void checkbox_click(col_u16 hwnd)
{
    Win *w;
    Ctl *c;

    w = object_from_hwnd(hwnd);
    if (w == 0)
        return;
    c = (*((void * *)((unsigned char *)((Ctl *)w) + 0x2)));
    SetFocus(hwnd);
    if (get_w_1e_1070_9921(c) == 0)
        set_w_1e_1070_993a(c, 1);
    else
        set_w_1e_1070_993a(c, 0);
    SetCapture(hwnd);
    InvalidateRect(hwnd, 0, 0);
    g_current_window = get_p_4_1070_96a8(c);
    call_handler_slot_26_1070_9952(c, get_w_2_1070_96c5(c), get_w_1e_1070_9921(c));
}
