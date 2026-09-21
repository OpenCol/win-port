/* parse_binary_field -- shipped at 1040:7023.
 *
 * Ported by tools/port.py from matched/game/parse_binary_field.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_field[];                           SEG29:0x008c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * next_csv_field(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

unsigned char parse_binary_field(void)
{
    unsigned char v = 0;
    char *p;

    next_csv_field();
    p = g_field;
    while ((col_i16)*p == '0' || (col_i16)*p == '1') {
        v *= 2;
        if ((col_i16)*p == '1')
            v++;
        p++;
    }
    return v;
}
