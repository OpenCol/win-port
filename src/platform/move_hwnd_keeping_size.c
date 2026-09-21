/* move_hwnd_keeping_size -- shipped at 1070:0b8b.
 *
 * Ported by tools/port.py from matched/platform/move_hwnd_keeping_size.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1070_9649(RECT *r);
col_i16 rect_width_1070_962a(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetWindowRect(col_u16 hwnd, RECT *r);
extern void pascal MoveWindow(col_u16 hwnd, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 repaint);
#endif

/* same object: 4-byte `push cs; call ` */



void move_hwnd_keeping_size(register col_u16 hwnd, col_i16 x, col_i16 y)
{
    RECT r;

    if (hwnd == 0)
        return;
    GetWindowRect(hwnd, &r);
    MoveWindow(hwnd, x, y, rect_width_1070_962a(&r), rect_height_1070_9649(&r), 1);
}
