/* close_dialog_1050_94ab -- shipped at 1050:94ab.
 *
 * Ported by tools/port.py from matched/game/close_dialog_1050_94ab.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1050_8ff2(void *w);

extern void focus_marked_ctl(void *w);



void close_dialog_1050_94ab(void *w)
{
    MSShowWindow_of_1050_8ff2(w);
    focus_marked_ctl(w);
}
