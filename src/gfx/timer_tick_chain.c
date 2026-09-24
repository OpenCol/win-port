/* timer_tick_chain -- shipped at 1068:3456.
 *
 * Ported by tools/port.py from matched/gfx/timer_tick_chain.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_anim;                             DGROUP:0x568c
 *   extern void far *g_anim2;                            DGROUP:0x5690
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void anim_start_chained(void *a, void *b);
void anim_stop(void *p);
void anim_tick(void *p);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */




void timer_tick_chain(col_i16 a, col_i16 b)
{
    anim_tick(g_anim);
    if (b == 0) {
        anim_stop(g_anim);
        anim_start_chained(g_anim, g_anim2);
    }
}
