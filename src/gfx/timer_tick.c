/* timer_tick -- shipped at 1068:3424.
 *
 * Ported by tools/port.py from matched/gfx/timer_tick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_anim;                             DGROUP:0x568c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void anim_tick(void *p);
void anim_unbind(void *p);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */



void timer_tick(col_i16 a, col_i16 b)
{
    anim_tick(g_anim);
    if (b == 0)
        anim_unbind(g_anim);
}
