/* read_line_1040_6141 -- shipped at 1040:6141.
 *
 * Ported by tools/port.py from matched/game/read_line_1040_6141.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 read_file_1040_a84d(void *f, void *buf, col_i32 n);
void strip_char(char *s, char c);

char read_line_1040_6141(char *dst, void *f)
{
    register col_i16 r;
    register col_i16 n = 0;
    char c;
    char *start;

    *dst = 0;
    start = dst;
    r = read_file_1040_a84d(f, &c, 1L);
    if (r == 0)
        return 0;
    while ((col_i16)c != -1 && (col_i16)c != '\n' && n < 0xff) {
        *dst = c;
        dst++;
        n++;
        r = read_file_1040_a84d(f, &c, 1L);
        if (r == 0)
            return 0;
    }
    *dst = 0;
    strip_char(start, '\n');
    strip_char(start, '\r');
    return 1;
}
