/* unregister_window_classes -- shipped at 1070:b2ae.
 *
 * Ported by tools/port.py from matched/platform/unregister_window_classes.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_hinstance;                         DGROUP:0x56e2
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_dc82(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal UnregisterClass(const char *cls, col_u16 hinst);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */



void unregister_window_classes(void)
{
    if (!UnregisterClass("MSAppWindow", g_hinstance))
        empty_1070_dc82(&(*g_error_ctx), "Error: Cannot unregister AppWindow class");
    if (!UnregisterClass("MSWindowClass", g_hinstance))
        empty_1070_dc82(&(*g_error_ctx), "Error: Cannot unregister window class");
    if (!UnregisterClass("MSControlClass", g_hinstance))
        empty_1070_dc82(&(*g_error_ctx), "Error: Cannot unregister control class");
    if (!UnregisterClass("MSMrTimerClass", g_hinstance))
        empty_1070_dc82(&(*g_error_ctx), "Error: Cannot unregister timer class");
}
