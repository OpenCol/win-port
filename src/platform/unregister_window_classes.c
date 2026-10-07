/* unregister_window_classes: candidate for 1070:b2ae (150 bytes).  Verdict: RELOCATABLE.
 *
 * UnregisterClass for the four MS window classes, each failure reported.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_dc82(void *ctx, char *msg);

void unregister_window_classes(void)
{
    if (!UnregisterClass("MSAppWindow", g_hinstance))
        report_error_1070_dc82(&g_error_log, "Error: Cannot unregister AppWindow class");
    if (!UnregisterClass("MSWindowClass", g_hinstance))
        report_error_1070_dc82(&g_error_log, "Error: Cannot unregister window class");
    if (!UnregisterClass("MSControlClass", g_hinstance))
        report_error_1070_dc82(&g_error_log, "Error: Cannot unregister control class");
    if (!UnregisterClass("MSMrTimerClass", g_hinstance))
        report_error_1070_dc82(&g_error_log, "Error: Cannot unregister timer class");
}
