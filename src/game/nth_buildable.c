/* nth_buildable -- shipped at 1018:8be3.
 *
 * Ported by tools/port.py from matched/game/nth_buildable.c.
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

col_i16 nth_buildable(col_i16 nth)
{
    col_i16 r = -2;
    register col_i16 k = -1;
    register col_i16 i;

    for (i = -1; r < -1 && i < 0x31; i++) {
        if (can_build_item(i) != 0) {
            k++;
            if (k == nth)
                r = i;
        }
    }
    return r;
}
