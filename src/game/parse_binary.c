/* parse_binary -- shipped at 1040:5d3a.
 *
 * Ported by tools/port.py from matched/game/parse_binary.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 toupper_rtl(col_i16 c);

col_i16 parse_binary(char *p)
{
    col_i16 v = 0;
    col_i16 c;

    while (*p != 0) {
        c = *p;
        p++;
        c = toupper_rtl(c);
        if (c == '0' || c == '1') {
            v = v * 2;
            v += c - '0';
        } else {
            while (*p != 0)
                p++;
        }
    }
    return v;
}
