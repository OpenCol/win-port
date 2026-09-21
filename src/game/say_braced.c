/* say_braced -- shipped at 1040:a365.
 *
 * Ported by tools/port.py from matched/game/say_braced.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * nth_text_string(col_i16 n);
void append_lbrace(char *dst);
void append_rbrace(char *dst);

void say_braced(char *dst, col_i16 n)
{
    append_lbrace(dst);
    strcat(dst, nth_text_string(n));
    append_rbrace(dst);
}
