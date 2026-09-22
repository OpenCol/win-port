/* wm_command -- shipped at 1070:692d.
 *
 * Ported by tools/port.py from matched/platform/wm_command.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_menu_depth;                             DGROUP:0x56de
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char call_handler_slot_124_flag(void *t, col_i16 a, col_i16 b);
char call_handler_slot_128_flag(void *t, col_i32 e);
col_i32 menu_find_item(col_u16 hmenu, col_u16 id);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
extern col_u16 pascal GetMenu(col_u16 hwnd);
#endif

/* same object: 4-byte `push cs; call ` */




void wm_command(register col_u16 hwnd, register col_u16 id)
{
    void *obj;
    col_u16 menu;
    col_i16 lo;
    col_i16 hi;
    col_i32 item;

    obj = (void *)GetWindowLong(hwnd, 4);
    if (call_handler_slot_128_flag(obj, (col_u32)id) == 0) {
        g_menu_depth = 0;
        menu = GetMenu(hwnd);
        if (menu != 0) {
            item = menu_find_item(menu, id);
            lo = (col_i16)item;
            hi = (col_i16)((col_u32)item >> 16);
            call_handler_slot_124_flag(obj, lo, hi);
        }
    }
}
