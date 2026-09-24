/* current_window_call -- shipped at 1060:2b6b.
 *
 * Ported by tools/port.py from matched/game/current_window_call.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_close_1060_2a47(void *w);

extern char * current_window(void);



void current_window_call(void)
{
    char *w;
    char *cw;

    cw = current_window();
    w = cw != 0 ? cw - 0x26 : cw;
    if (w == 0)
        return;
    win_close_1060_2a47(w);
}
