/* count_buildable -- shipped at 1018:8baf.
 *
 * Ported by tools/port.py from matched/game/count_buildable.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 can_build_item(col_i16 item);

/* same object: 4-byte `push cs; call ` */


col_i16 count_buildable(void)
{
    register col_i16 n;
    register col_i16 i;

    n = 0;
    for (i = -1; i < 0x31; i++) {
        if (can_build_item(i) != 0)
            n++;
    }
    return n;
}
