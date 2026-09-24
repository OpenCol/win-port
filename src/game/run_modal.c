/* run_modal -- shipped at 1060:9f50.
 *
 * Ported by tools/port.py from matched/game/run_modal.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Modal far *g_modal_win;                       DGROUP:0x2ae4
 *   extern char far g_stub_s[];                          
 *   extern int g_stub_a;                                 
 *
 * The file's own Modal dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of(void *o);
void MSDoTask_1060_add3(void);

extern void disable_parent(void *w);
extern void enable_parent(void *w);




void run_modal(Modal *o)
{
    o->running = 1;
    g_modal_win = o;
    disable_parent(window_of(o));
    while (o->running != 0)
        MSDoTask_1060_add3();
    enable_parent(window_of(o));
}
