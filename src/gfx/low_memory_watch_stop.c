/* low_memory_watch_stop -- shipped at 1068:bd25.
 *
 * Ported by tools/port.py from matched/gfx/low_memory_watch_stop.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_lowmem_timer;                           DGROUP:0x3252
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ResetTimer(col_i16 id);

void low_memory_watch_stop(void)
{
    if (g_lowmem_timer != 0)
        ResetTimer(g_lowmem_timer);
    g_lowmem_timer = 0;
}
