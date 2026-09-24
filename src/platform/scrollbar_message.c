/* scrollbar_message -- shipped at 1070:5d32.
 *
 * Ported by tools/port.py from matched/platform/scrollbar_message.c.
 *
 * The file's own Bar dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_22(void *r);
col_i16 get_w_26_1070_9ada(void *r);
void * get_p_4_1070_96a8(void *r);
void call_handler_slot_26_1070_9af3(void *t, col_i16 a);
void call_handler_slot_30_1070_9b19(void *t, col_i16 a);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetScrollPos(col_u16 hwnd, col_i16 bar);
extern col_i16 pascal SetScrollPos(col_u16 hwnd, col_i16 bar, col_i16 pos, col_i16 redraw);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
extern col_u16 pascal GetClassWord(col_u16 hwnd, col_i16 index);
#endif

#include "w16const.h"

/* +0x1a on_change, +0x1e on_track, +0x22 page */





extern void set_current_window(void *w);
/* same object: 4-byte `push cs; call ` (rule 47) */






void scrollbar_message(col_u16 hwnd, col_u16 msg, col_u16 code, col_i32 value)
{
    Bar *bar;
    col_i16 idx;
    register col_i16 pos;
    register col_u16 h;

    h = hwnd;
    idx = GetClassWord(h, GCW_CBWNDEXTRA) - 6;
    bar = (Bar *)GetWindowLong(h, idx);
    pos = get_w_26_1070_9ada(bar);
    switch (msg) {
    case WM_HSCROLL:
    case WM_VSCROLL:
        switch (code) {
        case SB_THUMBPOSITION:
            pos = (col_i16)value;
            SetScrollPos(h, SB_CTL, pos, 1);
            set_current_window(get_p_4_1070_96a8(bar));
            bar->pos = pos;
            call_handler_slot_26_1070_9af3(bar, bar->pos);
            break;
        case SB_THUMBTRACK:
            set_current_window(get_p_4_1070_96a8(bar));
            call_handler_slot_30_1070_9b19(bar, (col_i16)value);
            break;
        case SB_LINEDOWN:
            pos++;
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            set_current_window(get_p_4_1070_96a8(bar));
            bar->pos = pos;
            call_handler_slot_26_1070_9af3(bar, pos);
            break;
        case SB_LINEUP:
            pos--;
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            set_current_window(get_p_4_1070_96a8(bar));
            bar->pos = pos;
            call_handler_slot_26_1070_9af3(bar, pos);
            break;
        case SB_PAGEDOWN:
            pos += get_w_22(bar);
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            set_current_window(get_p_4_1070_96a8(bar));
            bar->pos = pos;
            call_handler_slot_26_1070_9af3(bar, pos);
            break;
        case SB_PAGEUP:
            pos -= get_w_22(bar);
            SetScrollPos(h, SB_CTL, pos, 1);
            pos = GetScrollPos(h, SB_CTL);
            set_current_window(get_p_4_1070_96a8(bar));
            bar->pos = pos;
            call_handler_slot_26_1070_9af3(bar, pos);
            break;
        }
        break;
    case WM_COMMAND:
        switch (code) {
        case 0x7f:
            SetScrollPos(h, SB_CTL, (col_i16)value, 1);
            pos = GetScrollPos(h, SB_CTL);
            bar->pos = pos;
            break;
        }
        break;
    }
}
