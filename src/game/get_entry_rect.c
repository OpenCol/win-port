/* get_entry_rect -- shipped at 1040:7420.
 *
 * Ported by tools/port.py from matched/game/get_entry_rect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * find_entry(col_i16 a, col_i16 b, col_i16 c);

void get_entry_rect(col_i16 a, col_i16 b, col_i16 c, RECT *r)
{
    char *p;

    p = find_entry(a, b, c);
    if (p == 0)
        return;
    *r = *(RECT *)(p + 2);
}
