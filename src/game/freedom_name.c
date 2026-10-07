/* freedom_name: candidate for 1020:9972 (91 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * s = "<word> <v - 24>": a global word, a space, then the number.
 *
 * RENAMED 2026-09-26 from say_word_and_offset: the Mac build's freedom_name(short, char*), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between set_father and show_fathers, where the Mac gap holds 4 functions and the Windows one 8, so the pairing inside it is by size and arity (the Mac body is 0.97x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

extern void space_string(char *buf);

void freedom_name(int nation, char *s)
{
    int number = nation - 24;

    s[0] = 0;
    say_string(s, g_founding_word[5]);
    space_string(s);
    say_number(s, number);
}
