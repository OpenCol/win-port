/* close_dialog_1038_e279 -- shipped at 1038:e279.
 *
 * Ported by tools/port.py from matched/game/close_dialog_1038_e279.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSSetFocus_of_1038_e53e(void *w);

extern void focus_marked_ctl(void *w);



void close_dialog_1038_e279(void *w)
{
    MSSetFocus_of_1038_e53e(w);
    focus_marked_ctl(w);
}
