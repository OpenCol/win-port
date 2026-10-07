/* read_line_1040_6141: candidate for 1040:6141 (204 bytes).  Verdict: RELOCATABLE.
 * symbol: read_line_1040_6141
 *
 * Reads a line of up to 255 bytes from the file into dst, one byte at a
 * time, stopping at 0xff or newline, then strips newlines and carriage
 * returns from it.  Returns 1, or 0 when a read fails.  The two helpers
 * are -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "records.h"
extern int g_stub_a;

extern int read_file_1040_a84d(void *file, void *buf, long n);

char read_line_1040_6141(char *dst, void *file)
{
    register int r;
    register int n = 0;
    char c;
    char *start;

    *dst = 0;
    start = dst;
    r = read_file_1040_a84d((MSFile *)file, &c, 1L);
    if (r == 0)
        return 0;
    while ((int)c != -1 && (int)c != '\n' && n < 0xff) {
        *dst = c;
        dst++;
        n++;
        r = read_file_1040_a84d((MSFile *)file, &c, 1L);
        if (r == 0)
            return 0;
    }
    *dst = 0;
    strip_char(start, '\n');
    strip_char(start, '\r');
    return 1;
}
