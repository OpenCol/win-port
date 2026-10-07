/* stoi: candidate for 1040:5d9d (186 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: stoi
 *
 * Parses a number with an optional radix prefix: "0x" hex, "0b" binary,
 * "0d" decimal, anything else decimal (ATOL_1040_a82d).  The three parsers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from parse_number: the Mac build's stoi(char*), bound by call-graph agreement (1 of its Mac call edges already bound land here, runner-up 0) and by call-sequence alignment (the lone unbound callee of its gap in text_item_number); the arity agrees and the Mac body is 1.20x this one.
 */

#include "colonization.h"

extern int g_stub_a;

extern int xtoi(char *s);
extern int btoi(char *s);

int stoi(char *s)
{
    register int r = 0;
    unsigned char prefix;

    if ((int)s[0] == '0') {
        prefix = s[1];
        prefix = toupper_rtl(prefix);
        if (prefix == 'X')
            r = xtoi(s + 2);
        else if (prefix == 'B')
            r = btoi(s + 2);
        else if (prefix == 'D')
            r = ATOL_1040_a82d(s + 2);
        else
            r = ATOL_1040_a82d(s);
    } else {
        r = ATOL_1040_a82d(s);
    }
    return r;
}
