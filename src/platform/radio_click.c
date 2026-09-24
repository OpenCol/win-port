/* radio_click -- shipped at 1070:434d.
 *
 * Ported by tools/port.py from matched/platform/radio_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_current_window;              DGROUP:0x2ae0
 *
 * The file's own RadioItem, Ctl, Model, Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Obj * object_from_hwnd(col_u16 hwnd);
RadioItem * get_p_2e_1070_99c5(Ctl *c);
col_i16 get_w_2_1070_96c5(Ctl *c);
col_i16 get_w_2a_1070_99ac(Ctl *c);
void * get_p_4_1070_96a8(Ctl *c);
void call_handler_slot_26_1070_99e2(Ctl *c, col_i16 a, col_i16 b);
void set_w_2a_1070_9994(Ctl *c, col_i16 v);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal InvalidateRect(col_u16 hwnd, void *r, col_i16 erase);
extern col_u16 pascal SetFocus(col_u16 hwnd);
#endif

/* 0x92 bytes */
/* +0xa, +0xc */
/* same object: 4-byte `push cs; call ` */








void radio_click(register col_u16 hwnd)
{
    Obj *o;
    Ctl *c;
    Model *m;

    o = object_from_hwnd(hwnd);
    if (o == 0)
        return;
    c = (*((Ctl * *)((unsigned char *)(o) + 0x2)));
    m = (*((Model * *)((unsigned char *)(o) + 0x14)));
    SetFocus(hwnd);
    if (get_w_2a_1070_99ac(c) != m->sel && m->enabled == 1) {
        InvalidateRect(get_p_2e_1070_99c5(c)[get_w_2a_1070_99ac(c)].hwnd, 0L, 0);
        set_w_2a_1070_9994(c, m->sel);
        InvalidateRect(get_p_2e_1070_99c5(c)[get_w_2a_1070_99ac(c)].hwnd, 0L, 0);
    }
    g_current_window = get_p_4_1070_96a8(c);
    call_handler_slot_26_1070_99e2(c, get_w_2_1070_96c5(c), get_w_2a_1070_99ac(c));
}
