/* low_memory_timer -- shipped at 1068:bc53.
 *
 * Ported by tools/port.py from matched/gfx/low_memory_timer.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_lowmem_kb;                              DGROUP:0x324c
 *   extern int g_lowmem_res;                             DGROUP:0x324e
 *   extern int g_lowmem_flag;                            DGROUP:0x3250
 *   extern int g_lowmem_busy;                            DGROUP:0x3254
 *   extern int g_lowmem_count;                           DGROUP:0x3256
 *   extern long g_lowmem_deadline;                       DGROUP:0x3258
 *   extern void far *g_lowmem_win;                       DGROUP:0x3248
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_u32 MSGetTicks_1068_d953(void);
void low_memory_warn_show(col_i16 res, void *win, col_i16 flag);

extern col_i16 memory_is_low(col_i16 kb);

/* same object: 4-byte `push cs; call ` */



void low_memory_timer(void)
{
    if (memory_is_low(g_lowmem_kb) != 0) {
        if (g_lowmem_busy == 0
            && (g_lowmem_count == 0
                || (g_lowmem_count < 3
                    && (col_i32)MSGetTicks_1068_d953() > g_lowmem_deadline))) {
            g_lowmem_busy = 1;
            g_lowmem_count++;
            low_memory_warn_show(g_lowmem_res, g_lowmem_win, g_lowmem_flag);
        }
    } else {
        g_lowmem_count = 0;
    }
}
