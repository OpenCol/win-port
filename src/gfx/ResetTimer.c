/* ResetTimer -- shipped at 1068:90ba.
 *
 * Ported by tools/port.py from matched/gfx/ResetTimer.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_timer;                       DGROUP:0x2ae8
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1068_963d(void *ctx, char *msg);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void timer_free(void *timer, col_i16 t);

void ResetTimer(col_i16 t)
{
    if (g_timer == 0) {
        empty_1068_963d(&(*g_error_ctx), "Error: MrTimer not initialized in ResetTimer");
        return;
    }
    timer_free(g_timer, t);
}
