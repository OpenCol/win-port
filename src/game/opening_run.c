/* opening_run -- shipped at 1010:04b6.
 *
 * Ported by tools/port.py from matched/game/opening_run.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own Opening dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1010_2a54(void);
void MSDoTask_1010_2b2a(void);

extern col_i16 start_timer(void *fn, col_i16 ms, col_i16 flag);
extern void intro_tick(void);
extern void ResetTimer(col_i16 id);




void opening_run(Opening *w)
{
    col_i16 t;

    t = start_timer((void *)intro_tick, 150, -1);
    while (w->done == 0)
        MSDoTask_1010_2b2a();
    ResetTimer(t);
    MSDoAllTasks_1010_2a54();
}
