/* say_braced: candidate for 1040:a365 (66 bytes).  Verdict: RELOCATABLE.
 * symbol: say_braced
 *
 * Appends TEXT string n between braces to the message.  The three helpers
 * are -called and defined in-file as stubs.
 */

#include "colonization.h"

extern char g_stub_s[];
extern int g_stub_a;

extern char *strings(int n);

void say_braced(char *dst, int n)
{
    append_lbrace(dst);
    STRCAT(dst, strings(n));
    append_rbrace(dst);
}
