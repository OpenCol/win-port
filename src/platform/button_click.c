/* button_click -- shipped at 1070:1dbb.
 *
 * Ported by tools/port.py from matched/platform/button_click.c.
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
col_i16 ctl_id_1070_96c5(Ctl *c);
col_i16 get_w_1a_1070_981e(Ctl *c);
void * ctl_window_1070_96a8(Ctl *c);
void MSDoAllTasks_1070_9837(void);
void MSWaitTicks_1070_984a(col_i16 n);
void call_handler_slot_28_1070_9863(Ctl *c, col_i16 a);
void repaint_hwnd(col_u16 hwnd);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetFocus(col_u16 hwnd);
#endif

/* same object: 4-byte `push cs; call ` */









void button_click(col_u16 hwnd)
{
    Win *w;
    Ctl *c;

    w = object_from_hwnd(hwnd);
    if (w == 0)
        return;
    c = (*((void * *)((unsigned char *)((Ctl *)w) + 0x2)));
    if (get_w_1a_1070_981e(c) == 0)
        return;
    SetFocus(hwnd);
    (*((col_i16 *)((unsigned char *)(w) + 0xe))) = 1;
    repaint_hwnd(hwnd);
    (*((col_i16 *)((unsigned char *)(w) + 0xe))) = 0;
    MSDoAllTasks_1070_9837();
    MSWaitTicks_1070_984a(8);
    repaint_hwnd(hwnd);
    g_current_window = ctl_window_1070_96a8(c);
    call_handler_slot_28_1070_9863(c, ctl_id_1070_96c5(c));
}
