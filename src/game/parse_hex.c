/* parse_hex -- shipped at 1040:5cbb.
 *
 * Ported by tools/port.py from matched/game/parse_hex.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_char_class[];             DGROUP:0x46c1
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 toupper_rtl(col_i16 c);


col_i16 parse_hex(char *p)
{
    col_i16 v = 0;
    col_i16 c;

    while (*p != 0) {
        c = *p;
        p++;
        c = toupper_rtl(c);
        if (g_char_class[c] & 2) {
            v = v << 4;
            v += c - '0';
        } else if (c >= 'A' && c <= 'F') {
            v = v << 4;
            v += c - 0x37;
        } else {
            while (*p != 0)
                p++;
        }
    }
    return v;
}
