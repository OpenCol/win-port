/* to_pascal_string -- shipped at 1040:7808.
 *
 * Ported by tools/port.py from matched/game/to_pascal_string.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void * memcpy_rtl(void *dst, void *src, col_u16 n);

void to_pascal_string(char *s)
{
    char *p;
    char buf[0x100];
    col_i16 i;

    buf[0] = strlen(s);
    p = s;
    i = 1;
    while (*p != 0) {
        buf[i] = *p;
        p++;
        i++;
    }
    memcpy_rtl(s, buf, buf[0] + 1);
}
