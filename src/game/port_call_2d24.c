/* port_call_2d24 -- shipped at 1048:daa5.
 *
 * Ported by tools/port.py from matched/game/port_call_2d24.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of_1048_e6d0(void *w);

extern void win_set_capture(void *p);



void port_call_2d24(void *w)
{
    win_set_capture(window_of_1048_e6d0(w));
}
