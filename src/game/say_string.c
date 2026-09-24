/* say_string -- shipped at 1040:a33d.
 *
 * Ported by tools/port.py from matched/game/say_string.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * nth_text_string(col_i16 n);

void say_string(char *buf, col_i16 word)
{
    strcat(buf, nth_text_string(word));
}
