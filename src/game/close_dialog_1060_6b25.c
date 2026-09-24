/* close_dialog_1060_6b25 -- shipped at 1060:6b25.
 *
 * Ported by tools/port.py from matched/game/close_dialog_1060_6b25.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1060_6ce9(void *w);

extern void focus_marked_ctl(void *w);



void close_dialog_1060_6b25(void *w)
{
    MSShowWindow_of_1060_6ce9(w);
    focus_marked_ctl(w);
}
