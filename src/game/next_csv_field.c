/* next_csv_field -- shipped at 1040:6efc.
 *
 * Ported by tools/port.py from matched/game/next_csv_field.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_field[];                           SEG29:0x008c
 *   extern char far *g_text_cursor_b;                    DGROUP:0x4ca8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void trim_both(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */

char * next_csv_field(void)
{
    char *d = g_field;

    while (*g_text_cursor_b != 0 && (col_i16)*g_text_cursor_b != ',') {
        *d = *g_text_cursor_b;
        g_text_cursor_b++;
        d++;
    }
    if (*g_text_cursor_b != 0)
        g_text_cursor_b++;
    *d = 0;
    trim_both(g_field);
    return g_field;
}
