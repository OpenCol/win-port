/* check_ms_window_stack -- shipped at 1078:1e6b.
 *
 * Ported by tools/port.py from matched/platform/check_ms_window_stack.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_ms_window_count;                        DGROUP:0x3c80
 *   extern int g_ms_window_stack[];                      DGROUP:0x5728
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern char g_msg_window_stack[];                    DGROUP:0x3ebf
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void stub_reporter(void *ctx, char *msg);

/* same object: 4-byte `push cs; call ` (rule 47) */

void check_ms_window_stack(col_i16 h)
{
    col_i16 i;

    for (i = 0; i < g_ms_window_count; i++)
        if (g_ms_window_stack[i] == h)
            break;
    if (g_ms_window_count - 1 > i)
        for (; g_ms_window_count - 1 > i; i++)
            g_ms_window_stack[i] = g_ms_window_stack[i + 1];
    g_ms_window_count--;
    if (g_ms_window_count < 0)
        stub_reporter(&(*g_error_ctx), g_msg_window_stack);
}
