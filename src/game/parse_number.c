/* parse_number -- shipped at 1040:5d9d.
 *
 * Ported by tools/port.py from matched/game/parse_number.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ATOL_1040_a82d(char *s);
col_i16 parse_binary(char *s);
col_i16 parse_hex(char *s);

extern col_i16 toupper_rtl(col_i16 c);





col_i16 parse_number(char *s)
{
    register col_i16 r = 0;
    unsigned char c;

    if ((col_i16)s[0] == '0') {
        c = s[1];
        c = toupper_rtl(c);
        if (c == 'X')
            r = parse_hex(s + 2);
        else if (c == 'B')
            r = parse_binary(s + 2);
        else if (c == 'D')
            r = ATOL_1040_a82d(s + 2);
        else
            r = ATOL_1040_a82d(s);
    } else {
        r = ATOL_1040_a82d(s);
    }
    return r;
}
