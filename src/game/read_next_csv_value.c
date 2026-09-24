/* read_next_csv_value -- shipped at 1040:6ff0.
 *
 * Ported by tools/port.py from matched/game/read_next_csv_value.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_field[];                           SEG29:0x008c
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void next_csv_field(void);

extern col_i16 text_store_string(char *s);



col_i16 read_next_csv_value(void)
{
    col_i16 r;

    next_csv_field();
    r = text_store_string(g_field);
    return r;
}
