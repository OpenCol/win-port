/* window_relayout -- shipped at 1060:a249.
 *
 * Ported by tools/port.py from matched/game/window_relayout.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_current_window;                   DGROUP:0x2ae0
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void call_handler_slot_192(void *w);
void copy_all_to_screen_1060_ae72(void *w);

void window_relayout(char *w)
{
    g_current_window = w + 0x26;
    call_handler_slot_192(w);
    copy_all_to_screen_1060_ae72(w);
}
