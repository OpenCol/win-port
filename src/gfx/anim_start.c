/* anim_start -- shipped at 1068:234a.
 *
 * Ported by tools/port.py from matched/gfx/anim_start.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_anim;                             DGROUP:0x568c
 *   extern void far *g_anim2;                            DGROUP:0x5690
 *
 * The file's own Anim dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void port_bind_anim(void *port, Anim *a);
extern void timer_tick(void);

/* same object: 5-byte NOP form, so extern (rule 47) */
extern col_i16 start_timer(void ( *fn)(void), col_i16 interval, col_i16 id);

void anim_start(Anim *a, void *port)
{
    g_anim2 = (*((void * *)((unsigned char *)(a) + 0x570))) = port;
    port_bind_anim(port, a);
    g_anim = a;
    (*((col_i16 *)((unsigned char *)(a) + 0x56e))) = start_timer(timer_tick, (*((col_i16 *)((unsigned char *)(a) + 0x36))), (*((col_i16 *)((unsigned char *)(a) + 0x2c))));
}
