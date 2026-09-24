/* say_bucks -- shipped at 1040:a486.
 *
 * Ported by tools/port.py from matched/game/say_bucks.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void say_long(char *s, col_i32 v);

extern void ltoa_rtl(col_i32 v, char *s, col_i16 radix);




void say_bucks(char *s, col_i32 v)
{
    say_long(s, v);
    strcat(s, "\xa4");
}
