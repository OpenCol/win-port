/* low_memory_watch_start -- shipped at 1068:bcc3.
 *
 * Ported by tools/port.py from matched/gfx/low_memory_watch_start.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_lowmem_kb;                              DGROUP:0x324c
 *   extern int g_lowmem_res;                             DGROUP:0x324e
 *   extern int g_lowmem_flag;                            DGROUP:0x3250
 *   extern int g_lowmem_timer;                           DGROUP:0x3252
 *   extern int g_lowmem_busy;                            DGROUP:0x3254
 *   extern int g_lowmem_count;                           DGROUP:0x3256
 *   extern void far *g_lowmem_win;                       DGROUP:0x3248
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ResetTimer(col_i16 id);
extern col_i16 start_timer(void *fn, col_i16 ms, col_i16 flag);
extern void low_memory_timer(void);

void low_memory_watch_start(col_i16 kb, col_i16 secs, col_i16 res, void *win, col_i16 flag)
{
    if (g_lowmem_timer != 0)
        ResetTimer(g_lowmem_timer);
    g_lowmem_kb = kb;
    g_lowmem_res = res;
    g_lowmem_win = win;
    g_lowmem_flag = flag;
    g_lowmem_count = 0;
    g_lowmem_busy = 0;
    g_lowmem_timer = start_timer((void *)low_memory_timer, secs * 1000, -1);
}
