/* check_timer_tick -- shipped at 1040:843c.
 *
 * Ported by tools/port.py from matched/game/check_timer_tick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_flag_1d04;                              DGROUP:0x1d04
 *   extern int g_tick_done;                              DGROUP:0x1d06
 *   extern int g_tick_index;                             DGROUP:0x1d08
 *   extern long g_tick_last;                             DGROUP:0x1d0c
 *   extern char far g_opening_win[];                     SEG29:0x11ea
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 MSGetTicks_1040_aaa0(void);
void MSDoAllTasks_1040_ac99(void);
void MSHideWindow_of_1040_a80c(void *d);
void dialog_attach_menu_1040_acac(void *w);
void opening_scene(col_i16 n);

col_i16 check_timer_tick(col_i16 flag)
{
    col_i32 t;

    t = MSGetTicks_1040_aaa0();
    if (g_tick_last >= 0) {
        if (t - g_tick_last < 600L)
            goto skip;
    }
    if (g_tick_index >= 10) {
        g_tick_done = 1;
        g_tick_last = t;
    } else {
        opening_scene(g_tick_index);
        g_tick_index++;
        g_tick_last = t;
    }
skip:
    MSDoAllTasks_1040_ac99();
    if ((g_flag_1d04 == 1 && flag == 1) || g_tick_done == 1) {
        g_tick_done = 1;
        MSHideWindow_of_1040_a80c(g_opening_win + 0x26);
        MSDoAllTasks_1040_ac99();
        dialog_attach_menu_1040_acac(g_opening_win);
    }
    return g_tick_done;
}
