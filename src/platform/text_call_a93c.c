/* text_call_a93c -- shipped at 1070:a9c0.
 *
 * Ported by tools/port.py from matched/platform/text_call_a93c.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void wing_set_dib_colours(void *w, void *pal, col_i16 start, col_i16 count);

void text_call_a93c(void *w, void *pal)
{
    wing_set_dib_colours(w, pal, 0, 0x100);
}
