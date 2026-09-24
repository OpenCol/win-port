/* say_nation_gold -- shipped at 1018:40e6.
 *
 * Ported by tools/port.py from matched/game/say_nation_gold.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 nation_gold(col_i16 nation);

extern void say_bucks(char *buf, col_i32 amount);

/* same object: 4-byte `push cs; call ` */


void say_nation_gold(col_i16 nation, char *buf)
{
    say_bucks(buf, nation_gold(nation));
}
