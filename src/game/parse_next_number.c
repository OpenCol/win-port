/* parse_next_number -- shipped at 1040:6f76.
 *
 * Ported by tools/port.py from matched/game/parse_next_number.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *   extern int g_stub_i;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * next_csv_field(void);
col_i16 parse_number(char *s);

col_i16 parse_next_number(void)
{
    return parse_number(next_csv_field());
}
