/* create_timer_daemon_window -- shipped at 1078:1885.
 *
 * Ported by tools/port.py from matched/platform/create_timer_daemon_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void stub_reporter(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
extern col_u16 pascal CreateWindow(const char *cls, const char *title, col_u32 style, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_u16 parent, col_u16 menu, col_u16 hinst, void *param);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */




col_u16 create_timer_daemon_window(void *obj)
{
    col_u16 h = CreateWindow("MSMrTimerClass", "MrTimer", 0xcf0000L, 0x8000, 0x8000, 0x8000, 0x8000, 0, 0, g_hinstance, 0);

    if (h == 0)
        stub_reporter(&(*g_error_ctx), "Error: Cannot create timer daemon");
    SetWindowLong(h, 0, (col_i32)obj);
    return h;
}
