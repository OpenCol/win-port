/* nation_take_gold -- shipped at 1018:40c5.
 *
 * Ported by tools/port.py from matched/game/nation_take_gold.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long g_stub_l;                                
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void nation_add_gold(col_i16 n, col_i32 amount);

void nation_take_gold(col_i16 n, col_i32 amount)
{
    nation_add_gold(n, 0L - amount);
}
